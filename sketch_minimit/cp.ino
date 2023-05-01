void setupCP() {
  currentEcran = "CP";
  minitel.newScreen();
  minitel.noCursor();
  wifiConnect();
  retrieveDatasCP("couplesparfaits/getjson.php");
  myObject["proposed"] = "";
  myObject["nbtries"] = 0;
  myObject["nbcouplesok"] = 0;
  afficheCP();
  minitel.echo(true);
}
void afficheCP(){
  String vdt = "0c,14,1f,41,41,0e,1b,57,20,12,42,1b,45,40,24,38,30,34,34,1b,55,1b,47,43,1b,57,1b,45,30,34,40,24,38,20,1b,51,1b,47,43,1b,57,1b,41,30,38,30,1b,51,1b,47,43,1b,57,1b,41,30,1b,51,1b,47,43,1b,57,1b,41,40,44,28,28,1b,51,1b,47,43,1b,57,1b,41,40,24,20,1b,45,58,58,30,1b,41,58,58,30,20,12,42,1f,42,41,0e,1b,57,20,12,42,1b,45,4a,20,1b,55,1b,47,4a,12,43,48,1b,57,20,1b,55,4a,1b,57,1b,45,4a,21,49,20,1b,51,1b,47,48,1b,57,20,1b,51,48,4a,48,1b,57,1b,41,34,1b,51,1b,47,48,1b,57,1b,41,4a,1b,51,1b,47,34,1b,57,1b,41,4a,20,1b,51,1b,47,4a,1b,57,1b,41,22,34,20,1b,55,1b,47,30,20,1b,57,1b,45,25,1b,51,1b,47,30,20,1b,57,1b,41,25,20,12,42,1f,43,41,0e,1b,57,20,12,43,1b,45,21,22,20,22,20,21,20,22,20,21,21,20,1b,41,21,20,21,12,44,22,12,42,20,21,22,20,12,42,1b,45,21,20,20,1b,41,21,20,12,43,1f,44,41,0e,1b,57,20,12,67,1f,45,41,0e,1b,57,20,12,67,1f,46,41,0e,23,12,67,1f,54,41,0e,50,12,67,1f,55,41,0e,1b,57,20,12,67,1f,56,41,0e,1b,57,20,12,67,1f,57,41,0e,1b,57,1b,40,50,12,67,1f,44,42,1b,40,54,61,70,65,20,64,65,73,20,63,6f,75,70,6c,65,73,20,64,65,20,6c,65,74,74,72,65,73,2c,20,74,6f,75,74,65,73,0d,0a,09,6c,65,75,72,73,20,6f,63,63,75,72,72,65,6e,63,65,73,20,73,65,72,6f,6e,74,20,72,19,42,65,76,19,42,65,6c,19,42,65,65,73";
  checkScreen(vdt, 0, 0);
  initCP();
}
void initCP() {
  minitel.textMode();
  minitel.attributs(CARACTERE_BLANC);
  JSONVar texteKO = myObject["myDatas"]["root"]["linesko"];
  int nbLines = texteKO.length();
  myObject["nblines"] = nbLines;
  myObject["nblinesOK"] = 0;
  for (int l = 0; l < nbLines; l++) {
    minitel.moveCursorXY(0, 7 + l);
    minitel.print((const char*)texteKO[l]);
  }
  minitel.moveCursorXY(21, 24);
  minitel.attributs(FOND_ROUGE);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" ENVOI ");
  minitel.attributs(FOND_NORMAL);
}

