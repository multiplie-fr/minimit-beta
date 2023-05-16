#include "eliza.h"

int lastLine = 0;

void setupEliza() {
  minitel.newScreen();
  currentEcran = "SOMMAIRE";
  String vdt = "0c,1f,41,41,20,12,42,7d,20,09,20,09,09,1b,45,44,49,41,4c,4f,47,55,45,20,12,4d,09,09,20,1b,47,5f,1f,42,41,20,20,09,7d,20,09,20,09,09,1b,45,45,4e,20,4c,49,42,45,52,54,45,09,09,09,09,09,09,09,09,09,09,09,09,09,09,1b,47,7d,7e,7e,1b,4e,1b,42,5c,1f,43,41,20,09,20,7d,20,09,20,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,0e,1b,5a,60,0f,20,1b,45,5f,20,1b,4e,1b,47,2f,1b,4c,20,09,09,09,1b,45,5c,1f,44,41,20,20,5f,7d,20,20,5f,5f,09,09,5f,09,09,09,09,1b,4e,2f,5c,09,09,09,09,09,09,09,09,2f,1b,4c,7e,7e,1b,45,7e,20,20,09,09,09,09,09,09,1f,45,41,20,2f,7d,12,42,09,20,7d,09,2f,7d,09,09,09,1b,42,2f,09,09,09,09,5c,09,09,09,09,09,09,2f,09,20,20,09,09,1b,47,7e,1b,45,5c,09,09,09,09,09,1b,4d,1b,41,5c,1f,46,41,7d,20,7d,7d,20,09,20,2f,7d,09,7d,09,09,1b,42,2f,09,09,09,09,09,09,5c,0e,1b,46,1b,5a,70,1b,47,70,12,42,30,1f,47,41,7d,5f,2f,7d,7d,09,2f,09,7d,09,7d,09,09,09,09,20,0e,1b,43,1b,5a,78,20,09,09,20,1b,5a,2b,7f,7f,3f,0f,20,12,42,09,09,09,09,09,0e,1b,5a,20,1b,5a,34,1b,59,20,20,1b,5a,60,7e,7f,1f,48,41,7d,09,20,7d,12,42,20,09,7d,09,7d,09,09,0e,20,1b,46,1b,5a,60,7e,7f,75,09,09,1b,59,20,20,1b,5a,22,23,21,0f,1b,5a,20,09,09,09,09,0e,1b,5a,60,7e,1b,59,20,20,1b,5a,7f,7c,7e,7f,12,42,1f,49,42,5c,20,7d,12,42,5f,5f,20,5c,5f,5c,09,09,0e,20,12,43,0f,1b,45,5c,09,09,09,09,09,09,09,09,09,0e,1b,42,1b,5a,78,7e,7f,3f,09,09,7f,12,45,1f,4a,43,1b,45,7e,0e,20,12,43,0f,20,0e,20,20,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,09,0f,1b,42,5f,12,43,20,09,09,0e,1b,5a,7f,12,45,1f,4b,41,0e,1b,45,1b,5a,7f,12,43,7d,70,0f,1b,5a,20,0e,1b,5a,20,70,70,78,7c,12,43,74,70,12,43,30,0f,1b,5a,20,0e,1b,5a,20,1b,59,20,12,43,0f,7e,7e,0e,20,12,43,1b,5a,60,7f,12,45,1f,4c,41,0e,1b,45,1b,5a,7f,12,54,7c,70,12,42,78,7c,12,46,7f,12,46,1f,4d,41,0e,1b,55,1b,45,1b,5a,23,12,67,1f,4e,41,0e,1b,55,20,12,67,1f,4f,41,0e,1b,55,20,12,67,1f,50,41,0e,1b,55,20,12,67,1f,51,41,0e,1b,55,20,12,67,1f,52,41,0e,1b,55,20,12,67,1f,53,41,0e,1b,55,20,12,67,1f,54,41,0e,1b,55,20,12,67,1f,55,41,0e,1b,55,20,12,67,1f,56,41,0e,1b,55,20,12,67,1f,57,41,0e,1b,55,20,12,67,1f,58,41,0e,1b,55,20,12,67,1f,4e,42,56,6f,75,73,20,6c,27,61,74,74,65,6e,64,69,65,7a,2c,20,65,6c,6c,65,20,65,73,74,20,64,65,20,72,65,74,6f,75,72,2c,1f,4f,42,6d,61,69,73,20,73,6f,75,73,20,66,6f,72,6d,65,20,64,65,20,63,68,61,74,62,6f,74,2c,20,65,74,20,70,61,73,1f,50,42,6e,27,69,6d,70,6f,72,74,65,20,6c,65,71,75,65,6c,2e,20,1f,51,42,44,19,42,65,76,65,6c,6f,70,70,19,42,65,65,20,65,6e,20,31,39,36,34,20,70,61,72,20,4a,6f,73,65,70,68,1f,52,42,57,65,69,7a,65,6e,62,61,75,6d,20,61,75,20,4d,49,54,2c,20,65,6c,6c,65,20,73,69,6d,75,6c,61,69,74,20,75,6e,1f,53,42,70,73,79,63,68,6f,74,68,19,42,65,72,61,70,65,75,74,65,20,52,6f,67,65,72,69,65,6e,2e,20,1f,54,42,45,73,73,61,79,65,7a,2e,20,56,6f,75,73,20,61,6c,6c,65,7a,20,79,20,70,61,73,73,65,72,20,64,65,73,1f,55,42,68,65,75,72,65,73,2e,20,43,6f,6d,6d,65,20,19,41,61,20,6c,27,19,42,65,70,6f,71,75,65,2e,1f,57,41,1b,45,1b,5d,60,12,67,1f,58,4b,44,69,61,6c,6f,67,75,65,7a,20,61,76,65,63,20,45,6c,69,7a,61,20,19,2e,20,1b,5d,20,53,55,49,54,45,20";  
  //displayInterface();
  checkScreen(vdt, 0, 0);
  minitel.echo(false);
}

void displayInterface() {
  currentEcran="DIALOGUE";
  minitel.moveCursorXY(1, 3);
  minitel.attributs(CARACTERE_MAGENTA);
  // minitel.writeByte(0x60);
  // minitel.repeat(32);
  minitel.attributs(INVERSION_FOND);
  minitel.print(" Confiez-vous à Eliza ");
   minitel.writeByte(0x60);
   minitel.repeat(9);
  minitel.print("→ ENVOI ");
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
  if(currentEcran=="DIALOGUE"){
    champVideEliza(premiereLigne, nbLignes);
  }
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
          minitel.moveCursorXY(1, 4+lastLine);
          
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
          minitel.attributs(CARACTERE_MAGENTA);
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
    case SUITE:
      if(currentEcran=="SOMMAIRE"){
        minitel.newScreen();
        minitel.echo(true);
        displayInterface();
        champVideEliza(1, 2);
        break;
      }
     
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