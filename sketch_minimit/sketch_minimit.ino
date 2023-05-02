#include <WiFi.h>
#include <HTTPClient.h>
#include "SPIFFS.h"
#include "FS.h"
#include <Arduino_JSON.h>
#include <Minitel1B_Hard.h>  // Voir https://github.com/eserandour/Minitel1B_Hard
#include <WebSocketsClient.h> // src: https://github.com/Links2004/arduinoWebSockets.git
Minitel minitel(Serial1);    // Le deuxième port série matériel de l'ATMega 1284P (RXD1 TXD1).

#define SERVICE_MIRE 0
#define SERVICE_ANNUAIRE 1
#define SERVICE_ASTRO 2
#define SERVICE_CP 3
#define SERVICE_COOKIES 5
#define SERVICE_ELIZA 4
#define SERVICE_GALERIE 6
#define SERVICE_LEMONDE 7
#define SERVICE_METEO 8
#define SERVICE_NABAZTAG 9
#define SERVICE_PENDU 10
#define SERVICE_PONG 11
#define SERVICE_PPP 12
#define SERVICE_TAROT 13
#define SERVICE_TELNET 14
#define SERVICE_CONFIG 999
boolean isConnected = false;
String userInput = "";
int userInputLength = 0;
unsigned long touche;
JSONVar myObject = {};
JSONVar myConfig = {};
int currentService = SERVICE_MIRE;
String currentEcran = "SOMMAIRE";
String confFile = "/conf3.txt";
int saisieColor = CARACTERE_JAUNE;
const char* serverName = "https://xn--multipli-i1a.fr/minimit/prod/";
boolean interruption = false;
int timeoutAmount = 5000;
long nextUpdate = 0;

void setup() {
  Serial.begin(115200);
  minitel.pageMode();
  minitel.noCursor();
  minitel.echo(false);
  SPIFFS.begin(true);
  JSONVar config = getConfig();
  myConfig = config;
  Serial.println("laconfig");
  Serial.println(myConfig);
  displayDemarrage();
}

