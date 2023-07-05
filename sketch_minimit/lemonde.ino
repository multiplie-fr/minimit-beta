// LEMONDE
// SETUP
void setupLeMonde() {
  minitel.echo(false);
  Serial.println("currentServiceinemonde");
  Serial.println(currentService);
  minitel.pageMode();
  minitel.newScreen();
  minitel.textMode();
  wifiConnect();
  initLEMONDE(0);
  afficheUNELEMONDE(0);
  myObject["wip"] = false;
}

// LOOP
void loopLeMonde() {
  while (1) {
    champVide(13, 23, 3);


    // Input
    wait_for_user_action();

    // Processing ....
    switch (touche) {
      case GUIDE:
        guideLeMonde();
        break;
      case CONNEXION_FIN:
        return;
        break;
      case SUITE:
        {
          Serial.println("suite");
          int displayMode = myObject["displayMode"];
          switch (displayMode) {
            case 1:
              {
                suiteListe();
              }
              break;
            case 2:
              {
                suiteArticle();
              }
              break;
          }
        }

        break;
      case RETOUR:
        {
          int displayMode = myObject["displayMode"];
          switch (displayMode) {
            case 1:
              {
                if ((int)myObject["currentPage"] == 0) {
                  break;
                }
                retourListe();
              }
              break;
            case 2:
              {
                retourArticle();
              }
              break;
          }
        }

        break;


      case ENVOI:
        userInput.toUpperCase();
        if (userInputLength > 0) {
          if (!isValidNumber(userInput)) {
            if (userInput == "BRE") {
              afficheBRELEMONDE();
              myObject["format"] = "BRE";
              updateBandeauLEMONDE("BRE");
              myObject["currentPage"] = (int)0;
              myObject["displayMode"] = (int)1;
            }
            if (userInput == "UNE") {
              myObject["format"] = "UNE";
              updateBandeauLEMONDE("UNE");
              displayNavList();
              myObject["currentPage"] = (int)0;
              myObject["displayMode"] = (int)1;
              retrieveDatasLEMONDE("lemonde/getjson.php?mode=liste&format=une", 1, 0);
              effacementEcran(5, 21, CARACTERE_NOIR, FOND_NOIR);
              afficheDatasLEMONDE(0);
            }
          }

          else {
            int userInputInt = userInput.toInt();
            userInputInt--;
            retrieveDatasLEMONDE("lemonde/getjson.php?mode=article&item=" + String(userInputInt), 2, userInputInt);
            afficheDatasLEMONDE(0);
          }
          userInput = "";
        }
        break;
      case SOMMAIRE:
        {
          Serial.println("sommairelemn");

          if (currentEcran == "SOMMAIRE") {
            return;
            break;
          }

          else {
            userInput = "";
            userInputLength = 0;
            sommaireLEMONDE();
          }
        }
        break;
      case ANNULATION:
        userInput = "";
        userInputLength = 0;
        champVide(13, 23, 3);
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
          }
        }
        break;
    }
  }
}

// VARIOUS LE MONDE
void initLEMONDE(int currentPage) {
  minitel.newScreen();
  myObject["currentPage"] = currentPage;
  myObject["currentArticle"] = (int)0;
  myObject["displayMode"] = (int)1;
  myObject["wip"] = true;
  //Displaymode
  //1 : liste
  //2 : fiche
  myObject["format"] = "UNE";
  ///UNE ou BRE
  displayBandeau();
  userInput = "";
  userInputLength = 0;
}
void resetPagesLEMONDE() {
  myObject["currentPage"] = (int)0;
  myObject["displayMode"] = (int)1;
}
void afficheUNELEMONDE(int currentPage) {
  currentEcran = "SOMMAIRE";
  myObject["format"] = "UNE";
  updateBandeauLEMONDE("UNE");
  displayNavList();
  retrieveDatasLEMONDE("lemonde/getjson.php?current=", 1, 1);
  afficheDatasLEMONDE(currentPage);
}
void afficheBRELEMONDE() {
  myObject["format"] = "BRE";
  minitel.newScreen();
  displayBandeau();
  updateBandeauLEMONDE("BRE");
  displayNavList();
  retrieveDatasLEMONDE("lemonde/getjson.php?mode=liste&format=bre", 1, 0);
  afficheDatasLEMONDE(0);
}

