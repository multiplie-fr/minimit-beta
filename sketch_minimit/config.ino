void setupConfig() {

  minitel.pageMode();
  JSONVar config = readConfig();
  myObject["currentLine"] = (int)0;
  char json[] = "[[14, 12, 27],[14, 13, 27],[14, 15, 27],[14, 16, 27]]";
  JSONVar coords = JSON.parse(json);
  myObject["coords"] = coords;
  char jsond[] = "[\"\", \"\", \"\", \"\"]";
  JSONVar input = JSON.parse(jsond);
  // on cherche les codes wifi dans le fichier config.txt
  myObject["wifiMessage"] = "";
  if (JSON.typeof(config) == "object") {
    JSONVar sauvInput = config["input"];
    myObject["input"] = sauvInput;
    JSONVar sauvPseudo = config["pseudo"];
    myObject["pseudo"] = sauvPseudo;
  } else {
    myObject["input"] = input;
    myObject["pseudo"] = "";
  }
  displayConfig();
}

void displayConfig() {

  minitel.noCursor();
  String vdt = "14,0c,1f,41,45,0e,1b,46,58,1b,56,1b,40,23,1b,50,1b,46,30,09,09,09,09,09,09,1b,47,50,40,09,30,50,09,30,50,40,09,30,54,1f,42,43,0e,1b,46,40,1b,56,1b,40,21,20,20,20,1b,50,1b,46,54,09,09,09,09,09,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,4a,1b,50,1b,47,34,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,1f,43,42,0e,1b,46,48,1b,56,20,20,20,20,1b,45,40,1b,55,1b,46,21,09,09,09,09,09,1b,50,1b,47,12,4b,22,1f,44,42,0e,1b,56,20,12,43,1b,45,58,1b,55,1b,40,40,34,1f,45,42,0e,1b,56,20,12,42,1b,55,1b,46,21,1b,40,58,1b,50,20,1b,45,4a,1f,46,42,0e,1b,56,1b,40,34,20,20,1b,55,4a,1b,50,20,1b,45,40,1b,55,1b,40,21,1b,50,1b,45,54,1f,47,43,0e,1b,56,1b,40,54,20,1b,55,4a,1b,50,1b,45,58,1b,55,20,20,20,1b,40,22,1b,50,1b,45,30,1f,48,44,0e,1b,46,22,1b,55,1b,40,30,20,20,20,20,58,1f,49,46,0e,1b,55,1b,40,54,20,40,1b,50,1b,45,21,1f,4a,47,0e,1b,45,22,1f,42,5b,1b,46,4c,41,20,43,4f,4e,46,49,47,1f,44,4e,1b,45,50,6f,75,72,20,71,75,65,20,4d,69,6e,69,6d,69,74,20,66,6f,6e,63,74,69,6f,6e,6e,65,1f,45,4e,1b,45,61,75,20,6d,69,65,75,78,2c,20,63,6f,6e,6e,65,63,74,65,7a,2d,6c,65,20,19,41,61,20,1f,46,4e,1b,45,76,6f,74,72,65,20,77,69,66,69,2c,20,6d,19,43,65,6d,65,20,73,27,69,6c,20,70,65,75,74,20,1f,47,4e,1b,45,73,27,65,6e,20,70,61,73,73,65,72,20,70,6f,75,72,20,71,75,65,6c,71,75,65,73,1f,48,4e,1b,45,73,65,72,76,69,63,65,73,2e,1f,4b,41,1b,45,1b,5d,18,20,12,4c,4d,4f,4e,20,52,45,53,45,41,55,1f,4c,41,1b,46,20,4e,6f,6d,20,64,75,20,77,69,66,69,3a,0a,0d,4d,6f,74,20,64,65,20,70,61,73,73,65,3a,1f,4e,41,1b,45,1b,5d,18,20,12,4c,4d,4f,4e,20,50,52,4f,46,49,4c,1f,4f,41,1b,46,20,12,45,50,73,65,75,64,6f,3a,0a,0d,4d,6f,74,20,64,65,20,70,61,73,73,65,3a,1f,52,41,1b,54,20,18,0a,1b,54,08,1b,54,20,18,0a,08,1b,54,20,18,1f,55,41,1b,44,60,12,67,1f,56,4d,1b,45,45,6e,74,72,65,20,6c,65,73,20,6c,69,67,6e,65,73,20,20,19,2e,1b,5a,20,1b,5d,20,53,55,49,54,45,20,20,1f,57,4d,56,61,6c,69,64,65,72,20,6c,61,20,63,6f,6e,66,69,67,20,19,2e,1b,5a,20,1b,47,1b,5d,20,45,4e,56,4f,49,20,20,1f,58,4d,1b,45,4e,65,20,70,61,73,20,63,6f,6e,66,69,67,75,72,65,72,20,19,2e,20,1b,5d,53,4f,4d,4d,41,49,52,45,1f,4c,4e,11";
  checkScreen(vdt, 0, 0);
  minitel.echo(false);
  minitel.textMode();
  myObject["currentLine"] = 0;
  afficheConfigChampsSaisie();
  minitel.echo(true);
}
void configSuite() {

  int currentLine = myObject["currentLine"];
  if (currentLine < 3) {
    myObject["input"][currentLine] = (const char*)myObject["input"][currentLine]+userInput;
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
void configRetour() {

  int currentLine = myObject["currentLine"];
  myObject["input"][currentLine] = (const char*)myObject["input"][currentLine]+userInput;
  if (currentLine > 0) {
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
void loopConfig() {

  while (1) {
    wait_for_user_action();
    switch (touche) {
      case CONNEXION_FIN:
      case SOMMAIRE:
        return;
        break;
      
      case SUITE:
        {
          configSuite();
        }
        break;
      case RETOUR:
        {
          configRetour();
        }
        break;
      case ANNULATION:
        {
          int currentLine = myObject["currentLine"];
          JSONVar coords = myObject["coords"][currentLine];
          myObject["input"][currentLine] = "";
          champVide(myObject["coords"][currentLine][0], myObject["coords"][currentLine][1], myObject["coords"][currentLine][2]);
        }
        break;
      case CORRECTION:
        {
         int currentLine = myObject["currentLine"];
          String acorriger = (const char*)myObject["input"][currentLine];
          int nbCaracteres = acorriger.length()+userInput.length();
          if (nbCaracteres > 0) {
            minitel.moveCursorLeft(1);
            minitel.attributs(saisieColor);
            minitel.print(".");
            minitel.attributs(CARACTERE_BLANC);
            minitel.moveCursorLeft(1);
            //userInput = userInput.substring(0, userInput.length() - 1);
            acorriger = acorriger.substring(0, acorriger.length() - 1);
            myObject["input"][myObject["currentLine"]] = acorriger;
          }
        }
        break;
      case ENVOI:
        {
          int currentLine = myObject["currentLine"];
          myObject["input"][currentLine] = (const char*)myObject["input"][currentLine]+userInput;
          JSONVar myconfig = {};
          File file = SPIFFS.open(confFile, "w");
          if (!file) {
            Serial.println("Failed to write config file");
            return;
          } else {
            JSONVar input = myObject["input"];
            myconfig["input"] = input;
            myconfig["pseudo"] = (const char*)myObject["input"][2];
            file.print(myconfig);
            myConfig = myconfig;

            file.close();
            checkWifi();
            return;
          }
        }
        break;
    }
  }

}
void checkWifi() {

  minitel.noCursor();
  JSONVar config = myObject["input"];
  const char* ssid = (const char*)config[0];
  const char* password = (const char*)config[1];
  
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  effacementEcran(18,19, CARACTERE_BLEU, FOND_BLEU);
  minitel.newXY(2, 19);
  minitel.print("Connexion à ");
  minitel.print(ssid);
  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(1000);
    minitel.print(".");
    Serial.println("Wifi status "+WiFi.status());
    if (cnt == 10) {
      effacementEcran(18, 19, CARACTERE_BLEU, FOND_BLEU);
      minitel.moveCursorXY(2, 19);
      minitel.attributs(CARACTERE_BLANC);
      minitel.print("Erreur : ");
      Serial.println("Erreur connexion");
      switch (WiFi.status()) {
        case 1:
          minitel.print("Borne wifi non détectée");
          myObject["currentLine"] = 1;
          configRetour();
          break;
        case 6:
          minitel.print("Mauvais nom/mot de passe wifi");
          myObject["currentLine"] = 1;
          configRetour();
          break;
      }
      minitel.cursor();

      effacementEcran(18, 19, CARACTERE_BLEU, FOND_BLEU);
      minitel.moveCursorXY(2, 19);
      minitel.attributs(CARACTERE_BLANC);
      minitel.print("Minimit non connecté");
      delay(2000);


      return; // on abandonne le Wifi
    }
    cnt++;
  }
  effacementEcran(18, 19, CARACTERE_BLEU, FOND_BLEU);
  minitel.moveCursorXY(2, 19);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print("Minimit connecté");
  minitel.echo(false);
  minitel.noCursor();
  isConnected = true;
  delay(1000);

  // CHECK OTA HERE
  Serial.println("Config : WIFI connecté, OTA ?");
  check_and_launch_OTA(get_minimitVersion());

  //le psuedo
  String pseudo = (const char*)config[2];
  String pseudopassword = (const char*)config[3];
  String oldpseudo = (const char*)myObject["pseudo"];
  if (pseudo == "" || pseudopassword == "") {
    delay(1000);
    return;
  } else {
    retrieveDatasPseudo(pseudo, pseudopassword, oldpseudo);
  }
}
void pseudoOK() {

  effacementEcran(18,19, CARACTERE_BLEU, FOND_BLEU);
  minitel.moveCursorXY(2, 19);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print("Très bon pseudo. ");
  minitel.attributs(INVERSION_FOND);
  minitel.print("SOMMAIRE pour continuer");
  delay(1000);
}
void pseudoKO() {
  effacementEcran(18,19, CARACTERE_BLEU, FOND_BLEU);
   minitel.moveCursorXY(2, 19);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print("Ce pseudo existe déjà ! ");
  JSONVar coords = myObject["coords"];
  myObject["input"][2] = "";
  myObject["input"][3] = "";
  JSONVar input = myObject["input"];
  for (int i = 2; i < 4; i++) {
    champVide(coords[i][0], coords[i][1], coords[i][2]);
    minitel.print((const char*)input[i]);
  }
  minitel.moveCursorXY(coords[2][0], coords[2][1]);
  myObject["currentLine"] = 2;
}
void afficheConfigChampsSaisie() {
  JSONVar coords = myObject["coords"];
  JSONVar input = myObject["input"];
  int nc = coords.length();
  for (int i = 0; i < nc; i++) {
    champVide(coords[i][0], coords[i][1], coords[i][2]);
    minitel.print((const char*)input[i]);
  }
  userInput = (const char*)input[0];
  
  int nx = coords[0][0];
  int offsetX = strlen((const char*)input[0]);
  minitel.moveCursorXY(nx + offsetX, coords[0][1]);
  minitel.cursor();
  minitel.echo(true);
}
void retrieveDatasPseudo(String pseudo, String passwd, String oldpseudo) {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("users/login_from_minimit.php?pseudo=") + pseudo + String("&password=" + passwd) + String("&oldpseudo=" + oldpseudo);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      if (payload == "pwko") {
        pseudoKO();
      } else {
        pseudoOK();
      }
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected : can't retrieveDatasPseudo");
  }
}