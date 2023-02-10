#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <pgmspace.h>
#include <Minitel1B_Hard.h>  // Voir https://github.com/eserandour/Minitel1B_Hard
Minitel minitel(Serial1);    // Le deuxième port série matériel de l'ATMega 1284P (RXD1 TXD1).


typedef unsigned char PROGMEM prog_uchar;
const char* ssid = "Livebox-Xine";
const char* password = "malakoff";
//const char* ssid = "SFR_81F0";
//const char* password = "dogmysodsedcebjiags4";
//const char* ssid = "iPhone";
//const char* password = "tttttttt";
const char* serverName = "https://xn--multipli-i1a.fr/minimit/prod/";

///
///Les params du service
///
JSONVar myObject = {};

///
String userInput = "";
int userInputNeeded = 0;
unsigned long touche;
String chaine;

void setup() {
  Serial.begin(115200);
  Serial.println("");
  minitel.newScreen();
  wifiConnect();
  
  //populate les params
  myObject["currentPage"] = (int)0;
  myObject["displayMode"] = (int)1;
  //Displaymode
  //1 : liste
  //2 : fiche
  JSONVar myArray;
  myArray[0] = 0;
  myObject["pageOffset"] = myArray;
  ///

  //ON affiche le fond d'écran
  afficheRemoteVDT("bandeaulemonde.vdt", 0, 0);
  minitel.textMode();
  displayNavList();
  retrieveDatas("lemonde/getjson.php?current=", 1, 1);
}
void displayNavArticle(int previousDisplayMode){
if(previousDisplayMode==1){
    minitel.moveCursorXY(1,24);
    minitel.print(" ");
    minitel.repeat(22);
    minitel.moveCursorXY(1,24);
    minitel.attributs(CARACTERE_MAGENTA);
    minitel.print("Revenir au ");
    minitel.attributs(INVERSION_FOND);
    minitel.print(" SOMMAIRE ");
    minitel.attributs(FOND_NORMAL);
  }
}
void displayNavList(){
  effacementEcran(23, 24);
  minitel.attributs(CARACTERE_BLEU);
  minitel.moveCursorXY(0, 22);
  minitel.writeByte(0x60);
  minitel.repeat(39);
  minitel.moveCursorXY(1, 23);
  minitel.attributs(CARACTERE_MAGENTA);
  minitel.print("Votre choix ...");
  minitel.attributs(DEBUT_LIGNAGE);
  minitel.writeByte(0x20);
  minitel.attributs(INVERSION_FOND);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print(" ENVOI ");
  minitel.attributs(FOND_NORMAL);
  minitel.moveCursorXY(2, 24);
  minitel.attributs(CARACTERE_MAGENTA);
  minitel.print("Les brèves ");
  minitel.attributs(CARACTERE_BLANC);
  minitel.print("BRE");
  minitel.attributs(DEBUT_LIGNAGE);
  minitel.writeByte(0x20);
  minitel.attributs(INVERSION_FOND);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print(" ENVOI ");
  minitel.attributs(FOND_NORMAL);

  minitel.moveCursorXY(26, 23);
  minitel.attributs(CARACTERE_CYAN);
  minitel.attributs(DEBUT_LIGNAGE);
  minitel.writeByte(0x20);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" RETOUR");
  minitel.writeByte(0x7C);
  minitel.print("SUITE ");
  minitel.attributs(FOND_NORMAL);  
}
void lectureChamp(int x, int y, int longueurchamp) {
  Serial.print("lc");
  champVide(x, y, longueurchamp);
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (userInputNeeded < longueurchamp) {
        userInputNeeded++;
        userInput += char(touche);
        Serial.print(userInput);
      } else {
        //minitel.moveCursorLeft(1);
        //minitel.print(" ");
        //minitel.moveCursorLeft(1);
      }
    }
    switch (touche) {
      case SUITE:
        Serial.println("suite");
        myObject["currentPage"] = (int)(myObject["currentPage"]) + 1;
        Serial.println(myObject);
        effacementEcran(5, 21);
        fin = true;
        afficheListe(myObject["currentPage"]);
        break;
      case RETOUR:
        if ((int)myObject["currentPage"] == 0) {
          break;
        }
        myObject["currentPage"] = (int)(myObject["currentPage"]) - 1;
        effacementEcran(5, 21);
        fin = true;
        afficheListe(myObject["currentPage"]);
        break;
      case ENVOI:
        fin = true;
        Serial.println("envoi");
        if (userInputNeeded > 0) {
          Serial.println("choix");
          Serial.println(userInput);
          int userInputInt = userInput.toInt();
          userInputInt--;
          //retrieveDatas();
          retrieveDatas("lemonde/getjson.php?mode=article&item=" + String(userInputInt), 2, userInputInt);
        }
        break;
      case SOMMAIRE:
        effacementEcran(5, 21);
        displayNavList();
        afficheListe(myObject["currentPage"]);
      break;
      case ANNULATION:
        champVide(x, y, longueurchamp);
        break;
      case CORRECTION:
        //correction(longueurchamp); TODO
        break;
    }
  }
}

void loop() {
  lectureChamp(13, 23, 3);
  //userInput = "";
}



