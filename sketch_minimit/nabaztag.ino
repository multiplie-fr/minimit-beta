// NABASTAG
#include <WiFiClient.h>
const char* host = "nabaztag.local";
const int httpPort = 80;

WiFiClient client;

String CSRF_token;
String cookieValue;

// SETUP du service
void setupNabaztag() {
  Serial.println("setup Nabaztag");
  initMinitelService();
  wifiConnect();
  saisieColor=CARACTERE_MAGENTA;
  afficheSommaireNabaztag();
}


// LOOP du service
void loopNabaztag() {
  Serial.println("loop Nabaztag");
  while (1) {
  champVide(32, 24, 1);
      
    // Input
    wait_for_user_action();

    // Traitement de l'action utilisateur (maj des variables globales dans wait_for___)
    switch (touche) {

      // SI CONNEXION FIN on sort de la loop, on revient à la loop principale
      case CONNEXION_FIN:
        Serial.println("CONNEXION_FIN");
        return;
        break;

      // SI ENVOI on affiche la page du signe
      case ENVOI:
      {
          ligneZeroSafe("Connexion à votre lapin en cours");
            
          if(userInput=="1")
          {
            nabaztag_guru();
           
          }
          if(userInput=="2")
          {
            nabaztag_put("/nabsurprised/settings","");
          } 
          if(userInput=="2")
          {
            nabaztag_put("/nabsurprised/settings","");
          }
          if(userInput=="3")
          {
            nabaztag_put("/nabweatherd/settings", "type=today");
          }
           if(userInput=="4")
          {
            nabaztag_put("/nabairqualityd/settings", "");
            
          }
          ligneZeroSafe(" ");
            
      }
        
        break;

      case ANNULATION:
        Serial.println("ANNULATION");
        champVide(32, 24, 1);
        break;
    }
  }
}
// FIN LOOP

// Pour surprises :
// nabaztag_put("/nabsurprised/settings","");

// Pour la météo : 
// nabaztag_put("/nabweatherd/settings", "type=today");

// Pour l'air ;
// //nabaztag_put("/nabairqualityd/settings", "");

// Et pour le gourou :
// nabaztag_guru();

// Fonctions d'affichage et de traitement liées au service

