void displayWiFiLogin(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 50);
  tft.println("   Connect your phone to");
  tft.println("   the 'Messagebox' WiFi");
  tft.println("   network to set me up!");
}

void displayConnecting(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(30, 50);
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
  tft.println("Can't contact MessageBox\n       :(");
}

void displayMessage(){
  File messageFile = SPIFFS.open(imageName, "r");
  if(DEBUG){Serial.print("File size: "); Serial.println(messageFile.size());}
  uint16_t pixel;
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      pixel = messageFile.read();
      pixel += messageFile.read() << 8;
      tft.drawPixel(j, i, pixel);
    }
  }
}

void drawHeart(int x, int y){
  tft.fillCircle(x+4, y+4, 4, 0xB882);
  tft.fillCircle(x+12, y+4, 4, 0xB882);
  tft.fillTriangle(x, y+4, x+16, y+4, x+8, y+16, 0xB882);
}