void retrieveDatas(String phpFile, int displayMode, int articleItem) {
  int previousDisplayMode = myObject["displayMode"];
  myObject["displayMode"] = displayMode;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath;
    int currentPage = myObject["currentPage"];
    int pageOffset = myObject["pageOffset"][currentPage];
    switch (displayMode) {
      case 1:
        serverPath = serverName + String(phpFile) + String(pageOffset);
        break;
      case 2:
        serverPath = serverName + String(phpFile);
        Serial.println(serverPath);
        break;
    }
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      JSONVar myDatas = JSON.parse(payload);

      if (JSON.typeof(myDatas) == "undefined") {
        Serial.println(payload);
        Serial.println("Parsing input failed!");
        return;
      }

      switch (displayMode) {
        case 1:
        {
          myObject["myDatas"] = myDatas;
          //affiche date en haut
          JSONVar date = myDatas["root"]["date"];
          JSONVar myarrayPagesOffset = myDatas["root"]["pages"];
          int nbPagesOfList = myarrayPagesOffset.length();
          for (int po = 0; po < nbPagesOfList - 1; po++) {
            Serial.println(String(po));
            myObject["pageOffset"][(po + 1)] = (int)myarrayPagesOffset[(po + 1)];
          }

          minitel.moveCursorXY(18, 2);
          minitel.attributs(CARACTERE_CYAN);
          minitel.print("A LA ");
          minitel.attributs(CARACTERE_BLANC);
          minitel.print("UNE ");
          minitel.attributs(CARACTERE_CYAN);
          minitel.print("CE JOUR");
          minitel.moveCursorXY(18, 3);
          minitel.attributs(CARACTERE_BLANC);
          minitel.print((const char*)date);
          afficheListe(currentPage);
        }
          break;

        case 2:
        {
          myObject["myArticle"] = myDatas;
          myObject["currentPageFolio"] = 0;
          affichePage(previousDisplayMode);
          break;
        }
      }
    }
    Serial.print(myObject);
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
void affichePage(int previousDisplayMode) {
  displayNavArticle(previousDisplayMode);
  
  Serial.print(myObject["myArticle"]);
  effacementEcran(5, 21);
  minitel.noCursor();
  JSONVar myDatas = myObject["myArticle"];
  minitel.textMode();
  JSONVar myarray = myDatas["article"];
  JSONVar article = (myarray[0]);
  JSONVar titre = article["titre"];
  JSONVar texte = article["texte"];
  int nbLines = 0;
  int posY = 5;
  int j = 0;
  int titreNbLines = titre.length();
  int textNbLines = texte.length();
  Serial.println(String(titreNbLines));
  Serial.println(String(textNbLines));
  float nbFolio = (titreNbLines+textNbLines+1)/17.00;
  Serial.println("---");
  Serial.println("nbFolio");
  int folioMax = ceil(nbFolio);
  Serial.println(String(folioMax));
  minitel.attributs(CARACTERE_BLANC);
  for (j = 0; j < titreNbLines; j++) {
    minitel.moveCursorXY(0, posY);
    minitel.print((const char*)titre[j]);
    posY++;
    nbLines++;
  }
  posY++;
  nbLines++;
  minitel.attributs(CARACTERE_CYAN);

  for (j = 0; j < textNbLines; j++) {
    minitel.moveCursorXY(0, posY);
    minitel.print((const char*)texte[j]);
    posY++;
    nbLines++;
    if(nbLines==17){
      break;
    }
  }

}
void afficheListe(int page) {
  minitel.noCursor();
  JSONVar myDatas = myObject["myDatas"];
  minitel.textMode();
  JSONVar myarray = myDatas["root"]["articles"];
  JSONVar myarrayPagesOffset = myObject["pageOffset"];
  Serial.println("--");
  Serial.println(myObject["pageOffset"][2]);
  Serial.println("--");
  Serial.println(myarrayPagesOffset);
  //Serial.println()
  int nbArticles = (int)myarrayPagesOffset[(page + 1)] - (int)myarrayPagesOffset[(page)];
  nbArticles += (int)myarrayPagesOffset[(page)];
  Serial.println("nbarcticles");
  Serial.println(myarrayPagesOffset[(page + 1)]);
  int i = 0;
  int posY = 5;

  for (i = myarrayPagesOffset[(page)]; i < nbArticles; i++) {
    JSONVar article = (myarray[i]);
    JSONVar titre = article["titre"];
    int j = 0;
    int titreNbLines = titre.length();

    for (j = 0; j < titreNbLines; j++) {
      if (j == 0) {
        minitel.attributs(CARACTERE_BLANC);
        minitel.moveCursorXY(0, posY);
        minitel.print(String(i + 1));
      }
      minitel.moveCursorXY(4, posY);
      minitel.attributs(CARACTERE_BLANC);
      minitel.print((const char*)titre[j]);
      posY++;
    }
    if (i < (nbArticles - 1)) {
      minitel.attributs(CARACTERE_BLEU);
      minitel.moveCursorXY(0, posY);
      minitel.writeByte(0x60);
      minitel.repeat(39);
      posY++;
    }
  }
  minitel.cursor();
}