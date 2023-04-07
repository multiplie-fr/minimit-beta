#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Minitel1B_Hard.h>  // Voir https://github.com/eserandour/Minitel1B_Hard
Minitel minitel(Serial1);    // Le deuxième port série matériel de l'ATMega 1284P (RXD1 TXD1).

#define SERVICE_MIRE 0
#define SERVICE_ANNUAIRE 1
#define SERVICE_ASTRO 2
#define SERVICE_CP 3
#define SERVICE_COOKIES 4
#define SERVICE_GALERIE 5
#define SERVICE_LEMONDE 6
#define SERVICE_METEO 7
#define SERVICE_NABAZTAG 8
#define SERVICE_PENDU 9
#define SERVICE_PONG 10
#define SERVICE_PPP 11
#define SERVICE_TAROT 12

String userInput = "";
int userInputLength = 0;
unsigned long touche;
JSONVar myObject = {};
int currentService = SERVICE_MIRE;
String currentEcran = "SOMMAIRE";
const char* ssid = "Livebox-Xine";
const char* password = "malakoff";
// const char* ssid = "SFR_81F0";
// const char* password = "dogmysodsedcebjiags4";
//const char* ssid = "iPhone";
//const char* password = "tttttttt";
// const char* ssid = "LaVillamour";
// const char* password = "lavlina12";
// const char* ssid = "Dogtown";
// const char* password = "west100-;";
const char* serverName = "https://xn--multipli-i1a.fr/minimit/prod/";
void setup() {
  Serial.begin(115200);
  //wifiConnect();
  minitel.noCursor();
  minitel.echo(false);
  displayMire();
  minitel.echo(true);
}
void loop() {
  switch (currentService) {
    case SERVICE_MIRE:
      {
        Serial.print("servicemire");
        if (currentEcran == "SOMMAIRE") {
          lectureChampMire(12, 17, 29);
        }
        if (currentEcran == "GUIDE") {
          lectureChampMire(24, 24, 9);
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
    lectureChampCookies(1, 1,1);
    break;
    case SERVICE_GALERIE:
    lectureChampGalerie(28, 3, 0);
    loopGalerie();
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
    case SERVICE_TAROT:
    lectureChampTarots(16, 5, 5);
    break;
  }

}

void displayMire() {
  currentService=SERVICE_MIRE;
  currentEcran = "SOMMAIRE";
  minitel.newScreen();
  minitel.newXY(1, 0);
  minitel.attributs(FOND_NORMAL);
  minitel.print(" ");
  minitel.repeat(30);
  String vdt = "0c,14,1f,42,41,0e,1b,5a,31,0b,08,60,26,23,23,6d,30,0a,0a,08,60,0a,08,38,08,08,0a,26,08,08,2c,08,08,2c,08,08,2b,0b,08,30,08,08,61,31,21,21,0b,08,08,08,08,60,66,12,42,26,0b,08,08,08,38,12,43,1f,41,4a,0e,1b,54,18,0a,18,0a,18,0a,18,1f,42,4b,0e,1b,54,20,12,42,30,20,20,30,20,12,42,30,30,1f,43,4b,0e,1b,57,1b,44,48,48,1b,54,1b,47,34,34,1b,57,1b,44,48,1b,54,1b,47,34,34,1b,57,1b,44,48,48,1b,54,1b,47,34,34,1b,57,1b,44,48,1f,44,4b,0e,1b,54,25,12,4a,29,1f,45,4b,0e,1b,44,23,12,4b,1f,45,4a,1b,44,60,12,5e,1f,44,60,1b,4f,1b,44,1b,5d,33,36,31,35,1f,44,5f,1b,44,1b,5d,1b,4d,1b,51,20,08,1b,55,20,08,1b,52,20,08,1b,56,20,08,1b,53,20,08,1b,57,20,1f,46,49,1b,44,20,1b,5d,18,53,69,20,6f,6e,20,19,42,65,74,61,69,74,20,65,6e,63,6f,72,65,20,65,6e,20,31,39,38,33,2c,20,12,42,1f,47,49,1b,5a,20,1b,44,1b,5d,18,63,65,20,6d,69,6e,69,74,65,6c,20,76,6f,75,73,20,61,75,72,61,69,74,20,63,6f,19,43,75,74,19,42,65,20,20,1f,48,49,1b,41,1b,5a,20,1b,5d,18,1f,49,49,1b,44,1b,5a,20,1b,5d,18,30,2c,31,32,46,20,19,41,61,20,6c,61,20,63,6f,6e,6e,65,78,69,6f,6e,20,1f,4a,49,1b,44,1b,5a,20,1b,5d,18,70,75,69,73,20,31,2c,32,39,46,20,6c,61,20,6d,69,6e,75,74,65,1f,4b,49,1b,44,1b,5a,20,1b,5d,64,6f,6e,74,20,46,2e,54,45,4c,45,43,4f,4d,20,4f,2c,31,32,20,19,41,61,20,4f,2c,35,4f,46,2f,6d,69,6e,1b,5c,1b,59,20,12,47,1b,44,1b,5a,20,1b,5d,1b,45,18,20,12,44,53,45,52,56,49,43,45,53,20,44,49,53,50,4f,4e,49,42,4c,45,53,20,1b,44,1f,4d,49,1b,44,20,1b,5d,18,41,6e,6e,75,61,69,72,65,2c,20,6d,19,42,65,74,19,42,65,6f,2c,20,61,63,74,75,61,6c,69,74,19,42,65,73,2c,20,12,42,1b,5c,20,12,47,1b,44,1b,5a,20,1b,5d,18,61,73,74,72,6f,2c,20,74,61,72,6f,74,73,2e,12,42,20,20,74,61,70,65,7a,20,47,55,49,44,45,1b,5c,1f,52,41,1b,44,20,12,67,1f,4f,41,1b,44,60,12,67,1f,50,41,63,6f,64,65,0a,0d,64,75,20,73,65,72,76,69,63,65,3a,1f,53,41,1b,44,60,12,67,0a,28,43,29,0a,0d,6d,75,6c,74,69,70,6c,69,19,42,65,0a,0d,32,30,32,33,1f,53,41,1b,44,60,12,67,1f,54,4b,18,0a,18,0a,18,0a,18,0a,18,0a,1f,53,61,1b,5d,20,45,6e,76,6f,69,20,20,1f,56,4b,1b,4d,41,4e,4e,55,41,49,52,45,20,44,45,53,20,53,45,52,56,49,43,45,53,1b,5a,1b,47,1b,4d,20,1b,5d,20,47,75,69,64,65,20,20,1f,57,47,1b,47,20,12,43,50,61,72,61,6d,19,41,65,74,72,65,73,20,64,75,20,6d,69,6e,69,6d,69,74,1b,5a,1b,42,20,1b,5d,53,6f,6d,6d,61,69,72,65,1f,58,4b,1b,47,20,12,51,66,69,6e,1b,5a,1b,42,20,1b,5d,20,43,78,2f,46,69,6e,20,1f,51,4c,1b,4c,1b,43,2e,12,5c,1f,51,4c,1b,4c,11";
  checkScreen(vdt, 0, 0);
  userInput="";
  userInputLength=0;
}

void displayGuideMire() {
  currentService=SERVICE_MIRE;
  currentEcran = "GUIDE";
  String vdt = "14,0c,1f,41,45,0e,1b,46,58,1b,56,1b,40,23,1b,50,1b,46,30,09,09,09,09,09,09,1b,47,50,40,09,30,50,09,30,50,40,09,30,54,1f,42,43,0e,1b,46,40,1b,56,1b,40,21,20,20,20,1b,50,1b,46,54,09,09,09,09,09,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,4a,1b,50,1b,47,34,1b,57,1b,40,4a,12,42,1b,50,1b,47,34,1b,57,1b,40,4a,1f,43,42,0e,1b,46,48,1b,56,20,20,20,20,1b,45,40,1b,55,1b,46,21,09,09,09,09,09,1b,50,1b,47,12,4b,22,1f,44,42,0e,1b,56,20,12,43,1b,45,58,1b,55,1b,40,40,34,1f,45,42,0e,1b,56,20,12,42,1b,55,1b,46,21,1b,40,58,1b,50,20,1b,45,4a,1f,46,42,0e,1b,56,1b,40,34,20,20,1b,55,4a,1b,50,20,1b,45,40,1b,55,1b,40,21,1b,50,1b,45,54,1f,47,43,0e,1b,56,1b,40,54,20,1b,55,4a,1b,50,1b,45,58,1b,55,20,20,20,1b,40,22,1b,50,1b,45,30,1f,48,44,0e,1b,46,22,1b,55,1b,40,30,20,20,20,20,58,1f,49,46,0e,1b,55,1b,40,54,20,40,1b,50,1b,45,21,1f,4a,47,0e,1b,45,22,1f,44,4e,1b,46,4c,45,20,47,55,49,44,45,20,44,45,53,20,53,45,52,56,49,43,45,53,1f,46,4e,1b,45,52,65,76,69,76,65,7a,20,6c,27,65,78,70,19,42,65,72,69,65,6e,63,65,20,64,75,20,1f,47,4e,1b,45,6d,69,6e,69,74,65,6c,20,76,69,61,20,63,65,73,20,71,75,65,6c,71,75,65,73,1f,48,4e,1b,45,73,65,72,76,69,63,65,73,2c,20,63,6f,6d,6d,65,20,64,61,6e,73,20,6c,65,73,20,1f,49,4e,1b,45,61,6e,6e,19,42,65,65,73,20,38,30,2e,1f,4a,41,1b,44,60,12,67,1f,57,41,1b,44,60,12,69,1f,58,41,1b,45,54,61,70,65,7a,20,75,6e,20,6e,19,30,20,6f,75,20,73,6f,6e,20,6e,6f,6d,20,2e,12,48,20,1b,47,1b,5d,20,45,4e,56,4f,49,20,1f,4b,43,31,20,41,4e,4e,55,41,49,52,45,1f,4c,43,32,20,41,53,54,52,4f,1f,4d,43,33,20,43,4f,55,50,4c,45,53,50,41,52,46,41,49,54,53,1f,4e,43,34,20,46,4f,52,54,55,4e,45,1f,4f,43,35,20,47,41,4c,45,52,49,45,1f,50,43,36,20,4c,45,4d,4f,4e,44,45,1f,51,43,37,20,4d,45,54,45,4f,1f,52,43,38,20,4e,41,42,41,5a,54,41,47,1f,53,43,39,20,50,45,4e,44,55,1f,54,42,31,30,20,50,4f,4e,47,1f,55,42,31,31,20,50,50,50,20,28,70,6c,75,69,65,20,70,61,73,20,70,6c,75,69,65,20,3f,29,1f,56,42,31,32,20,54,41,52,4f,54";
  checkScreen(vdt, 0, 0);
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
      fin=true;
      minitel.connexion(false);
      displayMire();
      break;
      case ENVOI:
        {
          fin = true;
          if (currentEcran == "SOMMAIRE") {
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
          userInput = "";
          userInputLength = 0;
          if (currentEcran == "SOMMAIRE") {
          }
          if (currentEcran == "GUIDE") {
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
  if (input == "ANNUAIRE" || input == "1") {
    goAnnuaire();
    return;
  }
  if (input == "ASTRO" || input == "2") {
    goAstro();
    return;
  }
  if (input == "COUPLESPARFAITS" || input == "3") {
    goCP();
    return;
  }
   if (input == "FORTUNE" || input == "4") {
    goCookies();
    return;
  }
  if (input == "GALERIE" || input == "5") {
    goGalerie();
    return;
  }
  if (input == "LEMONDE" || input == "6") {
    goLeMonde();
    return;
  }
   if (input == "METEO" || input == "7") {
    goMeteo();
    return;
  }
   if (input == "PENDU" || input == "9") {
    goPendu();
    return;
  }
  if (input == "TAROTS" || input == "12") {
    goTarots();
    return;
  }
}
void goAstro() {
  currentService=SERVICE_ASTRO;
  setupAstro();
}
void goAnnuaire() {
  Serial.print("ann");
  currentService=SERVICE_ANNUAIRE;
  setupAnnuaire();
}
void goCP() {
  Serial.print("cp");
  currentService=SERVICE_CP;
  setupCP();
}
void goCookies(){
  currentService = SERVICE_COOKIES;
  setupCookies();
}
void goGalerie(){
  currentService = SERVICE_GALERIE;
  setupGalerie();
}
void goLeMonde(){
  currentService = SERVICE_LEMONDE;
  setupLeMonde();
}
void goMeteo(){
  currentService = SERVICE_METEO;
  setupMeteo();
}
void goPendu(){
  currentService = SERVICE_PENDU;
  setupPendu();
}
void goTarots(){
  currentService = SERVICE_TAROT;
  setupTarots();
}