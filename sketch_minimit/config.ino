void setupConfig() {
  Serial.println("muObject");
  Serial.println(myObject);
  minitel.pageMode();
  JSONVar config = readConfig();
  Serial.print("config");
  Serial.print(config);
  myObject["currentLine"] = (int)0;
  char json[] = "[[14, 12, 27],[14, 13, 27],[14, 15, 27],[14, 16, 27]]";
  JSONVar coords = JSON.parse(json);
  myObject["coords"] = coords;
  char jsond[] = "[\"\", \"\", \"\", \"\"]";
  JSONVar input = JSON.parse(jsond);
// on cherche les codes wifi dans le fichier config.txt
  myObject["wifiMessage"]="";
  Serial.println(JSON.typeof(config));
  if (JSON.typeof(config) == "object") {
    JSONVar sauvInput = config["input"];
    myObject["input"]=sauvInput;
    JSONVar sauvPseudo = config["pseudo"];
    myObject["pseudo"]=sauvPseudo;
  }
  else {
    myObject["input"]=input;
    myObject["pseudo"]="";
  }
  Serial.print(myObject);
  displayConfig();
}

JSONVar getConfig(){
 JSONVar config = readConfig();
 return config;
}
JSONVar readConfig() {
  File file = SPIFFS.open(confFile, "r");
  //Serial.println(file);
  JSONVar theresult = {};
  if (!file) {
    Serial.println("Failed to open file for reading");
    return theresult;
  } else {
    int s = file.size();
    char content[s + 1] = {};
    int i = 0;
    while (file.available()) {
      char c = file.read();
      content[i] = c;
      i++;
    }
    //Serial.println(content);
    //Serial.println("");
    String myString = String(content);
    Serial.println(myString);
    theresult = JSON.parse(content);
    //Serial.println(theresult);
    return theresult;
  }
}
void displayConfig() {
  minitel.noCursor();
  String vdt = "14,0c,1f,41,45,0e,1b,46,58,1b,56,1b,40,23,1b,50,1b,46,30,09,09,09,09,09,09,1b,47,50,40,09,30,50,09,30,50,40,09,30,54,1f,42,43,0e,1b,46,40,1b,56,1b,40,21,20,12,42,1b,50,1b,46,54,09,09,09,09,09,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,4a,1b,50,1b,47,34,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,1f,43,42,0e,1b,46,48,1b,56,20,12,43,1b,45,40,1b,55,1b,46,21,09,09,09,09,09,1b,50,1b,47,21,12,4a,22,1f,44,42,0e,1b,56,20,12,43,1b,45,58,1b,55,1b,40,40,34,1f,45,42,0e,1b,56,20,12,42,1b,55,1b,46,21,1b,40,58,1b,50,20,1b,45,4a,1f,46,42,0e,1b,56,1b,40,34,20,20,1b,55,4a,1b,50,20,1b,45,40,1b,55,1b,40,21,1b,50,1b,45,54,1f,47,43,0e,1b,56,1b,40,54,20,1b,55,4a,1b,50,1b,45,58,1b,55,20,12,42,1b,40,22,1b,50,1b,45,30,1f,48,44,0e,1b,46,22,1b,55,1b,40,30,20,12,43,58,1f,49,46,0e,1b,55,1b,40,54,20,40,1b,50,1b,45,21,1f,4a,47,0e,1b,45,22,1f,44,4e,1b,46,4c,41,20,43,4f,4e,46,49,47,1f,4b,41,1b,45,1b,5d,18,20,12,4c,4d,4f,4e,20,57,49,46,49,1f,4c,41,1b,46,20,4e,6f,6d,20,64,75,20,77,69,66,69,3a,0a,0d,4d,6f,74,20,64,65,20,70,61,73,73,65,3a,1f,4e,41,1b,45,1b,5d,18,20,12,4c,4d,4f,4e,20,50,53,45,55,44,4f,1f,4f,41,1b,46,20,12,45,50,73,65,75,64,6f,3a,0d,0a,4d,6f,74,20,64,65,20,70,61,73,73,65,3a,1f,55,41,1b,44,60,12,67,1f,56,4e,1b,45,45,6e,74,72,65,20,6c,65,73,20,6c,69,67,6e,65,73,20,20,19,2e,20,1b,5d,20,53,55,49,54,45,20,1f,57,4e,1b,44,60,12,5a,1f,58,4e,1b,45,56,61,6c,69,64,65,72,20,6c,61,20,63,6f,6e,66,69,67,20,19,2e,20,1b,47,1b,5d,20,45,4e,56,4f,49";
  checkScreen(vdt, 0, 0);
  minitel.echo(false);
  minitel.textMode();
  myObject["currentLine"] = 0;
  minitel.echo(true);
  afficheConfigChampsSaisie();
}
void gereTouchesConfig() {
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
     case SOMMAIRE:
        minitel.connexion(false);
        displayMire();
      break;
    case SUITE:
      {
        int currentLine = myObject["currentLine"];
        if (currentLine < 3) {
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
      break;
    case RETOUR:
      {
        int currentLine = myObject["currentLine"];
        Serial.println(currentLine);
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
        if (nbCaracteres > 0) {
          minitel.moveCursorLeft(1);
          minitel.attributs(saisieColor);
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
        JSONVar myconfig ={};
        Serial.println(myObject);
        File file = SPIFFS.open(confFile, "w");
        if (!file) {
          // File not found | le fichier de test n'existe pas
          Serial.println("Failed to open test file");
          return;
        } else {
          JSONVar input = myObject["input"];
          myconfig["input"] = input; 
          myconfig["pseudo"] =  (const char*)myObject["input"][2];         
          file.print(myconfig);
          file.close();
          checkWifi();          
        }
        //test connexion
      }
      break;
      case GUIDE:
      String vdt = "";
      break;
      }
}
void checkWifi(){
    minitel.noCursor();
    JSONVar config = myObject["input"];
    const char* ssid = (const char*)config[0];
    const char* password = (const char*)config[1];
    // //String wifistatus = wifiConnect();  
    WiFi.begin(ssid, password);
    minitel.newXY(1, 19);
    minitel.print("Connexion à ");
    minitel.print(ssid);
    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(1000);
      minitel.print(".");
      Serial.println(WiFi.status());
      if(cnt==10)
      {
        effacementEcran(17, 19, CARACTERE_NOIR, FOND_NOIR);
        minitel.moveCursorXY(1, 19);
        minitel.print("Erreur : ");
        switch(WiFi.status()){
          case 1:
          minitel.print("Borne wifi non détectée");
          break;
          case 6:
          minitel.print("Mauvais nom / mot de passe wifi");
          break;
        }
        minitel.cursor();
        return;
      }
      cnt ++;
    }
    Serial.println(cnt);
    minitel.moveCursorXY(1, 19);
    effacementEcran(17, 19, CARACTERE_NOIR, FOND_NOIR);
    minitel.attributs(CARACTERE_BLANC);
    minitel.print("Minimit connecté avec IP ");
    Serial.println(WiFi.localIP());
    minitel.print(WiFi.localIP().toString().c_str());
    minitel.echo(false);
    minitel.noCursor();
    isConnected = true;
    delay(1000);
    //le psuedo
    String pseudo = (const char*)config[2];
    String pseudopassword = (const char*)config[3];
    String oldpseudo = (const char*)myObject["pseudo"];
    if(pseudo=="" || pseudopassword == "")
    {

    }
    else
    {
      retrieveDatasPseudo(pseudo, pseudopassword, oldpseudo);
    }
}
void pseudoOK(){
    effacementEcran(19, 21, CARACTERE_NOIR, FOND_NOIR);
    minitel.attributs(CARACTERE_BLANC);
    minitel.print("Pseudo OK ! ");
    minitel.attributs(INVERSION_FOND);
    minitel.print(" SOMMAIRE ");
}
void pseudoKO(){
    effacementEcran(19, 21, CARACTERE_NOIR, FOND_NOIR);
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
  myObject["currentLine"]= 2;
}
void afficheConfigChampsSaisie() {
  JSONVar coords = myObject["coords"];
  JSONVar input = myObject["input"];
  Serial.print(myObject);
  int nc = coords.length();
  Serial.println("nc");
  Serial.print(nc);
  for (int i = 0; i < nc; i++) {
    champVide(coords[i][0], coords[i][1], coords[i][2]);
    minitel.print((const char*)input[i]);
  }
  int nx = coords[0][0];
  int offsetX = strlen((const char*)input[0]);
  minitel.moveCursorXY(nx+offsetX, coords[0][1]);
  minitel.cursor();
  minitel.echo(true);
}
void retrieveDatasPseudo(String pseudo, String passwd, String oldpseudo) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("users/login.php?pseudo=") + pseudo + String("&password="+passwd) + String("&oldpseudo="+oldpseudo);
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("payload");
      Serial.print(payload);
      Serial.println("");
      if (payload=="pwko")
      {
        pseudoKO();
      }
      else {
        pseudoOK();
      }
   }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}