// ANNUAIRE

// SETUP du service
void setupAnnuaire() {
  Serial.println("setup Annuaire");
  initMinitelService();
  wifiConnect();
  initAnnuaire();
  afficheAnnuaire();
}


// LOOP du service
void loopAnnuaire() {
  Serial.println("loop Annuaire");
  while (1) {

    // Input
    wait_for_user_action();
    myObject["input"][myObject["currentLine"]] = userInput;
    // Traitement de l'action utilisateur (maj des variables globales dans wait_for___)
    switch (touche) {

      // SI CONNEXION FIN on sort de la loop, on revient à la loop principale
      case CONNEXION_FIN:
        Serial.println("CONNEXION_FIN");
        minitel.connexion(false);
        return;
        break;

      case SUITE:
        {
          int currentLine = myObject["currentLine"];
          int currentPage = myObject["currentPage"];
          int nbPages = myObject["nbPages"];
          if (currentEcran == "RESULTATS") {
            if (currentPage < nbPages - 1) {
              currentPage++;
              myObject["currentPage"] = currentPage;
              afficheResultats();
            }
          } else {
            if (currentLine < 5) {
              Serial.println(currentLine);
              currentLine += 1;
              JSONVar nextSaisie = myObject["input"][currentLine];
              userInput = (const char*)nextSaisie;
              int offsetX = strlen(nextSaisie);
              myObject["currentLine"] = currentLine;
              JSONVar coords = myObject["coords"][currentLine];
              int nx = coords[0];
              int ny = coords[1];
              minitel.moveCursorXY(nx + offsetX, ny);
            }
          }
        }
        break;

      case RETOUR:
        {
          int currentLine = myObject["currentLine"];
          int currentPage = myObject["currentPage"];
          if (currentEcran == "RESULTATS") {
            if (currentPage > 0) {
              currentPage--;
              myObject["currentPage"] = currentPage;
              afficheResultats();
            }
          } else {
            if (currentLine > 0) {
              userInput = "";
              currentLine -= 1;
              JSONVar previousSaisie = myObject["input"][currentLine];
              userInput = (const char*)previousSaisie;
              int offsetX = strlen(previousSaisie);
              myObject["currentLine"] = currentLine;
              JSONVar coords = myObject["coords"][currentLine];
              int nx = coords[0];
              int ny = coords[1];
              minitel.moveCursorXY(nx + offsetX, ny);
            }
          }
        }
        break;

      // SI ENVOI
      case ENVOI:
        retrieveDatasANNUAIRE("annuaire/getjson.php?s=");
        myObject["currentPage"] = (int)0;
        afficheResultats();
        break;

      case ANNULATION:
      {
        int currentLine = myObject["currentLine"];
        JSONVar coords = myObject["coords"][currentLine];
        myObject["input"][currentLine] = "";
        champVide(myObject["coords"][currentLine][0], myObject["coords"][currentLine][1], myObject["coords"][currentLine][2]);
        Serial.println(myObject["input"]);
      }
        break;

      case CORRECTION:
        {
          int nbCaracteres = userInput.length();
          Serial.println(nbCaracteres);
          if (nbCaracteres > 0) {
            minitel.moveCursorLeft(1);
            minitel.attributs(CARACTERE_BLEU);
            minitel.print(".");
            minitel.attributs(CARACTERE_BLANC);
            minitel.moveCursorLeft(1);
            userInput = userInput.substring(0, userInput.length() - 1);
            myObject["input"][myObject["currentLine"]] = userInput;
          }
        }
        break;

      case SOMMAIRE:
        initAnnuaire();
        afficheAnnuaire();
        break;
    }
  }
}
// FIN LOOP

// Fonctions d'affichage et de traitement liées au service
void retrieveDatasANNUAIRE(String phpFile) {
  //minitel.aiguillage(false, CODE_EMISSION_CLAVIER, CODE_RECEPTION_CLAVIER);
  if (WiFi.status() == WL_CONNECTED) {
    ligneZero("Recherche...");
    HTTPClient http;
    String serverPath;
    //
    JSONVar input = myObject["input"];
    int nc = input.length();
    String requete = "";
    for (int i = 0; i < nc; i++) {
      requete += (const char*)input[i] + String(" ");
    }
    requete.trim();
    requete.replace(" ", "+");
    serverPath = serverName + String(phpFile) + requete;
    Serial.println(serverPath);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("payload");
      Serial.println(payload);
      // Free resources
      http.end();
      JSONVar myDatas = JSON.parse(payload);
      minitel.aiguillage(true, CODE_EMISSION_CLAVIER, CODE_RECEPTION_CLAVIER);
      if (JSON.typeof(myDatas) == "undefined") {
        Serial.println(payload);
        Serial.println("Parsing input failed!");
        return;
      } else {
        myObject["myDatas"] = myDatas;
        JSONVar coords = myObject["myDatas"]["root"]["coords"];
        int cl = coords.length();
        int nbPages = ceil(ceil(float(cl) / 4));
        myObject["nbPages"] = nbPages;
      }
    }
    ligneZero(" ");
  }
}

