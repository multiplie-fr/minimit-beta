
#include "OTA.h"

void setupOTA() {
  // put your setup code here, to run once:
  Serial.println("Starting sketch B - OTA update");  
  delay(1000);
  String OTAfile = "https://xn--multipli-i1a.fr/minimit/prod/ota/"; 

}

void loopOTA() {
  // put your main code here, to run repeatedly:
  Do_OTA("https://www.tagtagtag.fr/minimit-ota.ino.bin");

}