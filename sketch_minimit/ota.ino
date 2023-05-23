#include "OTA.h"
String lastVersionOTA;

void setupOTA() {
  // put your setup code here, to run once:
  minitel.newScreen();
  String vdt ="14,0c,1f,46,54,0e,1b,46,40,50,1f,47,53,0e,1b,46,58,1b,56,20,20,1b,40,22,1b,50,1b,46,30,1f,48,51,0e,1b,46,40,1b,56,1b,40,21,20,20,20,20,1b,45,58,1f,49,51,0e,1b,56,1b,40,21,20,20,20,1b,45,40,1b,55,1b,46,21,1b,40,30,1f,4a,51,0e,1b,56,20,12,42,1b,45,58,1b,55,1b,40,40,1b,50,1b,45,21,4a,1f,4b,51,0e,1b,56,1b,40,30,20,20,1b,55,48,1b,50,20,20,1b,55,25,1b,50,1b,45,30,1f,4c,51,0e,1b,46,22,1b,56,1b,40,30,20,1b,55,4a,1b,50,1b,45,40,1b,55,1b,40,21,20,20,1b,50,1b,45,54,1f,4d,53,0e,1b,56,1b,40,54,1b,55,22,20,20,20,20,40,1b,50,1b,45,21,1f,4e,54,0e,1b,45,22,1b,55,1b,40,30,20,20,58,1f,4f,56,0e,1b,55,1b,40,54,1b,50,1b,45,21,09,09,1b,47,40,1f,50,4f,0e,4a,49,49,42,4a,49,42,4a,49,49,42,4a,21,1f,51,4f,0e,2a,12,4a,22,24,1f,54,46,1b,4d,1b,48,55,6e,65,20,6d,69,73,65,20,19,41,61,20,6a,6f,75,72,20,65,73,74,20,65,6e,20,63,6f,75,72,73,2e,12,42,1f,56,48,1b,46,4d,45,52,43,49,20,44,45,20,4e,45,20,54,4f,55,43,48,45,52,20,41,20,52,49,45,4e,20";
  checkScreen(vdt,0,0);
  delay(1000);
  //String OTAfile = "https://xn--multipli-i1a.fr/minimit/prod/ota/";
}
void setLastVersionOTA(String v){
  lastVersionOTA = v;  
}
void loopOTA() {
  // put your main code here, to run repeatedly:
  if (Do_OTA("https://xn--multipli-i1a.fr/minimit/prod/ota/sketch_minimit.ino.bin")){
    writeOTAConfig(lastVersionOTA);
    ESP.restart();
  }
}

String confOTA = "/confota.txt";

JSONVar getOTAConfig() {
  JSONVar config = readOTAConfig();
  return config;
}
void writeOTAConfig(String version) {
  File file = SPIFFS.open(confOTA, "w");
  if (!file) {
    Serial.println("Failed to open confOTA");
    return;
  } else {
    JSONVar myconfig = {};
    myconfig["version"] = version;
    file.print(myconfig);
    file.close();
  }
}
JSONVar readOTAConfig() {
  File file = SPIFFS.open(confOTA, "r");
  JSONVar theresult = {};
  if (!file) {
    Serial.println("Failed to open confOTA");
    return theresult;
  } else {
    int s = file.size();
    char content[s + 1] = {};
    int i = 0;
    while (file.available()) {
      char c = file.read();
      content[i] = c;
      i++;
    }
    String myString = String(content);
    theresult = JSON.parse(content);
    return theresult;
  }
}