void initAnnuaire() {
  minitel.pageMode();
  wifiConnect();
  myObject["currentLine"] = (int)0;
  char json[] = "[[13, 5, 28],[13, 7, 28],[13, 10, 28],[13, 13, 28],[13, 14, 28],[13, 15, 28]]";
  JSONVar coords = JSON.parse(json);
  myObject["coords"] = coords;
  char jsond[] = "[\"\", \"\", \"\", \"\", \"\", \"\"]";
  JSONVar input = JSON.parse(jsond);
  myObject["input"] = input;
  myObject["currentPage"] = (int)0;
  currentEcran = "RECHERCHE";
  Serial.println(myObject);
  //
}
void afficheAnnuaireChampsSaisie() {
  minitel.noCursor();
  JSONVar coords = myObject["coords"];
  JSONVar input = myObject["input"];
  int nc = coords.length();
  for (int i = 0; i < nc; i++) {
    champVide(coords[i][0], coords[i][1], coords[i][2]);
    minitel.print((const char*)input[i]);
  }
  minitel.moveCursorXY(coords[0][0], coords[0][1]);
  minitel.cursor();
  minitel.echo(true);
}
void afficheAnnuaire() {
  minitel.noCursor();
  String vdt = "0c,14,0e,1b,54,20,20,1b,5a,60,78,7c,74,1b,59,1f,41,4d,52,45,43,48,45,52,43,48,45,0d,0a,0e,1b,54,20,1b,5a,78,5f,27,2b,2f,1b,59,1f,42,4d,50,41,52,20,4e,4f,4d,0d,0a,0e,1b,5a,1b,54,68,3f,21,1b,59,1b,46,31,12,42,1b,40,1b,50,20,1f,43,4d,4f,55,20,50,41,52,20,52,55,42,52,49,51,55,45,0d,0a,0e,1b,5a,1b,54,5f,7d,1b,59,20,1b,46,24,12,42,0f,1b,40,1b,5d,60,12,61,0e,1b,47,1b,5a,1b,54,2b,27,1b,59,20,1b,46,21,12,42,1b,47,1b,50,20,20,0f,1b,42,4e,4f,4d,3a,1f,47,44,1b,42,52,55,42,52,49,51,55,45,3a,1f,4a,44,1b,42,4c,4f,43,41,4c,49,54,45,3a,1f,4c,41,66,61,63,75,6c,74,61,74,69,66,0d,0a,1b,42,44,45,50,41,52,54,45,4d,45,4e,54,3a,1f,4e,45,1b,42,41,44,52,45,53,53,45,3a,1f,4f,46,1b,42,50,52,45,4e,4f,4d,3a,0d,0a,1b,44,60,12,67,1f,51,4d,1b,42,50,6c,75,73,20,62,65,73,6f,69,6e,20,64,27,61,6c,6c,65,72,20,19,41,61,20,6c,61,1f,52,4d,1b,42,70,6f,73,74,65,20,70,6f,75,72,20,63,6f,6e,73,75,6c,74,65,72,20,6c,65,1f,53,4d,1b,42,62,6f,74,74,69,6e,2c,20,72,65,74,72,6f,75,76,65,7a,20,69,63,69,20,6c,65,73,1f,54,4d,1b,42,6e,75,6d,19,42,65,72,6f,73,20,64,65,20,74,19,42,65,6c,19,42,65,70,68,6f,6e,65,20,66,69,78,65,1f,55,4d,1b,44,60,12,5b,1f,56,4d,1b,42,4c,65,73,20,64,6f,6e,6e,19,42,65,65,73,20,70,72,6f,76,69,65,6e,6e,65,6e,74,20,64,65,1f,57,4d,68,74,74,70,73,3a,2f,2f,61,6e,6e,75,61,69,72,65,2e,31,31,38,37,31,32,2e,66,72,20,12,5a,1f,58,4d,1b,44,60,12,5b,14";
  checkScreen(vdt, 0, 0);
  minitel.echo(false);
  minitel.textMode();
  afficheAnnuaireChampsSaisie();
  myObject["currentLine"] = 0;
  minitel.echo(true);
}
void afficheResultats() {
  minitel.noCursor();

  JSONVar iscaptcha = myObject["myDatas"]["root"]["captcha"][0];
  String captchastatus = (const char*)iscaptcha;
  if (captchastatus == "YES") {
    ligneZeroSafe("Service indisponible");
    delay(3000);
    ligneZeroSafe(" ");
    minitel.cursor();
    return;
  }


  JSONVar coords = myObject["myDatas"]["root"]["coords"];
  int cl = coords.length();
  if (cl == 0) {
    ligneZeroSafe("Pas de réponse à votre demande");
    delay(3000);
    ligneZeroSafe(" ");
    minitel.cursor();
    return;
  }
  currentEcran = "RESULTATS";

  int nbPages = myObject["nbPages"];
  int currentPage = myObject["currentPage"];
  if (currentPage == 0) {
    minitel.newScreen();
    minitel.newXY(1, 1);
    minitel.print("RESULTATS DE VOTRE RECHERCHE");
    minitel.newXY(1, 2);
    minitel.print(String(cl));
    minitel.moveCursorRight(1);
    minitel.print("réponses");
    minitel.newXY(1, 3);
    minitel.attributs(CARACTERE_BLEU);
    minitel.writeByte(0x60);
    minitel.repeat(39);
    minitel.moveCursorXY(1, 24);
    minitel.attributs(CARACTERE_CYAN);
    minitel.print("Autre recherche ");
    minitel.attributs(INVERSION_FOND);
    minitel.print("SOMMAIRE");
    minitel.attributs(FOND_NORMAL);
    afficheNavAnnuaireSuite();
  } else {
    effacementEcran(4, 22, CARACTERE_NOIR, FOND_NOIR);
    minitel.noCursor();
    if ((currentPage == 1 && currentPage < nbPages - 1) || (currentPage == nbPages - 2 && currentPage > 0)) {
      afficheNavAnnuaireSuiteRetour();
    } else {
      if (currentPage == nbPages - 1)
        afficheNavAnnuaireRetour();
    }
  }
  minitel.newXY(35, 3);
  minitel.print(String(currentPage + 1));
  minitel.print("/");
  minitel.print(String(nbPages));

  int posY = 4;
  for (int i = 0 + (currentPage * 4); i < (4 + (currentPage * 4)); i++) {
    if (i >= cl) return;
    minitel.newXY(1, posY);
    minitel.attributs(CARACTERE_BLANC);
    JSONVar coord = coords[i];
    minitel.print((const char*)coord[0]);
    minitel.newXY(1, posY + 1);
    minitel.print((const char*)coord[1]);
    minitel.newXY(1, posY + 2);
    minitel.print((const char*)coord[2]);
    minitel.moveCursorRight(1);
    minitel.print((const char*)coord[3]);
    minitel.newXY(1, posY + 3);
    minitel.print((const char*)coord[4]);
    minitel.newXY(1, posY + 4);
    // Serial.println("");
    // Serial.print("mod");
    // Serial.print(i%4);
    minitel.attributs(CARACTERE_BLEU);
    minitel.writeByte(0x60);
    minitel.repeat(39);
    posY += 5;
  }
}
void afficheNavAnnuaireSuite() {
  minitel.moveCursorXY(34, 24);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" SUITE ");
  minitel.attributs(FOND_NORMAL);
}
void afficheNavAnnuaireRetour() {
  minitel.moveCursorXY(27, 24);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" RETOUR ");
  minitel.attributs(FOND_NORMAL);
  minitel.print(" ");
  minitel.repeat(5);
}
void afficheNavAnnuaireSuiteRetour() {
  minitel.moveCursorXY(35, 24);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(INVERSION_FOND);
  minitel.print("SUITE ");
  minitel.attributs(FOND_NORMAL);
  minitel.moveCursorXY(26, 24);
  minitel.attributs(CARACTERE_CYAN);
  //minitel.attributs(DEBUT_LIGNAGE);
  minitel.writeByte(0x20);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" RETOUR");
  minitel.writeByte(0x7C);
  minitel.attributs(FOND_NORMAL);
}