void afficheSommaireNabaztag() {
  String vdt = "0c,14,1f,41,53,0e,1b,54,20,12,55,1f,42,53,0e,1b,54,20,12,55,1f,43,53,0e,1b,54,20,12,55,1f,44,53,0e,1b,54,20,12,55,1f,45,53,0e,1b,54,20,12,55,1f,46,53,0e,1b,54,20,12,55,1f,47,53,0e,1b,54,20,12,55,1f,48,53,0e,1b,54,20,12,55,1f,49,53,0e,1b,54,20,12,55,1f,4a,53,0e,1b,54,20,12,55,1f,4b,53,0e,1b,54,20,12,55,1f,4c,53,0e,1b,54,20,12,55,1f,4d,53,0e,1b,54,20,12,55,1f,4e,53,0e,1b,54,20,12,55,1f,4f,53,0e,1b,54,20,12,55,1f,50,53,0e,1b,54,20,12,55,1f,51,53,0e,1b,54,20,12,55,1f,52,53,0e,1b,54,20,12,55,1f,53,53,0e,1b,54,20,12,55,1f,54,53,0e,1b,54,20,12,55,1f,55,53,0e,1b,54,20,12,55,1f,56,53,0e,1b,54,20,12,55,1f,42,46,0e,50,1f,43,45,0e,48,1b,57,20,1b,50,34,1f,44,45,0e,4a,1b,57,20,1b,40,4a,1f,45,45,0e,4a,1b,57,20,1b,40,4a,1f,46,45,0e,4a,1b,57,20,1b,40,4a,1f,47,45,0e,4a,1b,57,20,1b,40,4a,1f,48,45,0e,4a,1b,57,20,1b,40,4a,1b,50,1b,47,40,50,50,09,09,09,09,09,09,09,09,1b,54,1b,40,12,43,30,1f,49,45,0e,4a,1b,57,20,12,4e,1b,40,50,1b,50,1b,44,4a,1f,4a,45,0e,1b,57,20,12,48,1b,40,48,1b,50,1b,47,23,12,42,21,1b,54,1b,40,23,23,21,20,1f,4b,44,0e,48,1b,57,20,20,1b,40,48,20,20,20,48,20,20,22,1f,4c,44,0e,4a,1b,57,20,20,1b,40,22,40,50,50,22,20,20,20,1f,4d,44,0e,4a,1b,57,20,12,43,1b,50,54,1b,57,1b,40,21,20,20,20,20,1f,4e,44,0e,1b,57,20,12,4a,1b,40,4a,1f,4f,44,0e,1b,57,20,12,4a,1b,40,4a,1f,50,44,0e,1b,57,20,12,4a,1b,40,4a,1f,51,43,0e,4a,1b,57,20,12,4b,1f,52,43,0e,4a,1b,57,20,12,4b,1f,53,43,0e,4a,1b,57,20,1b,56,21,20,1b,57,1b,46,34,48,1b,56,20,1b,47,22,1b,57,20,1b,56,21,20,1b,57,1b,46,34,20,1f,54,43,0e,1b,57,20,20,1b,56,54,50,1b,57,1b,46,21,22,1b,56,1b,47,50,58,1b,57,20,1b,56,54,50,1b,57,1b,46,21,20,1b,40,4a,1f,55,43,0e,1b,57,20,12,4c,1b,40,4a,1f,56,43,0e,22,1b,57,1b,40,50,1b,45,30,20,20,20,20,20,20,20,20,50,1b,40,50,1b,50,1b,47,21,1f,57,45,0e,1b,45,22,23,1b,55,1b,40,50,12,44,58,1b,50,1b,45,23,1f,41,4a,33,36,0a,08,08,31,35,1f,41,4c,0e,1b,45,4a,23,34,43,1b,55,1b,40,26,1f,42,4c,0e,1b,45,4a,09,1b,55,1b,40,4a,1b,50,1b,45,45,1b,55,1b,40,25,1f,43,4a,0e,1b,55,1b,40,43,1b,50,1b,45,44,28,2c,30,2c,4c,1f,44,4a,0e,1b,55,1b,40,48,1b,50,1b,45,49,48,23,1b,55,1b,40,4a,1b,50,1b,45,38,21,1f,45,4a,0e,1b,55,1b,40,2c,1b,50,1b,45,51,40,1b,55,1b,40,2c,1b,50,1b,45,21,43,1b,55,1b,40,2c,1f,46,4a,0e,1b,45,4a,09,40,2c,1b,55,1b,40,4a,4a,1b,50,1b,45,4c,1f,47,4a,0e,1b,45,22,2c,22,2c,25,29,26,1f,42,54,49,6e,74,65,72,61,67,69,73,73,65,7a,20,61,76,65,63,20,1f,43,54,76,6f,74,72,65,20,6c,61,70,69,6e,2c,20,74,61,70,65,7a,1f,47,5d,1b,46,1b,5d,1b,4d,31,1f,48,56,20,70,6f,75,72,20,6c,65,20,47,6f,75,72,6f,75,1f,4b,5d,1b,4d,1b,5d,1b,46,32,1f,4c,55,70,6f,75,72,20,6c,65,73,20,73,75,72,70,72,69,73,65,73,1f,4f,5d,1b,4d,1b,5d,1b,46,33,1f,50,57,70,6f,75,72,20,6c,61,20,6d,19,42,65,74,19,42,65,6f,1f,53,5d,1b,4d,1b,46,1b,5d,34,1f,54,56,70,6f,75,72,20,6c,61,20,71,75,61,6c,69,74,19,42,65,1f,55,5a,64,65,20,6c,27,61,69,72,1f,58,54,1b,45,56,6f,74,72,65,20,63,68,6f,69,78,20,2e,20,1b,5d,20,45,4e,56,4f,49,20,1f,58,60,11,1b,47";
  checkScreen(vdt, 0, 0);
}
int nabaztag_guru() {
  // port is 8080  / nabblockly

  Serial.println("Sending PUT pour le Guru");
   if (!client.connect(host, 8080)) {
    Serial.println("Connection failed.");
    return 0;
  }
  // Define the request body
  String body = "sequence=[{\"audio\":[\"nab8balld/answers/*.mp3\"],\"choreography\":\"urn:x-chor:streaming\"}]";
  // Send the HTTP request
  client.print("PUT /api/command HTTP/1.1\r\n");
  client.print("Host: ");
  client.println(host);
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(body.length());
  client.print("\r\n");
  client.print(body);
  int result = 0;
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line.startsWith("HTTP/1.1 204")) {
      result = 1;
    }
    Serial.println(line);
  }  
  client.stop();
  return result;
}

void nabaztag_put(String url, String param) {

  // GETTING THE TOKEN AND COOKIE
  Serial.println("Sending GET to get cookie and token");
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed.");
    return;
  }
  String request = "GET "+url+" HTTP/1.1\r\nHost: " + String(host) + "\r\n\r\n";
  client.print(request);
  while (client.connected()) {
    String headerName = "Set-Cookie:";
    String line = client.readStringUntil('\n');
    if (line.startsWith(headerName)) {
      cookieValue = line.substring(headerName.length() + 1);
      Serial.println("Cookie: " + cookieValue);
    }
    if (line.indexOf("csrfmiddlewaretoken") != -1) {
      String newone = line.substring(line.indexOf("csrfmiddlewaretoken")+28);
      CSRF_token = newone.substring(0, newone.indexOf('"'));
      Serial.println("CSRF_token: " + CSRF_token);
      break;
    }
  }  
  client.stop();
  // SENDING THE PUT REQUEST
  Serial.println("Sending PUT");
   if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed.");
    return;
  }
  // Define the request headers
  String headers = "X-CSRFToken: "+CSRF_token+"\r\n"+"Cookie:"+cookieValue+ "\r\n";
  // Define the request body
  String body = param;
  // Send the HTTP request
  client.print("PUT "+url+" HTTP/1.1\r\n");
  client.print("Host: ");
  client.println(host);
  client.print(headers);
  client.print("Content-Length: ");
  client.println(body.length());
  client.print("\r\n");
  client.print(body);
  while (client.connected()) {
      String line = client.readStringUntil('\n');
    }  
  client.stop();

}

