// GALERIE

// SETUP du service
void setupGalerie() {
  Serial.println("setup GALERIE");
  initMinitelService();
  wifiConnect();
  minitel.echo(false);
  myObject["current"] = 0;
  retrieveDatasGALERIE();
  retrieveDatasGALERIEPerso();
  afficheSommaireGalerie();
  interruption=false;
}


// LOOP du service
void loopGalerie() {
  Serial.println("loop Galerie0");
 
  while (1) {
    Serial.println("loopGalerie");
    
     if (currentEcran == "SOMMAIRE") {
      champVide(32, 23, 1);
    }
    if (currentEcran == "GALERIE") {
      if (!isVdtEnCours) {
        if (millis() >= nextUpdate) {
          nextUpdate = millis() + timeoutAmount;  // set up the next timeout period
          int currentImage = myObject["current"];
          int nbImages = myObject["datas"]["root"].length();
          if (currentImage < (nbImages - 1)) {
            currentImage++;
          } else {
            currentImage = 0;
          }
          myObject["current"] = currentImage;
          afficheDatasGALERIE();
        }
      }
    }      
      // Input
      wait_for_user_action();
      // Traitement de l'action utilisateur (maj des variables globales dans wait_for___)
      switch (touche) {

        // SI CONNEXION FIN on sort de la loop, on revient à la loop principale
        case CONNEXION_FIN:
          isVdtEnCours = false;
          interruption=true;
          Serial.println("CONNEXION_FIN");
          return;
          break;
        case SOMMAIRE:
        interruption=true;
        isVdtEnCours=false;
        afficheSommaireGalerie();
        break;
        // SI ENVOI
        case ENVOI:
          {
            if (userInput == "1") {
              myObject["galerieMode"] = "COMMUN";
            }
            if (userInput == "2") {
              const char* pseudo = (const char*)myConfig["input"][2];
              if(pseudo==""){
                ligneZeroSafe("vous n'avez pas de pseudo !");
                return;
              }
              myObject["galerieMode"] = "PERSO";
            }
            if (userInput == "1" || userInput == "2") {
              Serial.println("ici");
              currentEcran = "GALERIE";
              nextUpdate = 0;
              userInput = "";
              userInputLength = 0;
            }
          }
          break;

        case ANNULATION:
          Serial.println("ANNULATION");
          champVide(32, 23, 1);
          break;

        case GUIDE:
          if (currentEcran == "SOMMAIRE") {
            afficheGuideGalerie();
          }
        break;
        
      }
    }
  }
  // FIN LOOP

  // Fonctions d'affichage et de traitement liées au service

  void retrieveDatasGALERIE() {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String serverPath = serverName + String("galerie/getjson.php");
      http.begin(serverPath.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        String payload = http.getString();
        JSONVar myJSONObject = JSON.parse(payload);
        Serial.println("");
        Serial.print(payload);
        Serial.println("");
        myObject["datas"] = myJSONObject;
        if (JSON.typeof(myObject) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
      }
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }

  void retrieveDatasGALERIEPerso() {
    Serial.println("retrieveDatasGaleriePerso");
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      const char* pseudo = (const char*)myConfig["input"][2];
      const char* pseudopassword = (const char*)myConfig["input"][3];
      String serverPath = serverName + String("users/get_galerie_from_minimit.php?pseudo=") + String(pseudo) + String("&password=") + String(pseudopassword);
       http.begin(serverPath.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        String payload = http.getString();
        JSONVar myJSONObject = JSON.parse(payload);
        Serial.println("");
        Serial.print(payload);
        Serial.println("");
        myObject["datasperso"] = myJSONObject;
        if (JSON.typeof(myJSONObject) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
      }
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
  void afficheDatasGALERIE() {
    Serial.println("affcicheDatasGalerie");
    minitel.noCursor();
    int currentImage = myObject["current"];
    myObject["current"] = currentImage;
    String galerieMode = (const char*)myObject["galerieMode"];
    String myImage;
    if(galerieMode == "PERSO")
    {
    myImage = (const char*)myObject["datasperso"]["root"][currentImage];
    vdt = getRemoteVDT("galerieperso/" + myImage, 0, 0);
    }
    else {
    myImage = (const char*)myObject["datas"]["root"][currentImage];
    vdt = getRemoteVDT("galerie/" + myImage, 0, 0);
    }
    Serial.println(myImage);
    startVdt(vdt, 0, 0);
  }
  void afficheSommaireGalerie() {
    interruption=false;
    currentEcran = "SOMMAIRE";
    String vdt = "14,0c,1f,41,41,0e,1b,54,20,12,67,1f,42,41,0e,1b,54,20,12,67,1f,43,41,0e,1b,54,20,12,67,1f,44,41,0e,1b,54,20,12,67,1f,45,41,0e,1b,54,20,12,67,1f,46,41,0e,1b,54,20,12,67,1f,47,41,0e,1b,54,20,12,67,1f,48,41,0e,1b,54,20,12,67,1f,49,41,0e,1b,54,20,12,67,1f,4a,41,0e,1b,54,20,12,67,1f,4b,41,0e,1b,54,20,12,67,1f,4c,41,0e,1b,54,20,12,67,1f,4d,41,0e,1b,54,20,12,67,1f,4e,41,0e,1b,54,20,12,67,1f,4f,41,0e,1b,54,20,12,67,1f,50,41,0e,1b,54,20,12,67,1f,51,41,0e,1b,54,20,12,67,1f,52,41,0e,1b,54,20,12,67,1f,53,41,0e,1b,54,20,12,67,1f,54,41,0e,1b,54,20,12,67,1f,55,41,0e,1b,54,20,12,67,1f,56,41,0e,1b,54,20,12,67,1f,57,41,0e,1b,54,20,12,67,1f,58,41,0e,1b,54,20,12,67,1f,41,41,0e,1b,54,20,12,43,40,2c,30,38,44,48,20,48,2c,48,2c,30,24,1b,57,1b,44,43,1b,54,1b,47,24,48,2c,30,34,24,1b,57,1b,44,43,1b,54,1b,47,38,30,38,44,40,2c,30,1b,57,1b,44,43,1b,54,1b,47,24,38,24,20,12,43,1f,42,41,0e,1b,54,20,12,43,4a,20,20,1b,57,1b,44,4a,1b,54,1b,47,12,42,20,4a,20,4a,20,1b,57,1b,44,4a,12,42,1b,54,20,1b,47,4a,20,1b,57,1b,44,4a,1b,54,20,1b,57,4a,12,44,1b,54,1b,47,12,42,20,20,1b,57,1b,44,4a,1b,54,20,1b,57,4a,1b,54,20,20,20,20,20,1f,43,41,0e,1b,54,20,12,43,4a,40,30,1b,57,1b,44,2a,25,1b,54,1b,47,4a,20,4a,30,4a,50,25,1b,57,1b,44,4a,2a,1b,54,20,1b,47,4a,20,1b,57,1b,44,4a,1b,54,20,1b,57,4a,12,43,2a,25,1b,54,1b,47,4a,40,30,1b,57,1b,44,2a,1b,54,20,1b,47,45,20,12,44,1f,44,41,0e,1b,54,20,12,43,2a,50,25,1b,57,1b,44,4a,1b,54,1b,47,12,42,50,4a,50,4a,20,1b,57,1b,44,4a,4a,2a,1b,54,1b,47,30,4a,50,25,20,1b,57,1b,44,4a,12,44,1b,54,1b,47,4a,2a,50,25,1b,57,1b,44,2a,1b,54,1b,47,30,50,25,20,12,43,1f,45,41,0e,1b,54,20,12,42,58,1b,57,1b,44,23,12,5f,1b,54,1b,47,54,20,12,42,1f,46,41,0e,1b,54,20,12,42,1b,57,20,1b,44,40,50,12,5d,30,20,1b,54,20,20,20,1f,47,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,1b,47,30,20,12,47,1b,43,40,50,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,48,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,1b,57,25,20,1b,54,1b,47,30,20,12,45,1b,43,58,1b,53,20,20,1b,44,22,1b,54,1b,43,30,20,20,20,20,20,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,49,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,20,20,20,20,20,20,1b,57,25,20,20,20,1b,54,1b,47,30,20,12,44,1b,53,12,44,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,4a,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,20,20,20,20,20,1b,52,25,1b,47,22,1b,57,1b,42,50,58,30,40,1b,54,30,20,20,20,20,1b,53,1b,44,54,20,20,40,1b,54,1b,43,21,20,20,20,20,20,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,4b,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,20,20,20,20,1b,52,25,20,20,20,20,20,20,20,1b,54,1b,42,30,20,20,20,20,1b,43,22,23,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,4c,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,20,20,20,1b,52,25,20,20,20,20,20,20,20,20,20,1b,54,1b,42,30,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,4d,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,20,20,1b,52,25,20,20,20,20,20,20,20,20,20,20,20,1b,54,1b,42,30,20,20,20,20,20,20,20,20,1b,56,1b,44,21,1b,54,1b,46,54,20,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,4e,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,20,1b,52,25,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,54,1b,42,30,20,20,20,20,20,20,1b,56,1b,44,21,20,20,1b,54,1b,46,54,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,4f,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,54,20,1b,52,25,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,54,1b,42,30,20,20,20,20,1b,56,1b,44,21,20,20,20,20,1b,54,1b,46,54,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,50,41,0e,1b,54,20,12,42,1b,57,20,1b,44,4a,1b,52,25,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,54,1b,42,30,20,20,1b,56,1b,44,21,20,20,20,20,20,20,1b,54,1b,46,54,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,51,41,0e,1b,54,20,12,42,1b,57,20,1b,42,4a,1b,52,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,54,30,1b,56,1b,44,21,20,20,20,20,20,20,20,20,2a,1b,54,1b,47,4a,1b,57,20,1b,54,12,43,1f,52,41,0e,1b,54,20,12,42,1b,57,20,1b,42,4a,1b,52,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,56,25,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1b,57,20,1b,54,12,43,1f,53,41,0e,1b,54,20,12,42,1b,57,20,1b,42,22,23,12,52,1b,46,12,4b,21,20,1b,54,20,20,20,1f,54,41,0e,1b,54,20,12,42,1b,57,12,62,1b,54,12,43,1f,55,41,0e,1b,54,20,12,42,1b,57,12,62,1b,54,12,43,1f,56,41,0e,1b,54,20,12,42,1b,57,1b,44,54,50,12,5f,58,1b,54,20,20,20,1f,55,46,1b,4d,1b,5d,1b,44,31,1f,54,48,1b,40,4c,41,20,47,41,4c,45,52,49,45,1f,55,48,1b,40,44,55,20,4d,49,4e,49,4d,49,54,1f,55,56,1b,42,1b,4d,1b,5d,32,1f,54,58,1b,40,56,4f,54,52,45,1f,55,58,1b,40,47,41,4c,45,52,49,45,1f,57,52,56,6f,74,72,65,20,63,68,6f,69,78,20,6e,19,30,2e,1b,5d,1b,44,1b,54,1b,5a,20,1b,46,1b,5d,20,45,6e,76,6f,69,20,1f,58,46,45,6e,76,6f,79,65,72,20,76,6f,73,20,19,42,65,63,72,61,6e,73,20,76,69,64,65,6f,74,65,78,1b,44,1b,5d,20,1b,42,20,47,75,69,64,65,20,1f,57,60,11";
    checkScreen(vdt, 0, 0);
  }
  void afficheGuideGalerie() {
    String vdt = "14,1f,46,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,47,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,48,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,49,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,4a,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,4b,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,4c,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,4d,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,4e,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,4f,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,50,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,51,45,0e,1b,57,1b,44,4a,1b,54,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,1b,47,4a,1f,52,45,0e,1b,57,1b,44,22,23,12,5d,21,1f,47,47,1b,46,43,72,19,42,65,65,7a,20,76,6f,73,20,19,42,65,63,72,61,6e,73,20,61,75,20,66,6f,72,6d,61,74,1f,48,47,1b,46,76,69,64,19,42,65,6f,74,65,78,2c,20,70,61,72,20,65,78,2e,20,69,63,69,20,1f,49,47,6d,69,6e,69,74,65,6c,2e,63,71,75,65,73,74,2e,6f,72,67,1f,4b,47,1b,46,50,75,69,73,20,65,6e,76,6f,79,65,7a,2d,6c,65,73,20,73,75,72,20,76,6f,74,72,65,1f,4c,47,1b,46,65,73,70,61,63,65,20,70,65,72,73,6f,6e,6e,65,6c,20,69,63,69,1f,4d,47,6d,75,6c,74,69,70,6c,69,19,42,65,2e,66,72,2f,6d,69,6e,69,6d,69,74,2f,67,61,6c,65,72,69,65,1f,4f,47,1b,46,56,6f,74,72,65,20,67,61,6c,65,72,69,65,20,70,65,72,73,6f,20,65,73,74,20,1f,50,47,1b,46,76,69,73,69,62,6c,65,20,73,75,72,20,76,6f,74,72,65,20,6d,69,6e,69,6d,69,74,20,21,1f,57,60,11";
    checkScreen(vdt, 0, 0);
  }