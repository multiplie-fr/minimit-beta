// PENDU
// setup
void setupPendu() {
  minitel.newScreen();
  minitel.pageMode();
  afficheSommairePendu(true, false);
  myObject["current"] = 0;
  myObject["nbMots"] = 0;
  saisieColor = CARACTERE_NOIR;
}

// loop
void loopPendu() {

  while (1) {
  if (currentEcran != "FIN") {
    Serial.println("champvide");
    champVide(11, 17, 1);
  }
    wait_for_user_action();

    switch (touche) {
      case CONNEXION_FIN:
        return;
        break;
      case ENVOI:
        {
          if (userInputLength == 1) {
            joueLettre(userInput, false);
            //champVide(11, 17, 1);
          }
        }
        break;
      case ANNULATION:
        Serial.println("annulationpendu");
        champVide(11, 17, 1);
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
          repetitionPENDU();
          champVide(11, 17, 1);
        }
        break;
      case SOMMAIRE:
        {
          if(currentEcran=="SOMMAIRE")
          {
            Serial.println("SOMMAIRE");
            return;
            break;
          }
          else
          {
            afficheSommairePendu(false, false);
            afficheDatasPENDU();
            userInput = "";
            userInputLength = 0;
          }
        }
        break;
    }
  }
}

// PENDU stuff
void localDatasPendu() {
  String rawDatas = "{\"mots\":[[\"W\",\"A\",\"L\",\"K\",\"M\",\"A\",\"N\"],[\"B\",\"A\",\"L\",\"A\",\"D\",\"E\",\"U\",\"R\"],[\"V\",\"I\",\"D\",\"E\",\"O\",\"C\",\"A\",\"S\",\"S\",\"E\",\"T\",\"T\",\"E\"],[\"M\",\"I\",\"N\",\"I\",\"T\",\"E\",\"L\"],[\"R\",\"E\",\"M\",\"B\",\"O\",\"B\",\"I\",\"N\",\"E\",\"R\"],[\"A\",\"U\",\"T\",\"O\",\"C\",\"U\",\"I\",\"S\",\"E\",\"U\",\"R\"],[\"C\",\"A\",\"T\",\"H\",\"O\",\"D\",\"I\",\"Q\",\"U\",\"E\"],[\"D\",\"I\",\"S\",\"Q\",\"U\",\"E\",\"T\",\"T\",\"E\"],[\"R\",\"A\",\"D\",\"I\",\"O\",\"C\",\"A\",\"S\",\"S\",\"E\",\"T\",\"T\",\"E\"],[\"M\",\"O\",\"N\",\"O\",\"C\",\"H\",\"R\",\"O\",\"M\",\"E\"],[\"C\",\"A\",\"L\",\"C\",\"U\",\"L\",\"E\",\"T\",\"T\",\"E\"],[\"P\",\"L\",\"A\",\"T\",\"I\",\"N\",\"E\"],[\"V\",\"I\",\"N\",\"Y\",\"L\",\"E\"],[\"M\",\"A\",\"G\",\"N\",\"E\",\"T\",\"O\",\"S\",\"C\",\"O\",\"P\",\"E\"],[\"C\",\"A\",\"M\",\"E\",\"S\",\"C\",\"O\",\"P\",\"E\"],[\"C\",\"A\",\"R\",\"A\",\"V\",\"A\",\"N\",\"E\"],[\"C\",\"O\",\"N\",\"C\",\"O\",\"R\",\"D\",\"E\"],[\"P\",\"I\",\"N\",\"\'\",\"S\"],[\"D\",\"Y\",\"N\",\"A\",\"S\",\"T\",\"I\",\"E\"],[\"V\",\"A\",\"C\",\"H\",\"E\",\"M\",\"E\",\"N\",\"T\"],[\"A\",\"R\",\"C\",\"A\",\"D\",\"E\"],[\"C\",\"U\",\"S\",\"T\",\"O\",\"M\",\"I\",\"S\",\"E\",\"R\"],[\"B\",\"I\",\"G\",\"O\",\"P\",\"H\",\"O\",\"N\",\"E\"],[\"C\",\"H\",\"E\",\"B\",\"R\",\"A\",\"N\"],[\"T\",\"O\",\"N\",\"T\",\"O\",\"N\"],[\"B\",\"O\",\"U\",\"M\"],[\"H\",\"E\",\"X\",\"A\",\"G\",\"O\",\"N\",\"A\",\"L\"],[\"A\",\"E\",\"R\",\"O\",\"B\",\"I\",\"C\"],[\"C\",\"R\",\"A\",\"I\",\"G\",\"N\",\"O\",\"S\"],[\"V\",\"E\",\"R\",\"L\",\"A\",\"N\"],[\"S\",\"L\",\"I\",\"B\",\"A\",\"R\",\"D\"],[\"B\",\"A\",\"N\",\"D\",\"A\",\"N\",\"A\"],[\"B\",\"O\",\"T\",\"T\",\"I\",\"N\"],[\"T\",\"O\",\"P\",\" \",\"M\",\"O\",\"U\",\"M\",\"O\",\"U\",\"T\",\"E\"]]}";
  JSONVar myDatas = JSON.parse(rawDatas);
  int cnt = myDatas["mots"].length();
  int arr[cnt];
  // Populate the array using a loop
  for (int i = 0; i < cnt; i++) {
    arr[i] = i;
  }
  // Get the length of the array
  int arrLength = sizeof(arr) / sizeof(arr[0]);
  // Shuffle the array
  for (int i = arrLength - 1; i >= 1; i--) {
    int j = random(i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
  JSONVar mots = JSON.parse("[]");
  for (int i = 0; i < cnt; i++) {
    JSONVar lemot = myDatas["mots"][arr[i]];
    if (JSON.typeof(lemot) == "array") {
      mots[i] = lemot;
    }
  }
  myDatas["mots"] = mots;
  myObject["myDatas"] = myDatas;
  afficheDatasPENDU();
}
void retrieveDatasPENDU() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("pendu/getjsonfull.php");
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
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
      afficheDatasPENDU();
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
void afficheSommairePendu(boolean firsttime, boolean refresh) {
  currentEcran = "SOMMAIRE";
  minitel.newScreen();
  String vdt = "0c,14,1f,41,41,0e,20,1b,45,42,1b,55,1b,40,4a,2a,1b,50,1b,45,32,1b,55,1b,40,4a,28,09,1b,57,28,4a,28,1b,50,1b,47,21,1b,57,1b,40,42,25,1b,50,1b,47,4a,49,4a,4a,20,12,55,1f,42,41,0e,1b,55,20,1b,50,1b,45,28,25,1b,55,1b,40,52,1b,50,1b,45,25,25,1b,55,1b,40,51,09,1b,50,1b,47,25,09,1b,57,1b,40,52,1b,50,1b,47,24,25,2a,2a,26,2a,1b,57,1b,40,51,1b,50,1b,45,4a,1b,55,20,12,54,1f,43,41,0e,1b,55,20,12,67,1f,44,41,0e,1b,55,20,12,67,1f,45,41,0e,1b,55,20,12,67,1f,46,41,0e,1b,55,20,12,67,1f,47,41,0e,1b,55,20,12,67,1f,48,41,0e,1b,55,20,12,67,1f,49,41,0e,1b,55,20,12,67,1f,4a,41,0e,1b,55,20,12,67,1f,4b,41,0e,1b,55,20,12,67,1f,4c,41,0e,1b,55,20,12,67,1f,4d,41,0e,1b,55,20,12,67,1f,4e,41,0e,1b,55,20,12,67,1f,4f,41,0e,1b,55,20,12,67,1f,50,41,0e,1b,55,20,12,67,1f,51,41,0e,1b,55,20,12,67,1f,52,41,0e,1b,55,20,12,67,1f,53,41,0e,1b,55,20,12,67,1f,54,41,0e,1b,55,20,12,67,1f,55,41,0e,1b,55,20,12,67,1f,56,41,0e,1b,55,20,12,67,1f,57,41,0e,1b,55,20,12,67,1f,58,41,0e,1b,55,20,12,67,1f,43,53,0e,1b,55,20,12,44,50,12,4f,30,1f,44,53,0e,1b,55,20,12,4e,22,44,20,12,42,1b,57,1b,45,4a,1b,55,20,1f,45,53,0e,1b,55,20,12,50,29,30,20,1b,57,1b,45,4a,1b,55,20,1f,46,53,0e,1b,55,20,12,51,22,44,1b,57,1b,45,4a,1b,55,20,1f,47,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,48,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,49,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,4a,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,4b,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,4c,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,4d,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,4e,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,4f,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,50,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,51,53,0e,1b,55,20,12,53,1b,57,1b,45,4a,1b,55,20,1f,52,53,0e,1b,55,20,12,46,23,12,4c,21,20,1f,53,53,0e,1b,55,20,12,55,1f,54,53,0e,1b,55,20,12,55,1f,55,53,0e,1b,55,20,12,55,1f,56,53,0e,1b,55,20,12,55,1f,57,53,0e,1b,55,20,12,55,1f,58,53,0e,1b,55,20,12,55,1f,44,42,0e,1b,55,1b,40,50,12,50,30,1f,45,42,0e,20,12,50,1b,45,4a,1f,46,42,0e,1b,55,1b,40,23,12,50,21,1f,47,42,0e,1b,55,1b,40,50,12,50,30,1f,48,42,0e,20,12,50,1b,45,4a,1f,49,42,0e,20,12,50,1b,45,4a,1f,4a,42,0e,20,12,50,1b,45,4a,1f,4b,42,0e,20,12,50,1b,45,4a,1f,4c,42,0e,20,12,50,1b,45,4a,1f,4d,42,0e,20,12,50,1b,45,4a,1f,4e,42,0e,20,12,50,1b,45,4a,1f,4f,42,0e,20,12,50,1b,45,4a,1f,50,42,0e,1b,55,1b,40,23,12,50,21,1f,51,42,1b,40,4c,65,74,74,72,65,20,3a,20,12,42,1b,57,20,45,4e,56,4f,49,20,1f,53,42,0e,1b,55,1b,46,50,12,65,1f,54,42,0e,1b,56,20,12,65,1f,55,42,0e,1b,56,20,12,65,1f,56,42,0e,1b,56,20,12,65,1f,57,42,0e,1b,56,20,12,65,1f,58,42,0e,1b,55,1b,46,23,12,65,1f,48,43";
  checkScreen(vdt, 0, 0);
  if (firsttime) {
    if (isConnected) {
      retrieveDatasPENDU();
    } else {
      localDatasPendu();
    }

  } else {
    if (!refresh) {
      int current = myObject["current"];
      JSONVar lesmots = myObject["myDatas"]["mots"];
      if (current >= myObject["myDatas"]["mots"].length() - 1) {
        current = 0;
      } else {
        current++;
      }
      myObject["current"] = current;
    }
  }
}

void finOK() {
  currentEcran = "FIN";
  minitel.newXY(4, 20);
  minitel.attributs(DOUBLE_GRANDEUR);
  minitel.attributs(CARACTERE_NOIR);
  minitel.print("Vous avez gagné !");
  //effacement nav
  minitel.attributs(GRANDEUR_NORMALE);
  minitel.newXY(2, 17);
  minitel.attributs(FOND_MAGENTA);
  minitel.graphicMode();
  minitel.print(" ");
  minitel.repeat(16);
  minitel.newXY(2, 18);
  minitel.attributs(FOND_MAGENTA);
  minitel.graphicMode();
  minitel.print(" ");
  minitel.repeat(16);
  minitel.newXY(2, 17);
  minitel.textMode();
  minitel.attributs(CARACTERE_NOIR);
  minitel.print("Rejouer ");
  minitel.attributs(INVERSION_FOND);
  minitel.print(" SOMMAIRE ");
  minitel.noCursor();
}
void finKO() {
  currentEcran = "FIN";
  int current = myObject["current"];
  JSONVar motok = myObject["myDatas"]["mots"][current];
  String goodmot = "";
  int nlettres = motok.length();
  for (int i = 0; i < nlettres; i++) {
    JSONVar lettre = motok[i];
    goodmot += (const char*)lettre;
  }
  minitel.newXY(4, 20);
  minitel.attributs(DOUBLE_GRANDEUR);
  minitel.attributs(CARACTERE_NOIR);
  minitel.print("Vous avez perdu !");
  minitel.newXY(4, 23);
  minitel.attributs(GRANDEUR_NORMALE);
  minitel.attributs(CARACTERE_NOIR);
  minitel.print("Le bon mot était ");
  minitel.print(goodmot);
  //effacement nav
  minitel.attributs(GRANDEUR_NORMALE);
  minitel.newXY(2, 17);
  minitel.attributs(FOND_MAGENTA);
  minitel.graphicMode();
  minitel.print(" ");
  minitel.repeat(16);
  minitel.newXY(2, 18);
  minitel.attributs(FOND_MAGENTA);
  minitel.graphicMode();
  minitel.print(" ");
  minitel.repeat(16);
  minitel.textMode();
  minitel.newXY(2, 17);
  minitel.print("Rejouer : ");
  minitel.attributs(INVERSION_FOND);
  minitel.print(" SOMMAIRE ");
  minitel.noCursor();
}
void repetitionPENDU() {
  afficheSommairePendu(true, true);
  Serial.println(" ");
  Serial.println("repetition");
  myObject["previousProposed"] = myObject["proposed"];
  String proposed = (const char*)myObject["previousProposed"];
  afficheDatasPENDU();
  Serial.println(proposed);
  unsigned int str_len = proposed.length() + 2;
  int i = 0;
  for (i = 0; i < str_len; i += 2) {
    Serial.println(proposed.substring(i, i + 1));
  }
}
void joueLettre(String input, boolean refresh) {
  minitel.noCursor();
  String proposed = (const char*)myObject["proposed"];
  int indexProposed = proposed.indexOf(input);
  if (indexProposed > -1) {
    return;
  }
  //ecrit la lettre proposée
  int offsetX=0;
  int posY;
  posY = 8+(proposed.length()/2);
  if((proposed.length()/2)>7){
    offsetX = 6;
    posY-=8;
  }
  minitel.newXY(3+offsetX, posY);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print(input);
  
  myObject["proposed"] = proposed + input + ",";

  int current = myObject["current"];
  JSONVar motok = myObject["myDatas"]["mots"][current];
  int matches = 0;
  for (int i = 0; i < motok.length(); i++) {
    String lettre = (const char*)motok[i];
    if (lettre == input) {
      minitel.moveCursorXY(3 + i, 5);
      minitel.attributs(CARACTERE_BLANC);
      minitel.print(userInput);
      matches++;
    }
  }
  if (matches == 0) {
    int ko = myObject["ko"];
    ko++;
    myObject["ko"] = ko;
    //afficheRemoteVDT("pendu/potence-"+String(ko)+".vdt", 0, 0);
    JSONVar vdt = myObject["potence" + String(ko)];
    String potence = (const char*)vdt;
    checkScreen(potence, 0, 0);
    if (ko == 7) {
      finKO();
      return;
    }
  } else {
    int ok = myObject["ok"];
    ok += matches;
    myObject["ok"] = ok;

    if (ok == motok.length()) {
      finOK();
      return;
    }
  }
  minitel.cursor();
}
void afficheDatasPENDU() {
  minitel.noCursor();
  int current = myObject["current"];
  JSONVar motok = myObject["myDatas"]["mots"][current];
  Serial.println("mot");
  Serial.print(motok);
  myObject["potence1"] = "1f,44,5e,0e,1b,55,1b,5a,35,1f,45,5e,0e,1b,55,1b,5a,35,1f,46,5e,0e,1b,55,1b,5a,35,1f,47,5e,0e,1b,55,1b,5a,35,1f,4a,43";
  myObject["potence2"] = "1f,48,5d,0e,1b,57,1b,45,49,1b,55,1b,47,23,34,1f,49,5d,0e,1b,57,1b,45,4a,1b,55,20,1b,57,4a,1f,4a,5d,0e,1b,55,22,23,1f,4b,43";
  myObject["potence3"] = "1f,4a,5e,0e,1b,57,1b,45,48,1f,4b,5e,0e,1b,57,1b,45,4a,1f,4c,5e,0e,1b,57,1b,45,4a,1f,4d,5e,0e,1b,57,1b,45,4a,1f,4e,5e,0e,1b,55";
  myObject["potence4"] = "1f,4d,43,1f,4a,5d,0e,1b,55,42,1f,4b,5c,0e,1b,55,38,21,1f,4c,5b,0e,1b,55,26,1f,4e,43";
  myObject["potence5"] = "1f,4a,5e,0e,1b,57,1b,45,28,1f,4b,5f,0e,1b,55,29,30,1f,4c,60,0e,1b,55,22,24,1f,48,4b";
  myObject["potence6"] = "1f,4e,5d,0e,1b,55,38,1f,4f,5a,0e,1b,55,40,40,26,1f,50,5b,0e,1b,55,21,1f,49,4b";
  myObject["potence7"] = "1f,4e,5e,1f,4e,5e,0e,1b,55,29,30,1f,4f,5f,0e,1b,55,22,44,40,1f,50,61,0e,1b,55,21";

  myObject["ok"] = 0;
  myObject["ko"] = 0;
  myObject["proposed"] = "";
  minitel.newXY(3, 5);
  int n = motok.length();
  for (int i = 0; i < n; i++) {
    String lettre = (const char*)motok[i];
    if (lettre == " " || lettre == "'") {
      minitel.print(lettre);
      myObject["ok"] = 1;
    } else {
      minitel.print("-");
    }
  }
}