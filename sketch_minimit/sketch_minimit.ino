#include <WiFi.h>
#include <HTTPClient.h>
#include "SPIFFS.h"
#include "FS.h"
#include <Arduino_JSON.h>
#include <Minitel1B_Hard.h>
#include <WebSocketsClient.h>
Minitel minitel(Serial1, 14, 27);


// Globales user interface
String userInput = "";
int userInputLength = 0;
unsigned long touche;
unsigned long lasttouche;
int saisieColor = CARACTERE_JAUNE;

// Config
JSONVar myObject = {};
JSONVar myConfig = {};
JSONVar myOTAConfig = {};
String confFile = "/conf6.txt";

// Cloud
const char* serverName = "https://xn--multipli-i1a.fr/minimit/prod/";

// ??
int timeoutAmount = 5000;  //timer Galerie
long nextUpdate = 0;       // timer Galerie
int interruption;          // interrompre boucle galerie

String currentEcran;  //dans chaque service, sur quel écran est-on
int PONGactive;       // ? voir si ca sert encore

int isConnected;        //l'utilsateur est connecté
String currentService;  //le minimit service courant, voir si ca sert encore

//

// ** SETUP MAIN
// on fait l'init du Minitel, flash et on affiche la mire
// si la config n'est pas OK ou que c'est la première fois on appelle une fonction qui va gérer ça
// et qui retournera quand tout sera OK

void setup() {
  Serial.begin(115200);
  Serial.println("**** Minimit is launching");
  minitel.aiguillage(true, CODE_EMISSION_CLAVIER, CODE_RECEPTION_CLAVIER);
  minitel.pageMode();
  minitel.noCursor();
  minitel.echo(false);
  minitel.changeSpeed(1200);
  SPIFFS.begin(true);
  displayDemarrage();
  init_and_displayMire(0);
}
//AFFICHAGE LOGO MINIMIT


// ** MAIN LOOP
// la main loop attend une action utilisateur sur la mire MiniMit
// en fonction de ça elle lance via launchService les différentes services, que ce soit GUIDE ou Astro ...

void loop() {
  champVide(12, 17, 29);
  while (1) {
    wait_for_user_action();
    // GESTION TOUCHE CMD
    switch (touche) {
      // ON LANCE LE SERVICE PUIS ON RETABLIT LA MIRE QUAND LE SERVICE A TERMINE
      case CONNEXION_FIN:
      minitel.connexion(false);
      break;

      case ENVOI:
        {
          int isConnexionNeeded = launchService(userInput);
          if (isConnexionNeeded == 1) {
            ligneZeroSafe("Ce service nécessite une connexion Internet.");
            delay(2000);
            ligneZeroSafe(" ");
            return;
          }
          if (isConnexionNeeded == 2) {
            ligneZeroSafe("Ce service nous est inconnu.");
            delay(2000);
            ligneZeroSafe(" ");
            return;
          }
          init_and_displayMire(1);
        }
        break;
      case SUITE:
        launchService("CREDITS");
        init_and_displayMire(1);
        break;
      case GUIDE:
        launchService("GUIDE");
        init_and_displayMire(1);
        break;
      case SOMMAIRE:
        launchService("CONFIG");
        init_and_displayMire(0);
        break;
      case ANNULATION:
        champVide(12, 17, 29);
        break;
      case CORRECTION:
        {
          if (userInputLength > 0) {
            minitel.moveCursorLeft(1);
            minitel.print(".");
            minitel.attributs(CARACTERE_BLANC);
            minitel.moveCursorLeft(1);
            userInput = userInput.substring(0, userInput.length() - 1);
            userInputLength--;
          }
        }
        break;
    }
  }
}

// *** launchService
// cette fonction lance un service en fonction de son code
// elle retourne quand le service a terminé
// généralement parce qu'il y a eu un CNX/FIN

