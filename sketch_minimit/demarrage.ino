JSONVar readConfig() {
  File file = SPIFFS.open(confFile, "r");
  JSONVar theresult = {};
  if (!file) {
    Serial.println("SPIFFS error ...");
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

  JSONVar config = readConfig();
  myConfig = config;
  String minimitVersion;

  boolean isConfig;
  if (JSON.typeof(myConfig) == "undefined") {
    isConfig = 0;
  } else {
    isConfig = 1;
  }


  minimitVersion = get_minimitVersion();
  unsigned long value = minitel.identifyDevice();
  byte arr[3];

  arr[0] = value & 0xFF;          // 0x78
  arr[1] = (value >> 8) & 0xFF;   // 0x56
  arr[2] = (value >> 16) & 0xFF;  // 0x34


  //le modele du minitel
  String stringDevice = String(arr[1], HEX);
  String minitelModele = "unknown";
  int modeleInt = stringDevice.toInt();


  switch (modeleInt) {
    case 62:
    case 63:
    case 72:
      minitelModele = "Minitel 1";
      break;
    case 73:
      minitelModele = "Minitel 1 Couleur";
      break;
    case 64:
    case 66:
      minitelModele = "Minitel 10";
      break;
    case 75:
      minitelModele = "Minitel 1 Bistandard";
      break;
    case 76:
      minitelModele = "Minitel 2";
      break;
    case 77:
      minitelModele = "Minitel 10 Bistandard";
      break;
    case 122:
      minitelModele = "Minitel 12";
      break;
    case 79:
      minitelModele = "Minitel 5";
      break;
  }

  String vdt = "14,0c,1f,46,54,0e,1b,46,40,50,1f,47,53,0e,1b,46,58,1b,56,20,20,1b,40,22,1b,50,1b,46,30,1f,48,51,0e,1b,46,40,1b,56,1b,40,21,20,12,43,1b,45,58,1f,49,51,0e,1b,56,1b,40,21,20,12,42,1b,45,40,1b,55,1b,46,21,1b,40,30,1f,4a,51,0e,1b,56,20,12,42,1b,45,58,1b,55,1b,40,40,1b,50,1b,45,21,4a,1f,4b,51,0e,1b,56,1b,40,30,20,20,1b,55,48,1b,50,20,20,1b,55,25,1b,50,1b,45,30,1f,4c,51,0e,1b,46,22,1b,56,1b,40,30,20,1b,55,4a,1b,50,1b,45,40,1b,55,1b,40,21,20,20,1b,50,1b,45,54,1f,4d,53,0e,1b,56,1b,40,54,1b,55,22,20,12,43,40,1b,50,1b,45,21,1f,4e,54,0e,1b,45,22,1b,55,1b,40,30,20,20,58,1f,4f,56,0e,1b,55,1b,40,54,1b,50,1b,45,21,09,09,1b,47,40,1f,50,4f,0e,4a,49,49,42,4a,49,42,4a,49,49,42,4a,21,1f,51,4f,0e,2a,12,4a,22,24";
  minitel.noCursor();
  checkScreen(vdt, 0, 0);
  minitel.attributs(CARACTERE_BLANC);
  minitel.newXY(1, 24);
  minitel.print(minimitVersion);
  minitel.print(" ");
  minitel.print(minitelModele);
  minitel.echo(false);
  if (isConfig) {
    isConnected = checkConnexion();
    if (isConnected) {
      Serial.println("Connexion WiFi OK");
      check_and_launch_OTA(minimitVersion);
    } else {
      Serial.println("Wifi not connected");
    }

    minitel.echo(true);
    return;
  } else {
    launchService("CONFIG");
  }
}


JSONVar retrieveDatasMAJ(String phpFile) {
  JSONVar badreturn = {};

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath;
    serverPath = serverName + String(phpFile);
    Serial.println("retreiveDatamaj");
    Serial.println(serverPath);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    Serial.println("https");
    Serial.println(httpResponseCode);
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("payload");
      Serial.println(payload);
      http.end();
      JSONVar myDatas = JSON.parse(payload);
      if (JSON.typeof(myDatas) == "undefined") {
        return badreturn;
      } else {
        return myDatas;
      }

    } else {
      return badreturn;
    }
  }
  return badreturn;
}

void check_and_launch_OTA(String minimitVersion) {
  const char* pseudo = (const char*)myConfig["input"][2];
  Serial.println("pseudoChackOTA");
  Serial.println(pseudo);
  Serial.println("checkAndLaucheOTA");
  ligneZeroSafe("Recherche des mises à jour ...");

  JSONVar datasMaj = retrieveDatasMAJ("ota/getjson.php?currentversion=" + minimitVersion + "&pseudo=" + pseudo);
  boolean flag_ota;
  if (JSON.typeof_(datasMaj) == "undefined") {
    ligneZeroSafe(" ");
    return;
  } else {
    flag_ota = datasMaj["params"]["update"];
  }
  String lastVersion = (const char*)datasMaj["params"]["version"];
  if (flag_ota == true) {
    //On lance le service OTA
    ligneZeroSafe("");
    Serial.println("Launching OTA");
    setLastVersionOTA(lastVersion);
    setupOTA();
    loopOTA();
  }
  ligneZeroSafe("");
}

String get_minimitVersion() {

  String minimitVersion;
  myOTAConfig = getOTAConfig();
  if (JSON.typeof(myOTAConfig) == "undefined") {
    writeOTAConfig("v0");
    minimitVersion = "v0";
  } else {
    minimitVersion = (const char*)myOTAConfig["version"];
    Serial.println("Current version OTA=" + minimitVersion);
  }

  return minimitVersion;
}

boolean checkConnexion() {
  Serial.println("checkConnexion");
  Serial.println(myConfig);

  if (JSON.typeof(myConfig) == "undefined") {
    Serial.println("undefined");
    return false;
  } else {
    JSONVar config = myConfig["input"];
    const char* ssid = (const char*)config[0];
    const char* password = (const char*)config[1];
    Serial.println(ssid);
    Serial.println(password);
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("wifiStatus");
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