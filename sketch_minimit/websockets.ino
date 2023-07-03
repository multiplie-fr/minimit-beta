WebSocketsClient webSocket;

void setupWS(int x, int y, int first_ws_index) {
  //checkConnexion();
  if (WiFi.status() != WL_CONNECTED) return;
  retrieveDatasWS();
  afficheListeWS(x,y, first_ws_index);
  minitel.cursor();
}

void retrieveDatasWS() {
  Serial.println("retrieveDatasWS");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("websockets/getjson.php");
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      myObject = JSON.parse(payload);
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
void afficheListeWS(int first_x, int first_y, int first_ws_index){
  
  if (WiFi.status() != WL_CONNECTED) return;
  JSONVar entries = myObject["entries"];
  int n = entries.length();
  int posY = first_y;
  for (int i = 0; i<n; i++){
      minitel.newXY(first_x, posY);
      minitel.attributs(CARACTERE_VERT);
      minitel.print(String(i+first_ws_index));
      minitel.newXY(first_x+3,posY);
      minitel.attributs(CARACTERE_VERT);
      minitel.print((const char*)entries[i]["name"]);
      posY ++;
  }
}

void loopWS() {
  Serial.println("loopWS");
  while(1) {
    
    // Websocket -> Minitel
    webSocket.loop();
  // Minitel -> Websocket
    uint32_t key = minitel.getKeyCode(false);
    if (key != 0) {
      if(key==4953)
      {
        Serial.println("websockets 4953");
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
}
void webSocketEvent(WStype_t type, uint8_t * payload, size_t len) {
  if (currentEcran=="SOMMAIRE"){
    return;
  }
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("disconnected");
      webSocket.disconnect();
      return;
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
void connectToWS(int num){
  Serial.println("connectToWS");
  
  JSONVar myService = myObject["entries"][num];
  Serial.println(myService);
  const char* host = (const char*)myService["host"];
  int port = (int)myService["port"];
  const char* path = (const char*)myService["path"];
  bool ssl = (bool)myService["port"];;
  int ping_ms = (int)myService["ping_ms"];
  const char* protocol = (const char*)myService["protocol"];;
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
