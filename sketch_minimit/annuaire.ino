void setupAnnuaire() {
  initAnnuaire();
  afficheAnnuaire();
}
void initAnnuaire() {
  minitel.pageMode();
  wifiConnect();
  myObject["currentLine"] = (int)0;
  char json[] = "[[13, 5, 27],[13, 7, 27],[13, 10, 27]]";
  JSONVar coords = JSON.parse(json);
  myObject["coords"] = coords;
  char jsond[] = "[\"\", \"\", \"\"]";
  JSONVar input = JSON.parse(jsond);
  myObject["input"] = input;
  myObject["currentPage"] = (int)0;
  currentEcran = "RECHERCHE";
  Serial.println(myObject);
  //
}
void afficheAnnuaire() {
  minitel.noCursor();
  String vdt = "0c,0e,1b,54,20,20,1b,5a,60,78,7c,74,1b,59,1f,41,4d,52,45,43,48,45,52,43,48,45,0d,0a,0e,1b,54,20,1b,5a,78,5f,27,2b,2f,1b,59,1f,42,4d,50,41,52,20,4e,4f,4d,0d,0a,0e,1b,5a,1b,54,68,3f,21,1b,59,1b,46,31,12,42,1b,40,1b,50,20,1f,43,4d,4f,55,20,50,41,52,20,52,55,42,52,49,51,55,45,0d,0a,0e,1b,5a,1b,54,5f,7d,1b,59,20,1b,46,24,12,42,0f,1b,40,1b,5d,60,12,61,0e,1b,47,1b,5a,1b,54,2b,27,1b,59,20,1b,46,21,12,42,1b,47,1b,50,20,20,0f,1b,42,4e,4f,4d,3a,1f,47,44,1b,42,52,55,42,52,49,51,55,45,3a,1f,4a,44,1b,42,4c,4f,43,41,4c,49,54,45,3a,1f,31,32,66,61,63,75,6c,74,61,74,69,66,0d,0a,1b,42,44,45,50,41,52,54,45,4d,45,4e,54,3a,1f,4e,45,1b,42,41,44,52,45,53,53,45,3a,1f,4f,46,1b,42,50,52,45,4e,4f,4d,3a,0d,0a,1b,44,60,12,67,1f,51,4d,1b,42,43,6f,6d,6d,65,6e,74,20,66,6f,72,6d,75,6c,65,72,20,12,57,6c,61,20,64,65,6d,61,6e,64,65,20,12,47,16,2e,1b,5a,1b,50,20,1b,5d,20,47,55,49,44,45,20,20,1f,53,4d,1b,44,60,12,5b,1f,54,4d,1b,42,4c,65,73,20,73,65,72,76,69,63,65,73,20,12,5b,65,74,20,6c,65,20,74,61,72,69,66,20,12,46,16,2e,1b,5a,1b,50,20,1b,5d,53,4f,4d,4d,41,49,52,45,1f,56,4d,1b,44,60,12,5b,1f,57,4d,4e,55,4d,45,52,4f,53,20,44,27,55,52,47,45,4e,43,45,20,1b,42,16,2e,1b,5a,1b,50,20,1b,5d,20,52,45,54,4f,55,52,20,1f,58,4d,1b,44,60,12,5b,1f,45,4d,1b,46,2e,12,5b,1f,46,4d,1b,46,2e,12,5a,1f,47,4d,1b,46,2e,12,5b,1f,48,4d,1b,46,2e,12,5b,1f,49,4d,1b,46,2e,12,5a,1f,4a,4d,1b,46,2e,12,5b,1f,4b,4d,1b,46,2e,12,5a,1f,4d,4d,1b,46,2e,12,5a,1f,4e,4d,1b,46,2e,12,5a,1f,4f,4d,1b,46,2e,12,5a,14,1f,45,4d,11,1b";
  checkScreen(vdt, 0, 0);
  minitel.echo(false);
  minitel.textMode();
  afficheAnnuaireChampsSaisie();
  myObject["currentLine"] = 0;
  minitel.echo(true);
}

