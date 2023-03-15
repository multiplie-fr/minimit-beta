#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <pgmspace.h>
#include "eliza.h"
#include <Minitel1B_Hard.h>  // Voir https://github.com/eserandour/Minitel1B_Hard
Minitel minitel(Serial1);    // Le deuxième port série matériel de l'ATMega 1284P (RXD1 TXD1).
String input = "";
int nbCaracteres = 0;
const int PREMIERE_LIGNE_EXPRESSION = 1;
const int NB_LIGNES_EXPRESSION = 4;
const String VIDE = ".";
const char* ssid = "Livebox-Xine";
const char* password = "malakoff";
//const char* ssid = "SFR_81F0";
//const char* password = "dogmysodsedcebjiags4";
//const char* ssid = "iPhone";
//const char* password = "tttttttt";
const char* serverName = "https://xn--multipli-i1a.fr/minimit/prod/";
unsigned long touche;
int lastLine = 0;

void setup() {
  Serial.begin(115200);
  minitel.newScreen();
  //displayInterface();
  
}

void loop() {
  lectureChamp(PREMIERE_LIGNE_EXPRESSION, NB_LIGNES_EXPRESSION);
}

void displayInterface() {
  minitel.moveCursorXY(35, 4);
  minitel.attributs(INVERSION_FOND);
  minitel.print("ENVOI");
  minitel.attributs(FOND_NORMAL);
  //minitel.moveCursorDown(premiereLigne - 1);
  //minitel.cursor();
}
void champVide(int premiereLigne, int nbLignes) {
  minitel.noCursor();
  minitel.moveCursorXY(1, premiereLigne);
  //minitel.clearScreenFromCursor();
  for (int j = 0; j < nbLignes; j++) {
    minitel.attributs(CARACTERE_BLEU);
    minitel.print(VIDE);
    minitel.repeat(39);
    minitel.attributs(CARACTERE_BLANC);
  }
  input = "";
  nbCaracteres = 0;
  displayInterface();
  minitel.moveCursorXY(1, premiereLigne);
  minitel.cursor();
}
void lectureChamp(int premiereLigne, int nbLignes) {
  champVide(premiereLigne, nbLignes);
  boolean fin = false;
  Serial.println("le");
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (nbCaracteres < 40 * nbLignes) {
        nbCaracteres++;
        input += char(touche);
        Serial.println(input);
      }
      if (nbCaracteres == 40 * nbLignes) {
        minitel.moveCursorXY(40, (premiereLigne - 1) + nbLignes);
      }
    }
    switch (touche) {
      case ENVOI:
        {
          minitel.scrollMode();
          input.trim();
          input.toLowerCase();
          std::string output = eliza_prompt(input.c_str());
          minitel.moveCursorXY(1, 6+lastLine);
          minitel.attributs(CARACTERE_BLANC);
          
          //on affiche la question
          String strq = String(">ME : ") + String(input.c_str());
          // Length (with one extra character for the null terminator)
          int strq_len = strq.length() + 1;
          // Prepare the character array (the buffer)
          char char_arrayq[strq_len];
          // Copy it over
          strq.toCharArray(char_arrayq, strq_len);
          wrap_string(char_arrayq);
          char* pq = char_arrayq;
          char* striq;

          while ((striq = strtok_r(pq, "+", &pq)) != NULL){  // delimiter is the semicolon
            minitel.println(striq);
            lastLine++;
        }

          String str = String(">ELIZA : ") + String(output.c_str());
          // Length (with one extra character for the null terminator)
          int str_len = str.length() + 1;
          // Prepare the character array (the buffer)
          char char_array[str_len];
          // Copy it over
          str.toCharArray(char_array, str_len);
          wrap_string(char_array);
          Serial.println(char_array);
          //effacementEcran(4, 7);
          //minitel.print(output.c_str());
          //minitel.print(String(char_array.length()));

          Serial.print(char_array);
          char* p = char_array;
          char* stri;

          while ((stri = strtok_r(p, "+", &p)) != NULL){  // delimiter is the semicolon
            minitel.println(stri);
            lastLine++;
        }
         //minitel.println(String(char_array[0]));
        champVide(premiereLigne, nbLignes);
        
       
        fin = true;
    }
    break;
    case ANNULATION:
      champVide(premiereLigne, nbLignes);
      break;
    case CORRECTION:
      correction(nbLignes);
      break;
  }
}
}
void wrap_string(char* str) {
  const int wrap_len = 40;  // la longueur de la ligne de wrapper
  int len = strlen(str);    // la longueur de la chaîne de caractères
  if (len<wrap_len)return;
  int i = 0;

  while (i < len) {
    // Trouver la position du dernier espace avant la limite de 40 caractères
    int space_pos = -1;
    int j;
    for (j = i + wrap_len; j > i; j--) {
      if (isspace(str[j])) {
        space_pos = j;
        Serial.println(str);
        break;
      }
    }
    if (space_pos != -1) {  // Si un espace a été trouvé, couper la chaîne à cette position
       str[space_pos] = '+';
      if((len-space_pos)<wrap_len)return;
      
      i = space_pos + 1;
    } else {  // Si aucun espace n'a été trouvé, couper la chaîne à la limite de 40 caractères
      str[i + wrap_len] = '=';
      i += wrap_len + 1;
    }
  }
}

void correction(int nbLignes) {
  if ((nbCaracteres > 0) && (nbCaracteres <= 40 * nbLignes)) {
    if (nbCaracteres != 40 * nbLignes) { minitel.moveCursorLeft(1); }
    minitel.attributs(CARACTERE_BLEU);
    minitel.print(VIDE);
    minitel.attributs(CARACTERE_BLANC);
    minitel.moveCursorLeft(1);
    input = input.substring(0, input.length() - 1);
    nbCaracteres--;
  }
}