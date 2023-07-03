// Fortune Cookie


// setup
void setupCookies() {
  minitel.newScreen();
  wifiConnect();
  afficheRemoteVDT("fc_accueil.vdt", 0, 0);
  FC_retrieveDatas();
  minitel.noCursor();
  minitel.echo(false);
}

// loop
void loopCookies() {

  while (1) {

	  wait_for_user_action();

	  switch (touche) {
	      case CONNEXION_FIN:
        case SOMMAIRE:
        return;
	      break;
      case ENVOI:
        affichePhrase();
        break;

    }
  }
}

// Fortune Cookie stuff
void affichePhrase(){
      afficheRemoteVDT("fortunecookie_phrase.vdt", 0, 0);
      int current=myObject["current"];
      JSONVar cookietexte = myObject["phrases"]["phrases"][current];
      Serial.println(cookietexte);
      int nbLines = cookietexte.length();
      minitel.attributs(CARACTERE_NOIR);
      minitel.attributs(FOND_BLANC);
      int posY = 15;
      minitel.moveCursorXY(6, posY);
      for (int j = 0; j < nbLines; j++) {
        minitel.moveCursorXY(6, posY);
        minitel.print((const char*)cookietexte[j]);
        posY++;
      }
    if (current >= (int) myObject["nbPhrases"] - 1) {
      Serial.println("ici zéro");
        current = 0;
      } else {
        Serial.println("ici ++");
        current++;
      }
      myObject["current"] = current;
}
void FC_retrieveDatas() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("fortunecookies/getjsonfull.php");
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      JSONVar myDatas = JSON.parse(payload);
      if (JSON.typeof(myDatas) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      myObject["phrases"] = myDatas;
      myObject["current"] = 0;
      myObject["nbPhrases"] = (int) myObject["phrases"]["phrases"].length();
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}