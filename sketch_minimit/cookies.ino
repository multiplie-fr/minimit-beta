// Fortune Cookie


// setup
void setupCookies() {
  minitel.newScreen();
  wifiConnect();
  afficheRemoteVDT("fc_accueil.vdt", 0, 0);
  minitel.noCursor();
  minitel.echo(false);
}

// loop
void loopCookies() {

  while (1) {

	  wait_for_user_action();

	  switch (touche) {
	      case CONNEXION_FIN:
        minitel.connexion(false);
		  	return;
	      break;
      case ENVOI:
        FC_retrieveDatas();
        break;

    }
  }
}

// Fortune Cookie stuff
void FC_retrieveDatas() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("fortunecookies/getjson.php");
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
     JSONVar myObject = JSON.parse(payload);
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      afficheRemoteVDT("fc_phrase.vdt", 0, 0);
      JSONVar cookietexte = myObject["texte"];
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
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}