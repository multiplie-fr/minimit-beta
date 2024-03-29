void champVide(int x, int y, int longueurchamp) {
  minitel.noCursor();
  minitel.moveCursorXY(x, y);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print(".");
  minitel.repeat(longueurchamp-1);
  userInput = "";
  userInputNeeded = 0;
  minitel.moveCursorXY(x, y);
  minitel.cursor();
  minitel.echo(true);
}
String wifiConnect() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println(WiFi.status());
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  return WiFi.localIP().toString().c_str();
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
  minitel.attributs(FOND_NORMAL);
  minitel.attributs(CARACTERE_NOIR);
  for (int i = 0; i <= (y2 - y1); i++) {
    minitel.moveCursorXY(1, (y2 - i));
    minitel.writeByte(0x20);
    minitel.repeat(39);
  }
  minitel.cursor();
}