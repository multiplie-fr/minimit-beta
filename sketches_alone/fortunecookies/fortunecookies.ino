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
// const char* ssid = "LaVillamour";
// const char* password = "lavlina12";
const char* serverName = "https://xn--multipli-i1a.fr/minimit/prod/";

void setup() {
  Serial.begin(115200);
  minitel.newScreen();
  wifiConnect();
  afficheRemoteVDT("fc_accueil.vdt", 0, 0);
  minitel.noCursor();
  minitel.echo(false);
}

void lectureChamp(int x, int y, int longueurchamp) {
  //champVide(x, y, longueurchamp);
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
        FC_retrieveDatas();
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
  lectureChamp(16, 5, 5);
  //userInput = "";
}



void FC_retrieveDatas() {
  Serial.print(userInput);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("fortunecookies/getjson.php");
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      //Serial.println(buf[i], HEX);
      //afficheVDT({payload}, 130, 2,2);
      JSONVar myObject = JSON.parse(payload);
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      //JSONVar description = myObject["weather"][0]["description"];
      // Serial.println("OOO");
      //Serial.println(description);
      // Serial.print(mainmeteo["temp"]);
      afficheRemoteVDT("fc_phrase.vdt", 0, 0);
      JSONVar cookietexte = myObject["texte"];
      int nbLines = cookietexte.length();
      minitel.attributs(CARACTERE_NOIR);
      minitel.attributs(FOND_BLANC);
      int posY = 15;
      minitel.moveCursorXY(6, posY);
      for (int j = 0; j < nbLines; j++) {
        minitel.moveCursorXY(6, posY);
        minitel.print((const char*)cookietexte[j]);
        posY++;
      }
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}