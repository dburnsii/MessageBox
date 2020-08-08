// TFT LCD Imports
#define ESP8266

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS         D8
#define TFT_RST        D4
#define TFT_DC         D3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//ESP8266 specific imports

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include <FS.h>

#define LED D1
#define LID D0
#define BACKLIGHT D2

#define AUTOCONNECT_URI         "/_ac/config"

#define HEIGHT 128
#define WIDTH 160

#define DEBUG 1

ESP8266WebServer Server;
HTTPClient client;
AutoConnect Portal(Server);
AutoConnectConfig PortalConfig;
String key;
String user;
String code;
String message = "";
int httpInterval = 0;
boolean state = true;
boolean serverAvailable = true;
boolean wifiConnected = false;
uint8_t buffer[5840];
String host = "messagebox.unitfi.com";
int port = 80;
String imageName = "/message.tft";

const char * headerKeys[] = {"user", "code", "Content-Type", "status"};
const size_t numberOfHeaders = 4;

void resetBox() {
  SPIFFS.remove("/key.txt");
  SPIFFS.remove("/user.txt");
  ESP.restart();
}

void displayWiFiLogin(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 50);
  tft.println("   Connect your phone to");
  tft.println("   the 'Messagebox' WiFi");
  tft.println("   network to set me up!");
}

void displayConnecting(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 50);
  tft.println("Connecting to WiFi");
}


void displayWiFiConnected(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10,60);
  tft.println("WiFi Network Connected!");
}

void displayActivationCode(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,40);
  tft.println("       Activate me @");
  tft.println(host + "/register");
  tft.println("    in your browser.");
  tft.println("");
  tft.println("      Activation Code:");
  tft.println("         " + code);
}

void displayNoServer(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10,80);
  tft.println("Can't contact MessageBox :(");
}

void setup(void)
{
  if(DEBUG){
    Serial.begin(9600);
    Serial.println("Starting messagebox");
  }
  //Initialize the LED and the box lid
  pinMode(LED, OUTPUT);
  analogWrite(LED, 128);
  pinMode(BACKLIGHT, OUTPUT);
  pinMode(LID, INPUT);

  //Intitialize the lcd screen
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(0xFFFF);
  tft.setCursor(50, 60);
  tft.print("MessageBox");
  digitalWrite(BACKLIGHT, HIGH);
  delay(3000);

  displayConnecting();

  // Start the wifi autoconnect service
  PortalConfig.apid = "Messagebox";
  PortalConfig.bootUri = AC_ONBOOTURI_HOME;
  PortalConfig.homeUri = "/_ac/config";
  PortalConfig.psk = "";
  PortalConfig.portalTimeout = 1;
  PortalConfig.retainPortal = true;
  Portal.config(PortalConfig);
  Portal.begin();

  Serial.println("Portal initialized");

  // Open the file system to get saved keys and user ids
  SPIFFS.begin();

  // Check if a generated key exists. If it does not exist,
  // Create a new one (randomly generated 32-byte key)
  if (SPIFFS.exists("/key.txt"))
  {
    File keyFile = SPIFFS.open("/key.txt", "r");
    key = keyFile.readString();
    keyFile.close();
  } else {
    key = "";
    int i = 0;
    for( int i = 0; i < 32; i++)
    {
      key += String(random(255), HEX);
    }
    File keyFile = SPIFFS.open("/key.txt", "w");
    keyFile.print(key);
    keyFile.close();
  }

  if (SPIFFS.exists("/user.txt"))
  {
    File userFile = SPIFFS.open("/user.txt", "r");
    user = userFile.readString();
  }
  else
  {
    user = "";
  }
}



void drawHeart(int x, int y){
  tft.fillCircle(x+4, y+4, 4, 0xB882);
  tft.fillCircle(x+12, y+4, 4, 0xB882);
  tft.fillTriangle(x, y+4, x+16, y+4, x+8, y+16, 0xB882);
}

