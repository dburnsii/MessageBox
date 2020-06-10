#define _Digole_Serial_UART_

#include <DigoleSerial.h>

DigoleSerialDisp screen(&Serial, 115200); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>

ESP8266WebServer Server;
AutoConnect Portal(Server);

void rootPage() {
  char content[] = "Hello world!";
  Server.send(200, "text/plain", content);
}

void setup(void)
{
  screen.begin();
  screen.clearScreen();
  screen.setColor(255);
  screen.clearScreen();
  screen.print("Hello World!");
  screen.write('F');  //disable auto refresh screen
  screen.write('S');
  screen.write('1');
  screen.setColor(195);

  
  Server.on("/", rootPage);
  Portal.begin();
}

void loop(void)
{
  Portal.handleClient();
}
