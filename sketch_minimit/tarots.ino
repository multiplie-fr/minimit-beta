// Tarots

void setupTarots() {
  wifiConnect();
  minitel.noCursor();
  minitel.echo(false);
  displaySommaireTarots();
  TAROTS_retrieveDatas();

}

void loopTarots() {
  while (1) {

	  wait_for_user_action();

    switch (touche) {
      case CONNEXION_FIN:
        return;
        break;
      case SUITE:
        {
         Serial.println(currentEcran);
          if (currentEcran == "SOMMAIRE") {
            retourneCartes();
          } else {
            suiteTarots();
          }
        }
        //
        break;
      case RETOUR:
        retourTarots();
        break;
      case REPETITION:
        repetitionTarots();
        break;
      case SOMMAIRE:
        displaySommaireTarots();
        break;
    }
  }
}

// Tarots misc stuff

void displaySommaireTarots() {
  minitel.newScreen();
  currentEcran = "SOMMAIRE";
  myObject["currentCard"] = (int)-1;
  afficheRemoteVDT("tarots/tarots.vdt", 0, 0);
}

void hideNavTarotsSuite() {
  minitel.newXY(19, 24);
  minitel.attributs(CARACTERE_BLANC);
  minitel.attributs(FOND_NORMAL);
  minitel.print(" ");
  minitel.repeat(21);
}
void displayNavTarotsSuite() {
  minitel.newXY(19, 24);
  minitel.attributs(CARACTERE_BLANC);
  minitel.attributs(FOND_NORMAL);
  minitel.print("Carte suivante");
  minitel.moveCursorRight(1);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" SUITE ");
}
void displayNavTarotsRetour() {
  minitel.newXY(1, 24);
  minitel.attributs(CARACTERE_MAGENTA);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" RETOUR ");
}
void retourneCartes() {
  minitel.echo(false);
  currentEcran = "RETOURNEES";
  effacementEcran(24,24, CARACTERE_NOIR, FOND_NOIR);
  JSONVar currentArticle = myObject["datas"]["root"]["articles"][0];
  JSONVar numCard = currentArticle["num"];
  int nnn = currentArticle["num"];
  afficheRemoteVDT("tarots/tarot_" + String(nnn) + ".vdt", 3, 1);
  currentArticle = myObject["datas"]["root"]["articles"][1];
  numCard = currentArticle["num"];
  nnn = currentArticle["num"];
  afficheRemoteVDT("tarots/tarot_" + String(nnn) + ".vdt", 3, 14);
  currentArticle = myObject["datas"]["root"]["articles"][2];
  numCard = currentArticle["num"];
  nnn = currentArticle["num"];
  afficheRemoteVDT("tarots/tarot_" + String(nnn) + ".vdt", 3, 27);
   minitel.newXY(2,24);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print("Interprétation de votre tirage → ");
  minitel.attributs(INVERSION_FOND);
  minitel.print(" SUITE");
  minitel.attributs(FOND_NORMAL);
  minitel.echo(true);
}

void suiteTarots() {
  int currentCard = myObject["currentCard"];
  if (currentCard < 2) {
    currentCard++;
    myObject["currentCard"] = currentCard;
    afficheCarte();
  }
}
void retourTarots() {
  int currentCard = myObject["currentCard"];
  if (currentCard > 0) {
    currentCard--;
    myObject["currentCard"] = currentCard;
    if (currentCard == 1) {
      displayNavTarotsSuite();
    }
    afficheCarte();
  } else {
    displaySommaireTarots();
  }
}
void repetitionTarots() {
  int currentCard = myObject["currentCard"];
  if (currentCard < 0) {
    displaySommaireTarots();
  } else {
    afficheRemoteVDT("tarots/tarots_bandeau.vdt", 0, 0);
    displayNavTarotsRetour();
    if (currentCard < 2) {
      displayNavTarotsSuite();
    }
    afficheCarte();
  }
}
void afficheCarte() {
  effacementEcran(3, 22, FOND_BLEU, CARACTERE_BLEU);
  int currentCard = myObject["currentCard"];
  if (currentCard == 0) {
    effacementEcran(23, 24, FOND_NOIR, CARACTERE_NOIR);
    displayNavTarotsRetour();
    displayNavTarotsSuite();
  }
  if (currentCard == 2) {
    hideNavTarotsSuite();
  }
  JSONVar currentArticle = myObject["datas"]["root"]["articles"][currentCard];
  JSONVar numCard = currentArticle["num"];
  int nnn = currentArticle["num"];
  afficheRemoteVDT("tarots/tarot_" + String(nnn) + ".vdt", 3, 1);
  afficheTexteCurrentCard();
}

void afficheTexteCurrentCard() {
  int currentCard = myObject["currentCard"];
  JSONVar thema = myObject["datas"]["root"]["thema"][currentCard];
  minitel.newXY(16, 4);
  minitel.print((const char*)thema);
  JSONVar currentArticle = myObject["datas"]["root"]["articles"][currentCard];
  JSONVar texte = currentArticle["texte"];
  int nbLines = texte.length();
  int posY = 6;
  for (int j = 0; j < nbLines; j++) {
    minitel.newXY(15, posY);
    minitel.attributs(FOND_BLEU);
    minitel.attributs(CARACTERE_BLANC);
    minitel.print((const char*)texte[j]);
    posY++;
  }
}


void TAROTS_retrieveDatas() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("tarots/getjson.php");
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      JSONVar myJSONObject = JSON.parse(payload);
      if (JSON.typeof(myJSONObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      myObject["datas"] = myJSONObject;
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}