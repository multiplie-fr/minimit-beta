void setupTarots() {
  wifiConnect();
  minitel.noCursor();
  minitel.echo(false);
  initTarots();
}
void displaySommaireTarots() {
  minitel.newScreen();
  myObject["currentCard"] = (int)-1;
  afficheRemoteVDT("tarots/tarots_accueil.vdt", 0, 0);
}

void initTarots() {
  displaySommaireTarots();
  TAROTS_retrieveDatas();
}
void hideNavTarotsSuite(){
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
void displayNavTarotsRetour(){
  minitel.newXY(1, 24);
  minitel.attributs(CARACTERE_MAGENTA);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" RETOUR ");
}

void suiteTarots() {
  int currentCard = myObject["currentCard"];
  if (currentCard < 3) {
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
    if(currentCard==1){
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
  if(currentCard<2){
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
  if(currentCard==2){
    hideNavTarotsSuite();
  }
  JSONVar currentArticle = myObject["datas"]["root"]["articles"][currentCard];
  Serial.println(currentArticle); 
  JSONVar numCard = currentArticle["num"];
  Serial.println("--carte--");
  Serial.println(numCard);
  int nnn = currentArticle["num"];
  afficheRemoteVDT("tarots/tarot_" + String(nnn) + ".vdt", 3, 1);
  afficheTexteCurrentCard();
}



void afficheTexteCurrentCard() {
  int currentCard = myObject["currentCard"];
  JSONVar thema = myObject["datas"]["root"]["thema"][currentCard];
  minitel.newXY(16, 4);
  minitel.print((const char*)thema);
  Serial.println(thema);
  JSONVar currentArticle = myObject["datas"]["root"]["articles"][currentCard];
  JSONVar texte = currentArticle["texte"];
  Serial.println("-----");
  Serial.println(currentCard);
  Serial.println(texte);
  Serial.println(currentArticle["titre"]);
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
void lectureChampTarots(int x, int y, int longueurchamp) {
  //champVide(x, y, longueurchamp);
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    switch (touche) {
      case CONNEXION_FIN:
        fin=true;
        minitel.connexion(false);
        displayMire();
      break;
      case SUITE:
        fin = true;
        suiteTarots();
        break;
      case RETOUR:
        fin = true;
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



void TAROTS_retrieveDatas() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("tarots/getjson.php");
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
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