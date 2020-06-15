#define _Digole_Serial_UART_

#include <DigoleSerial.h>

DigoleSerialDisp screen(&Serial, 115200); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>

#include "FS.h"

#define LED D1
#define LID D2

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

void newMessage(){
  digitalWrite(LED, HIGH);
  //TODO: Start glowing LED
  screen.clearScreen();
  screen.setFont(10);
  screen.print(message);
}


void setup(void)
{
  //Initialize the LED and the box lid
  pinMode(LED, OUTPUT);
  analogWrite(LED, 128);
  pinMode(LID, INPUT_PULLUP);

  //Intitialize the lcd screen
  delay(100);
  screen.begin();
  screen.screenOnOff(0);
  delay(100);
  screen.screenOnOff(1);
  delay(100);
  screen.backLightOn();
  screen.setRotation(2);
  screen.clearScreen();
  screen.setColor(255);
  screen.write('F');  //disable auto refresh screen
  screen.write('S');
  screen.write('1');
  screen.setFont(10);
  screen.print("\n\n\n\n\n\n");
  screen.print(center("MessageBox"));
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

  //attachInterrupt(digitalPinToInterrupt(LID), showMessage, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(LID), clearMessage, RISING);
}




void drawHeart(int x, int y){
  screen.drawCircle(x, y, 28, 1);
}


void loop(void)
{ 
  if(digitalRead(LID) == HIGH){
    if(message != ""){
      screen.clearScreen();
      screen.setFont(10);
      screen.setColor(255);
      screen.print(message);
      digitalWrite(LED, LOW);
      message = "";
    } else if(!state) {
      screen.clearScreen();
      screen.setTrueColor(0,0,230);
      drawHeart(56, 56);
    }
    state = true;
    screen.backLightOn();
  } else {
    screen.backLightOff();
    screen.clearScreen();
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
        //screen.backLightOff();
        
        client.begin(String("http://messagebox.unitfi.com/message/read") +
                      "?key=" + key +
                      "&user=" + user);
        client.collectHeaders(headerKeys, numberOfHeaders);
        int httpCode = client.GET();       
  
        message = client.getString();
        if(message == "No Messages"){
          //screen.clearScreen();
          //screen.setFont(10);
          //screen.print(".");
          message = "";
        } else if(message == "No box"){
          resetBox();
        } else {
          newMessage();
          
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
          screen.clearScreen();
          screen.print("Activation code: \r\n" + client.header("code")); 
        } else if(client.hasHeader("user")){
          user = client.header("user");
          screen.clearScreen();
          screen.setFont(10);
          screen.print("Activated! :D\r\nUser: " + user);
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
