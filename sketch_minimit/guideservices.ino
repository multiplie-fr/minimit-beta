// GUIDE DES SERVICES


String LocalService[] = { "ANNUAIRE",
                          "ASTRO",
                          "COUPLESPARFAITS",
                          "ELIZA",
                          "FORTUNE",
                          "GALERIE",
                          "LEMONDE",
                          "METEO",
                          "NABAZTAG",
                          "PENDU",
                          "PONG",
                          "PPP",
                          "TAROT" };

int NB_LOCAL_SERVICES = 13;




// SETUP du service
void setupGuide() {
  Serial.println("setup Guide");
  displayGuideMire();
}


// LOOP du service
void loopGuide() {
  Serial.println("loop Guide");
  champVide(11, 24, 22);
  while (1) {

    // Input
    wait_for_user_action();

    // Traitement de l'action utilisateur (maj des variables globales dans wait_for___)
    switch (touche) {

      // SI CONNEXION FIN on sort de la loop, on revient à la loop principale
      case CONNEXION_FIN:
        Serial.println("CONNEXION_FIN");
        return;
        break;

        // SI ENVOI on affiche le service
      case ENVOI:
        {
          Serial.println("ENVOI depuis guide");
          Serial.println(userInput);


          if (isValidNumber(userInput)) {
            int index_s = userInput.toInt();
            int nbWSServices = 0;
            if (WiFi.status() == WL_CONNECTED) {
              JSONVar entries = myObject["entries"];
              nbWSServices = entries.length();
            }


            if (index_s > NB_LOCAL_SERVICES + nbWSServices) {
              champVide(11, 24, 22);
              break;
              return;
            }


            if (index_s < NB_LOCAL_SERVICES + 1) {
              launchService(LocalService[index_s - 1]);
              return;
            } else {  // WS
              Serial.println("WS service");
              connectToWS(index_s - NB_LOCAL_SERVICES - 1);
              loopWS();
              init_and_displayMire(1);
              return;
            }
          } else {
            if (isIndexByKey((const String*)LocalService, userInput, NB_LOCAL_SERVICES)) {
              launchService(userInput);
              return;
            }
            else
            {
              champVide(11, 24, 22);
              break;
              return;
            }
          }
        }

        break;
      case ANNULATION:
        Serial.println("annulatoin depuis guide");
        champVide(11, 24, 22);
        break;

      case CORRECTION:
        {
          Serial.println("correction depuis guide");
          if (userInputLength > 0) {
            minitel.moveCursorLeft(1);
            minitel.print(".");
            minitel.attributs(CARACTERE_BLANC);
            minitel.moveCursorLeft(1);
            userInput = userInput.substring(0, userInput.length() - 1);
            Serial.println("userInput");
            Serial.println(userInput);
            userInputLength--;
          }
        }
        break;
    }
  }
}
// FIN LOOP

// Fonctions d'affichage et de traitement liées au service

void displayGuideMire() {
  minitel.noCursor();
  String index;
  currentEcran = "GUIDE";
  String vdt = "14,0c,1f,41,45,0e,1b,46,58,1b,56,1b,40,23,1b,50,1b,46,30,09,09,09,09,09,09,1b,47,50,40,09,30,50,09,30,50,40,09,30,54,1f,42,43,0e,1b,46,40,1b,56,1b,40,21,20,12,42,1b,50,1b,46,54,09,09,09,09,09,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,4a,1b,50,1b,47,34,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,1f,43,42,0e,1b,46,48,1b,56,20,12,43,1b,45,40,1b,55,1b,46,21,09,09,09,09,09,1b,50,1b,47,21,12,4a,22,1f,44,42,0e,1b,56,20,12,43,1b,45,58,1b,55,1b,40,40,34,1f,45,42,0e,1b,56,20,12,42,1b,55,1b,46,21,1b,40,58,1b,50,20,1b,45,4a,1f,46,42,0e,1b,56,1b,40,34,20,20,1b,55,4a,1b,50,20,1b,45,40,1b,55,1b,40,21,1b,50,1b,45,54,1f,47,43,0e,1b,56,1b,40,54,20,1b,55,4a,1b,50,1b,45,58,1b,55,20,12,42,1b,40,22,1b,50,1b,45,30,1f,48,44,0e,1b,46,22,1b,55,1b,40,30,20,12,43,58,1f,49,46,0e,1b,55,1b,40,54,20,40,1b,50,1b,45,21,1f,4a,47,0e,1b,45,22,1f,44,4e,1b,46,4c,45,20,47,55,49,44,45,20,44,45,53,20,53,45,52,56,49,43,45,53,1f,46,4e,1b,45,52,65,76,69,76,65,7a,20,6c,27,65,78,70,19,42,65,72,69,65,6e,63,65,20,64,75,20,1f,47,4e,1b,45,6d,69,6e,69,74,65,6c,20,76,69,61,20,63,65,73,20,71,75,65,6c,71,75,65,73,1f,48,4e,1b,45,73,65,72,76,69,63,65,73,2c,20,63,6f,6d,6d,65,20,64,61,6e,73,20,6c,65,73,20,1f,49,4e,1b,45,61,6e,6e,19,42,65,65,73,20,38,30,2e,1f,4a,41,1b,44,60,12,67,1f,57,41,1b,44,60,12,69,1f,58,41,1b,45,6e,19,30,20,6f,75,20,6e,6f,6d,20,2e,12,54,20,1b,47,1b,5d,20,45,4e,56,4f,49";
  checkScreen(vdt, 0, 0);
  minitel.attributs(CARACTERE_BLANC);

  // local services
  for (int i = 0; i < NB_LOCAL_SERVICES; i++) {

    if (i < 12) minitel.newXY(3, 11 + i);
    else minitel.newXY(24, 11 + i - 12);
    index = String(i + 1);
    minitel.print(index);
    minitel.print(" ");
    minitel.print(LocalService[i]);
  }

  // Ws services
  setupWS(24, 13, NB_LOCAL_SERVICES + 1);

  minitel.cursor();
}

int isIndexByKey(const String* myArray, String key, int lenght) {
  for (uint8_t i = 0; i < lenght; i++) {
    if (myArray[i] == key) {
      return true;
    }
  }

  return false;
}