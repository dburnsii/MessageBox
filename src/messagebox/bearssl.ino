
// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");  
  if(DEBUG){Serial.print("Waiting for NTP time sync: ");}
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    if(DEBUG){Serial.print(".");}
    now = time(nullptr);
  }
  if(DEBUG){Serial.println("");}
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  if(DEBUG){
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
  }
}