int launchService(String minimit_service) {

  myObject = (JSONVar){};

  minimit_service.toUpperCase();

  Serial.println("Launch service " + minimit_service);

  if (minimit_service == "ANNUAIRE" || minimit_service == "1") {
    if (!isConnected) {
      return 1;
    }
    setupAnnuaire();
    loopAnnuaire();
    return 0;
  }

  if (minimit_service == "ASTRO" || minimit_service == "2") {
    if (!isConnected) {
      return 1;
    }
    setupAstro();
    loopAstro();
    return 0;
  }

  if (minimit_service == "COUPLESPARFAITS" || minimit_service == "3" || minimit_service == "CP") {
    if (!isConnected) {
      return 1;
    }
    setupCP();
    loopCP();
    return 0;
  }

  if (minimit_service == "ELIZA" || minimit_service == "4") {
    setupEliza();
    loopEliza();
    return 0;
  }

  if (minimit_service == "FORTUNE" || minimit_service == "5") {
    if (!isConnected) {
      return 1;
    }
    setupCookies();
    loopCookies();
    return 0;
  }

  if (minimit_service == "GALERIE" || minimit_service == "6") {
    if (!isConnected) {
      return 1;
    }
    setupGalerie();
    loopGalerie();
    return 0;
  }

  if (minimit_service == "LEMONDE" || minimit_service == "7") {
    if (!isConnected) {
      return 1;
    }
    setupLeMonde();
    loopLeMonde();
    return 0;
  }

  if (minimit_service == "METEO" || minimit_service == "8") {
    if (!isConnected) {
      return 1;
    }
    setupMeteo();
    loopMeteo();
    return 0;
  }

  if (minimit_service == "NABAZTAG" || minimit_service == "9") {
    if (!isConnected) {
      return 1;
    }
    setupNabaztag();
    loopNabaztag();
    return 0;
  }

  if (minimit_service == "PENDU" || minimit_service == "10") {
    setupPendu();
    loopPendu();
    return 0;
  }

  if (minimit_service == "PONG" || minimit_service == "11") {
    setupPong();
    loopPong();
    return 0;
  }


  if (minimit_service == "PPP" || minimit_service == "12") {
    if (!isConnected) {
      return 1;
    }
    setupPPP();
    loopPPP();
    return 0;
  }
  if (minimit_service == "TAROT" || minimit_service == "13") {
    if (!isConnected) {
      return 1;
    }
    setupTarots();
    loopTarots();
    return 0;
  }
  if (minimit_service == "CONFIG") {
    setupConfig();
    loopConfig();
    return 0;
  }

  if (minimit_service == "CREDITS") {
    setupCredits();
    loopCredits();
    return 0;
  }

  if (minimit_service == "GUIDE") {
    setupGuide();
    loopGuide();
    return 0;
  }

  if (minimit_service == "OTA") {
    setupOTA();
    loopOTA();
    return 0;
  }

  return 2;
}

