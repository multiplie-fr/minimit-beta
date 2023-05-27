// ASTROLOGIE

// SETUP du service
void setupAstro() {
  Serial.println("setup Astro");
  initMinitelService();
  wifiConnect();
  afficheSommaireAstro();
}


// LOOP du service
void loopAstro() {
  Serial.println("loop Astro");
  champVide(22, 24, 3);
  while (1) {

    // Input
    wait_for_user_action();

    // Traitement de l'action utilisateur (maj des variables globales dans wait_for___)
    switch (touche) {

      // SI CONNEXION FIN on sort de la loop, on revient à la loop principale
      case CONNEXION_FIN:
        return;
        break;

      // SI ENVOI on affiche la page du signe
      case ENVOI:
        Serial.println("ENVOI " + userInput);
        retrieveDatasASTRO();
        afficheDatasASTRO();
        break;

      // SI RETOUR ON AFFICHE LE SOMMAIRE
      case RETOUR:
      if(currentEcran=="PAGE")
        {
          afficheSommaireAstro();
        }        
        break;

      case ANNULATION:
        Serial.println("ANNULATION");
        champVide(22, 24, 3);
        break;
    }
  }
}
// FIN LOOP

// Fonctions d'affichage et de traitement liées au service
void afficheDatasASTRO() {
  currentEcran = "PAGE";
  minitel.noCursor();
  effacementEcran(6, 22, CARACTERE_BLEU, FOND_BLEU);
  effacementEcran(24, 24, CARACTERE_BLEU, FOND_BLEU);
  minitel.textMode();
  minitel.newXY(31, 24);
  minitel.attributs(INVERSION_FOND);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print(" RETOUR ");
  minitel.attributs(FOND_NORMAL);
  int posY = 6;
  int posX = 2;
  JSONVar signe = myObject["root"]["signe"];
  minitel.newXY(posX, posY);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print((const char*)signe);
  minitel.print(" ");
  minitel.attributs(CARACTERE_NOIR);
  JSONVar date = myObject["root"]["date"];
  minitel.print((const char*)date);
  posY++;
  posY++;
  JSONVar articles = myObject["root"]["articles"];

  int nbarticles = articles.length();
  for (int i = 0; i < nbarticles; i++) {
    int nblignes = articles[i].length();
    for (int j = 0; j < nblignes; j++) {
      minitel.newXY(posX, posY);
      String textLigne = (const char*)articles[i][j];
      if (j == 0) {
        minitel.attributs(CARACTERE_BLANC);
      } else {
        minitel.attributs(CARACTERE_CYAN);
      }
      posY++;
      minitel.print(textLigne);
    }
    //posY++;
  }
  minitel.noCursor();
}
void retrieveDatasASTRO() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("astro/getjson.php?signe=") + userInput;
    // Your Domain name with URL path or IP address with path
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
void afficheSommaireAstro() {
  afficheRemoteVDT("astro/astro.vdt", 0, 0);
}