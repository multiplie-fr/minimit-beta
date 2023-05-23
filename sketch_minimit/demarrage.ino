
JSONVar readConfig() {
  Serial.println(__func__);
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
    theresult = JSON.parse(content);
    return theresult;
  }
}
void displayDemarrage() {
  Serial.println(__func__);

  JSONVar config = readConfig();
  myConfig = config;
  Serial.println("displayDemarrage, config "+myConfig);

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
    minimitVersion = "v0";
  } else {
    minimitVersion = (const char*)myOTAConfig["version"];
    Serial.println("Current version OTA"+minimitVersion);
  }

  String vdt = "14,0c,1f,46,54,0e,1b,46,40,50,1f,47,53,0e,1b,46,58,1b,56,20,20,1b,40,22,1b,50,1b,46,30,1f,48,51,0e,1b,46,40,1b,56,1b,40,21,20,12,43,1b,45,58,1f,49,51,0e,1b,56,1b,40,21,20,12,42,1b,45,40,1b,55,1b,46,21,1b,40,30,1f,4a,51,0e,1b,56,20,12,42,1b,45,58,1b,55,1b,40,40,1b,50,1b,45,21,4a,1f,4b,51,0e,1b,56,1b,40,30,20,20,1b,55,48,1b,50,20,20,1b,55,25,1b,50,1b,45,30,1f,4c,51,0e,1b,46,22,1b,56,1b,40,30,20,1b,55,4a,1b,50,1b,45,40,1b,55,1b,40,21,20,20,1b,50,1b,45,54,1f,4d,53,0e,1b,56,1b,40,54,1b,55,22,20,12,43,40,1b,50,1b,45,21,1f,4e,54,0e,1b,45,22,1b,55,1b,40,30,20,20,58,1f,4f,56,0e,1b,55,1b,40,54,1b,50,1b,45,21,09,09,1b,47,40,1f,50,4f,0e,4a,49,49,42,4a,49,42,4a,49,49,42,4a,21,1f,51,4f,0e,2a,12,4a,22,24";
  minitel.noCursor();
  checkScreen(vdt, 0, 0);
  minitel.attributs(CARACTERE_BLANC);
  minitel.newXY(1,24);
  minitel.print(minimitVersion);  
  minitel.echo(false);
  if (isConfig) {
    isConnected = checkConnexion();
    if(isConnected)
    {
      Serial.println("Connexion WiFi OK");

      //check last minimit version pour OTA
      ligneZeroSafe("Recherche des mises à jour ...");

      JSONVar datasMaj = retrieveDatasMAJ("ota/getjson.php?currentversion="+minimitVersion);
      boolean flag_ota = datasMaj["params"]["update"];
      String lastVersion = (const char*)datasMaj["params"]["version"];
      if(flag_ota == true)
      {
        //On lance le service OTA
        Serial.println("Launching OTA");
        setLastVersionOTA(lastVersion);
        launchService("OTA");
      }
      ligneZeroSafe("");

  
    }
    else
    {
      Serial.println("Wifi not connected");
    }    

    minitel.echo(true);
    return;
  } else {
    launchService("CONFIG");
  }
}


JSONVar retrieveDatasMAJ(String phpFile) {
    Serial.println(__func__);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath;
    serverPath = serverName + String(phpFile);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
     if (httpResponseCode > 0) {
      String payload = http.getString();
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
    Serial.println(__func__);

  if (JSON.typeof(myConfig) == "undefined") {
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
      Serial.println("Wifi status "+WiFi.status());
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