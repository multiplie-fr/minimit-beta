# Minitel1B_Hard
<b>Minitel Library for Arduino (with HardwareSerial)</b>

Documentation utilisée : Spécifications Techniques d'Utilisation du Minitel 1B<br>
http://543210.free.fr/TV/stum1b.pdf

<b>Historique</b> :

25/06/2022 - Sur proposition de iodeo :<br>
Correction d'un bug dans la fonction byte reset()

24/04/2022 - Ajout d'un exemple : Portrait.ino (affichage d'une image)<br>

13/03/2022 - Ajout d'un exemple basique : HelloWorld.ino<br>

21/02/2022 - Sur proposition de iodeo :<br>
Ajout d'une option qui permet de désactiver la conversion des caractères en ascii étendue (cela sert en particulier pour la communication avec des services Minitel externes) :<br>
unsigned long getKeyCode(bool ascii = true)

08/10/2021 - Sur proposition de iodeo :<br>
Correction de bugs mineurs dans echo(boolean commande) et changeSpeed(int bauds).<br>
Ajout d'une nouvelle fonction liée au modem (équivalent de la touche CONNEXION / FIN) :<br>
byte connexion(boolean commande)<br>
Patch pour Arduino-ESP32 core v1.0.6 https://github.com/espressif/arduino-esp32<br>

02/07/2021 - 1 nouvelle fonction :<br>
byte reset()<br>

01/07/2021 - 1 nouvelle fonction :<br>
byte statusAiguillage(byte module)<br>
et mise à jour de :<br>
byte pageMode()<br>
byte scrollMode()<br>
byte modeMixte()<br>
byte modeVideotex()<br>
byte standardTeleinformatique()<br>
byte standardTeletel()<br>
byte smallMode()<br>
byte capitalMode()<br>
byte extendedKeyboard()<br>
byte standardKeyboard()<br>
byte echo(boolean commande)<br>
byte aiguillage(boolean commande, byte emetteur, byte recepteur)<br>

29/06/2021 - 5 nouvelles fonctions :<br>
void writeWord(word w)<br>
int modeMixte()<br>
int modeVideotex()<br>
int standardTeleinformatique()<br>
int standardTeletel()<br>


28/06/2021 - Modification de la fonction :<br>
unsigned long getKeyCode()<br>
pour tenir compte des codes du clavier étendu.<br>
Dans le fichier d'entête (.h), ajout de plusieurs #define dans la partie "Clavier".<br>
2 nouvelles fonctions :<br>
void echo(boolean commande)<br>
void aiguillage(boolean commande, byte emetteur, byte recepteur)

11/05/2018 - Modification de deux fonctions :<br>
void printDiacriticChar(unsigned char caractere)<br>
boolean isDiacritic(unsigned char caractere)<br>
pour tenir compte de la remarque de BorisFR sur les caractères diacritiques majuscules.

18/06/2017 - Exemple Tweet_3615.ino<br>

15/06/2017 - 2 nouvelles fonctions qui donnent la position du curseur : <br>
int getCursorX()<br>
int getCursorY()<br>

06/06/2017 - Exemple d'utilisation du clavier et d'affichage d'une image :<br>
Couleurs_3615.ino<br>

06/06/2017 - 2 nouvelles fonctions graphiques :<br>
void graphic(byte b, int x, int y)<br>
void graphic(byte b)<br>

12/03/2016 - Début de la version HardwareSerial de Minitel1B sur GitHub<br>
