int nbCaracteres = 0;
const int CP_PREMIERE_LIGNE_AFFICHAGE = 8;

void setupCP() {
  minitel.newScreen();
  minitel.noCursor();
  wifiConnect();
  retrieveDatasCP("couplesparfaits/getjson.php");
  myObject["proposed"] = "";
  myObject["nbtries"] = 0;
  myObject["nbcouplesok"] = 0;
  afficheCP();
}
void afficheCP(){
  afficheRemoteVDT("bandeaucp.vdt", 0, 0);
  initCP();
}
void initCP() {
  minitel.textMode();
  minitel.attributs(CARACTERE_BLANC);
  JSONVar texteKO = myObject["myDatas"]["root"]["linesko"];
  Serial.println(texteKO);
  int nbLines = texteKO.length();
  myObject["nblines"] = nbLines;
  myObject["nblinesOK"] = 0;
  for (int l = 0; l < nbLines; l++) {
    minitel.moveCursorXY(0, CP_PREMIERE_LIGNE_AFFICHAGE + l);
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
        Serial.println(payload);
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
/////////////////////////CORRECTION A CORRIGER !!!///////////////////////////////////////////////
void correction() {
  nbCaracteres = userInput.length();
  Serial.println(nbCaracteres);
  if (nbCaracteres > 0) {
    minitel.moveCursorLeft(1);
    minitel.attributs(CARACTERE_BLEU);
    minitel.print("+");
    minitel.attributs(CARACTERE_BLANC);
    minitel.moveCursorLeft(1);
    userInput = userInput.substring(0, userInput.length() - 1);
    Serial.println("correc");
    Serial.println(userInput);
    nbCaracteres--;
    //formulaireConfigData[myObject["currentLine"]] = userInput;
    //myObject["input"][myObject["currentLine"]] = userInput;
  }
}
////////TESTER DEUX CARACTERES SAISIS, ni plus ni moins///
void lectureChampCP(int x, int y, int longueurchamp) {
  champVide(x, y, longueurchamp);
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (nbCaracteres < longueurchamp) {
        nbCaracteres++;
        userInput += char(touche);
      } else {
        //minitel.moveCursorLeft(2);
        //minitel.print(".");
        //minitel.moveCursorLeft(1);
      }
      // if (nbCaracteres == 40*nbLignes) {
      //   minitel.moveCursorXY(40,(premiereLigne-1)+nbLignes);
      // }
    }
    switch (touche) {
      case CONNEXION_FIN:
       fin=true;
        minitel.connexion(false);
        displayMire();
      break;
      
      case ENVOI:
      {
        Serial.println("nbcar");
        Serial.println(nbCaracteres);

        if (nbCaracteres >= longueurchamp) {
          //ligneZero("ok");
          fin = true;
          nbCaracteres = 0;
          
        }

        }
        break;
      case ANNULATION:
        champVide(x, y, longueurchamp);
        nbCaracteres = 0;
        break;
      case CORRECTION:
        correction();
        break;
    case REPETITION:
      afficheCP();
      userInput="";
      fin=true;
      nbCaracteres=0;
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
      minitel.moveCursorXY(index + 1, CP_PREMIERE_LIGNE_AFFICHAGE + num);
      minitel.print(String(char1));
      minitel.moveCursorXY(index + 2, CP_PREMIERE_LIGNE_AFFICHAGE + num);
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

void insertCouple(String couple) {
  String message;
  Serial.println("insert");
  Serial.println(couple);
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
  Serial.println("mess");
  Serial.println(message);
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

void loopCP() {
  Serial.print("loopcp");
  lectureChampCP(16, 24, 2);
  insertCouple(userInput);
  userInput = "";
}