void repetitionLEMONDE() {
  int currentPage = myObject["currentPage"];
  JSONVar format = myObject["format"];
  String sformat = (const char*)format;
  int displayMode = myObject["displayMode"];
  if (sformat == "UNE") {
    if (displayMode == 1) {
      afficheUNELEMONDE(currentPage);
    }
  }
}
void sommaireLEMONDE() {
  int currentArticle = myObject["currentArticle"];
  int logicalPage = getLogicalPage(currentArticle);
  initLEMONDE(logicalPage);
  afficheUNELEMONDE(logicalPage);
}
int getLogicalPage(int currentArticle) {
  JSONVar myDatas = myObject["myDatas"];
  JSONVar myarrayPagesOffset = myDatas["root"]["pages"];
  int n = myarrayPagesOffset.length();
  for (int i = 0; i < n; i++) {
    int pageoffset = myarrayPagesOffset[i];
    if (currentArticle < pageoffset) {
      return i - 1;
    }
  }
  return 0;
}
void displayBandeau() {
  String vdt = "14,0c,1f,41,41,0e,1b,54,20,38,24,20,2c,38,38,30,20,12,43,28,30,20,12,59,1f,42,41,0e,1b,54,20,1b,57,1b,44,4a,1b,54,1b,47,48,1b,57,1b,44,46,1b,54,20,1b,57,4a,12,42,49,1b,54,1b,47,34,1b,57,1b,44,48,1b,54,1b,47,34,1b,57,1b,44,49,4a,29,1b,54,1b,47,24,20,12,57,1f,43,41,0e,1b,54,40,45,2a,38,40,25,1b,57,1b,44,4a,1b,54,1b,47,45,45,25,25,1b,57,1b,44,4a,1b,54,1b,47,45,25,45,24,20,12,57,1f,44,41,0e,1b,44,23,12,67";
  checkScreen(vdt, 0, 0);
  minitel.textMode();
}
void displayNavArticle(int previousDisplayMode) {
  if (previousDisplayMode == 1) {
    minitel.newXY(28, 23);
    minitel.clearLineFromCursor();
    delay(10);
    minitel.moveCursorXY(30, 23);
    minitel.attributs(FOND_NORMAL);
    minitel.attributs(CARACTERE_BLANC);
    minitel.attributs(DEBUT_LIGNAGE);
    minitel.writeByte(0x20);
    minitel.attributs(INVERSION_FOND);
    minitel.print(" SOMMAIRE ");
    minitel.attributs(FOND_NORMAL);
  }
}
void displayNavList() {
  JSONVar format = myObject["format"];
  String sformat = (const char*)format;
  effacementEcran(23, 24, CARACTERE_NOIR, FOND_NOIR);
  minitel.attributs(CARACTERE_BLEU);
  minitel.moveCursorXY(0, 22);
  minitel.writeByte(0x60);
  minitel.repeat(39);
  minitel.moveCursorXY(1, 23);
  minitel.attributs(CARACTERE_MAGENTA);
  minitel.print("Votre choix ...");
  minitel.attributs(DEBUT_LIGNAGE);
  minitel.writeByte(0x20);
  minitel.attributs(INVERSION_FOND);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print(" ENVOI ");
  minitel.attributs(FOND_NORMAL);
  if (sformat == "UNE") {
    minitel.newXY(27, 23);
    minitel.print(" Infos");
    minitel.attributs(DEBUT_LIGNAGE);
    minitel.writeByte(0x20);
    minitel.attributs(INVERSION_FOND);
    minitel.print(" GUIDE ");
    minitel.attributs(FOND_NORMAL);
  }
  minitel.moveCursorXY(2, 24);
  minitel.attributs(CARACTERE_MAGENTA);
  if (sformat == "UNE") {
    minitel.print("Les brèves ");
    minitel.attributs(CARACTERE_BLANC);
    minitel.print("BRE");
    minitel.attributs(DEBUT_LIGNAGE);
    minitel.writeByte(0x20);
    minitel.attributs(INVERSION_FOND);
    minitel.attributs(CARACTERE_BLANC);
  } else {
    minitel.print("  A la Une ");
    minitel.attributs(CARACTERE_BLANC);
    minitel.print("UNE");
    minitel.attributs(DEBUT_LIGNAGE);
    minitel.writeByte(0x20);
    minitel.attributs(INVERSION_FOND);
    minitel.attributs(CARACTERE_BLANC);
  }

  minitel.print(" ENVOI ");
  minitel.attributs(FOND_NORMAL);

  minitel.moveCursorXY(26, 24);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(DEBUT_LIGNAGE);
  minitel.writeByte(0x20);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" RETOUR");
  minitel.writeByte(0x7C);
  minitel.print("SUITE ");
  minitel.attributs(FOND_NORMAL);
}
boolean isValidNumber(String str) {
  for (byte i = 0; i < str.length(); i++) {
    if (isDigit(str.charAt(i))) return true;
  }
  return false;
}
void suiteListe() {
  int currentPage = myObject["currentPage"];
  int nbPages = myObject["nbPages"];
  if (currentPage == nbPages - 1) {
    return;
  }
  myObject["currentPage"] = (int)(myObject["currentPage"]) + 1;
  Serial.println(myObject);
  effacementEcran(5, 21, CARACTERE_NOIR, FOND_NOIR);
  afficheListe(myObject["currentPage"]);
}
void retourListe() {
  int currentPage = myObject["currentPage"];
  int nbPages = myObject["nbPages"];
  if (currentPage == 0) {
    return;
  }
  myObject["currentPage"] = (int)(myObject["currentPage"]) - 1;
  Serial.println(myObject);
  effacementEcran(5, 21, CARACTERE_NOIR, FOND_NOIR);
  afficheListe(myObject["currentPage"]);
}
void retourArticle() {
  int folioMax = myObject["folioMax"];
  int currentFolio = myObject["currentFolio"];
  if (folioMax > 1 && currentFolio > 0) {
    currentFolio--;
    myObject["currentFolio"] = currentFolio;
    afficheArticleNextPrevious(-1);
  } else {
    int currentPage = myObject["currentPage"];
    int nbPages = myObject["nbPages"];
    if (currentPage == 0) {
      return;
    }
    Serial.print("icinextarticle");
    previousArticle();
  }
}
void suiteArticle() {
  int folioMax = myObject["folioMax"];
  if (folioMax > 1) {
    int currentFolio = myObject["currentFolio"];
    currentFolio++;
    myObject["currentFolio"] = currentFolio;
    afficheArticleNextPrevious(1);
  } else {
    int currentPage = myObject["currentPage"];
    int nbPages = myObject["nbPages"];
    if (currentPage == nbPages - 1) {
      return;
    }
    nextArticle();
  }
}
void nextArticle() {
  int currentArticle = myObject["currentArticle"];
  currentArticle++;
  retrieveDatasLEMONDE("lemonde/getjson.php?mode=article&item=" + String(currentArticle), 2, currentArticle);
  afficheArticle((int)myObject["displayMode"]);
}
void previousArticle() {
  int currentArticle = myObject["currentArticle"];
  currentArticle--;
  retrieveDatasLEMONDE("lemonde/getjson.php?mode=article&item=" + String(currentArticle), 2, currentArticle);
  afficheArticle((int)myObject["displayMode"]);
}
void guideLeMonde() {
  if ((int)myObject["displayMode"] == 1) {
    if (currentEcran == "GUIDE") {
      effacementEcran(5, 21, CARACTERE_NOIR, FOND_NOIR);
      afficheUNELEMONDE((int)myObject["currentPage"]);
    } else {
      currentEcran = "GUIDE";
      String vdt = "14,1f,45,41,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,0a,18,1f,56,41,1b,44,60,12,67,1f,58,41,18,0b,18,1f,45,41,0e,1b,54,20,12,67,1f,46,41,0e,1b,54,20,12,67,1f,47,41,0e,1b,54,20,12,67,1f,48,41,0e,1b,54,20,12,67,1f,49,41,0e,1b,54,20,12,67,1f,4a,41,0e,1b,54,20,12,67,1f,4b,41,0e,1b,54,20,12,67,1f,4c,41,0e,1b,54,20,12,67,1f,4d,41,0e,1b,54,20,12,67,1f,4e,41,0e,1b,54,20,12,67,1f,4f,41,0e,1b,54,20,12,67,1f,50,41,0e,1b,54,20,12,67,1f,51,41,0e,1b,54,20,12,67,1f,52,41,0e,1b,54,20,12,67,1f,53,41,0e,1b,54,20,12,67,1f,54,41,0e,1b,54,20,12,67,1f,55,41,0e,1b,54,20,12,67,1f,46,42,4c,27,61,63,74,75,61,6c,69,74,19,42,65,20,64,27,69,6c,20,79,20,61,20,34,30,20,61,6e,73,20,74,6f,75,74,20,70,69,6c,65,1f,47,42,76,69,65,6e,74,20,64,65,73,20,61,72,63,68,69,76,65,73,20,64,65,20,4c,65,20,4d,6f,6e,64,65,2c,1f,48,42,76,69,73,69,62,6c,65,73,20,69,63,69,20,3a,1f,49,42,6c,65,6d,6f,6e,64,65,2e,66,72,2f,61,72,63,68,69,76,65,73,2d,64,75,2d,6d,6f,6e,64,65,1f,4b,42,56,6f,75,73,20,70,6f,75,72,72,65,7a,20,79,20,6c,69,72,65,20,6c,65,20,64,19,42,65,74,61,69,6c,20,64,65,1f,4c,42,63,68,61,71,75,65,20,61,63,74,75,61,6c,69,74,19,42,65,20,64,6f,6e,74,20,6c,65,20,74,69,74,72,65,1f,4d,42,61,70,70,61,72,61,19,43,69,74,20,69,63,69,2e,1f,4f,42,4e,6f,75,73,20,72,65,6d,65,72,63,69,6f,6e,73,20,4c,65,20,4d,6f,6e,64,65,20,64,65,20,6c,61,20,6d,69,73,65,20,19,41,61,1f,50,42,64,69,73,70,6f,73,69,74,69,6f,6e,20,64,65,20,63,65,73,20,70,72,19,42,65,63,69,65,75,73,65,73,1f,51,42,61,63,74,75,61,6c,69,74,19,42,65,73,20,76,69,6e,74,61,67,65,2e,1f,53,42,46,69,6e,61,6c,65,6d,65,6e,74,2c,20,74,6f,75,74,20,6e,27,61,20,70,61,73,20,74,61,6e,74,20,63,68,61,6e,67,19,42,65,1f,54,42,71,75,65,20,19,4b,63,61,2c,20,76,6f,75,73,20,76,65,72,72,65,7a,2e,1f,58,4c,1b,45,52,65,76,65,6e,69,72,20,19,41,61,20,6c,27,61,63,74,75,61,6c,69,74,19,42,65,20,1b,5d,20,47,55,49,44,45,20,1f,58,41,11";
      checkScreen(vdt, 0, 0);
      minitel.noCursor();
    }
  }
}




