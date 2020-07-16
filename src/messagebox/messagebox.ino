// TFT LCD Imports
#define ESP8266

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Fonts/FreeMono9pt7b.h>
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



#include "FS.h"

#define LED D1
#define LID D0
#define BACKLIGHT D2

ESP8266WebServer Server;
HTTPClient client;
AutoConnect Portal(Server);
String key;
String user;
String code;
String message = "";
int httpInterval = 0;
boolean state = true;

String host = "messagebox.unitfi.com";
int port = 80;

const char * headerKeys[] = {"user", "code", "reset"};
const size_t numberOfHeaders = 3;

void resetBox() {
  SPIFFS.remove("/key.txt");
  SPIFFS.remove("/user.txt");
  ESP.restart();
}

void ICACHE_RAM_ATTR showMessage();
void ICACHE_RAM_ATTR clearMessage();

void setup(void)
{
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
  //tft.setTextSize(1);
  //tft.setFont(&FreeMono9pt7b);
  tft.setCursor(50, 60);
  tft.print("MessageBox");
  digitalWrite(BACKLIGHT, HIGH);
  delay(2000);

  // Start the wifi autoconnect service
  Portal.begin();

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


/*void newMessage(){
  digitalWrite(LED, HIGH);
  //TODO: Start glowing LED
  screen.clearScreen();
  screen.setFont(10);
  screen.print(message);
}*/


void drawHeart(int x, int y){
  tft.fillCircle(x+4, y+4, 4, 0xB882);
  tft.fillCircle(x+12, y+4, 4, 0xB882);
  tft.fillTriangle(x, y+4, x+16, y+4, x+8, y+16, 0xB882);
}


void loop(void)
{ 
  if(digitalRead(LID) == HIGH){
    if(message != ""){
      tft.fillScreen(ST77XX_BLACK);
      tft.print(message);
      digitalWrite(LED, LOW);
      message = "";
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
    ///screen.clearScreen();
    //screen.print("Connect to MessageBox WiFi access point to set me up!");
    Portal.handleClient();
  } else {
    if(httpInterval >= 20){
      if(message != ""){
        //We have a message waiting to be read. Do nothing.
        //delay(10000);
      } else if(user != "") {
        // Begin asking the server if we have any new messages
       
        client.begin(String("http://messagebox.unitfi.com/message/read") +
                      "?key=" + key +
                      "&user=" + user);
        client.collectHeaders(headerKeys, numberOfHeaders);
        int httpCode = client.GET();       
  
        message = client.getString();
        if(message == "No Messages"){
          message = "";
        } else if(message == "No box"){
          resetBox();
        } else {
          digitalWrite(LED, HIGH);
        }
        
        //screen.clearScreen();
        //screen.print(client.getString());
  
        client.end();
  
        httpInterval = 0;
      }
      else
      {
        // We haven't authenticated yet, send credentials to server until
        // we get a user id
        client.begin(String("http://messagebox.unitfi.com/box/activate") +
                      "?key=" + key);
  
        client.collectHeaders(headerKeys, numberOfHeaders);
        int httpCode = client.GET();  
  
        // Check activation headers:
        // If "code", display the activation code
        // If "user", we've been acivated and assigned to a user,
        //    so save that information.
        if(client.hasHeader("code")){
          tft.fillScreen(ST77XX_BLACK);
          tft.print("Activation code: \n" + client.header("code")); 
        } else if(client.hasHeader("user")){
          user = client.header("user");
          tft.fillScreen(ST77XX_BLACK);
          tft.print("Activated! :D\n\nUser: " + user);
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
