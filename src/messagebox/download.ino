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
    for(int i = 0; i < size; i++){
      image.write(stream->read());
      int c = 1;
      if(len > 0){
        len -= c;
      }
    }
    delay(1);
  }
  image.close();
  if(DEBUG){Serial.println("Download complete.");}
  uint32_t color = ring.Color(15, 15, 255);
  for(int i=0; i<ring.numPixels(); i++){
    //color = ring.Color(8, 8, 255);
    ring.setPixelColor(i,color);
  }
  ring.setBrightness(64);
  ring.show();
}