// VARIOUS LE MONDE
void updateBandeauLEMONDE(String format) {
  if (format == "BRE") {
    minitel.moveCursorXY(18, 2);
    minitel.attributs(CARACTERE_CYAN);
    minitel.print("EN ");
    minitel.attributs(CARACTERE_BLANC);
    minitel.print("BRE");
    minitel.attributs(CARACTERE_CYAN);
    minitel.print("F CE JOUR");
  }
  if (format == "UNE") {
    minitel.moveCursorXY(18, 2);
    minitel.attributs(CARACTERE_CYAN);
    minitel.attributs(FOND_BLEU);
    minitel.print(" A LA ");
    minitel.attributs(CARACTERE_BLANC);
    minitel.print("UNE ");
    minitel.attributs(CARACTERE_CYAN);
    minitel.print("CE JOUR");
  }
}
void afficheDatasLEMONDE(int currentPage) {
  int displayMode = myObject["displayMode"];
  myObject["currentPage"] = currentPage;
  switch (displayMode) {
    case 1:
      {
        //affiche date en haut
        JSONVar date = myObject["myDatas"]["root"]["date"];
        JSONVar myarrayPagesOffset = myObject["myDatas"]["root"]["pages"];
        int nbPagesOfList = myarrayPagesOffset.length() - 1;
        myObject["nbPages"] = nbPagesOfList;

        minitel.moveCursorXY(18, 3);
        minitel.attributs(FIN_LIGNAGE);
        minitel.attributs(CARACTERE_BLANC);
        minitel.attributs(FOND_BLEU);
        minitel.print((const char*)date);
        afficheListe(currentPage);
      }
      break;

    case 2:
      {
        //myObject["myArticle"] = myObject["myDatas"];
        myObject["currentFolio"] = 0;
        int previousDisplayMode = myObject["previousDisplayMode"];
        afficheArticle(previousDisplayMode);
        break;
      }
  }
}

