#include "OTA.h"
#include <Update.h>
#include <HTTPClient.h>

#define OTA_BUF_SIZ  (1024) // not too large : stack overflow
#define OTA_TIMEOUT_MS 10000



// ex : Do_OTA("https://www.antoineschmitt.com/tests/Squikit.ino.bin")

bool Do_OTA(String url, OTA_progress_cb cb) { 
  Serial.println("OTA Dooing OTA from: " + url);
  // Connect to host

  HTTPClient http;
  if (!http.begin(url)) {
    Serial.println("OTA HTTPClient begin failed");
    return false;
  }
  
  http.setTimeout(OTA_TIMEOUT_MS);

  // collect Content-Type
  const char * headerkeys[] = {"Content-Type"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  http.collectHeaders(headerkeys,headerkeyssize);
   
  int status = http.GET();
  if (status < 0) {
    // bad server
    Serial.printf("OTA connect failed, error: %s\n", http.errorToString(status).c_str());
    http.end();
    return false;  
  }
  if (status != HTTP_CODE_OK) {
    // bad URI
    Serial.println("OTA GET failed : status: " + String(status));
    http.end();
    return false;  
  }

  int len = http.getSize();
  if (len <= 0) {
    Serial.println("OTA Content-Length=0");
    http.end();
    return false;
  }

  String contenttyp = http.header("Content-Type");
  if (!(contenttyp == "application/octet-stream")) {
    Serial.println("OTA Bad Content-Type: '" + contenttyp +"'");
    http.end();
    return false;
  }
  
  // Check if there is enough to OTA Update
  bool canBegin = Update.begin(len);
  if (!canBegin) {
    Serial.println("Not enough space to begin OTA");
    http.end();
    return false;
  }
  
  Serial.println("OTA. Doing...");

  // simple way :
  // WiFiClient& stream = http.getStream();
  // size_t written = Update.writeStream(stream);

  // looped way:
  size_t written = 0;
  
  // create buffer for read
  uint8_t buff[OTA_BUF_SIZ] = { 0 };
  
  WiFiClient* stream = http.getStreamPtr();

  float ratio = 0.0;
  if (cb) cb(ratio);
  
  while (http.connected() && (written < len)) {
      // get available data size
      size_t avail = stream->available();

      if (avail) {
          // read up to OTA_BUF_SIZ
          int readd = stream->readBytes(buff, ((avail > OTA_BUF_SIZ) ? OTA_BUF_SIZ : avail));

          // Update
          if (Update.write(buff, readd) != readd) {
            Serial.println("OTA. Error writing OTA :" + String(Update.getError()));
            Update.printError(Serial);
            Update.abort();
            http.end();
            return false;  
          }

          written += readd;
          ratio = float(written)/float(len);
          
          if (cb) cb(ratio);
          //Serial.println("ratio="+String(ratio));
      }
      delay(1);
  }
  
  if (written != len) {
    Serial.println("OTA failed. Written only : " + String(written) + "/" + String(len));
    http.end();
    return false;
  }
  
  Serial.println("OTA Written : " + String(written) + " successfully");

  if (!Update.end()) {
    Serial.println("OTA end failed : " + String(Update.getError()));
    http.end();
    return false;
  }
  
  Serial.println("OTA done!");
  if (!Update.isFinished()) {
    Serial.println("OTA finish failed");
    http.end();
    return false;
  }

  http.end();
  Serial.println("Update successfully completed. Rebooting.");
  //ESP.restart();

  // never reached
  return true;
}
