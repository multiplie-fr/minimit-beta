void champVide(int x, int y, int longueurchamp) {
  minitel.textMode();
  minitel.noCursor();
  minitel.moveCursorXY(x, y);
  minitel.attributs(CARACTERE_BLANC);
  for (int j = 0; j < longueurchamp; j++) {
    minitel.print(".");
  }
  minitel.moveCursorXY(x, y);
  minitel.cursor();
  minitel.echo(true);
}

void wifiConnect() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println(WiFi.status());
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
// byte stringToByte(char * src){
//     return byte(atoi(src));
// }
void afficheRemoteVDT(String vdtFile, int offsetY, int offsetX) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("vdt2.php?f=") + vdtFile;
    Serial.println(serverPath);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      checkScreen(payload, offsetY, offsetX);
    }
  }
}
void checkScreen(String s, int offsetY, int offsetX){
  unsigned int str_len = s.length() + 2;
  int i = 0;
  int positionnement = 0;
  int startpositionnement = -1;
  int cnt = 0;
  for (i=0; i<str_len; i+=3)
  {
    String myByte = "0x" + s.substring(i,i+2);
    int val = strtoul(myByte.c_str(), NULL, 16);
    if (positionnement == 1) {

      if (i == startpositionnement + 3) {
        val += offsetY;
      }
      if (i == startpositionnement + 6) {
        val += offsetX;
      }
    }
    if (i > startpositionnement + 3) {
      positionnement = 0;
    }
    minitel.writeByte(val);

     if (val == 31) {
      positionnement = 1;
      startpositionnement = i;
    }
    
   
  }
}
void drawScreen(String s, int offsetY, int offsetX) {
  unsigned int str_len = s.length() + 1;
  char charArray[str_len];
  s.toCharArray(charArray, str_len);
  int NUMBER_OF_ENTRIES = (str_len / 6) + 1;
  uint8_t byteArray[NUMBER_OF_ENTRIES];
  int i = 0;
  // split charArray at each ',' and convert to uint8_t
  char* p = strtok(charArray, ",");
  while (p != NULL) {
    byteArray[i++] = strtoul(p, NULL, 16);
    p = strtok(NULL, ",");
  }
  int positionnement = 0;
  int startpositionnement = -1;
  // // Print the byteArray
  for (i = 0; i < sizeof(byteArray) / sizeof(char); i++) {
    int val = byteArray[i];
    if (positionnement == 1) {
      if (i == startpositionnement + 1) {
        val += offsetY;
      }
      if (i == startpositionnement + 2) {
        val += offsetX;
      }
    }
    if (i > startpositionnement + 1) {
      positionnement = 0;
    }
    minitel.writeByte(val);
    if (val == 31) {
      positionnement = 1;
      startpositionnement = i;
    }
  }
}

void effacementEcran(int y1, int y2) {
  minitel.noCursor();
  minitel.attributs(FIN_LIGNAGE);
  //minitel.attributs(FOND_BLEU);
  //minitel.attributs(CARACTERE_BLEU);
  for (int i = 0; i <= (y2 - y1); i++) {
    minitel.moveCursorXY(1, (y2 - i));
    minitel.clearLine();
  }
  minitel.cursor();
}