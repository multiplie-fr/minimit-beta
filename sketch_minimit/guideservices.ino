// GUIDE DES SERVICES

// SETUP du service
void setupGuide() {
  Serial.println("setup Guide");
  initMinitelService();
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
          int isConnexionNeeded = launchService(userInput);
          if (isConnexionNeeded == 1) {
            ligneZeroSafe("Connexion wifi nécessaire");
            delay(2000);
            ligneZeroSafe(" ");
            return;
          }
          if (isConnexionNeeded == 2) {
            ligneZeroSafe("Service inconnu");
            delay(2000);
            ligneZeroSafe(" ");
            return;
          }
          return;
        }

        break;
      case ANNULATION:
        Serial.println("annulatoin depuis guide");
        champVide(11, 24, 22);
        break;
    }
  }
}
// FIN LOOP

// Fonctions d'affichage et de traitement liées au service

void displayGuideMire() {
  minitel.noCursor();
  currentEcran = "GUIDE";
  String vdt = "14,0c,1f,41,45,0e,1b,46,58,1b,56,1b,40,23,1b,50,1b,46,30,09,09,09,09,09,09,1b,47,50,40,09,30,50,09,30,50,40,09,30,54,1f,42,43,0e,1b,46,40,1b,56,1b,40,21,20,12,42,1b,50,1b,46,54,09,09,09,09,09,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,4a,1b,50,1b,47,34,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,1f,43,42,0e,1b,46,48,1b,56,20,12,43,1b,45,40,1b,55,1b,46,21,09,09,09,09,09,1b,50,1b,47,21,12,4a,22,1f,44,42,0e,1b,56,20,12,43,1b,45,58,1b,55,1b,40,40,34,1f,45,42,0e,1b,56,20,12,42,1b,55,1b,46,21,1b,40,58,1b,50,20,1b,45,4a,1f,46,42,0e,1b,56,1b,40,34,20,20,1b,55,4a,1b,50,20,1b,45,40,1b,55,1b,40,21,1b,50,1b,45,54,1f,47,43,0e,1b,56,1b,40,54,20,1b,55,4a,1b,50,1b,45,58,1b,55,20,12,42,1b,40,22,1b,50,1b,45,30,1f,48,44,0e,1b,46,22,1b,55,1b,40,30,20,12,43,58,1f,49,46,0e,1b,55,1b,40,54,20,40,1b,50,1b,45,21,1f,4a,47,0e,1b,45,22,1f,44,4e,1b,46,4c,45,20,47,55,49,44,45,20,44,45,53,20,53,45,52,56,49,43,45,53,1f,46,4e,1b,45,52,65,76,69,76,65,7a,20,6c,27,65,78,70,19,42,65,72,69,65,6e,63,65,20,64,75,20,1f,47,4e,1b,45,6d,69,6e,69,74,65,6c,20,76,69,61,20,63,65,73,20,71,75,65,6c,71,75,65,73,1f,48,4e,1b,45,73,65,72,76,69,63,65,73,2c,20,63,6f,6d,6d,65,20,64,61,6e,73,20,6c,65,73,20,1f,49,4e,1b,45,61,6e,6e,19,42,65,65,73,20,38,30,2e,1f,4a,41,1b,44,60,12,67,1f,57,41,1b,44,60,12,69,1f,58,41,1b,45,6e,19,30,20,6f,75,20,6e,6f,6d,20,2e,12,54,20,1b,47,1b,5d,20,45,4e,56,4f,49";
  checkScreen(vdt, 0, 0);
  minitel.attributs(CARACTERE_BLANC);
  minitel.newXY(3, 11);
  minitel.print("1 ANNUAIRE");
  minitel.newXY(3, 12);
  minitel.print("2 ASTRO");
  minitel.newXY(3, 13);
  minitel.print("3 COUPLESPARFAITS");
  minitel.newXY(3, 14);
  minitel.print("4 ELIZA");
  minitel.newXY(3, 15);
  minitel.print("5 FORTUNE");
  minitel.newXY(3, 16);
  minitel.print("6 GALERIE");
  minitel.newXY(3, 17);
  minitel.print("7 LEMONDE");
  minitel.newXY(3, 18);
  minitel.print("8 METEO");
  minitel.newXY(3, 19);
  minitel.print("9 NABAZTAG");
  minitel.newXY(2, 20);
  minitel.print("10 PENDU");
  minitel.newXY(2, 21);
  minitel.print("11 PONG");
  minitel.newXY(2, 22);
  minitel.print("12 PPP (pluie pas pluie ?)");
  minitel.newXY(24, 11);
  minitel.print("13 TAROT");
  minitel.newXY(24, 12);
  minitel.print("AMIS");
  retrieveDatasWS();
  int nextNum = 14;
  Serial.println(myObject);
  JSONVar entries = myObject["ws"]["entries"];
  int n = entries.length();
  int posY = 14;
  for (int i = 0; i<n; i++){
      minitel.newXY(24, posY);
      minitel.print(String(i+nextNum));
      minitel.newXY(24,posY);
      Serial.println(entries[i]["name"]);
      minitel.print((const char*)entries[i]["name"]);
      posY ++;
  }
  
  minitel.cursor();
}