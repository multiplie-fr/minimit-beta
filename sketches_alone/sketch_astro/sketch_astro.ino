#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <pgmspace.h>
#include <Minitel1B_Hard.h>  // Voir https://github.com/eserandour/Minitel1B_Hard
Minitel minitel(Serial1);    // Le deuxième port série matériel de l'ATMega 1284P (RXD1 TXD1).

String userInput = "";
int userInputLength = 0;
unsigned long touche;
const char* ssid = "Livebox-Xine";
const char* password = "malakoff";
//const char* ssid = "SFR_81F0";
//const char* password = "dogmysodsedcebjiags4";
//const char* ssid = "iPhone";
//const char* password = "tttttttt";
const char* serverName = "https://xn--multipli-i1a.fr/minimit/prod/";
JSONVar myObject = {};

void setup() {
  Serial.begin(115200);
  minitel.pageMode();
  wifiConnect();
  afficheSommaireAstro();
  //champVide(16, 3, 5);
}
void afficheSommaireAstro() {
  myObject["currentPage"] = "SOMMAIRE";
  minitel.newScreen();
  afficheRemoteVDT("astro/astro.vdt", 0, 0);
}
void lectureChamp(int x, int y, int longueurchamp) {
  String currentPage = (const char*)myObject["currentPage"];
  if (currentPage == "SOMMAIRE") {
    champVide(x, y, longueurchamp);
  }
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (userInputLength < longueurchamp) {
        userInputLength++;
        userInput += char(touche);
        Serial.print(userInput);
      } else {
        //minitel.moveCursorLeft(1);
        //minitel.print(" ");
        //minitel.moveCursorLeft(1);
      }
    }
    switch (touche) {
      case ENVOI:
        fin = true;
        if (userInputLength == 3) {
          retrieveDatasASTRO();
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
         String page = (const char*)myObject["currentPage"];
         if(page=="SOMMAIRE"){
          champVide(x, y, longueurchamp);
          userInput = "";
          userInputLength = 0;
          }
         else {
          afficheRemoteVDT("astro/bandeau-astro.vdt", 0, 0);
          afficheDatasASTRO();
         }
        }
        break;
      case SOMMAIRE:
      case RETOUR:
        {
          String page = (const char*)myObject["currentPage"];
          if (page=="PAGE")
          {
            afficheSommaireAstro();
            userInput = "";
            userInputLength = 0;
        }
        }
        break;
    }
  }
}

void loop() {
  lectureChamp(22, 24, 3);
  //userInput = "";
}


void afficheDatasASTRO() {
  myObject["currentPage"] = "PAGE";
  minitel.noCursor();
  effacementEcran(6, 22);
  effacementEcran(24, 24);
  minitel.textMode();
  minitel.newXY(31, 24);
  minitel.attributs(INVERSION_FOND);
  minitel.attributs(CARACTERE_CYAN);
  minitel.print(" RETOUR ");
  minitel.attributs(FOND_NORMAL);  
  int posY = 6;
  int posX = 2;
  JSONVar signe = myObject["root"]["signe"];
  minitel.newXY(posX, posY);
  minitel.attributs(CARACTERE_BLANC);
  minitel.print((const char*)signe);
  minitel.print(" ");
  minitel.attributs(CARACTERE_NOIR);
  JSONVar date = myObject["root"]["date"];
  minitel.print((const char*)date);
  posY++;posY++;
  JSONVar articles = myObject["root"]["articles"];
  
  int nbarticles = articles.length();
  for (int i = 0; i < nbarticles; i++) {
    int nblignes = articles[i].length();
    for (int j = 0; j < nblignes; j++) {
      minitel.newXY(posX, posY);
      String textLigne = (const char*)articles[i][j];
      if (j == 0) {
        minitel.attributs(CARACTERE_BLANC);
      } else {
        minitel.attributs(CARACTERE_CYAN);
      }
      posY++;
      minitel.print(textLigne);
    }
    //posY++;
  }
  minitel.noCursor();
}
void retrieveDatasASTRO() {
  Serial.print(userInput);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("astro/getjson.php?signe=") + userInput;
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("");
      Serial.print(payload);
      Serial.println("");
      myObject = JSON.parse(payload);
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      afficheDatasASTRO();
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}