void afficheAnnuaireChampsSaisie() {
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
void afficheResultats() {
  currentEcran = "RESULTATS";
  minitel.noCursor();
  JSONVar coords = myObject["myDatas"]["root"]["coords"];
  int cl = coords.length();
  Serial.println("nbage");
  Serial.println(ceil(ceil(float(cl) / 4)));
  int currentPage = myObject["currentPage"];
  if (currentPage == 0) {
    minitel.newScreen();
    minitel.newXY(1, 1);
    minitel.print("RESULTATS DE VOTRE RECHERCHE");
    minitel.newXY(1, 2);
    minitel.print(String(cl));
    minitel.moveCursorRight(1);
    minitel.print("réponses");
    afficheNavAnnuaireSuite();
  } else {
    effacementEcran(3, 22, CARACTERE_NOIR, FOND_NOIR);
    minitel.noCursor();
  }

  int posY = 3;
  for (int i = 0 + (currentPage * 4); i < (4 + (currentPage * 4)); i++) {
    if (i >= cl) return;
    minitel.newXY(1, posY);
    minitel.writeByte(0x60);
    minitel.repeat(39);
    minitel.newXY(1, posY + 1);
    JSONVar coord = coords[i];
    minitel.print((const char*)coord[0]);
    minitel.newXY(1, posY + 2);
    minitel.print((const char*)coord[1]);
    minitel.newXY(1, posY + 3);
    minitel.print((const char*)coord[2]);
    minitel.moveCursorRight(1);
    minitel.print((const char*)coord[3]);
    minitel.newXY(1, posY + 4);
    minitel.print((const char*)coord[4]);
    posY += 5;
  }
}
void afficheNavAnnuaireSuite() {
  minitel.moveCursorXY(33, 24);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(DEBUT_LIGNAGE);
  minitel.attributs(INVERSION_FOND);
  minitel.writeByte(0x7C);
  minitel.print("SUITE ");
  minitel.attributs(FOND_NORMAL);
}
void afficheNavAnnuaireRetour() {
  minitel.moveCursorXY(26, 24);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(DEBUT_LIGNAGE);
  minitel.writeByte(0x20);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" RETOUR");
  minitel.attributs(FOND_NORMAL);
}
void gereTouches() {
}