void retrieveDatasCP(String phpFile) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath;
    serverPath = serverName + String(phpFile);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      JSONVar myDatas = JSON.parse(payload);
      if (JSON.typeof(myDatas) == "undefined") {
         Serial.println("Parsing input failed!");
        return;
      }
      myObject["myDatas"] = myDatas;
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}
void retrieveClassementCP(String phpFile) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath;
    serverPath = serverName + String(phpFile);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
        
      JSONVar myDatas = JSON.parse(payload);
      if (JSON.typeof(myDatas) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      myObject["classement"] = myDatas;
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}
/////////////////////////CORRECTION A CORRIGER !!!///////////////////////////////////////////////
void correction() {
  Serial.println(userInputLength);
  if (userInputLength > 0) {
    minitel.moveCursorLeft(1);
    minitel.attributs(CARACTERE_BLEU);
    minitel.print("+");
    minitel.attributs(CARACTERE_BLANC);
    minitel.moveCursorLeft(1);
    userInput = userInput.substring(0, userInput.length() - 1);
    Serial.println("correc");
    Serial.println(userInput);
    userInputLength--;
  }
}
////////TESTER DEUX CARACTERES SAISIS, ni plus ni moins///
void lectureChampCP(int x, int y, int longueurchamp) {
 if(currentEcran!="FIN"){
      champVide(x, y, longueurchamp);
 }
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (userInputLength < longueurchamp) {
        userInputLength++;
        userInput += char(touche);
        Serial.println(userInput);
      } else {
        //minitel.moveCursorLeft(2);
        //minitel.print(".");
        //minitel.moveCursorLeft(1);
      }
    }
    switch (touche) {
      case CONNEXION_FIN:
        fin=true;
        userInput="";
        userInputLength=0;
        minitel.connexion(false);
        displayMire();
      break;
      case SUITE:
      fin=true;
      afficheClassement();
      break;
      case ENVOI:
      {
       if (userInputLength >= longueurchamp) {
          insertCouple(userInput);
          fin = true;
          userInput = "";
          userInputLength=0;
        }
      }
        break;
      case ANNULATION:
        champVide(x, y, longueurchamp);
        userInputLength = 0;
        break;
      case CORRECTION:
        correction();
        break;
    case REPETITION:
      afficheCP();
      userInput="";
      fin=true;
      userInputLength=0;
    break;
    case SOMMAIRE:
      userInput="";
      fin=true;
      userInputLength=0;
      setupCP();
      
    break;
  }
   
  }
}
void remplitLigne(int num, String couple) {
  int positions[] = { -1, -1, -1, -1, -1, -1 };
  minitel.attributs(CARACTERE_BLANC);
  JSONVar arraySoluce = myObject["myDatas"]["root"]["linesok"];
  JSONVar arrayUser = myObject["myDatas"]["root"]["linesko"];
  String ligneOK = (const char*)arraySoluce[num];
  String ligneKO = (const char*)arrayUser[num];
  int idx = ligneOK.indexOf(couple);
  positions[0] = idx;
  int counter = 0;
  int absolute_idx = idx;
  while (idx > -1) {
    idx = ligneOK.indexOf(couple, absolute_idx + 1);
    if (idx < 0) break;
    counter++;
    absolute_idx = (absolute_idx + idx);
    positions[counter] = idx;
  }
  for (int i = 0; i < 6; i++) {
    int index = positions[i];
    char char1 = couple.charAt(0);
    char char2 = couple.charAt(1);
    if (index > -1) {
      int nbCouplesOk = myObject["nbcouplesok"];
      nbCouplesOk++;
      myObject["nbcouplesok"] = nbCouplesOk;
      //mettre le tableau en cours à jour
      ligneKO.setCharAt(index, char1);
      ligneKO.setCharAt(index + 1, char2);
      //on se met sur la bonne ligne
      minitel.moveCursorXY(index + 1, 7 + num);
      minitel.print(String(char1));
      minitel.moveCursorXY(index + 2, 7 + num);
      minitel.print(String(char2));
      arrayUser[num] = ligneKO;
    }
  }
  if (ligneKO == ligneOK) {
    int nbLinesOk = myObject["nblinesok"];
    myObject["nblinesok"] = nbLinesOk + 1;
    Serial.println(String(nbLinesOk + 1));
  }
}
void displayNavFin()
{
  currentEcran = "FIN";
  effacementEcran(24, 24, CARACTERE_NOIR, FOND_NOIR);
  minitel.newXY(1, 24);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print("Un autre ? ");
  minitel.attributs(FOND_MAGENTA);
  minitel.print (" SOMMAIRE ");
  if(isConnected){
  minitel.attributs(FOND_NOIR);
  minitel.print(" Classement ");
  minitel.attributs(FOND_MAGENTA);
  minitel.print (" SUITE ");
  }
  minitel.echo(false);

}
void afficheClassement(){
  //currentEcran="CLASSEMENT";
  effacementEcran(4, 5, CARACTERE_BLANC, FOND_BLANC);
  effacementEcran(7,23, CARACTERE_NOIR, FOND_NOIR);
  minitel.attributs(CARACTERE_BLANC);
  minitel.newXY(1, 23);
  minitel.writeByte(0x60);
  minitel.repeat(39);
  minitel.newXY(2,5);
  minitel.attributs(CARACTERE_NOIR);
  minitel.print("LE CLASSEMENT");
  ligneZero("Recherche...");
  retrieveClassementCP("users/classementcp.php");
  ligneZero(" ");
  minitel.newXY(23,24);
  minitel.print(" ");
  minitel.repeat(16);
  JSONVar classement = myObject["classement"]["classement"];
  int nu = classement.length();
  int posY = 8;
  minitel.attributs(CARACTERE_BLANC);
  for(int i=0; i<nu; i++){
    JSONVar item = classement[i];
    minitel.newXY(2,posY);
    minitel.print(String(i+1));
    minitel.newXY(5,posY);
    minitel.print((const char*)item["pseudo"]);
    Serial.println(item["pseudo"]);
    minitel.newXY(25,posY);
    minitel.print((const char*)item["score"]);
    posY++;
  }
}
void insertCouple(String couple) {
  String message;
  myObject["nblinesok"] = 0;
  minitel.echo(false);
  minitel.noCursor();
  myObject["nbcouplesok"] = 0;
  String proposed = (const char*)myObject["proposed"];
  int indexProposed = proposed.indexOf(couple);
  minitel.attributs(CARACTERE_NOIR);
  minitel.moveCursorXY(2, 21);
  if (indexProposed > -1) {
    CPMessage("Tu l'as déjà proposé");
    return;
  } else {
    int nbtries = myObject["nbtries"];
    nbtries++;
    myObject["nbtries"] = nbtries;
    CPScore(nbtries);

    myObject["proposed"] = proposed + couple + ",";
    minitel.noCursor();
    int nbLines = myObject["nblines"];
    for (int l = 0; l < nbLines; l++) {
      remplitLigne(l, couple);
    }
  }

  int nblinesOK = myObject["nblinesok"];
  JSONVar author = myObject["myDatas"]["root"]["author"];
  JSONVar oeuvre = myObject["myDatas"]["root"]["oeuvre"];
  JSONVar texteKO = myObject["myDatas"]["root"]["linesko"];
  int nbLines = texteKO.length();
  if (nblinesOK == nbLines){
    CPMessage("Bravo !");
    CPAuthor((const char*)author, (const char*)oeuvre);
    displayNavFin();
    return;
  }
  
  if (indexProposed == -1) {
  int nbCouplesOk = myObject["nbcouplesok"];
  if (nbCouplesOk > 0) {
    message = "Bravo, tu as trouvé ";
    message += String(nbCouplesOk);
    message += " couple";
    if (nbCouplesOk > 1) {
      message += "s";
    }
  } else {
    message = "Raté !";
  }
  }  
  CPMessage(message);
  minitel.cursor();
  minitel.echo(true);
}

void CPMessage(String st) {
  minitel.moveCursorXY(2, 21);
  minitel.attributs(FOND_BLANC);
  minitel.modeVideotex();
  minitel.print(" ");
  minitel.repeat(38);
  minitel.textMode();
  minitel.attributs(FOND_NORMAL);
  minitel.attributs(CARACTERE_NOIR);
  minitel.moveCursorXY(2, 21);
  minitel.print(st);
}
void CPScore(int score) {
  minitel.moveCursorXY(2, 22);
  minitel.attributs(CARACTERE_ROUGE);
  minitel.print(String(score));
  minitel.print(" essai");
  if (score > 1) {
    minitel.print("s");
  }
}
void CPAuthor(String st, String oe) {
  minitel.attributs(CARACTERE_ROUGE);
  minitel.moveCursorXY(1, 18);
  minitel.print(st);
  minitel.attributs(CARACTERE_ROUGE);
  minitel.moveCursorXY(1, 19);
  minitel.print(oe);
}
void loopCP() {
  //Serial.print("loopcp");
  lectureChampCP(16, 24, 2);
  //insertCouple(userInput);
  //userInput = "";
}