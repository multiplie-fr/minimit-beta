void setupGalerie() {
  minitel.pageMode();
  minitel.newScreen();
  wifiConnect();
  minitel.echo(false);
  myObject["current"] = 0;
  retrieveDatasGALERIE();
}
void lectureChampGalerie(int x, int y, int longueurchamp) {
  //champVide(x, y, longueurchamp);
  Serial.print("ggggg");
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    switch (touche) {
      case CONNEXION_FIN:
      fin=true;
      minitel.connexion(false);
      displayMire();
      case REPETITION:
        break;
      case SOMMAIRE:
        break;
  }
}
}
void loopGalerie() {
  delay(5000);
  int currentImage = myObject["current"];
  int nbImages = myObject["datas"]["root"].length();
  Serial.println("nbimages");
  Serial.println(nbImages);
  if(currentImage<(nbImages-1))
  {
    currentImage++;
  }
  else {
    currentImage=0;
  }
  myObject["current"]= currentImage; 
  afficheDatasGALERIE();
}


void afficheDatasGALERIE() {
  minitel.noCursor();
  int currentImage = myObject["current"];
  //currentImage++;
  myObject["current"]= currentImage;  
  String myImage = (const char*)myObject["datas"]["root"][currentImage];
  Serial.println(currentImage);
  Serial.println(myImage);
  afficheRemoteVDT("galerie/"+myImage, 0, 0);
  
}
void retrieveDatasGALERIE() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("galerie/getjson.php");
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      JSONVar myJSONObject = JSON.parse(payload);
      Serial.println("");
      Serial.print(payload);
      Serial.println("");
       myObject["datas"] = myJSONObject;
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      afficheDatasGALERIE();
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}