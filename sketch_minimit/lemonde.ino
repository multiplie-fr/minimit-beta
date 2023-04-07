int userInputNeeded = 0;
void setupLeMonde() {
  minitel.pageMode();
  minitel.newScreen();
  minitel.textMode();
  wifiConnect();
  initLEMONDE();
  afficheUNELEMONDE();
}

void initLEMONDE(){
  minitel.newScreen();
  
  myObject["currentPage"] = (int)0;
  myObject["displayMode"] = (int)1;
  //Displaymode
  //1 : liste
  //2 : fiche
  myObject["format"] = "UNE";
  ///UNE ou BRE
  displayBandeau();
  
}
void resetPagesLEMONDE(){
  myObject["currentPage"] = (int)0;
  myObject["displayMode"] = (int)1;
}
void afficheUNELEMONDE(){
  myObject["format"] = "UNE";
  updateBandeauLEMONDE("UNE");
  displayNavList();
  retrieveDatasLEMONDE("lemonde/getjson.php?current=", 1, 1);
  afficheDatasLEMONDE();
}
void afficheBRELEMONDE(){
  myObject["format"] = "BRE";
  minitel.newScreen();
  displayBandeau();
  updateBandeauLEMONDE("BRE");
  displayNavList();
  retrieveDatasLEMONDE("lemonde/getjson.php?mode=liste&format=bre", 1, 0);
  afficheDatasLEMONDE();
}

void repetitionLEMONDE(){
  JSONVar format = myObject["format"];
  String sformat = (const char*)format; 
  int displayMode = myObject["displayMode"];
  if(sformat=="UNE"){
    if (displayMode==1){
      afficheUNELEMONDE();      
    }
  }
}
void sommaireLEMONDE(){
  initLEMONDE();
  afficheUNELEMONDE();
}           
void displayBandeau() {
  String vdt = "14,0c,1f,41,41,0e,1b,54,20,38,24,20,2c,38,38,30,20,12,43,28,30,20,12,59,1f,42,41,0e,1b,54,20,1b,57,1b,44,4a,1b,54,1b,47,48,1b,57,1b,44,46,1b,54,20,1b,57,4a,12,42,49,1b,54,1b,47,34,1b,57,1b,44,48,1b,54,1b,47,34,1b,57,1b,44,49,4a,29,1b,54,1b,47,24,20,12,57,1f,43,41,0e,1b,54,40,45,2a,38,40,25,1b,57,1b,44,4a,1b,54,1b,47,45,45,25,25,1b,57,1b,44,4a,1b,54,1b,47,45,25,45,24,20,12,57,1f,44,41,0e,1b,44,23,12";
  checkScreen(vdt, 0, 0);
  minitel.textMode();
  
}
void displayNavArticle(int previousDisplayMode) {
  if (previousDisplayMode == 1) {
    minitel.moveCursorXY(1, 24);
    minitel.print(" ");
    minitel.repeat(22);
    minitel.moveCursorXY(1, 24);
    minitel.attributs(CARACTERE_MAGENTA);
    minitel.print("Revenir au ");
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
  minitel.moveCursorXY(2, 24);
  minitel.attributs(CARACTERE_MAGENTA);
  if (sformat=="UNE")
  {
    minitel.print("Les brèves ");
    minitel.attributs(CARACTERE_BLANC);
    minitel.print("BRE");
    minitel.attributs(DEBUT_LIGNAGE);
    minitel.writeByte(0x20);
    minitel.attributs(INVERSION_FOND);
    minitel.attributs(CARACTERE_BLANC);    
  }
  else {
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

  minitel.moveCursorXY(26, 23);
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
void lectureChampLeMonde(int x, int y, int longueurchamp) {
  Serial.print("lc");
  champVide(x, y, longueurchamp);
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (userInputNeeded < longueurchamp) {
        userInputNeeded++;
        userInput += char(touche);
        Serial.print(userInput);
      } else {
        //minitel.moveCursorLeft(1);
        //minitel.print(" ");
        //minitel.moveCursorLeft(1);
      }
    }
    switch (touche) {
      case CONNEXION_FIN:
        fin=true;
        minitel.connexion(false);
        displayMire();
      break;
      case SUITE:
        {
          Serial.println("suite");
          int currentPage = myObject["currentPage"];
          int nbPages = myObject["nbPages"];
          if (currentPage == nbPages - 1) {
            break;
          }
          Serial.println("nbpages");
          Serial.println(myObject["nbPages"]);
          Serial.println("currentpage");
          Serial.println(myObject["currentPage"]);
          myObject["currentPage"] = (int)(myObject["currentPage"]) + 1;
          Serial.println(myObject);
          effacementEcran(5, 21, CARACTERE_NOIR, FOND_NOIR);
          fin = true;
          afficheListe(myObject["currentPage"]);
        }
        break;
      case RETOUR:
        if ((int)myObject["currentPage"] == 0) {
          break;
        }
        myObject["currentPage"] = (int)(myObject["currentPage"]) - 1;
        effacementEcran(5, 21,CARACTERE_NOIR, FOND_NOIR);
        fin = true;
        afficheListe(myObject["currentPage"]);
        break;
      case ENVOI:
        fin = true;
        Serial.println("envoi");
        userInput.toUpperCase();
        if (userInputNeeded > 0) {
          Serial.println("choix");
          Serial.println(userInput);
          if(!isValidNumber(userInput)){
            if (userInput == "BRE") {
             afficheBRELEMONDE();
            }
            if (userInput == "UNE") {
              myObject["format"] = "UNE";
              updateBandeauLEMONDE("UNE");
              myObject["currentPage"] = (int)0;
              myObject["displayMode"] = (int)1;
              retrieveDatasLEMONDE("lemonde/getjson.php?mode=liste&format=une", 1, 0);
              effacementEcran(5, 21,CARACTERE_NOIR, FOND_NOIR);
              afficheDatasLEMONDE();
            }
          }
          
          else {
            int userInputInt = userInput.toInt();
            userInputInt--;
            retrieveDatasLEMONDE("lemonde/getjson.php?mode=article&item=" + String(userInputInt), 2, userInputInt);
            afficheDatasLEMONDE();
          }
        }
        break;
      case SOMMAIRE:
        fin = true;
        sommaireLEMONDE();
        break;
      case ANNULATION:
        champVide(x, y, longueurchamp);
        break;
      case CORRECTION:
        //correction(longueurchamp); TODO
        break;
    }
  }
}

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
    minitel.print("A LA ");
    minitel.attributs(CARACTERE_BLANC);
    minitel.print("UNE ");
    minitel.attributs(CARACTERE_CYAN);
    minitel.print("CE JOUR");
  }
}
void afficheDatasLEMONDE() {
  int displayMode = myObject["displayMode"];
  int currentPage = myObject["currentPage"];
  switch (displayMode) {
    case 1:
      {
        //affiche date en haut
        JSONVar date = myObject["myDatas"]["root"]["date"];
        JSONVar myarrayPagesOffset = myObject["myDatas"]["root"]["pages"];
        int nbPagesOfList = myarrayPagesOffset.length() - 1;
        myObject["nbPages"] = nbPagesOfList;

        minitel.moveCursorXY(18, 3);
        minitel.attributs(CARACTERE_BLANC);
        minitel.print((const char*)date);
        afficheListe(currentPage);
      }
      break;

    case 2:
      {
        myObject["myArticle"] =  myObject["myDatas"];
        myObject["currentPageFolio"] = 0;
        int previousDisplayMode = myObject["previousDisplayMode"];
        affichePage(previousDisplayMode);
        break;
      }
  }
}

