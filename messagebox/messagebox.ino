#define _Digole_Serial_UART_

#include <DigoleSerial.h>

DigoleSerialDisp screen(&Serial, 115200); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>

#include "FS.h"

ESP8266WebServer Server;
WiFiClient client;
AutoConnect Portal(Server);
String key;
String user;
String code;

String host = "192.168.0.21";
int port = 3000;

void rootPage() {
  char content[] = "Hello world!";
  Server.send(200, "text/plain", content);
}

int activated(String input){
  String line = "";
  String target = "user:";
  int i = 0;
  // Clear first line
  while(i < input.length()){
    if(input.charAt(i) == '\n'){
      i++;
      break;
    }
    i++;
  }
  while(i < input.length()){
    if(input.charAt(i) == '\n'){
      line = "";
      i++;
    }
    line += input.charAt(i);
    if(line == target){
      screen.clearScreen();
      screen.print("Activated!");
      return 100;
    }
    i++;
  }
  return 0;
}

String activationCode(String input){
  String line = "";
  String target = "code:";
  String output = "";
  int i = 0;
  // Clear first line
  while(i < input.length()){
    if(input.charAt(i) == '\n'){
      i++;
      break;
    }
    i++;
  }
  while(i < input.length()){
    if(input.charAt(i) == '\n'){
      line = "";
      i++;
    }
    line += input.charAt(i);
    if(line == target){
      i++;
      while(input.charAt(i) != '\n' && input.charAt(i) != '\r'){
        output += input.charAt(i);
        i++;
      }
      return output;
    }
    i++;
  }
}

void setup(void)
{
  screen.begin();
  screen.clearScreen();
  screen.setColor(255);
  screen.setFont(6);
  screen.clearScreen();
  screen.print("Hello World!");
  screen.write('F');  //disable auto refresh screen
  screen.write('S');
  screen.write('1');

  
  Server.on("/", rootPage);
  Portal.begin();

  SPIFFS.begin();
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
    user = userFile.read();
  }
  else
  {
    user = "";
  }
}

void loop(void)
{
  if(WiFi.status() != WL_CONNECTED)
  {
    ///screen.clearScreen();
    //screen.print("Connect to MessageBox WiFi access point to set me up!");
    Portal.handleClient();
  } else {
    if ( client.connect(host, port) )
    {
      if(user != "")
      {
        while(1)
        {
          // We have been authenticated and should check for new messages
          client.print(String("GET /message/read?key=") + key +
                "&user=" + user + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n" +
                "\r\n"
          );
          String response = "";
          while( client.connected() || client.available())
          {
            if(client.available())
            {
              response += client.readString();
            }
          }
          //TODO: Process input from http response for message

          
          screen.clearScreen();
          screen.print(response);
          
          delay(10000);
        }
      }
      else
      {
          // We haven't authenticated yet, send credentials to server until
          // we get a user id
          client.print(String("GET /box/activate?key=") + key +
                " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n" +
                "\r\n"
          );
          String response = "Response: ";
          while( client.connected() || client.available())
          {
            if(client.available())
            {
              response += client.readString();
            }
          }
          client.stop();

          if(activated(response) > 0){
            screen.clearScreen();
            screen.print("Activated!");
            //TODO: Handle activation
          } else {
            code = activationCode(response);
            screen.clearScreen();
            screen.print("Activation code: " + code);
          }

          //screen.clearScreen();
          //screen.print(response);
          
          delay(10000);
      }
    }
    else
    {
      screen.clearScreen();
      screen.print("Unable to connect to host: " + host + ":" + port);
    }
  }
  delay(1000);
}
