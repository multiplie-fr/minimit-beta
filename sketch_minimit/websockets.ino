WebSocketsClient webSocket;

void setupWS() {
  afficheInterfaceWS();
  wifiConnect();
  ligneZero("Recherche...");
  retrieveDatasWS();
  ligneZero(" ");
  afficheListeWS();
  minitel.cursor();
}
void afficheInterfaceWS(){
  currentEcran = "SOMMAIRE";
  minitel.pageMode();
  minitel.newScreen();
  minitel.noCursor();
  minitel.attributs(CARACTERE_BLANC);
  minitel.newXY(1,1);
  minitel.print("*");
  minitel.repeat(39);
  minitel.newXY(1, 2);
  minitel.print("SERVICES WEBSOCKETS");
  minitel.newXY(1,3);
  minitel.print("*");
  minitel.repeat(39);
}
void afficheListeWS(){
  Serial.println(myObject);
  Serial.println("");
  JSONVar entries = myObject["entries"];
  int n = entries.length();
  int posY = 5;
  for (int i = 0; i<n; i++){
      minitel.newXY(1, posY);
      minitel.print(String(i+1));
      minitel.newXY(4,posY);
      Serial.println(entries[i]["name"]);
      minitel.print((const char*)entries[i]["name"]);
      posY ++;
  }
}
void backtoListe(){
  ligneZero(".                                      ");
  minitel.connexion(false);
  minitel.echo(true);
  currentEcran= "SOMMAIRE";
  minitel.newScreen();
  afficheInterfaceWS();
  afficheListeWS();
}
void loopWS() {
  // Websocket -> Minitel
  webSocket.loop();
 // Minitel -> Websocket
  uint32_t key = minitel.getKeyCode(false);
  if (key != 0) {
    // Serial.println("thekey");
    // Serial.println(key);
    if(key==4953)
    {
      backtoListe();
      return;
    }
   // prepare data to send over websocket
    uint8_t payload[4];
    size_t len = 0;
    for (len = 0; key != 0 && len < 4; len++) {
      payload[3-len] = uint8_t(key);
      key = key >> 8;
    }
    webSocket.sendTXT(payload+4-len, len);
  }
}
void webSocketEvent(WStype_t type, uint8_t * payload, size_t len) {
  if (currentEcran=="SOMMAIRE"){
    return;
  }
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("disconnected");
      webSocket.disconnect();
      backtoListe();
      break;
      
    case WStype_CONNECTED:
       break;
      
    case WStype_TEXT:
      if (len > 0) {
        for (size_t i = 0; i < len; i++) {
          minitel.writeByte(payload[i]);
        }
      }
      break;
  }
}
void connectToWS(String s){
  int num = s.toInt()-1;
  JSONVar myService = myObject["entries"][num];
  Serial.println(myService);
  const char* host = (const char*)myService["host"];
  int port = (int)myService["port"];
  const char* path = (const char*)myService["path"];
  bool ssl = (bool)myService["port"];;
  int ping_ms = (int)myService["ping_ms"];
  const char* protocol = (const char*)myService["protocol"];;
  currentEcran = "WSON";
  minitel.echo(false);
  if (protocol[0] == '\0') {
    if (ssl){webSocket.beginSSL(host, port, path);}
    else {webSocket.begin(host, port, path);}
  }
  else {
     if (ssl) webSocket.beginSSL(host, port, path, protocol);
    else webSocket.begin(host, port, path, protocol);
  }
  
  webSocket.onEvent(webSocketEvent);
   if (ping_ms != 0) {
    // start heartbeat (optional)
    // ping server every ping_ms
    // expect pong from server within 3000 ms
    // consider connection disconnected if pong is not received 2 times
    webSocket.enableHeartbeat(ping_ms, 3000, 2);
  }
}
void loopTelnet(){
  if (currentEcran=="WSON"){
    loopWS();
  }
  else{
    lectureChampWS(30, 24, 2);
  }
}
void lectureChampWS(int x, int y, int longueurchamp) {
  champVide(x, y, longueurchamp);
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (userInputLength < longueurchamp) {
        userInputLength++;
        userInput += char(touche);
        Serial.print(userInput);
      }
    }
    switch (touche) {
      case CONNEXION_FIN:
        fin=true;      
        userInput = "";
        userInputLength = 0;
        return;
        break;
      break;
      case ENVOI:
        fin = true;
        if (userInputLength > 0) {
          Serial.println("userInput");
          Serial.println(userInput);
          connectToWS(userInput);
          userInput = "";
          userInputLength = 0;
        }
        break;
      case ANNULATION:
        champVide(x, y, longueurchamp);
        userInput = "";
        userInputLength = 0;
        break;
      case CORRECTION:
        {
          int nbCaracteres = userInput.length();
          if (nbCaracteres > 0) {
            minitel.moveCursorLeft(1);
            minitel.print(".");
            minitel.attributs(CARACTERE_BLANC);
            minitel.moveCursorLeft(1);
            userInput = userInput.substring(0, userInput.length() - 1);
            userInputLength--;
            Serial.println(userInput);
          }
        }
        break;
    }
  }
}
void retrieveDatasWS() {
  Serial.println("retrieveDatasWS");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("websockets/getjson.php");
    Serial.println(serverPath);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      myObject["ws"] = JSON.parse(payload);
      Serial.println(myObject);
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}