void downloadMessage(){
  if(DEBUG){Serial.println("Downloading message.");}
  if(SPIFFS.exists(imageName)){
    SPIFFS.remove(imageName);
  }
  File image = SPIFFS.open(imageName, "w");
  if(!image){
    //Image open failed!
    return;
  }
  int len = client.getSize();
  if(DEBUG){Serial.print("Download size: "); Serial.println(client.getSize());}
  WiFiClient* stream = client.getStreamPtr();
  while(client.connected() && (len > 0 || len == -1)){
    size_t size = stream->available();
    if(size){
      int c = stream->readBytes(buffer, ((size > sizeof(buffer)) ? sizeof(buffer) : size));
      image.write(buffer, c);
      if(len > 0){
        len -= c;
      }
    }
    delay(1);
  }
  image.close();
  if(DEBUG){Serial.println("Download complete.");}
}

void displayMessage(){
  File messageFile = SPIFFS.open(imageName, "r");
  if(DEBUG){Serial.print("File size: "); Serial.println(messageFile.size());}
  uint16_t pixel;
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      pixel = messageFile.read();
      pixel += messageFile.read() << 8;
      if(DEBUG && i < 5 && j < 5){
        Serial.println(pixel);
      }
      tft.drawPixel(j, i, pixel);
    }
  }
}

void loop(void)
{
  if(digitalRead(LID) == LOW){
    if(!state && WiFi.status() != WL_CONNECTED){
      displayWiFiLogin();
    } else if(!state && WiFi.status() == WL_CONNECTED && !wifiConnected) {
      wifiConnected = true;
      WiFi.softAPdisconnect(true);
      displayWiFiConnected();
      delay(3000);
      state = false;
    } else if(!state && SPIFFS.exists(imageName)){
      displayMessage();
      SPIFFS.remove(imageName);
      digitalWrite(LED, LOW);
    } else if(!state && user == "" && code != "") {
      displayActivationCode();
    } else if(!state && !serverAvailable){
      displayNoServer();
    } else if(!state) {
      tft.fillScreen(ST77XX_BLACK);
      drawHeart((160 / 2) - 8, (128/2) - 8);
    }
    state = true;
    digitalWrite(BACKLIGHT, HIGH);
  } else {
    digitalWrite(BACKLIGHT, LOW);
    tft.fillScreen(ST77XX_BLACK);
    state = false;
  }

  if(WiFi.status() != WL_CONNECTED){
    Portal.handleClient();
  } else {
    if(!wifiConnected){state = false;}
    if(httpInterval >= 20){
      if(SPIFFS.exists(imageName)){
        //We have a message waiting to be read. Do nothing.
        //delay(10000);
      } else if(user != "") {
        // Begin asking the server if we have any new messages
        client.begin(String("http://" + host + "/message/read") +
                      "?key=" + key +
                      "&user=" + user);
        client.collectHeaders(headerKeys, numberOfHeaders);
        int httpCode = client.GET();

        if(httpCode >= 200 && httpCode <= 299){
          serverAvailable = true;
        } else {
          serverAvailable = false;
        }

        if(client.hasHeader("status")){
          String status = client.header("status");
          if(status == "No Messages"){
            //No message, do nothing.
          } else if(status == "New Message" && client.header("Content-Type") == "application/octet-stream"){
            downloadMessage();
          } else if(status == "No Box"){
            resetBox();
          } else if(status == "Update"){
            //Update system
          }
        }

        client.end();

        httpInterval = 0;
      }
      else
      {
        // We haven't authenticated yet, send credentials to server until
        // we get a user id
        client.begin(String("http://" + host + "/box/activate") +
                      "?key=" + key);

        client.collectHeaders(headerKeys, numberOfHeaders);
        int httpCode = client.GET();

         if(httpCode >= 200 && httpCode <= 299){
          serverAvailable = true;
        } else {
          serverAvailable = false;
        }

        // Check activation headers:
        // If "code", display the activation code
        // If "user", we've been acivated and assigned to a user,
        //    so save that information.
        if(client.hasHeader("code") && code == ""){
          code = client.header("code");
          state = false;
        } else if(client.hasHeader("user")){
          user = client.header("user");
          state = false;
        } else if(client.hasHeader("reset")){
          resetBox();
        }

        client.end();

        httpInterval = 0;
      }
    }
    httpInterval++;

  }
  delay(50);
}