void retrieveDatasLEMONDE(String phpFile, int displayMode, int articleItem) {
  minitel.aiguillage(false, CODE_EMISSION_CLAVIER, CODE_RECEPTION_CLAVIER);
  ligneZero("Recherche...");
  int previousDisplayMode = myObject["displayMode"];
  myObject["previousDisplayMode"] = previousDisplayMode;
  myObject["displayMode"] = displayMode;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath;
    int currentPage = myObject["currentPage"];
    switch (displayMode) {
      case 1:
        serverPath = serverName + String(phpFile);
        break;
      case 2:
        serverPath = serverName + String(phpFile);
        myObject["currentArticle"] = articleItem;
        break;
    }
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    Serial.println("response");
    if (httpResponseCode > 0) {
      String payload = http.getString();
      minitel.aiguillage(true, CODE_EMISSION_CLAVIER, CODE_RECEPTION_CLAVIER);
      // Free resources
      http.end();
      JSONVar myDatas = JSON.parse(payload);
      if (JSON.typeof(myDatas) == "undefined") {
        return;
      } else {
        switch (displayMode) {
          case 1:
            myObject["myDatas"] = myDatas;
            break;
          case 2:
            myObject["myArticle"] = myDatas;
            break;
        }
      }
      ligneZero(" ");
    }
  }
}
void afficheArticleNextPrevious(int sens) {
  effacementEcran(5, 21, CARACTERE_NOIR, FOND_NOIR);
  afficheFolio();
  JSONVar myDatas = myObject["myArticle"];
  minitel.textMode();
  JSONVar myarray = myDatas["article"];
  JSONVar article = (myarray[0]);
  //JSONVar article = (myarray[0]);
  Serial.println("--article");
  Serial.println(article);
  JSONVar texte = article["texte"];
  int lastLineArticle = myObject["lastlineArticle"];
  if (lastLineArticle >= texte.length()) {
    if (sens == 1) {
      nextArticle();
    } else {
      previousArticle();
    }
    return;
  }
  int posY = 5;
  int nbLines = 0;
  int j;
  minitel.attributs(CARACTERE_CYAN);
  for (j = lastLineArticle; j < (lastLineArticle + 17); j++) {
    minitel.moveCursorXY(0, posY);
    minitel.print((const char*)texte[j]);
    posY++;
    nbLines++;
    lastLineArticle++;
    if (nbLines == 17) {
      myObject["lastlineArticle"] = lastLineArticle;
      break;
    }
  }
}
void afficheFolio() {
  int folioMax = myObject["folioMax"];
  int currentFolio = myObject["currentFolio"];
  if (folioMax > 1) {
    minitel.newXY(35, 22);
    minitel.attributs(CARACTERE_BLEU);
    minitel.print(String(currentFolio + 1));
    minitel.print("/");
    minitel.print(String(folioMax));
  } else {
    minitel.attributs(CARACTERE_BLEU);
    minitel.moveCursorXY(35, 22);
    minitel.writeByte(0x60);
    minitel.repeat(4);
  }
}
void afficheArticle(int previousDisplayMode) {
  currentEcran="ARTICLE";
  myObject["currentFolio"] = 0;
  displayNavArticle(previousDisplayMode);
  effacementEcran(5, 21, CARACTERE_NOIR, FOND_NOIR);
  minitel.noCursor();
  JSONVar myDatas = myObject["myArticle"];
  minitel.textMode();
  JSONVar myarray = myDatas["article"];
  JSONVar article = (myarray[0]);
  JSONVar titre = article["titre"];
  JSONVar texte = article["texte"];
  int nbLines = 0;
  int posY = 5;
  int j = 0;
  int titreNbLines = titre.length();
  int textNbLines = texte.length();
  float nbFolio = (titreNbLines + textNbLines + 1) / 17.00;
  int folioMax = ceil(nbFolio);
  myObject["folioMax"] = folioMax;
  afficheFolio();
  minitel.attributs(CARACTERE_BLANC);
  for (j = 0; j < titreNbLines; j++) {
    minitel.moveCursorXY(0, posY);
    minitel.print((const char*)titre[j]);
    posY++;
    nbLines++;
  }
  posY++;
  nbLines++;
  minitel.attributs(CARACTERE_CYAN);

  for (j = 0; j < textNbLines; j++) {
    minitel.moveCursorXY(0, posY);
    minitel.print((const char*)texte[j]);
    posY++;
    nbLines++;
    if (nbLines == 17) {
      myObject["lastlineArticle"] = j + 1;
      break;
    }
  }
}
void afficheListe(int page) {
  minitel.noCursor();
  JSONVar myDatas = myObject["myDatas"];
  minitel.textMode();
  JSONVar myarray = myDatas["root"]["articles"];
  JSONVar myarrayPagesOffset = myDatas["root"]["pages"];
  //Serial.println()
  int nbArticles = (int)myarrayPagesOffset[(page + 1)] - (int)myarrayPagesOffset[(page)];
  nbArticles += (int)myarrayPagesOffset[(page)];
  int i = 0;
  int posY = 5;
  JSONVar format = myObject["format"];
  String sformat = (const char*)format;
  for (i = myarrayPagesOffset[(page)]; i < nbArticles; i++) {
    JSONVar article = (myarray[i]);
    JSONVar titre = article["titre"];
    int j = 0;
    int titreNbLines = titre.length();

    for (j = 0; j < titreNbLines; j++) {

      if (sformat == "UNE") {
        if (j == 0) {
          minitel.attributs(CARACTERE_BLANC);
          minitel.moveCursorXY(0, posY);
          minitel.print(String(i + 1));
        }
        minitel.moveCursorXY(4, posY);
        minitel.attributs(CARACTERE_BLANC);
        minitel.print((const char*)titre[j]);
      } else {
        minitel.moveCursorXY(1, posY);
        minitel.attributs(CARACTERE_BLANC);
        minitel.print((const char*)titre[j]);
      }
      posY++;
    }

    if (i < (nbArticles - 1)) {
      minitel.attributs(CARACTERE_BLEU);
      minitel.moveCursorXY(0, posY);
      minitel.writeByte(0x60);
      minitel.repeat(39);
      posY++;
    }
  }
  minitel.cursor();
}