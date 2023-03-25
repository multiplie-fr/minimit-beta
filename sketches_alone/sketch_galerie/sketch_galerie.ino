#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <pgmspace.h>
#include <Minitel1B_Hard.h>  // Voir https://github.com/eserandour/Minitel1B_Hard
Minitel minitel(Serial1);    // Le deuxième port série matériel de l'ATMega 1284P (RXD1 TXD1).

String userInput = "";
int userInputLength = 0;
unsigned long touche;
// const char* ssid = "Livebox-Xine";
// const char* password = "malakoff";
const char* ssid = "SFR_81F0";
const char* password = "dogmysodsedcebjiags4";
//const char* ssid = "iPhone";
//const char* password = "tttttttt";
const char* serverName = "https://xn--multipli-i1a.fr/minimit/prod/";
JSONVar myObject = {};

void setup() {
  Serial.begin(115200);
  minitel.pageMode();
  minitel.newScreen();
  wifiConnect();
  minitel.echo(false);
  myObject["current"] = 0;
  retrieveDatasGALERIE();
}
void lectureChamp(int x, int y, int longueurchamp) {
  //champVide(x, y, longueurchamp);
  boolean fin = false;
    switch (touche) {
      
      case REPETITION:
        break;
      case SOMMAIRE:
        break;
  }
}

void loop() {
  lectureChamp(28, 3, 0);
  delay(5000);
  int currentImage = myObject["current"];
  int nbImages = myObject["datas"]["root"].length();
  Serial.println("nbimages");
  Serial.println(nbImages);
  if(currentImage<(nbImages-1))
  {
    currentImage++;
  }
  else {
    currentImage=0;
  }
  myObject["current"]= currentImage; 
  afficheDatasGALERIE();
}


void afficheDatasGALERIE() {
  minitel.noCursor();
  int currentImage = myObject["current"];
  //currentImage++;
  myObject["current"]= currentImage;  
  String myImage = (const char*)myObject["datas"]["root"][currentImage];
  Serial.println(currentImage);
  Serial.println(myImage);
  afficheRemoteVDT("galerie/"+myImage, 0, 0);
  
}
void retrieveDatasGALERIE() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + String("galerie/getjson.php");
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      JSONVar myJSONObject = JSON.parse(payload);
      Serial.println("");
      Serial.print(payload);
      Serial.println("");
       myObject["datas"] = myJSONObject;
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      afficheDatasGALERIE();
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}