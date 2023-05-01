#include "eliza.h"

int lastLine = 0;

void setupEliza() {
  minitel.newScreen();
  displayInterface();
  
}

void displayInterface() {
  minitel.moveCursorXY(35, 4);
  minitel.attributs(INVERSION_FOND);
  minitel.print("ENVOI");
  minitel.attributs(FOND_NORMAL);
 }
void champVideEliza(int premiereLigne, int nbLignes) {
  minitel.noCursor();
  minitel.moveCursorXY(1, premiereLigne);
  //minitel.clearScreenFromCursor();
  for (int j = 0; j < nbLignes; j++) {
    minitel.attributs(CARACTERE_BLEU);
    minitel.print(".");
    minitel.repeat(39);
  }
  minitel.attributs(CARACTERE_BLANC);
  
  userInput = "";
  userInputLength = 0;
  displayInterface();
  minitel.moveCursorXY(1, premiereLigne);
  minitel.cursor();
  Serial.print("champvideEliza");
}
void lectureChampEliza(int premiereLigne, int nbLignes) {
  champVideEliza(premiereLigne, nbLignes);
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    if ((touche != 0) && (touche != CONNEXION_FIN) && (touche != SOMMAIRE) && (touche != ANNULATION) && (touche != RETOUR) && (touche != REPETITION) && (touche != GUIDE) && (touche != CORRECTION) && (touche != SUITE) && (touche != ENVOI)) {
      if (userInputLength < 40 * nbLignes) {
        userInputLength++;
        userInput += char(touche);
        Serial.println(userInput);
      }
      if (userInputLength == 40 * nbLignes) {
        minitel.moveCursorXY(40, (premiereLigne - 1) + nbLignes);
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
          minitel.scrollMode();
          userInput.trim();
          userInput.toLowerCase();
          std::string output = eliza_prompt(userInput.c_str());
          minitel.moveCursorXY(1, 6+lastLine);
          
          //on affiche la question
          String strq = String(">ME : ") + String(userInput.c_str());
          Serial.println("strq");
          Serial.println(strq);          
          // Length (with one extra character for the null terminator)
          int strq_len = strq.length() + 1;
          // Prepare the character array (the buffer)
          char char_arrayq[strq_len];
          // Copy it over
          strq.toCharArray(char_arrayq, strq_len);
          wrap_string(char_arrayq);
          char* pq = char_arrayq;
          char* striq;
          minitel.attributs(CARACTERE_CYAN);
          while ((striq = strtok_r(pq, "+", &pq)) != NULL){  
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
          minitel.attributs(CARACTERE_BLANC);          
          while ((stri = strtok_r(p, "+", &p)) != NULL){  // delimiter is the semicolon
            minitel.println(stri);
            lastLine++;
        }
         //minitel.println(String(char_array[0]));
        champVideEliza(premiereLigne, nbLignes);
        fin = true;
    }
    break;
    case ANNULATION:
      champVideEliza(premiereLigne, nbLignes);
      break;
    case CORRECTION:
      correctionEliza(4);
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

void correctionEliza(int nbLignes) {
  if ((userInputLength > 0) && (userInputLength <= 40 * nbLignes)) {
    if (userInputLength != 40 * nbLignes) { minitel.moveCursorLeft(1); }
    minitel.attributs(CARACTERE_BLEU);
    minitel.print(".");
    minitel.attributs(CARACTERE_BLANC);
    minitel.moveCursorLeft(1);
    userInput = userInput.substring(0, userInput.length() - 1);
    userInputLength--;
  }
}