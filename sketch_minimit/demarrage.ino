
JSONVar getConfig() {
  JSONVar config = readConfig();
  return config;
}
JSONVar readConfig() {
  File file = SPIFFS.open(confFile, "r");
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
    String myString = String(content);
    Serial.println(myString);
    theresult = JSON.parse(content);
    return theresult;
  }
}
void displayDemarrage() {

  JSONVar config = getConfig();
  myConfig = config;
  Serial.println("laconfig");
  Serial.println(myConfig);

  boolean isConfig;
  if (JSON.typeof(myConfig) == "undefined") {
    isConfig = 0;
  } else {
    isConfig = 1;
  }
  String minimitVersion;
  myOTAConfig = getOTAConfig();
  if (JSON.typeof(myOTAConfig) == "undefined") {
    writeOTAConfig("v0");
    Serial.println("ici");
    minimitVersion = "v0";
  } else {
    Serial.println("il y a un fichier de config OTA");
    Serial.println(myOTAConfig);
    minimitVersion = (const char*)myOTAConfig["version"];
  }

  String vdt = "14,0c,1f,46,54,0e,1b,46,40,50,1f,47,53,0e,1b,46,58,1b,56,20,20,1b,40,22,1b,50,1b,46,30,1f,48,51,0e,1b,46,40,1b,56,1b,40,21,20,12,43,1b,45,58,1f,49,51,0e,1b,56,1b,40,21,20,12,42,1b,45,40,1b,55,1b,46,21,1b,40,30,1f,4a,51,0e,1b,56,20,12,42,1b,45,58,1b,55,1b,40,40,1b,50,1b,45,21,4a,1f,4b,51,0e,1b,56,1b,40,30,20,20,1b,55,48,1b,50,20,20,1b,55,25,1b,50,1b,45,30,1f,4c,51,0e,1b,46,22,1b,56,1b,40,30,20,1b,55,4a,1b,50,1b,45,40,1b,55,1b,40,21,20,20,1b,50,1b,45,54,1f,4d,53,0e,1b,56,1b,40,54,1b,55,22,20,12,43,40,1b,50,1b,45,21,1f,4e,54,0e,1b,45,22,1b,55,1b,40,30,20,20,58,1f,4f,56,0e,1b,55,1b,40,54,1b,50,1b,45,21,09,09,1b,47,40,1f,50,4f,0e,4a,49,49,42,4a,49,42,4a,49,49,42,4a,21,1f,51,4f,0e,2a,12,4a,22,24";
  minitel.noCursor();
  checkScreen(vdt, 0, 0);
  minitel.attributs(CARACTERE_BLANC);
  minitel.newXY(1,24);
  minitel.print(minimitVersion);  
  minitel.echo(false);
  if (isConfig) {
    ligneZeroSafe("Test connexion wifi...");
    isConnected = checkConnexion();
    Serial.println("isConnected");
    Serial.println(isConnected);
    if(isConnected)
    {
      ligneZeroSafe("Connexion WiFi OK");
      //check last minimit version pour OTA
      JSONVar datasMaj = retrieveDatasMAJ("ota/getjson.php?currentversion="+minimitVersion);
      Serial.println(datasMaj);
      Serial.println(datasMaj["params"]["update"]);
      boolean ttt = datasMaj["params"]["update"];
      String lastVersion = (const char*)datasMaj["params"]["version"];
      if(ttt == true)
      {
        //On lance le service OTA
        
        setLastVersionOTA(lastVersion);
        launchService("OTA");
      }
  
    }
    else
    {
      Serial.println("not connectedd");
      ligneZeroSafe("Pas de connexion wifi");
    }    

    delay(3000);
    ligneZeroSafe(" ");
    minitel.echo(true);
    init_and_displayMire(0);
  } else {
    delay(1000);
    launchService("CONFIG");
  }
}


JSONVar retrieveDatasMAJ(String phpFile) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath;
    serverPath = serverName + String(phpFile);
    http.begin(serverPath.c_str());
    Serial.println(serverPath.c_str());
    int httpResponseCode = http.GET();
     if (httpResponseCode > 0) {
      String payload = http.getString();
       Serial.println(payload);
      //minitel.aiguillage(true, CODE_EMISSION_CLAVIER, CODE_RECEPTION_CLAVIER);
      // Free resources
      http.end();
      JSONVar myDatas = JSON.parse(payload);
      if (JSON.typeof(myDatas) == "undefined") {
        return false;
      } else {
        return myDatas;
      }
      
    }
  }
  return false;
}
boolean checkConnexion() {
  if (JSON.typeof(myConfig) == "undefined") {
    Serial.println("----");
    Serial.println("xixixi");
    Serial.print(WiFi.status());
    return false;
  } else {
    JSONVar config = myConfig["input"];
    const char* ssid = (const char*)config[0];
    const char* password = (const char*)config[1];
    //   const char* ssid = "Dogtown";
    //  const char* password = "west100-;";
    // const char* ssid = "Livebox-Xine";
    // const char* password = "malakoff";
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED) {
      //WiFi.begin(ssid, password);
      delay(1000);
      Serial.println(WiFi.status());
      if (cnt == 15) {
        switch (WiFi.status()) {
          case 1:
            ligneZero("Borne wifi non détectée");
            break;
          case 6:
            ligneZero("Mauvais identifiants wifi");
            break;
        }
        minitel.cursor();
        return false;
      }
      cnt++;
    }
  }
  return true;
}