// *** init_and_displayMire
// l'init et l'affichage de la mire
// j'ajoute init parce que l'idée est dêtre sur que les paramètres sont bien rétablis
// au cas où des services ont fait n'importe quoi
void init_and_displayMire(boolean deconnecter) {

  currentService = "";
  //minitel.connexion(false); // Si je mets ca la, ca s'arrete quand on vient de demarrage et qu'on est pas connecté
  if (deconnecter) {
    minitel.connexion(false);
  }
  saisieColor = CARACTERE_JAUNE;
  minitel.attributs(CARACTERE_JAUNE);

  minitel.aiguillage(true, CODE_EMISSION_CLAVIER, CODE_RECEPTION_CLAVIER);
  minitel.pageMode();
  minitel.newScreen();
  minitel.newXY(1, 0);
  minitel.attributs(FOND_NORMAL);
  minitel.print(" ");
  minitel.repeat(30);
  minitel.attributs(CARACTERE_BLANC);

  String mire = "14,0c,1f,41,41,0e,1b,5a,40,26,23,23,4d,30,1f,42,41,0e,1b,5a,31,09,38,12,43,1f,43,41,0e,1b,5a,40,46,12,42,26,40,1f,44,41,0e,1b,5a,41,31,21,21,09,38,1f,45,42,0e,1b,5a,2b,2c,2c,26,1f,41,4a,0e,1b,54,20,12,5e,1f,42,4a,0e,1b,54,20,12,5e,1f,43,4a,0e,1b,54,20,12,5e,1f,44,4a,0e,1b,54,20,12,5e,1f,42,4b,0e,1b,54,20,12,42,30,20,20,30,20,12,42,30,30,1f,43,4b,0e,1b,57,1b,44,48,48,1b,54,1b,47,34,34,1b,57,1b,44,48,1b,54,1b,47,34,34,1b,57,1b,44,48,48,1b,54,1b,47,34,34,1b,57,1b,44,48,1f,44,4b,0e,1b,54,25,12,4a,29,1f,44,60,1b,4f,33,36,31,35,1f,45,4a,1b,44,60,12,5e,1f,46,4a,1b,44,1b,5d,18,53,69,20,6f,6e,20,19,42,65,74,61,69,74,20,65,6e,63,6f,72,65,20,64,61,6e,73,20,6c,65,73,20,1f,47,4a,1b,44,1b,5d,18,61,6e,6e,19,42,65,65,73,20,38,30,2c,20,6c,61,20,63,6f,6e,6e,65,78,69,6f,6e,20,64,65,1f,48,49,1b,5a,20,1b,5d,1b,44,18,63,65,20,6d,69,6e,69,74,65,6c,20,76,6f,75,73,20,61,75,72,61,69,74,20,63,6f,19,43,75,74,19,42,65,1f,49,49,1b,5a,20,1b,44,1b,5d,18,30,2c,31,32,46,20,19,41,61,20,6c,61,20,63,6f,6e,6e,65,78,69,6f,6e,1f,4a,49,1b,5a,20,1b,44,1b,5d,18,70,75,69,73,20,31,2c,32,39,46,20,6c,61,20,6d,69,6e,75,74,65,1f,4b,49,1b,5a,20,1b,44,1b,5d,18,64,6f,6e,74,20,46,2e,54,45,4c,45,43,4f,4d,20,30,2c,31,32,20,19,41,61,20,30,2c,35,30,46,2f,6d,69,6e,1f,4c,49,1b,5a,20,1b,5d,1b,45,18,20,12,45,53,45,52,56,49,43,45,53,20,44,49,53,50,4f,4e,49,42,4c,45,53,1f,4d,4a,1b,44,1b,5d,18,41,6e,6e,75,61,69,72,65,2c,20,6d,19,42,65,74,19,42,65,6f,2c,20,61,63,74,75,61,6c,69,74,19,42,65,73,2c,1f,4e,4a,1b,44,1b,5d,18,61,73,74,72,6f,2c,20,74,61,72,6f,74,73,2e,12,42,20,12,42,74,61,70,65,7a,20,47,55,49,44,45,1f,4f,41,1b,44,60,12,67,1f,50,41,63,6f,64,65,0d,0a,64,75,20,73,65,72,76,69,63,65,3a,1b,43,2e,12,5c,1b,44,0a,60,12,67,0a,28,63,29,0a,0d,6d,75,6c,74,69,70,6c,69,19,42,65,0a,0d,32,30,32,33,1f,53,61,1b,5d,20,45,6e,76,6f,69,20,20,1f,56,4b,1b,4d,41,4e,4e,55,41,49,52,45,20,44,45,53,20,53,45,52,56,49,43,45,53,1b,5a,20,1b,5d,20,47,75,69,64,65,20,20,1f,57,4b,50,61,72,61,6d,19,41,65,74,72,65,73,20,64,75,20,6d,69,6e,69,6d,69,74,1b,5a,20,1b,5d,1b,42,53,6f,6d,6d,61,69,72,65,1f,58,51,51,75,69,20,61,20,66,61,69,74,20,19,4b,63,61,20,3f,20,1b,42,1b,5d,20,53,75,69,74,65,20,20,1f,51,4c";
  checkScreen(mire, 0, 0);
  champVide(12, 17, 29);
  minitel.echo(true);

  
}