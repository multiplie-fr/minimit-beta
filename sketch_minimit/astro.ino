void setupAstro() {
  minitel.pageMode();
  wifiConnect();
  afficheSommaireAstro();
}
void afficheSommaireAstro() {
  currentEcran = "SOMMAIRE";
  minitel.newScreen();
  afficheRemoteVDT("astro/astro.vdt", 0, 0);
}
void lectureChampAstro(int x, int y, int longueurchamp) {
  if (currentEcran == "SOMMAIRE") {
    champVide(x, y, longueurchamp);
  }
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
      case CONNEXION:
      Serial.println("CONNEXION");
      break;
      case CONNEXION_FIN:
        Serial.println("connnefinastro");
        interruption=true;
        fin=true;      
        userInput = "";
        userInputLength = 0;
        minitel.connexion(false);
        displayMire();
      break;
      case ENVOI:
        fin = true;
        if (userInputLength == 3) {
          retrieveDatasASTRO();
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
      case REPETITION:
        {
        // String page = (const char*)myObject["currentPage"];
         if(currentEcran=="SOMMAIRE"){
          champVide(x, y, longueurchamp);
          userInput = "";
          userInputLength = 0;
          }
         else {
          afficheRemoteVDT("astro/bandeau-astro.vdt", 0, 0);
          afficheDatasASTRO();
         }
        }
        break;
      case SOMMAIRE:
      case RETOUR:
        {
          if (currentEcran=="PAGE")
          {
            afficheSommaireAstro();
            userInput = "";
            userInputLength = 0;
        }
        }
        break;
    }
  }
}


void afficheDatasASTRO() {
  currentEcran = "PAGE";
  minitel.noCursor();
  effacementEcran(6, 22,CARACTERE_BLEU,FOND_BLEU);
  effacementEcran(24, 24,CARACTERE_BLEU,FOND_BLEU);
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
  posY++;posY++;
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
  Serial.print(userInput);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("astro/getjson.php?signe=") + userInput;
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("");
      Serial.print(payload);
      Serial.println("");
      myObject = JSON.parse(payload);
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      afficheDatasASTRO();
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}