void setupMeteo() {
  minitel.pageMode();
  wifiConnect();
  afficheInterfaceMeteo();
 }
void afficheInterfaceMeteo() {
  minitel.newScreen();
  afficheRemoteVDT("masquemeteo.vdt", 0, 0);
  minitel.textMode();
  minitel.moveCursorXY(15, 3);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print("Code postal : ");
  minitel.moveCursorXY(34, 3);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" ENVOI ");
}
void lectureChampMeteo(int x, int y, int longueurchamp) {
  champVide(x, y, longueurchamp);
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (userInputLength < longueurchamp) {
        userInputLength++;
        userInput += char(touche);
        Serial.print(userInput);
      }
    }
    switch (touche) {
      case CONNEXION_FIN:
        fin=true;
        minitel.connexion(false);
        displayMire();
      break;
      case ENVOI:
        fin = true;
        if (userInputLength == 5) {
          retrieveDatasMETEO();
          userInput = "";
          userInputLength = 0;
        }
        break;
      case ANNULATION:
        champVide(x, y, longueurchamp);
        userInput = "";
        userInputLength = 0;
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
          afficheInterfaceMeteo();
          if (myObject.keys().length() > 0) {
            afficheDatasMETEO();
          }
          champVide(x, y, longueurchamp);
          userInput = "";
          userInputLength = 0;
        }
        break;
      case SOMMAIRE:
        {
          userInput = "";
          userInputLength = 0;
          afficheInterfaceMeteo();
          
        }
        break;
    }
  }
}
void afficheDatasMETEO() {
  minitel.noCursor();
  effacementEcran(11, 20, CARACTERE_NOIR, FOND_NOIR);
  afficheRemoteVDT("meteo_effacement.vdt", 0, 0);
  minitel.textMode();
  JSONVar forecast = myObject["root"]["forecast"];
  JSONVar station = myObject["root"]["station"];
  JSONVar currentweather = forecast[0];
  JSONVar icon = currentweather["icon"];
  JSONVar oldMeteo = myObject["root"]["oldmeteo"];

  minitel.moveCursorXY(15, 1);
  minitel.attributs(DOUBLE_HAUTEUR);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print((const char*)station["name"]);
  String stationName = (const char*)station["name"];
  int nn = 25-stationName.length();
  minitel.print(" ");
  minitel.repeat(nn);
  
  //la date du jour
  minitel.moveCursorXY(2, 5);
  minitel.attributs(GRANDEUR_NORMALE);
  minitel.print((const char*)station["date"]);
  //Humidité
  minitel.moveCursorXY(8, 7);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print("Humidité");
  minitel.attributs(CARACTERE_BLANC);
  minitel.moveCursorXY(17, 7);
  Serial.println("humidité");
  Serial.println(currentweather["humidity"]);
  minitel.print((const char*)currentweather["humidity"]);
  //
  minitel.moveCursorXY(2, 8);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print("Précipitations");
  minitel.attributs(CARACTERE_BLANC);
  minitel.moveCursorXY(17, 8);
  //minitel.print((const char*)currentweather["humidity"]);
  Serial.print(myObject["root"]["precipitations"]);
  JSONVar precipitations = myObject["root"]["precipitations"];
  Serial.print(precipitations["mode"]);
  String modeprecipitations = (const char*)precipitations["mode"];
  String valuePrecipations = "";
  if (modeprecipitations == "no") {
    //valuePrecipations = "0";
  } else {
    // valuePrecipations = precipitations["value"];
    
  }
  //minitel.print(valuePrecipations);
  minitel.print("0mm");
  minitel.moveCursorXY(12, 9);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print("Vent");
  minitel.attributs(CARACTERE_BLANC);
  minitel.moveCursorXY(17, 9);
  minitel.print((const char*)currentweather["vent"]);

  minitel.moveCursorXY(29, 6);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(DOUBLE_HAUTEUR);
  minitel.print("min");
  minitel.attributs(CARACTERE_BLANC);
  minitel.moveCursorRight(1);
  minitel.print((const char*)currentweather["temp_min"]);

  minitel.moveCursorXY(29, 8);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(DOUBLE_HAUTEUR);
  minitel.print("max");
  minitel.attributs(CARACTERE_BLANC);
  minitel.moveCursorRight(1);
  minitel.print((const char*)currentweather["temp_max"]);

  JSONVar nextweather = forecast[1];
  JSONVar nexticon = nextweather["icon"];
  minitel.moveCursorXY(10, 11);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(GRANDEUR_NORMALE);
  minitel.attributs(FOND_NORMAL);
  //minitel.attributs(FOND_BLEU);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print((const char*)currentweather["dt"]);
  afficheRemoteVDT((const char*)icon + String(".vdt"), 11, 4);
  //
  minitel.textMode();
  minitel.moveCursorXY(30, 11);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print((const char*)nextweather["dt"]);
  afficheRemoteVDT((const char*)nexticon + String(".vdt"), 11, 24);
  minitel.textMode();
  minitel.moveCursorXY(2, 22);
  minitel.attributs(CARACTERE_BLANC);
  minitel.attributs(FOND_BLEU);
  minitel.print((const char*)oldMeteo["date"]);
  minitel.print(" à ");
  minitel.print((const char*)oldMeteo["station"]);
  minitel.moveCursorXY(2, 23);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print("Précipit. ");
  minitel.attributs(CARACTERE_BLANC);
  minitel.print((const char*)oldMeteo["pluie"]);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print(" min ");
  minitel.attributs(CARACTERE_BLANC);
  minitel.print((const char*)oldMeteo["min"]);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print(" max ");
  minitel.attributs(CARACTERE_BLANC);
  minitel.print((const char*)oldMeteo["max"]);
}
void retrieveDatasMETEO() {
  Serial.print(userInput);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("meteo/getjson.php?cp=") + userInput;
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("PAYLOADMETEO");
      Serial.print(payload);
      Serial.println("");
      //Serial.println(buf[i], HEX);
      //afficheVDT({payload}, 130, 2,2);
      myObject = JSON.parse(payload);
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      afficheDatasMETEO();
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void effacementEcranMeteo(int y1, int y2) {
  minitel.noCursor();
  minitel.attributs(FIN_LIGNAGE);
  //minitel.attributs(FOND_BLEU);
  //minitel.attributs(CARACTERE_BLEU);
  for (int i = 0; i <= (y2 - y1); i++) {
    minitel.moveCursorXY(1, (y2 - i));
    minitel.clearLine();
  }
  minitel.cursor();
}