void retrieveDatasLEMONDE(String phpFile, int displayMode, int articleItem) {
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
        Serial.println(serverPath);
        break;
    }
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      JSONVar myDatas = JSON.parse(payload);

      if (JSON.typeof(myDatas) == "undefined") {
        Serial.println(payload);
        Serial.println("Parsing input failed!");
        return;
      }
      else {
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
void affichePage(int previousDisplayMode) {
  Serial.println("affichepage");
  displayNavArticle(previousDisplayMode);
  Serial.print(myObject["myDatas"]["article"]);
  effacementEcran(5, 21,CARACTERE_NOIR, FOND_NOIR);
  minitel.noCursor();
  JSONVar myDatas = myObject["myDatas"];
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
  Serial.println(String(titreNbLines));
  Serial.println(String(textNbLines));
  float nbFolio = (titreNbLines + textNbLines + 1) / 17.00;
  Serial.println("---");
  Serial.println("nbFolio");
  int folioMax = ceil(nbFolio);
  Serial.println(String(folioMax));
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
  Serial.println("mydataspagesoffset");
  Serial.println(myarrayPagesOffset);
  //Serial.println()
  int nbArticles = (int)myarrayPagesOffset[(page + 1)] - (int)myarrayPagesOffset[(page)];
  nbArticles += (int)myarrayPagesOffset[(page)];
  Serial.println("nbarcticles");
  Serial.println(nbArticles);
  Serial.println(myarrayPagesOffset[(page + 1)]);
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

      if(sformat=="UNE"){
      if (j == 0) {
        minitel.attributs(CARACTERE_BLANC);
        minitel.moveCursorXY(0, posY);
        minitel.print(String(i + 1));
      }
      minitel.moveCursorXY(4, posY);
      minitel.attributs(CARACTERE_BLANC);
      minitel.print((const char*)titre[j]);
      }
      else {
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