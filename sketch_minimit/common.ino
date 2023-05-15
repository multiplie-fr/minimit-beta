void champVide(int x, int y, int longueurchamp) {
  minitel.textMode();
  minitel.noCursor();
  minitel.moveCursorXY(x, y);
  minitel.attributs(saisieColor);
  for (int j = 0; j < longueurchamp; j++) {
    minitel.print(".");
  }
  minitel.moveCursorXY(x, y);
  minitel.cursor();
  minitel.echo(true);
  userInput = "";
  userInputLength = 0;
}

void wifiConnect() {
  if (WiFi.status() == WL_CONNECTED) return;

  JSONVar config = myConfig["input"];
  const char* ssid = (const char*)config[0];
  const char* password = (const char*)config[1];

  WiFi.begin(ssid, password);
  Serial.print(ssid);
  Serial.print(password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(2000);
    Serial.println("wifi status");
    Serial.println(WiFi.status());
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
String getRemoteVDT(String vdtFile, int offsetY, int offsetX) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("vdt2.php?f=") + vdtFile;
    Serial.println(serverPath);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      return(payload);
    }
  }
  return "";
}
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
boolean getInterruption() {
  return interruption;
}
String vdt;
int vdtByteEncours;
int vdtLength;
int vdtoffsetY;
int vdtoffsetX;
boolean isVdtEnCours;

void startVdt(String stvdt, int offsetX, int offsetY) {
  minitel.noCursor();
  isVdtEnCours = true;
  vdt = stvdt;
  vdtoffsetX = offsetX;
  vdtoffsetY = offsetY;
  vdtLength = stvdt.length() + 2;
  Serial.println("vdtlength");
  Serial.println(vdtLength);
  vdtByteEncours = 0;
}
void loopVDT() {
  if (!isVdtEnCours) {
    return;
  }
  if(vdtByteEncours==(vdtLength-1))
  {
    isVdtEnCours = false;
    Serial.println("onsrotlimagest finie");
    nextUpdate = millis() + timeoutAmount;
    return;
  }
 if (getInterruption() == true) {
    interruption = false;
    isVdtEnCours = false;
    Serial.println("interruption");
    return;
  }
  String myByte = "0x" + vdt.substring(vdtByteEncours, vdtByteEncours + 2);
  int val = strtoul(myByte.c_str(), NULL, 16);
  minitel.writeByte(val);
  vdtByteEncours+=3;
}
void checkScreen(String s, int offsetY, int offsetX) {
  Serial.println("checkscreen");
  minitel.noCursor();
  unsigned int str_len = s.length() + 2;
  Serial.println(str_len);
  int i = 0;
  int positionnement = 0;
  int startpositionnement = -1;
  lasttouche=0;
  bool jesors = false;
  for (i = 0; i < str_len; i += 3) {

  //touche = minitel.getKeyCode();
  //    if(touche==CONNEXION_FIN && lasttouche==CONNEXION_FIN){
  //     Serial.println("ccccccc2fois");
  //    minitel.connexion(false);
  // }
  //   if (touche == CONNEXION_FIN && lasttouche != CONNEXION_FIN) {
  //     Serial.println("lalalalFisrt time");
  //     Serial.println(lasttouche);
  //     lasttouche=CONNEXION_FIN;
  //     i=str_len-1;
  //     minitel.connexion(false);
  //     displayMire();
  //     jesors = true;
  //  }
  //  else
  //  {
  //    lasttouche=0;
  //  }
  
  //   if(jesors){
  //     break;      
  //   }
  // if (getInterruption()==1){
  //   Serial.println("interruption");
  //   i=str_len-1;
  // }
    String myByte = "0x" + s.substring(i, i + 2);
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

void effacementEcran(int y1, int y2, int attribut1, int attribut2) {
  minitel.noCursor();
  minitel.attributs(FIN_LIGNAGE);
  minitel.attributs(attribut1);
  //minitel.attributs(INVERSION_FOND);
  minitel.attributs(attribut2);
  for (int i = 0; i <= (y2 - y1); i++) {
    minitel.moveCursorXY(1, (y2 - i));
    minitel.graphicMode();
    //minitel.writeByte(0x20);
    minitel.graphic(0b111100);
    minitel.repeat(39);
    //minitel.clearLine();
  }
  //minitel.cursor();
  //minitel.attributs(FOND_NORMAL);
  minitel.textMode();
}
void ligneZero(String message) {
  int mx = minitel.getCursorX();
  int my = minitel.getCursorY();
  minitel.newXY(1, 0);
  minitel.print(" ");
  minitel.repeat(30);
  minitel.newXY(1, 0);
  minitel.print(message);
  minitel.newXY(mx, my);
}