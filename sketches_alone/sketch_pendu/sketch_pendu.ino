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
  afficheSommairePendu();
}
void afficheSommairePendu() {
  minitel.newScreen();
  afficheRemoteVDT("pendu/pendu-1.vdt", 0, 0);
  retrieveDatasPENDU();
  
}
void lectureChamp(int x, int y, int longueurchamp) {
  champVide(x, y,longueurchamp);
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (userInputLength < longueurchamp) {
        userInputLength++;
        userInput += char(touche);
        minitel.echo(false);
     } else {
        //minitel.moveCursorLeft(1);
        //minitel.print(" ");
        //minitel.moveCursorLeft(1);
      }
    }
    switch (touche) {
      case ENVOI:
        fin = true;
        if (userInputLength == 1) {
          joueLettre();
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
         
        }
        break;
      case SOMMAIRE:
        {
          fin=true;
          afficheSommairePendu();
          userInput = "";
          userInputLength = 0;
        }
        break;
    }
  }
}

void loop() {
  lectureChamp(11, 17, 1);
  //userInput = "";
}
void finOK(){
  minitel.newXY(4, 21);
  minitel.attributs(DOUBLE_GRANDEUR);
  minitel.attributs(CARACTERE_NOIR);
  minitel.print("Vous avez gagné !");
  //effacement nav
  minitel.attributs(GRANDEUR_NORMALE);
  minitel.newXY(2,17);
  minitel.attributs(FOND_MAGENTA);
  minitel.graphicMode();
  minitel.print(" ");
  minitel.repeat(16);
  minitel.newXY(2,18);
  minitel.attributs(FOND_MAGENTA);
  minitel.graphicMode();
  minitel.print(" ");
  minitel.repeat(16);
  minitel.newXY(2,17);
  minitel.print("Rejouer : SOMMAIRE");
}
void finKO(){
  minitel.newXY(4, 21);
  minitel.attributs(DOUBLE_GRANDEUR);
  minitel.attributs(CARACTERE_NOIR);
  minitel.print("Vous avez perdu !");
  //effacement nav
  minitel.attributs(GRANDEUR_NORMALE);
  minitel.newXY(2,17);
  minitel.attributs(FOND_MAGENTA);
  minitel.graphicMode();
  minitel.print(" ");
  minitel.repeat(16);
  minitel.newXY(2,18);
  minitel.attributs(FOND_MAGENTA);
  minitel.graphicMode();
  minitel.print(" ");
  minitel.repeat(16);
  minitel.newXY(2,17);
  minitel.print("Rejouer : SOMMAIRE");
}
void joueLettre(){
  String proposed = (const char*)myObject["proposed"];
  int indexProposed = proposed.indexOf(userInput);
  if (indexProposed > -1)
  {
   return;
  }
  myObject["proposed"] = proposed + userInput + ",";

  JSONVar motok = myObject["myDatas"]["root"]["motok"];
  int matches = 0;
  for(int i = 0; i<motok.length();i++)
  {
    String lettre = (const char*)motok[i];
    if (lettre==userInput) 
    {
      minitel.moveCursorXY(3+i, 5);
      minitel.print(userInput);
      matches++;
    }
   
  }
   if(matches==0)
    {
      int ko = myObject["ko"];
      ko++ ;  
      myObject["ko"]=ko;   
      //afficheRemoteVDT("pendu/potence-"+String(ko)+".vdt", 0, 0);
      JSONVar vdt = myObject["potence"+String(ko)];
      String potence = (const char*)vdt;
      checkScreen(potence,0,0);
      if(ko==7)
      {
        finKO();
      }
    }
    else {
    int ok = myObject["ok"];
      ok+=matches;  
      myObject["ok"]=ok;  

      if(ok == motok.length())
      {
          finOK();        
      }
    }
    Serial.println(myObject);
}
void afficheDatasPENDU() {
  minitel.noCursor();
  Serial.println(myObject["myDatas"]["root"]);
  JSONVar motok = myObject["myDatas"]["root"]["motok"];
  myObject["potence1"]="1f,44,5e,0e,1b,55,1b,5a,35,1f,45,5e,0e,1b,55,1b,5a,35,1f,46,5e,0e,1b,55,1b,5a,35,1f,47,5e,0e,1b,55,1b,5a,35,1f,4a,43";
  myObject["potence2"]="1f,48,5d,0e,1b,57,1b,45,49,1b,55,1b,47,23,34,1f,49,5d,0e,1b,57,1b,45,4a,1b,55,20,1b,57,4a,1f,4a,5d,0e,1b,55,22,23,1f,4b,43";
  myObject["potence3"]="1f,4a,5e,0e,1b,57,1b,45,48,1f,4b,5e,0e,1b,57,1b,45,4a,1f,4c,5e,0e,1b,57,1b,45,4a,1f,4d,5e,0e,1b,57,1b,45,4a,1f,4e,5e,0e,1b,55";
  myObject["potence4"]="1f,4d,43,1f,4a,5d,0e,1b,55,42,1f,4b,5c,0e,1b,55,38,21,1f,4c,5b,0e,1b,55,26,1f,4e,43";
  myObject["potence5"]="1f,4a,5e,0e,1b,57,1b,45,28,1f,4b,5f,0e,1b,55,29,30,1f,4c,60,0e,1b,55,22,24,1f,48,4b";
  myObject["potence6"]="1f,4e,5d,0e,1b,55,38,1f,4f,5a,0e,1b,55,40,40,26,1f,50,5b,0e,1b,55,21,1f,49,4b";
  myObject["potence7"]="1f,4e,5e,1f,4e,5e,0e,1b,55,29,30,1f,4f,5f,0e,1b,55,22,44,40,1f,50,61,0e,1b,55";
  myObject["ok"]= 0;
  myObject["ko"]= 0;
  myObject["proposed"] = "";
  minitel.newXY(3, 5);
  minitel.print("-");
  minitel.repeat(motok.length()-1);
}
void retrieveDatasPENDU() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("pendu/getjson.php");
    Serial.println(serverPath);
    // Your Domain name with URL path or IP address with path
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
      myObject["myDatas"] = myDatas;
      afficheDatasPENDU();
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}