void displayDemarrage() {
  Serial.println(JSON.typeof(myConfig));
  boolean isConfig;
  if (JSON.typeof(myConfig) == "undefined") {
    isConfig = 0;
  } else {
    isConfig = 1;
  }
  String vdt = "14,0c,1f,46,54,0e,1b,46,40,50,1f,47,53,0e,1b,46,58,1b,56,20,20,1b,40,22,1b,50,1b,46,30,1f,48,51,0e,1b,46,40,1b,56,1b,40,21,20,12,43,1b,45,58,1f,49,51,0e,1b,56,1b,40,21,20,12,42,1b,45,40,1b,55,1b,46,21,1b,40,30,1f,4a,51,0e,1b,56,20,12,42,1b,45,58,1b,55,1b,40,40,1b,50,1b,45,21,4a,1f,4b,51,0e,1b,56,1b,40,30,20,20,1b,55,48,1b,50,20,20,1b,55,25,1b,50,1b,45,30,1f,4c,51,0e,1b,46,22,1b,56,1b,40,30,20,1b,55,4a,1b,50,1b,45,40,1b,55,1b,40,21,20,20,1b,50,1b,45,54,1f,4d,53,0e,1b,56,1b,40,54,1b,55,22,20,12,43,40,1b,50,1b,45,21,1f,4e,54,0e,1b,45,22,1b,55,1b,40,30,20,20,58,1f,4f,56,0e,1b,55,1b,40,54,1b,50,1b,45,21,09,09,1b,47,40,1f,50,4f,0e,4a,49,49,42,4a,49,42,4a,49,49,42,4a,21,1f,51,4f,0e,2a,12,4a,22,24";
  minitel.noCursor();
  checkScreen(vdt, 0, 0);
  minitel.echo(false);
  if (isConfig) {
    ligneZero("Test connexion wifi...");
    isConnected = checkConnexion();
    Serial.println("isConnected");
    Serial.println(isConnected);
    delay(3000);
    minitel.echo(true);
    displayMire();
  } else {
    delay(1000);
    goConfig();
  }
}
boolean checkConnexion() {
  if (JSON.typeof(myConfig) == "undefined") {
    Serial.println("----");
    Serial.println("xixixi");
    Serial.print(WiFi.status());
    return false;
  } else {
    JSONVar config = myConfig["input"];
    // const char* ssid = (const char*)config[0];
    // const char* password = (const char*)config[1];
    //   const char* ssid = "Dogtown";
    //  const char* password = "west100-;";
    const char* ssid = "Livebox-Xine";
    const char* password = "malakoff";

    WiFi.begin(ssid, password);
    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(1000);
      Serial.println(WiFi.status());
      if (cnt == 10) {
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
void loop() {
  switch (currentService) {
    case SERVICE_MIRE:
      {
        if (currentEcran == "SOMMAIRE") {
          lectureChampMire(12, 17, 29);
        }
        if (currentEcran == "GUIDE") {
          lectureChampMire(11, 24, 22);
        }
      }
      break;
    case SERVICE_ASTRO:
      lectureChampAstro(22, 24, 3);
      break;
    case SERVICE_ANNUAIRE:
      lectureChampAnnuaire();
      break;
    case SERVICE_CP:
      loopCP();
      break;
    case SERVICE_COOKIES:
      lectureChampCookies(1, 1, 1);
      break;
    case SERVICE_ELIZA:
      lectureChampEliza(1, 4);
      break;
    case SERVICE_GALERIE:
      loopGalerie();
      //lectureChampGalerie(32, 23, 1);
      break;
    case SERVICE_LEMONDE:
      lectureChampLeMonde(13, 23, 3);
      break;
    case SERVICE_METEO:
      lectureChampMeteo(28, 3, 5);
      break;
    case SERVICE_PENDU:
      lectureChampPendu(11, 17, 1);
      break;
    case SERVICE_PPP:
      lectureChampPPP(25, 12, 5);
      break;
    case SERVICE_TAROT:
      lectureChampTarots(16, 5, 5);
      break;
    case SERVICE_CONFIG:
      gereTouchesConfig();
      break;
    case SERVICE_PONG:
      loopPong();
      break;
    case SERVICE_TELNET:
      loopTelnet();
      break;
  }
}

void displayMire() {
  Serial.println("displaymire");
  currentService = SERVICE_MIRE;
  currentEcran = "SOMMAIRE";
  ligneZero(" ");
  saisieColor = CARACTERE_JAUNE;
  minitel.pageMode();
  minitel.newScreen();
  minitel.newXY(1, 0);
  minitel.attributs(FOND_NORMAL);
  minitel.print(" ");
  minitel.repeat(30);
  String vdt = "0c,14,1f,42,41,0e,1b,5a,31,0b,08,60,26,23,23,6d,30,0a,0a,08,60,0a,08,38,08,08,0a,26,08,08,2c,08,08,2c,08,08,2b,0b,08,30,08,08,61,31,21,21,0b,08,08,08,08,60,66,12,42,26,0b,08,08,08,38,12,43,1f,41,4a,0e,1b,54,18,0a,18,0a,18,0a,18,1f,42,4b,0e,1b,54,20,12,42,30,20,20,30,20,12,42,30,30,1f,43,4b,0e,1b,57,1b,44,48,48,1b,54,1b,47,34,34,1b,57,1b,44,48,1b,54,1b,47,34,34,1b,57,1b,44,48,48,1b,54,1b,47,34,34,1b,57,1b,44,48,1f,44,4b,0e,1b,54,25,12,4a,29,1f,45,4b,0e,1b,44,23,12,4b,1f,45,4a,1b,44,60,12,5e,1f,44,60,1b,4f,1b,44,1b,5d,33,36,31,35,1f,44,5f,1b,44,1b,5d,1b,4d,1b,51,20,08,1b,55,20,08,1b,52,20,08,1b,56,20,08,1b,53,20,08,1b,57,20,1f,46,49,1b,44,20,1b,5d,18,53,69,20,6f,6e,20,19,42,65,74,61,69,74,20,65,6e,63,6f,72,65,20,65,6e,20,31,39,38,33,2c,20,12,42,1f,47,49,1b,5a,20,1b,44,1b,5d,18,63,65,20,6d,69,6e,69,74,65,6c,20,76,6f,75,73,20,61,75,72,61,69,74,20,63,6f,19,43,75,74,19,42,65,20,20,1f,48,49,1b,41,1b,5a,20,1b,5d,18,1f,49,49,1b,44,1b,5a,20,1b,5d,18,30,2c,31,32,46,20,19,41,61,20,6c,61,20,63,6f,6e,6e,65,78,69,6f,6e,20,1f,4a,49,1b,44,1b,5a,20,1b,5d,18,70,75,69,73,20,31,2c,32,39,46,20,6c,61,20,6d,69,6e,75,74,65,1f,4b,49,1b,44,1b,5a,20,1b,5d,64,6f,6e,74,20,46,2e,54,45,4c,45,43,4f,4d,20,4f,2c,31,32,20,19,41,61,20,4f,2c,35,4f,46,2f,6d,69,6e,1b,5c,1b,59,20,12,47,1b,44,1b,5a,20,1b,5d,1b,45,18,20,12,44,53,45,52,56,49,43,45,53,20,44,49,53,50,4f,4e,49,42,4c,45,53,20,1b,44,1f,4d,49,1b,44,20,1b,5d,18,41,6e,6e,75,61,69,72,65,2c,20,6d,19,42,65,74,19,42,65,6f,2c,20,61,63,74,75,61,6c,69,74,19,42,65,73,2c,20,12,42,1b,5c,20,12,47,1b,44,1b,5a,20,1b,5d,18,61,73,74,72,6f,2c,20,74,61,72,6f,74,73,2e,12,42,20,20,74,61,70,65,7a,20,47,55,49,44,45,1b,5c,1f,52,41,1b,44,20,12,67,1f,4f,41,1b,44,60,12,67,1f,50,41,63,6f,64,65,0a,0d,64,75,20,73,65,72,76,69,63,65,3a,1f,53,41,1b,44,60,12,67,0a,28,43,29,0a,0d,6d,75,6c,74,69,70,6c,69,19,42,65,0a,0d,32,30,32,33,1f,53,41,1b,44,60,12,67,1f,54,4b,18,0a,18,0a,18,0a,18,0a,18,0a,1f,53,61,1b,5d,20,45,6e,76,6f,69,20,20,1f,56,4b,1b,4d,41,4e,4e,55,41,49,52,45,20,44,45,53,20,53,45,52,56,49,43,45,53,1b,5a,1b,47,1b,4d,20,1b,5d,20,47,75,69,64,65,20,20,1f,57,47,1b,47,20,12,43,50,61,72,61,6d,19,41,65,74,72,65,73,20,64,75,20,6d,69,6e,69,6d,69,74,1b,5a,1b,42,20,1b,5d,53,6f,6d,6d,61,69,72,65,1f,58,4b,1b,47,20,12,45,51,75,69,20,61,20,66,61,69,74,20,19,4b,63,61,20,3f,1b,5a,1b,42,20,1b,5d,20,53,75,69,74,65,20,20,1f,48,53,1b,41,1b,4e,1b,5d,1f,51,4c,1b,4c";
  checkScreen(vdt, 0, 0);
  userInput = "";
  userInputLength = 0;
}

void displayGuideMire() {
  minitel.noCursor();
  currentService = SERVICE_MIRE;
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
  minitel.print("14 TELNET");
  minitel.cursor();
}


void lectureChampMire(int x, int y, int longueurchamp) {
  champVide(x, y, longueurchamp);
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
      case CONNEXION_FIN:
        fin = true;
        minitel.connexion(false);
        displayMire();
        break;
      case ENVOI:
        {
          fin = true;
          if (currentEcran == "SOMMAIRE") {
            checkService(userInput);
          }
          if (currentEcran == "GUIDE") {
            checkService(userInput);
          }
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
            minitel.attributs(saisieColor);
            minitel.moveCursorLeft(1);
            userInput = userInput.substring(0, userInput.length() - 1);
            userInputLength--;
            Serial.println(userInput);
          }
        }
        break;
      case REPETITION:
        {
          userInput = "";
          userInputLength = 0;
          if (currentEcran == "SOMMAIRE") {
            displayMire();
            champVide(x, y, longueurchamp);
          }
        }
        break;
      case SOMMAIRE:
        {
          fin = true;
          if (currentEcran == "SOMMAIRE") {
            goConfig();
          }
          if (currentEcran == "GUIDE") {
            userInput = "";
            userInputLength = 0;
            displayMire();
          }
        }
        break;
      case GUIDE:
        {
          Serial.print("guide");
          fin = true;
          userInput = "";
          userInputLength = 0;
          displayGuideMire();
        }
        break;
    }
  }
}
void checkService(String input) {
  ligneZero(" ");
  myObject = (JSONVar){};
  if (input == "ANNUAIRE" || input == String(SERVICE_ANNUAIRE)) {
    goAnnuaire();
    return;
  }
  if (input == "ASTRO" || input == String(SERVICE_ASTRO)) {
    goAstro();
    return;
  }
  if (input == "COUPLESPARFAITS" || input == "CP" || input == String(SERVICE_CP)) {
    goCP();
    return;
  }
  if (input == "ELIZA" || input == String(SERVICE_ELIZA)) {
    goEliza();
    return;
  }
  if (input == "FORTUNE" || input == String(SERVICE_COOKIES)) {
    goCookies();
    return;
  }
  if (input == "GALERIE" || input == String(SERVICE_GALERIE)) {
    goGalerie();
    return;
  }
  if (input == "LEMONDE" || input == String(SERVICE_LEMONDE)) {
    goLeMonde();
    return;
  }
  if (input == "METEO" || input == String(SERVICE_METEO)) {
    goMeteo();
    return;
  }
  if (input == "PENDU" || input == String(SERVICE_PENDU)) {
    goPendu();
    return;
  }
  if (input == "PONG" || input == String(SERVICE_PONG)) {
    goPong();
    return;
  }
  if (input == "PPP" || input == String(SERVICE_PPP)) {
    goPPP();
    return;
  }
  if (input == "TELNET" || input == String(SERVICE_TELNET)) {
    goWS();
    return;
  }
  if (input == "TAROT" || input == String(SERVICE_TAROT)) {
    goTarots();
    return;
  }
}
void goAstro() {
  currentService = SERVICE_ASTRO;
  setupAstro();
}
void goAnnuaire() {
  Serial.print("ann");
  currentService = SERVICE_ANNUAIRE;
  setupAnnuaire();
}
void goCP() {
  Serial.print("cp");
  currentService = SERVICE_CP;
  setupCP();
}
void goCookies() {
  currentService = SERVICE_COOKIES;
  setupCookies();
}
void goEliza() {
  currentService = SERVICE_ELIZA;
  setupEliza();
}
void goGalerie() {
  currentService = SERVICE_GALERIE;
  setupGalerie();
}
void goLeMonde() {
  currentService = SERVICE_LEMONDE;
  setupLeMonde();
}
void goMeteo() {
  currentService = SERVICE_METEO;
  setupMeteo();
}
void goPendu() {
  currentService = SERVICE_PENDU;
  setupPendu();
}
void goPong() {
  currentService = SERVICE_PONG;
  setupPong();
}
void goPPP() {
  currentService = SERVICE_PPP;
  setupPPP();
}
void goTarots() {
  currentService = SERVICE_TAROT;
  setupTarots();
}
void goWS() {
  currentService = SERVICE_TELNET;
  setupWS();
}
void goConfig() {
  currentService = SERVICE_CONFIG;
  setupConfig();
}