void lectureChampAnnuaire() {
  touche = minitel.getKeyCode();
  if ((touche != 0)
      && (touche != CONNEXION_FIN)
      && (touche != SOMMAIRE)
      && (touche != ANNULATION)
      && (touche != RETOUR)
      && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
    userInput += char(touche);
    myObject["input"][myObject["currentLine"]] = userInput;
    Serial.println(myObject["input"]);
  }
  int currentLine;
  switch (touche) {
    case CONNEXION_FIN:
        minitel.connexion(false);
        displayMire();
      break;
    case SUITE:
      {
        int currentLine = myObject["currentLine"];
        int currentPage = myObject["currentPage"];
        if (currentEcran == "RESULTATS") {
          currentPage++;
          myObject["currentPage"] = currentPage;
          afficheResultats();
        } else {
          if (currentLine < 2) {
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
          //formulaireConfigData[myObject["currentLine"]] = userInput;
          myObject["input"][myObject["currentLine"]] = userInput;
        }
      }
      break;
    case ENVOI:
      {
        retrieveDatasANNUAIRE("annuaire/getjson.php?s=", 1, 1);
        myObject["currentPage"] = (int)0;
        afficheResultats();
      }
      break;
    case SOMMAIRE:
      initAnnuaire();
      afficheAnnuaire();
      break;
    case GUIDE:
      String vdt = "0x14, 0x0c, 0x1f, 0x41, 0x45, 0x0e, 0x1b, 0x46, 0x58, 0x1b, 0x56, 0x1b, 0x40, 0x23, 0x1b, 0x50, 0x1b, 0x46, 0x30, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x1b, 0x47, 0x50, 0x40, 0x09, 0x30, 0x50, 0x09, 0x30, 0x50, 0x40, 0x09, 0x30, 0x54, 0x1f, 0x42, 0x43, 0x0e, 0x1b, 0x46, 0x40, 0x1b, 0x56, 0x1b, 0x40, 0x21, 0x20, 0x20, 0x20, 0x1b, 0x50, 0x1b, 0x46, 0x54, 0x09, 0x09, 0x09, 0x09, 0x09, 0x1b, 0x57, 0x1b, 0x40, 0x4a, 0x12, 0x42, 0x1b, 0x50, 0x1b, 0x47, 0x34, 0x1b, 0x57, 0x1b, 0x40, 0x4a, 0x4a, 0x1b, 0x50, 0x1b, 0x47, 0x34, 0x1b, 0x57, 0x1b, 0x40, 0x4a, 0x12, 0x42, 0x1b, 0x50, 0x1b, 0x47, 0x34, 0x1b, 0x57, 0x1b, 0x40, 0x4a, 0x1f, 0x43, 0x42, 0x0e, 0x1b, 0x46, 0x48, 0x1b, 0x56, 0x20, 0x20, 0x20, 0x20, 0x1b, 0x45, 0x40, 0x1b, 0x55, 0x1b, 0x46, 0x21, 0x09, 0x09, 0x09, 0x09, 0x09, 0x1b, 0x50, 0x1b, 0x47, 0x12, 0x4b, 0x22, 0x1f, 0x44, 0x42, 0x0e, 0x1b, 0x56, 0x20, 0x12, 0x43, 0x1b, 0x45, 0x58, 0x1b, 0x55, 0x1b, 0x40, 0x40, 0x34, 0x1f, 0x45, 0x42, 0x0e, 0x1b, 0x56, 0x20, 0x12, 0x42, 0x1b, 0x55, 0x1b, 0x46, 0x21, 0x1b, 0x40, 0x58, 0x1b, 0x50, 0x20, 0x1b, 0x45, 0x4a, 0x1f, 0x46, 0x42, 0x0e, 0x1b, 0x56, 0x1b, 0x40, 0x34, 0x20, 0x20, 0x1b, 0x55, 0x4a, 0x1b, 0x50, 0x20, 0x1b, 0x45, 0x40, 0x1b, 0x55, 0x1b, 0x40, 0x21, 0x1b, 0x50, 0x1b, 0x45, 0x54, 0x1f, 0x47, 0x43, 0x0e, 0x1b, 0x56, 0x1b, 0x40, 0x54, 0x20, 0x1b, 0x55, 0x4a, 0x1b, 0x50, 0x1b, 0x45, 0x58, 0x1b, 0x55, 0x20, 0x20, 0x20, 0x1b, 0x40, 0x22, 0x1b, 0x50, 0x1b, 0x45, 0x30, 0x1f, 0x48, 0x44, 0x0e, 0x1b, 0x46, 0x22, 0x1b, 0x55, 0x1b, 0x40, 0x30, 0x20, 0x20, 0x20, 0x20, 0x58, 0x1f, 0x49, 0x46, 0x0e, 0x1b, 0x55, 0x1b, 0x40, 0x54, 0x20, 0x40, 0x1b, 0x50, 0x1b, 0x45, 0x21, 0x1f, 0x4a, 0x47, 0x0e, 0x1b, 0x45, 0x22, 0x1f, 0x44, 0x4e, 0x1b, 0x46, 0x4c, 0x45, 0x20, 0x47, 0x55, 0x49, 0x44, 0x45, 0x20, 0x44, 0x45, 0x53, 0x20, 0x53, 0x45, 0x52, 0x56, 0x49, 0x43, 0x45, 0x53, 0x1f, 0x46, 0x4e, 0x1b, 0x45, 0x52, 0x65, 0x76, 0x69, 0x76, 0x65, 0x7a, 0x20, 0x6c, 0x27, 0x65, 0x78, 0x70, 0x19, 0x42, 0x65, 0x72, 0x69, 0x65, 0x6e, 0x63, 0x65, 0x20, 0x64, 0x75, 0x20, 0x1f, 0x47, 0x4e, 0x1b, 0x45, 0x6d, 0x69, 0x6e, 0x69, 0x74, 0x65, 0x6c, 0x20, 0x76, 0x69, 0x61, 0x20, 0x63, 0x65, 0x73, 0x20, 0x71, 0x75, 0x65, 0x6c, 0x71, 0x75, 0x65, 0x73, 0x1f, 0x48, 0x4e, 0x1b, 0x45, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x73, 0x2c, 0x20, 0x63, 0x6f, 0x6d, 0x6d, 0x65, 0x20, 0x64, 0x61, 0x6e, 0x73, 0x20, 0x6c, 0x65, 0x73, 0x20, 0x1f, 0x49, 0x4e, 0x1b, 0x45, 0x61, 0x6e, 0x6e, 0x19, 0x42, 0x65, 0x65, 0x73, 0x20, 0x38, 0x30, 0x2e, 0x1f, 0x4a, 0x41, 0x1b, 0x44, 0x60, 0x12, 0x67, 0x1f, 0x57, 0x41, 0x1b, 0x44, 0x60, 0x12, 0x69, 0x1f, 0x58, 0x41, 0x1b, 0x45, 0x54, 0x61, 0x70, 0x65, 0x7a, 0x20, 0x75, 0x6e, 0x20, 0x6e, 0x19, 0x30, 0x20, 0x6f, 0x75, 0x20, 0x73, 0x6f, 0x6e, 0x20, 0x6e, 0x6f, 0x6d, 0x20, 0x2e, 0x12, 0x48, 0x20, 0x1b, 0x47, 0x1b, 0x5d, 0x20, 0x45, 0x4e, 0x56, 0x4f, 0x49, 0x20, 0x1f, 0x4b, 0x43, 0x31, 0x20, 0x41, 0x4e, 0x4e, 0x55, 0x41, 0x49, 0x52, 0x45, 0x1f, 0x4c, 0x43, 0x32, 0x20, 0x41, 0x53, 0x54, 0x52, 0x4f, 0x1f, 0x4d, 0x43, 0x33, 0x20, 0x43, 0x4f, 0x55, 0x50, 0x4c, 0x45, 0x53, 0x50, 0x41, 0x52, 0x46, 0x41, 0x49, 0x54, 0x53, 0x1f, 0x4e, 0x43, 0x34, 0x20, 0x46, 0x4f, 0x52, 0x54, 0x55, 0x4e, 0x45, 0x1f, 0x4f, 0x43, 0x35, 0x20, 0x47, 0x41, 0x4c, 0x45, 0x52, 0x49, 0x45, 0x1f, 0x50, 0x43, 0x36, 0x20, 0x4c, 0x45, 0x4d, 0x4f, 0x4e, 0x44, 0x45, 0x1f, 0x51, 0x43, 0x37, 0x20, 0x4d, 0x45, 0x54, 0x45, 0x4f, 0x1f, 0x52, 0x43, 0x38, 0x20, 0x4e, 0x41, 0x42, 0x41, 0x5a, 0x54, 0x41, 0x47, 0x1f, 0x53, 0x43, 0x39, 0x20, 0x50, 0x45, 0x4e, 0x44, 0x55, 0x1f, 0x54, 0x42, 0x31, 0x30, 0x20, 0x50, 0x4f, 0x4e, 0x47, 0x1f, 0x55, 0x42, 0x31, 0x31, 0x20, 0x50, 0x50, 0x50, 0x20, 0x28, 0x70, 0x6c, 0x75, 0x69, 0x65, 0x20, 0x70, 0x61, 0x73, 0x20, 0x70, 0x6c, 0x75, 0x69, 0x65, 0x20, 0x3f, 0x29, 0x1f, 0x56, 0x42, 0x31, 0x32, 0x20, 0x54, 0x41, 0x52, 0x4f, 0x54";
  }
}
void retrieveDatasANNUAIRE(String phpFile, int displayMode, int articleItem) {
  if (WiFi.status() == WL_CONNECTED) {
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
      JSONVar myDatas = JSON.parse(payload);

      if (JSON.typeof(myDatas) == "undefined") {
        Serial.println(payload);
        Serial.println("Parsing input failed!");
        return;
      } else {
        myObject["myDatas"] = myDatas;
      }
    }


    Serial.print(myObject);
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}


