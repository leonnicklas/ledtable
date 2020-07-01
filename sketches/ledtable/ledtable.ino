// ----------------------------------------------------------------------------------------------------
// Bibliotheken

#include <Wire.h>                  //Bibliothek für das Centipede Shield
#include <Centipede.h>             //Bibliothek für das Centipede Shield

#include <Adafruit_NeoPixel.h>     //Bibliothek für die RGB-SMD-LEDs von Sparkfun

Centipede CS;                      //Bibliothekenzusatz für das Centipede Shield

// ----------------------------------------------------------------------------------------------------
// Grundeinstellung

#define PIN            7       //Digitaler Pin für Datenübertragung an RGB-SMD-LEDs von Sparkfun
#define NUMPIXELS      100     //Anzahl von angeschlossenen RGB-SMD-LEDs von Sparkfun

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);     //Grundeinstellungen für RGB-SMD-LEDs von Sparkfun als einzelne Pixel genutzt
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);      //Grundeinstellungen für RGB-SMD-LEDs von Sparkfun als Streifen genutzt

// ----------------------------------------------------------------------------------------------------
// Startvariablen

byte rotVal     = 0;     //Value für "Rot" zum kompletten Ausfüllen bei "Malen"
byte gelbVal    = 0;     //Value für "Gelb" zum kompletten Ausfüllen bei "Malen"
byte gruenVal   = 0;     //Value für "Grün" zum kompletten Ausfüllen bei "Malen"
byte tuerkisVal = 0;     //Value für "Türkis" zum kompletten Ausfüllen bei "Malen"
byte blauVal    = 0;     //Value für "Blau" zum kompletten Ausfüllen bei "Malen"
byte lilaVal    = 0;     //Value für "Lila" zum kompletten Ausfüllen bei "Malen"
byte weissVal   = 0;     //Value für "Weiß" zum kompletten Ausfüllen bei "Malen"
byte schwarzVal = 0;     //Value für "Schwarz" zum kompletten Ausfüllen bei "Malen"

byte resetVal   = 0;     //Value für "Reset" zum Löschen bei "Malen"
int sperrenVal  = 0;     //Value für "Sperren" zum Sperren bei "Malen"

// ....................................................................................................

byte resetDelayVal        = 1;       //Abbau-Delay-Zeit für Löschen wie z.B. Löschen bei "Malen"; 
byte aufbauDelayVal       = 25;      //Aufbau-Delay-Zeit für Menüs wie z.B. Farbauswahl-Menü bei "Live-Malen"; Farbauswahl-Leiste bei "Malen"
byte musterAufbauDelayVal = 10;      //Aufbau-Delay-Zeit für Muster wie z.B. Spirale bei Ausfüllen in "Malen"
int programmWarteDelay    = 750;     //Wartezeit nach Programm-Wechsel bei z.B. "Malen"

// ....................................................................................................

byte fuellenVal = 35;     //Programm-Durchlauf-Anzahl als Zeit für komplettes Füllen mit einer Farbe bei "Malen"

// ....................................................................................................

byte pixelNr;     //Variable für serielle Ausgaben welche die Pixelnummer speichert

// ....................................................................................................

byte counter      = 0;      //Zähler zum einfachen hoch-zählen innerhalb von "For-Schleifen"

byte counterRot   = 255;    //Rot-Anteil-Zähler für z.B. Regenbogen-Farbwechsel bei "Live-Malen-RGB"; Regenbogen-Farbwechsel bei "Live-Malen-RGB-Komplimentär"
byte counterGruen = 0;      //Gruen-Anteil-Zähler für z.B. Regenbogen-Farbwechsel bei "Live-Malen-RGB"; Regenbogen-Farbwechsel bei "Live-Malen-RGB-Komplimentär"
byte counterBlau  = 255;    //Blau-Anteil-Zähler für z.B. Regenbogen-Farbwechsel bei "Live-Malen-RGB"; Regenbogen-Farbwechsel bei "Live-Malen-RGB-Komplimentär"

// ....................................................................................................

int menueArray1[2][10]     = {{0,19,20,39,40,59,60,79,80,99},{9,10,29,30,49,50,69,70,89,90}};                           //Felder-Array für Start-Menü (Streifen)
int menueArray2[6][4]      = {{16,17,22,23},{42,43,56,57},{76,77,82,83},{12,13,26,27},{46,47,52,53},{72,73,86,87}};     //Felder-Array für Start-Menü (Felder)

// ....................................................................................................

/*int ticTacToeArray[28]         = {38,37,36,35,34,33,32,31,61,62,63,64,65,66,67,68,16,23,43,56,76,83,13,26,46,53,73,86};     //Tic-Tac-Toe-Raster-Array (Grundmuster)
int ticTacToeFelderArray1[4]     = {17,18,21,22};     //Tic-Tac-Toe-Array (Feld1)
int ticTacToeFelderArray2[4]     = {41,42,57,58};     //Tic-Tac-Toe-Array (Feld2)
int ticTacToeFelderArray3[4]     = {77,78,81,82};     //Tic-Tac-Toe-Array (Feld3)
int ticTacToeFelderArray4[4]     = {14,15,24,25};     //Tic-Tac-Toe-Array (Feld4)
int ticTacToeFelderArray5[4]     = {44,45,54,55};     //Tic-Tac-Toe-Array (Feld5)
int ticTacToeFelderArray6[4]     = {74,75,84,85};     //Tic-Tac-Toe-Array (Feld6)
int ticTacToeFelderArray7[4]     = {11,12,27,28};     //Tic-Tac-Toe-Array (Feld7)
int ticTacToeFelderArray8[4]     = {47,48,51,52};     //Tic-Tac-Toe-Array (Feld8)
int ticTacToeFelderArray9[4]     = {71,72,87,88};     //Tic-Tac-Toe-Array (Feld9)
int ticTacToeFarbenArray[1][3];*/

// ....................................................................................................

int farbenAuswahlArray[16]     = {18,38,58,78,23,43,63,83,14,34,54,74,27,47,67,87};     //Farben-Auswahl-Felder-Array für z.B. "Live-Malen"

// ....................................................................................................

int touchSensorArray[100]           = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99};     //Touch-Sensor-Array (standart)

int malenTouchSensorArray[100]      = {4,15,24,35,44,55,64,75,84,95,5,14,25,34,45,54,65,74,85,94,6,13,26,33,46,53,66,73,86,93,0,19,20,39,40,59,60,79,80,99,1,18,21,38,41,58,61,78,81,98,2,17,22,37,42,57,62,77,82,97,3,16,23,36,43,56,63,76,83,96,7,12,27,32,47,52,67,72,87,92,8,11,28,31,48,51,68,71,88,91,9,10,29,30,49,50,69,70,89,90};     //Touch-Sensor-Array für z.B. "Malen" (test)
int menueTouchSensorArray[44]       = {0,19,20,39,40,59,60,79,80,99,9,10,29,30,49,50,69,70,89,90,17,22,16,23,42,57,43,56,77,82,76,83,13,26,12,27,46,53,47,52,73,86,72,87};     //Menü-Touch-Sensor-Array (Streifen/Felder)

// ....................................................................................................

int touchStateArray[100];     //Touch-Status-Array (standart)

int menueTouchStateArray[44]     = {0,19,20,39,40,59,60,79,80,99,9,10,29,30,49,50,69,70,89,90,17,22,16,23,42,57,43,56,77,82,76,83,13,26,12,27,46,53,47,52,73,86,72,87};     //Menü-Touch-Status-Array (Streifen/Felder)

// ....................................................................................................

int ledArray[100];     //Led-Felder-Array (standart)

int malenLedArray[100]       = {4,15,24,35,44,55,64,75,84,95,5,14,25,34,45,54,65,74,85,94,6,13,26,33,46,53,66,73,86,93,0,19,20,39,40,59,60,79,80,99,1,18,21,38,41,58,61,78,81,98,2,17,22,37,42,57,62,77,82,97,3,16,23,36,43,56,63,76,83,96,7,12,27,32,47,52,67,72,87,92,8,11,28,31,48,51,68,71,88,91,9,10,29,30,49,50,69,70,89,90};     //Led-Array für z.B. "Malen" (test)
int musterLedArray1[100]     = {55,44,35,36,43,56,63,64,65,54,45,34,25,24,23,22,37,42,57,62,77,76,75,74,73,66,53,46,33,26,13,14,15,16,17,18,21,38,41,58,61,78,81,82,83,84,85,86,87,72,67,52,47,32,27,12,7,6,5,4,3,2,1,0,19,20,39,40,59,60,79,80,99,98,97,96,95,94,93,92,8,11,28,31,48,51,68,71,88,91,90,89,70,69,50,49,30,29,10,9};     //Muster-Led-Array für z.B. "Malen" (Spirale aus der Mitte ohne unteren beiden Zeilen)

// ....................................................................................................

int malFarbeArray[3];      //Mal-Farben-Array für z.B. "Malen"; "RGB-Live-Malen"; "RGB-Komplimentär-Live-Malen"; "Löschen ("Malen")
int malFarbeArray2[3];     //Mal-Farben-Array für z.B. "Malen"; "RGB-Live-Malen"; "RGB-Komplimentär-Live-Malen"; "Löschen ("RGB-Live-Malen")
int malFarbeArray3[3];     //Mal-Farben-Array für z.B. "Malen"; "RGB-Live-Malen"; "RGB-Komplimentär-Live-Malen"; "Löschen ("RGB-Live-Malen"; "RGB-Komplimentär-Live-Malen")

// ....................................................................................................

int bild[101][3];     //Speichern eines Bildes von z.B. "Malen"

// ....................................................................................................





// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|

void setup() 
{
  // ----------------------------------------------------------------------------------------------------
  // Serielle Schnittstelle

  Serial.begin(9600);  //Start der seriellen Komunikation
  
  // ----------------------------------------------------------------------------------------------------
  // Bibliothekenstart
  
  Wire.begin();        //Start der Bibliothek für das Centipede Shield
  
  CS.initialize();     //Start der Bibliothek für das Centipede Shield
  
  pixels.begin();      //Start der Bibliothek für die RGB-SMD-LEDs von Sparkfun (als Pixel)
  strip.begin();       //Start der Bibliothek für die RGB-SMD-LEDs von Sparkfun (als Streifen)
  pixels.show();       //Alle RGB-SMD-LEDs von Sparkfun auf 0,0,0 setzen (als Pixel)
  strip.show();        //Alle RGB-SMD-LEDs von Sparkfun auf 0,0,0 setzen (als Streifen)
  
  // ----------------------------------------------------------------------------------------------------
  // Pinbelegung
  
  for (counter = 0; counter < 100; counter ++)
  {
    CS.pinMode(touchSensorArray[counter], INPUT);     //Ersten 100 digitalen Pins am Centipede Shield als INPUT deklarieren (Touch-Sensor-Singnale)
  }
  
  pinMode(7, OUTPUT);     //Pin 7 als OUTPUT deklarieren (Daten-Pin der RGB-SMD-LEDs von Sparkfun)
}





// -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|

void loop() 
{
  // ----------------------------------------------------------------------------------------------------
  // Grundform
  
  //start0:
  
  for (counter = 0; counter < 100; counter ++)     //Alle LED-Felder = 0,0,0 setzen
  {
    pixels.setPixelColor(counter, pixels.Color(0,0,0));
  }
  pixels.show();

  for (counter = 0; counter < 10; counter ++)     //Start-Menü anzeigen (Streifen)
  {
    pixels.setPixelColor(menueArray1[0][counter], pixels.Color(255,255,255));
    pixels.setPixelColor(menueArray1[1][counter], pixels.Color(255,255,255));
  }

  for (counter = 0; counter < 4; counter ++)         //Start-Menü anzeigen (Felder)
  {
    pixels.setPixelColor(menueArray2[0][counter], pixels.Color(255,0,0));
    pixels.setPixelColor(menueArray2[1][counter], pixels.Color(0,255,0));
    pixels.setPixelColor(menueArray2[2][counter], pixels.Color(0,0,255));
    pixels.setPixelColor(menueArray2[3][counter], pixels.Color(255,100,0));
    pixels.setPixelColor(menueArray2[4][counter], pixels.Color(255,0,150));
    pixels.setPixelColor(menueArray2[5][counter], pixels.Color(0,255,255));
  }
  pixels.show();
  
  // ----------------------------------------------------------------------------------------------------
  //Zuweisung der Touchsensoren

  start0_1:
  
  for (counter = 0; counter < 44; counter ++)     //Alle 44 Menü-Touch-Sensor-Signale einlesen und speichern
  {
    menueTouchStateArray[counter] = CS.digitalRead(menueTouchSensorArray[counter]);
  }
    
  // ----------------------------------------------------------------------------------------------------
  //Serielle Ausgabe
    
  /*Serial.println();                             //Leere Zeile schreiben
  Serial.println("-----Start-----");              //"-----Start-----" schreiben
  Serial.println();                               //Leere Zeile schreiben
    
  for(counter = 0; counter < 100; counter ++)     //Aktuellen Status anzeigen aller Touch-Sensor-Felder (zum Prüfen)
  {
    pixelNr = counter;                            //PixelNr wird von 0 bis 99 durchnummeriert
    Serial.print("Pixel ");                       //"Pixel " schreiben
    Serial.print(pixelNr);                        //PixelNr einsetzen
    Serial.print(": ");                           //": " schreiben
    Serial.println(touchStateArray[counter]);     //Aktuellen Status des jeweiligen Touch-Sensors einsetzen
  }*/
    
  // ----------------------------------------------------------------------------------------------------
  //Programmcode

  for (counter = 0; counter < 44; counter ++)     //Menü-Auswahl treffen wenn bestimmte Felder = 0 dann gehe zu Programm x
  {
    if (menueTouchStateArray[counter] == 0)
    {
      if (counter < 20)
      {
        goto programm7; 
      }
      else if (counter < 24)
      {
        goto programm1; 
      }
      else if (counter < 28)
      {
        goto programm2; 
      }
      else if (counter < 32)
      {
        goto programm3; 
      }
      else if (counter < 36)
      {
        goto programm4; 
      }
      else if (counter < 40)
      {
        goto programm5; 
      }
      else if (counter < 44)
      {
        goto programm6; 
      }
    }
  }
  goto start0_1;     //sonst wiederhole Vorgang

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  programm1:
  
  // ----------------------------------------------------------------------------------------------------
  // Grundform
  
  malFarbeArray[0] = 0;     //Farbe Rot = 0 setzen
  malFarbeArray[1] = 0;     //Farbe Grün = 0 setzen
  malFarbeArray[2] = 0;     //Farbe Blau = 0 setzen
  
  for (counter = 0; counter < 100; counter ++)     //Alle LED-Felder = 0,0,0 setzen
  {
    pixels.setPixelColor(counter, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
  }
  pixels.show();

       //Farben-Auswahl-Menü anzeigen

  pixels.setPixelColor(18, pixels.Color(255,0,0));
  pixels.setPixelColor(38, pixels.Color(255,127,0));
  pixels.setPixelColor(58, pixels.Color(255,255,0));
  pixels.setPixelColor(78, pixels.Color(127,255,0));

  pixels.setPixelColor(23, pixels.Color(0,255,0));
  pixels.setPixelColor(43, pixels.Color(0,255,127));
  pixels.setPixelColor(63, pixels.Color(0,255,255));
  pixels.setPixelColor(83, pixels.Color(0,127,255));

  pixels.setPixelColor(14, pixels.Color(0,0,255));
  pixels.setPixelColor(34, pixels.Color(127,0,255));
  pixels.setPixelColor(54, pixels.Color(255,0,255));
  pixels.setPixelColor(74, pixels.Color(255,0,127));

  pixels.setPixelColor(27, pixels.Color(255,255,255));
  pixels.setPixelColor(47, pixels.Color(255,155,155));
  pixels.setPixelColor(67, pixels.Color(155,255,155));
  pixels.setPixelColor(87, pixels.Color(155,155,255));

  pixels.show();

  delay(programmWarteDelay);

  start1:

  for (counter = 0; counter < 100; counter ++)     //Alle 100 Touch-Sensor-Signale einlesen und speichern
  {
    touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
  }

       //Farben-Auswahl treffen wenn bestimmte Felder = 0 dann malFarbenArray[x] = x

  if (touchStateArray[18] == 0)
  {
    malFarbeArray[0] = 255;
    malFarbeArray[1] = 0;
    malFarbeArray[2] = 0;
  }
  else if (touchStateArray[38] == 0)
  {
    malFarbeArray[0] = 255;
    malFarbeArray[1] = 127;
    malFarbeArray[2] = 0;
  }
  else if (touchStateArray[58] == 0)
  {
    malFarbeArray[0] = 255;
    malFarbeArray[1] = 255;
    malFarbeArray[2] = 0;
  }
  else if (touchStateArray[78] == 0)
  {
    malFarbeArray[0] = 127;
    malFarbeArray[1] = 255;
    malFarbeArray[2] = 0;
  }
  else if (touchStateArray[23] == 0)
  {
    malFarbeArray[0] = 0;
    malFarbeArray[1] = 255;
    malFarbeArray[2] = 0;
  }
  else if (touchStateArray[43] == 0)
  {
    malFarbeArray[0] = 0;
    malFarbeArray[1] = 255;
    malFarbeArray[2] = 127;
  }
  else if (touchStateArray[63] == 0)
  {
    malFarbeArray[0] = 0;
    malFarbeArray[1] = 255;
    malFarbeArray[2] = 255;
  }
  else if (touchStateArray[83] == 0)
  {
    malFarbeArray[0] = 0;
    malFarbeArray[1] = 127;
    malFarbeArray[2] = 255;
  }
  else if (touchStateArray[14] == 0)
  {
    malFarbeArray[0] = 0;
    malFarbeArray[1] = 0;
    malFarbeArray[2] = 255;
  }
  else if (touchStateArray[34] == 0)
  {
    malFarbeArray[0] = 127;
    malFarbeArray[1] = 0;
    malFarbeArray[2] = 255;
  }
  else if (touchStateArray[54] == 0)
  {
    malFarbeArray[0] = 255;
    malFarbeArray[1] = 0;
    malFarbeArray[2] = 255;
  }
  else if (touchStateArray[74] == 0)
  {
    malFarbeArray[0] = 255;
    malFarbeArray[1] = 0;
    malFarbeArray[2] = 127;
  }
  else if (touchStateArray[27] == 0)
  {
    malFarbeArray[0] = 255;
    malFarbeArray[1] = 255;
    malFarbeArray[2] = 255;
  }
  else if (touchStateArray[47] == 0)
  {
    malFarbeArray[0] = 255;
    malFarbeArray[1] = 155;
    malFarbeArray[2] = 155;
  }
  else if (touchStateArray[67] == 0)
  {
    malFarbeArray[0] = 155;
    malFarbeArray[1] = 255;
    malFarbeArray[2] = 155;
  }
  else if (touchStateArray[87] == 0)
  {
    malFarbeArray[0] = 155;
    malFarbeArray[1] = 155;
    malFarbeArray[2] = 255;
  }
  else
  {
    goto start1;     //sonst wiederhole Vorgang
  }
  
  // ----------------------------------------------------------------------------------------------------
  //Programmcode
  
  while (1)     //Endlosschleife Programm1
  {
    for (counter = 0; counter < 100; counter ++)     //Alle 100 Touch-Sensor-Signale einlesen und speichern
    {
      touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
    } 
    for (counter = 0; counter < 100; counter ++)     //LED-Felder färben wenn jeweiliges = 0 sonst entfärben
    {
      if (touchStateArray[counter] == 0)
      {
        pixels.setPixelColor(counter, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      }
      else
      {
        pixels.setPixelColor(counter, pixels.Color(0,0,0));
      }
    }
    pixels.show();
  }
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  programm2:
  
  // ----------------------------------------------------------------------------------------------------
  // Grundform
  
  malFarbeArray[0] = 0;     //Farbe Rot = 0 setzen
  malFarbeArray[1] = 0;     //Farbe Grün = 0 setzen
  malFarbeArray[2] = 0;     //Farbe Blau = 0 setzen
  
  for (counter = 0; counter < 100; counter ++)     //Alle LED-Felder = 0,0,0 setzen
  {
    pixels.setPixelColor(counter, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
  }
  pixels.show();
  
  // ----------------------------------------------------------------------------------------------------
  //Programmcode
  
  while (1)     //Endlosschleife Programm2
  {

  }
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  programm3:
  
  // ----------------------------------------------------------------------------------------------------
  // Grundform
  
  malFarbeArray[0] = 0;     //Farbe Rot = 0 setzen
  malFarbeArray[1] = 0;     //Farbe Grün = 0 setzen
  malFarbeArray[2] = 0;     //Farbe Blau = 0 setzen
  
  for (counter = 0; counter < 100; counter ++)     //Alle LED-Felder = 0,0,0 setzen
  {
    pixels.setPixelColor(counter, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
  }
  pixels.show();
  
  // ----------------------------------------------------------------------------------------------------
  //Programmcode
  
  while (1)     //Endlosschleife Programm3
  {
    
  }
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
  
  programm4:
  
  // ----------------------------------------------------------------------------------------------------
  // Grundform
  
  malFarbeArray[0] = 0;     //Farbe Rot = 0 setzen
  malFarbeArray[1] = 0;     //Farbe Grün = 0 setzen
  malFarbeArray[2] = 0;     //Farbe Blau = 0 setzen
  
  for (counter = 0; counter < 100; counter ++)     //Alle LED-Felder = 0,0,0 setzen
  {
    pixels.setPixelColor(counter, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
  }
  pixels.show();
  
  // ----------------------------------------------------------------------------------------------------
  //Programmcode
  
  while (1)     //Endlosschleife Programm4 (1/2)
  {
    start4:

         //Farb-Auswahl-Menü anzeigen inkl. aktuell gewählter Farbe und schwarzem Balken

    for (counter = 80; counter < 90; counter ++)
    {
      pixels.setPixelColor(malenLedArray[counter], pixels.Color(0,0,0));
    }
    
    pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
    pixels.setPixelColor(10, pixels.Color(0,0,0));
    pixels.setPixelColor(29, pixels.Color(255,0,0));
    pixels.setPixelColor(30, pixels.Color(255,255,0));
    pixels.setPixelColor(49, pixels.Color(0,255,0));
    pixels.setPixelColor(50, pixels.Color(0,255,255));
    pixels.setPixelColor(69, pixels.Color(0,0,255));
    pixels.setPixelColor(70, pixels.Color(255,0,255));
    pixels.setPixelColor(89, pixels.Color(255,255,255));
    pixels.setPixelColor(90, pixels.Color(0,0,0));  

    pixels.show();

    delay(programmWarteDelay);
    
    start4_1:

    for (counter = 0; counter < 100; counter ++)     //Alle 100 Touch-Sensor-Signale einlesen und speichern (in "Malen" Reihenfolge -> Array)
    {
      touchStateArray[counter] = CS.digitalRead(malenTouchSensorArray[counter]);
    }

    for (counter = 0; counter < 80; counter ++)     //Alle 80 Touch-Status-Signale der Mal-Felder der ausgewählten Farbe gleichsetzen + Gemaltes Bild speichern bzw. updaten in bild[x1][x2,1/x2,2/x2,3] (x1 = PixelNr. ; x2,1/x2,2/x2,3 = Farbe des jeweiligen Pixels x,x,x)
    {
      if (touchStateArray[counter] == 0)
      {
        pixels.setPixelColor(malenLedArray[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
        pixels.show();
        bild[counter][0] = malFarbeArray[0];
        bild[counter][1] = malFarbeArray[1];
        bild[counter][2] = malFarbeArray[2];
      }
    }
      
    if (touchStateArray[92] == 0)     //Wenn Rot ausgewählt wird -> malFarbeArray = 255,0,0 setzen und aktuelle Farbe aktualisieren + rotVal um 1 erhöhen für komplettes Füllen bei rotVal = fuellenVal + wenn komplett ausgefüllt -> rotVal = 0
    {
      rotVal++;

      malFarbeArray[0] = 255;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 0;
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();
      
      if (rotVal == fuellenVal)
      {
        for (counter = 0; counter < 80; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        rotVal = 0;
      }
    }
      
    else if (touchStateArray[93] == 0)     //Wenn Gelb ausgewählt wird -> malFarbeArray = 255,255,0 setzen und aktuelle Farbe aktualisieren + gelbVal um 1 erhöhen für komplettes Füllen bei gelbVal = fuellenVal + wenn komplett ausgefüllt -> gelbVal = 0
    {
      gelbVal++;
      
      malFarbeArray[0] = 255;
      malFarbeArray[1] = 255;
      malFarbeArray[2] = 0;
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (gelbVal == fuellenVal)
      {
        for (counter = 0; counter < 80; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        gelbVal = 0;
      }
    }
      
    else if (touchStateArray[94] == 0)     //Wenn Grün ausgewählt wird -> malFarbeArray = 0,255,0 setzen und aktuelle Farbe aktualisieren + gruenVal um 1 erhöhen für komplettes Füllen bei gruenVal = fuellenVal + wenn komplett ausgefüllt -> gruenVal = 0
    {
      gruenVal++;
      
      malFarbeArray[0] = 0;
      malFarbeArray[1] = 255;
      malFarbeArray[2] = 0;
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (gruenVal == fuellenVal)
      {
        for (counter = 0; counter < 80; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        gruenVal = 0;
      }
    }
      
    else if (touchStateArray[95] == 0)     //Wenn Türkis ausgewählt wird -> malFarbeArray = 0,255,255 setzen und aktuelle Farbe aktualisieren + tuerkisVal um 1 erhöhen für komplettes Füllen bei tuerkisVal = fuellenVal + wenn komplett ausgefüllt -> tuerkisVal = 0
    {
      tuerkisVal++;
      
      malFarbeArray[0] = 0;
      malFarbeArray[1] = 255;
      malFarbeArray[2] = 255;
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (tuerkisVal == fuellenVal)
      {
        for (counter = 0; counter < 80; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        tuerkisVal = 0;
      }
    }
      
    else if (touchStateArray[96] == 0)     //Wenn Blau ausgewählt wird -> malFarbeArray = 0,0,255 setzen und aktuelle Farbe aktualisieren + blauVal um 1 erhöhen für komplettes Füllen bei blauVal = fuellenVal + wenn komplett ausgefüllt -> blauVal = 0
    {
      blauVal++;
      
      malFarbeArray[0] = 0;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 255;
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (blauVal == fuellenVal)
      {
        for (counter = 0; counter < 80; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        blauVal = 0;
      }
    }
      
    else if (touchStateArray[97] == 0)     //Wenn Lila ausgewählt wird -> malFarbeArray = 255,0,255 setzen und aktuelle Farbe aktualisieren + lilaVal um 1 erhöhen für komplettes Füllen bei lilaVal = fuellenVal + wenn komplett ausgefüllt -> lilaVal = 0
    {
      lilaVal++;
      
      malFarbeArray[0] = 255;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 255;
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (lilaVal == fuellenVal)
      {
        for (counter = 0; counter < 80; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        lilaVal = 0;
      }
    }
      
    else if (touchStateArray[98] == 0)     //Wenn Weiß ausgewählt wird -> malFarbeArray = 255,255,255 setzen und aktuelle Farbe aktualisieren + weissVal um 1 erhöhen für komplettes Füllen bei weissVal = fuellenVal + wenn komplett ausgefüllt -> weissVal = 0
    {
      weissVal++;
      
      malFarbeArray[0] = 255;
      malFarbeArray[1] = 255;
      malFarbeArray[2] = 255;
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (weissVal == fuellenVal)
      {
        for (counter = 0; counter < 80; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        weissVal = 0;
      }
    }
      
    else if (touchStateArray[99] == 0)     //Wenn Schwarz ausgewählt wird -> malFarbeArray = 0,0,0 setzen und aktuelle Farbe aktualisieren + schwarzVal um 1 erhöhen für komplettes Füllen bei schwarzVal = fuellenVal + wenn komplett ausgefüllt -> schwarzVal = 0
    {
      schwarzVal++;
      
      malFarbeArray[0] = 0;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 0;
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (schwarzVal == fuellenVal)
      {
        for (counter = 0; counter < 80; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        schwarzVal = 0;
      }
    }

    else if (touchStateArray[90] == 0)     //Wenn Sperren ausgewählt wird -> gehe zu sperren1
    {
      goto sperren1;
    }
    
    else if (touchStateArray[91] == 0)     //Wenn Reset ausgewählt wird -> gehe zu reset1
    {
      goto reset1;
    }
      
    else     //sonst setze Value aller Farben xVal = 0 + wiederhole ganzen Vorgang
    {
      rotVal = 0;
      gelbVal = 0;
      gruenVal = 0;
      tuerkisVal = 0;
      blauVal = 0;
      lilaVal = 0;
      weissVal = 0;
      schwarzVal = 0;
      resetVal = 0;
      sperrenVal = 0;
      
      goto start4_1;
    }
    goto start4_1;
  }




  //     (1/2)
  // .................................................................................................... 
  //     (2/2)




  while (1)     //Endlosschleife Programm4 (2/2)
  {
    start4_5:

         //Farb-Auswahl-Menü anzeigen inkl. aktuell gewählter Farbe und schwarzem Balken

    for (counter = 0; counter < 30; counter ++)
    {
      pixels.setPixelColor(malenLedArray[counter], pixels.Color(0,0,0));
    }
    
    pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
    pixels.setPixelColor(14, pixels.Color(0,0,0));
    pixels.setPixelColor(25, pixels.Color(255,0,0));
    pixels.setPixelColor(34, pixels.Color(255,255,0));
    pixels.setPixelColor(45, pixels.Color(0,255,0));
    pixels.setPixelColor(54, pixels.Color(0,255,255));
    pixels.setPixelColor(65, pixels.Color(0,0,255));
    pixels.setPixelColor(74, pixels.Color(255,0,255));
    pixels.setPixelColor(85, pixels.Color(255,255,255));
    pixels.setPixelColor(94, pixels.Color(0,0,0));  

    pixels.show();

    delay(programmWarteDelay);
    
    start4_6:

    for (counter = 0; counter < 100; counter ++)     //Alle 100 Touch-Sensor-Signale einlesen und speichern (in "Malen" Reihenfolge -> Array)
    {
      touchStateArray[counter] = CS.digitalRead(malenTouchSensorArray[counter]);
    }

    for (counter = 30; counter < 100; counter ++)     //Alle 70 Touch-Status-Signale der Mal-Felder der ausgewählten Farbe gleichsetzen + Gemaltes Bild speichern bzw. updaten in bild[x1][x2,1/x2,2/x2,3] (x1 = PixelNr. ; x2,1/x2,2/x2,3 = Farbe des jeweiligen Pixels x,x,x)
    {
      if (touchStateArray[counter] == 0)
      {
        pixels.setPixelColor(malenLedArray[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
        pixels.show();
        bild[counter][0] = malFarbeArray[0];
        bild[counter][1] = malFarbeArray[1];
        bild[counter][2] = malFarbeArray[2];
      }
    }
      
    if (touchStateArray[12] == 0)     //Wenn Rot ausgewählt wird -> malFarbeArray = 255,0,0 setzen und aktuelle Farbe aktualisieren + rotVal um 1 erhöhen für komplettes Füllen bei rotVal = fuellenVal + wenn komplett ausgefüllt -> rotVal = 0
    {
      rotVal++;

      malFarbeArray[0] = 255;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 0;
      
      pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();
      
      if (rotVal == fuellenVal)
      {
        for (counter = 80; counter < 100; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        rotVal = 0;
      }
    }
      
    else if (touchStateArray[13] == 0)     //Wenn Gelb ausgewählt wird -> malFarbeArray = 255,255,0 setzen und aktuelle Farbe aktualisieren + gelbVal um 1 erhöhen für komplettes Füllen bei gelbVal = fuellenVal + wenn komplett ausgefüllt -> gelbVal = 0
    {
      gelbVal++;
      
      malFarbeArray[0] = 255;
      malFarbeArray[1] = 255;
      malFarbeArray[2] = 0;
      
      pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (gelbVal == fuellenVal)
      {
        for (counter = 80; counter < 100; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        gelbVal = 0;
      }
    }
      
    else if (touchStateArray[14] == 0)     //Wenn Grün ausgewählt wird -> malFarbeArray = 0,255,0 setzen und aktuelle Farbe aktualisieren + gruenVal um 1 erhöhen für komplettes Füllen bei gruenVal = fuellenVal + wenn komplett ausgefüllt -> gruenVal = 0
    {
      gruenVal++;
      
      malFarbeArray[0] = 0;
      malFarbeArray[1] = 255;
      malFarbeArray[2] = 0;
      
      pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (gruenVal == fuellenVal)
      {
        for (counter = 80; counter < 100; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        gruenVal = 0;
      }
    }
      
    else if (touchStateArray[15] == 0)     //Wenn Türkis ausgewählt wird -> malFarbeArray = 0,255,255 setzen und aktuelle Farbe aktualisieren + tuerkisVal um 1 erhöhen für komplettes Füllen bei tuerkisVal = fuellenVal + wenn komplett ausgefüllt -> tuerkisVal = 0
    {
      tuerkisVal++;
      
      malFarbeArray[0] = 0;
      malFarbeArray[1] = 255;
      malFarbeArray[2] = 255;
      
      pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (tuerkisVal == fuellenVal)
      {
        for (counter = 80; counter < 100; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        tuerkisVal = 0;
      }
    }
      
    else if (touchStateArray[16] == 0)     //Wenn Blau ausgewählt wird -> malFarbeArray = 0,0,255 setzen und aktuelle Farbe aktualisieren + blauVal um 1 erhöhen für komplettes Füllen bei blauVal = fuellenVal + wenn komplett ausgefüllt -> blauVal = 0
    {
      blauVal++;
      
      malFarbeArray[0] = 0;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 255;
      
      pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (blauVal == fuellenVal)
      {
        for (counter = 80; counter < 100; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        blauVal = 0;
      }
    }
      
    else if (touchStateArray[17] == 0)     //Wenn Lila ausgewählt wird -> malFarbeArray = 255,0,255 setzen und aktuelle Farbe aktualisieren + lilaVal um 1 erhöhen für komplettes Füllen bei lilaVal = fuellenVal + wenn komplett ausgefüllt -> lilaVal = 0
    {
      lilaVal++;
      
      malFarbeArray[0] = 255;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 255;
      
      pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (lilaVal == fuellenVal)
      {
        for (counter = 80; counter < 100; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        lilaVal = 0;
      }
    }
      
    else if (touchStateArray[18] == 0)     //Wenn Weiß ausgewählt wird -> malFarbeArray = 255,255,255 setzen und aktuelle Farbe aktualisieren + weissVal um 1 erhöhen für komplettes Füllen bei weissVal = fuellenVal + wenn komplett ausgefüllt -> weissVal = 0
    {
      weissVal++;
      
      malFarbeArray[0] = 255;
      malFarbeArray[1] = 255;
      malFarbeArray[2] = 255;
      
      pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (weissVal == fuellenVal)
      {
        for (counter = 80; counter < 100; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        weissVal = 0;
      }
    }
      
    else if (touchStateArray[19] == 0)     //Wenn Schwarz ausgewählt wird -> malFarbeArray = 0,0,0 setzen und aktuelle Farbe aktualisieren + schwarzVal um 1 erhöhen für komplettes Füllen bei schwarzVal = fuellenVal + wenn komplett ausgefüllt -> schwarzVal = 0
    {
      schwarzVal++;
      
      malFarbeArray[0] = 0;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 0;
      
      pixels.setPixelColor(5, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
      pixels.show();

      if (schwarzVal == fuellenVal)
      {
        for (counter = 80; counter < 100; counter ++)
        {
          pixels.setPixelColor(musterLedArray1[counter], pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
          pixels.show();
          bild[counter][0] = malFarbeArray[0];
          bild[counter][1] = malFarbeArray[1];
          bild[counter][2] = malFarbeArray[2];
          delay(musterAufbauDelayVal);
        }
        schwarzVal = 0;
      }
    }

    else if (touchStateArray[10] == 0)     //Wenn Sperren ausgewählt wird -> gehe zu sperren1
    {
      goto sperren2;
    }
    
    else if (touchStateArray[11] == 0)     //Wenn Reset ausgewählt wird -> gehe zu reset1
    {
      goto reset2;
    }
      
    else     //sonst setze Value aller Farben xVal = 0 + wiederhole ganzen Vorgang
    {
      rotVal = 0;
      gelbVal = 0;
      gruenVal = 0;
      tuerkisVal = 0;
      blauVal = 0;
      lilaVal = 0;
      weissVal = 0;
      schwarzVal = 0;
      resetVal = 0;
      sperrenVal = 0;
      
      goto start4_6;
    }
    goto start4_6;
  }





  // .................................................................................................... 
    
    reset1:

    resetVal++;     //resetVall um 1 erhöhen

    if (resetVal == 50)     //Wenn restVal = 50 dann lösche Bild und setze aktuelle Farbe auf 0,0,0
    {
      for (counter = 0; counter < 80; counter ++)
      {
        pixels.setPixelColor(malenLedArray[counter], pixels.Color(0,0,0));
      }
      pixels.show();
      
      resetVal = 0;             //Setze resteVal = 0

      malFarbeArray[0] = 0;     //Farbe Rot = 0 setzen
      malFarbeArray[1] = 0;     //Farbe Grün = 0 setzen
      malFarbeArray[2] = 0;     //Farbe Blau = 0 setzen
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));     //Anzeige aktueller Farbe auf 0,0,0 setzen
      pixels.show();

      for (counter = 0; counter < 100; counter ++)
      {
        bild[counter][0] = malFarbeArray[0];
        bild[counter][1] = malFarbeArray[1];
        bild[counter][2] = malFarbeArray[2];
      }

      goto start4;     //Wiederhole Mal-Vorgang ab start4
    }

    else
    {      
      goto start4_1;     //Wiederhole Mal-Vorgang ab start4_1
    }
    
  // .................................................................................................... 

    reset2:

    resetVal++;     //resetVall um 1 erhöhen

    if (resetVal == 50)     //Wenn restVal = 50 dann lösche Bild und setze aktuelle Farbe auf 0,0,0
    {
      for (counter = 0; counter < 80; counter ++)
      {
        pixels.setPixelColor(malenLedArray[counter], pixels.Color(0,0,0));
      }
      pixels.show();

      resetVal = 0;             //Setze resteVal = 0

      malFarbeArray[0] = 0;     //Farbe Rot = 0 setzen
      malFarbeArray[1] = 0;     //Farbe Grün = 0 setzen
      malFarbeArray[2] = 0;     //Farbe Blau = 0 setzen
      
      pixels.setPixelColor(9, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));     //Anzeige aktueller Farbe auf 0,0,0 setzen
      pixels.show();

      for (counter = 0; counter < 100; counter ++)
      {
        bild[counter][0] = malFarbeArray[0];
        bild[counter][1] = malFarbeArray[1];
        bild[counter][2] = malFarbeArray[2];
      }

      goto start4;     //Wiederhole Mal-Vorgang ab start4
    }

    else
    {      
      goto start4_6;     //Wiederhole Mal-Vorgang ab start4_1
    }
    
  // ....................................................................................................
    
    sperren1:
    
    sperrenVal++;     //sperrenVal um 1 erhöhen

    if (sperrenVal == 10)     //Wenn sperrenVal = 40 dann blende Farben-Auswahl aus und färbe diese Reihe in aktueller Farbe + wenn sperrenVal erneut 4000 dann blende Farben-Auswahl wieder ein und gehe zurück in Mal-Programm (gehe zu start 1_5)
    {
      for (counter = 80; counter < 100; counter ++)
      {
        pixels.setPixelColor(malenLedArray[counter], pixels.Color(bild[counter][0],bild[counter][1],bild[counter][2]));
      }
      
      sperrenVal = 0;

      malFarbeArray[0] = 0;
      malFarbeArray[1] = 0;
      malFarbeArray[2] = 0;

      goto start4_5;
    }

    else
    {
    goto start4_1;     //Wiederhole Mal-Vorgang ab start4_1
    }

    // ....................................................................................................
    
    sperren2:
    
    sperrenVal++;     //sperrenVal um 1 erhöhen

    if (sperrenVal == 10)     //Wenn sperrenVal = 40 dann blende Farben-Auswahl aus und färbe diese Reihe in aktueller Farbe + wenn sperrenVal erneut 4000 dann blende Farben-Auswahl wieder ein und gehe zurück in Mal-Programm (gehe zu start 1_5)
    {
      for (counter = 0; counter < 100; counter ++)
      {
        pixels.setPixelColor(malenLedArray[counter], pixels.Color(bild[counter][0],bild[counter][1],bild[counter][2]));
      }
      pixels.show();

      sperrenVal = 0;

      while (1)     //Endlosschleife im gesperrten Zusatand
      {
        touchStateArray[90] = CS.digitalRead(malenTouchSensorArray[90]);

        if (touchStateArray[90] == 0)
        {
          sperrenVal++;

          if (sperrenVal == 4000)
          {
            for (counter = 0; counter < 80; counter ++)
            {
              pixels.setPixelColor(malenLedArray[counter], pixels.Color(bild[counter][0],bild[counter][1],bild[counter][2]));
            }
            
            sperrenVal = 0;

            malFarbeArray[0] = 0;
            malFarbeArray[1] = 0;
            malFarbeArray[2] = 0;
              
            goto start4;     //Wiederhole Mal-Vorgang ab start4
          }
        }
        else
        {
          sperrenVal = 0;
        }
      }
    }

    else
    {
    goto start4_6;     //Wiederhole Mal-Vorgang ab start4_1
    }
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  programm5:
  
  // ----------------------------------------------------------------------------------------------------
  // Grundform
  
  malFarbeArray[0] = 0;
  malFarbeArray[1] = 0;
  malFarbeArray[2] = 0;
  
  malFarbeArray2[0] = 255;
  malFarbeArray2[1] = 0;
  malFarbeArray2[2] = 255;
  
  for (counter = 0; counter < 100; counter ++)
  {
    pixels.setPixelColor(counter, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
  }
  pixels.show();
  
  // ----------------------------------------------------------------------------------------------------
  //Programmcode
  
  while (1)
  {
    
    for (counterBlau = 255; counterBlau > 0; counterBlau --)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(0,0,0));
        }
      }
      pixels.show();
    }
    
    for (counterGruen = 0; counterGruen < 255; counterGruen ++)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(0,0,0));
        }
      }
      pixels.show();
    }
      
    for (counterRot = 255; counterRot > 0; counterRot --)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(0,0,0));
        }
      }
      pixels.show();
    }
      
    for (counterBlau = 0; counterBlau < 255; counterBlau ++)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(0,0,0));
        }
      }
      pixels.show();
    }
      
    for (counterGruen = 255; counterGruen > 0; counterGruen --)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(0,0,0));
        }
      }
      pixels.show();
    }
      
    for (counterRot = 0; counterRot < 255; counterRot ++)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(0,0,0));
        }
      }
      pixels.show();
    } 
    
  }
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  programm6:
  
  // ----------------------------------------------------------------------------------------------------
  // Grundform
  
  malFarbeArray[0] = 0;
  malFarbeArray[1] = 0;
  malFarbeArray[2] = 0;
  
  malFarbeArray2[0] = 255;
  malFarbeArray2[1] = 0;
  malFarbeArray2[2] = 255;
  
  malFarbeArray3[0] = 0;
  malFarbeArray3[1] = 255;
  malFarbeArray3[2] = 0;
  
  for (counter = 0; counter < 100; counter ++)
  {
    pixels.setPixelColor(counter, pixels.Color(malFarbeArray[0],malFarbeArray[1],malFarbeArray[2]));
  }
  pixels.show();
  
  // ----------------------------------------------------------------------------------------------------
  //Programmcode
  
  while (1)
  {
    
    for (counterBlau = 255; counterBlau > 0; counterBlau --)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      malFarbeArray3[0] = counterGruen;
      malFarbeArray3[1] = counterBlau;
      malFarbeArray3[2] = counterRot;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray3[0],malFarbeArray3[1],malFarbeArray3[2]));
        }
      }
      pixels.show();
    }
    
    for (counterGruen = 0; counterGruen < 255; counterGruen ++)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      malFarbeArray3[0] = counterGruen;
      malFarbeArray3[1] = counterBlau;
      malFarbeArray3[2] = counterRot;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray3[0],malFarbeArray3[1],malFarbeArray3[2]));
        }
      }
      pixels.show();
    }
      
    for (counterRot = 255; counterRot > 0; counterRot --)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      malFarbeArray3[0] = counterGruen;
      malFarbeArray3[1] = counterBlau;
      malFarbeArray3[2] = counterRot;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray3[0],malFarbeArray3[1],malFarbeArray3[2]));
        }
      }
      pixels.show();
    }
      
    for (counterBlau = 0; counterBlau < 255; counterBlau ++)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      malFarbeArray3[0] = counterGruen;
      malFarbeArray3[1] = counterBlau;
      malFarbeArray3[2] = counterRot;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray3[0],malFarbeArray3[1],malFarbeArray3[2]));
        }
      }
      pixels.show();
    }
      
    for (counterGruen = 255; counterGruen > 0; counterGruen --)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      malFarbeArray3[0] = counterGruen;
      malFarbeArray3[1] = counterBlau;
      malFarbeArray3[2] = counterRot;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray3[0],malFarbeArray3[1],malFarbeArray3[2]));
        }
      }
      pixels.show();
    }
      
    for (counterRot = 0; counterRot < 255; counterRot ++)
    {
      
      malFarbeArray2[0] = counterRot;
      malFarbeArray2[1] = counterGruen;
      malFarbeArray2[2] = counterBlau;
      
      malFarbeArray3[0] = counterGruen;
      malFarbeArray3[1] = counterBlau;
      malFarbeArray3[2] = counterRot;
      
      for (counter = 0; counter < 100; counter ++)
      {
        touchStateArray[counter] = CS.digitalRead(touchSensorArray[counter]);
      }
        
      for (counter = 0; counter < 100; counter ++)
      {
        if (touchStateArray[counter] == 0)
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray2[0],malFarbeArray2[1],malFarbeArray2[2]));
        }
        else
        {
          pixels.setPixelColor(counter, pixels.Color(malFarbeArray3[0],malFarbeArray3[1],malFarbeArray3[2]));
        }
      }
      pixels.show();
    } 
  }
    
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
  
  programm7:

  rainbowCycle(50); // Durchlaufzeit
  
}
  
void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;
  
  while (1)
  {
    for (j=0; j<256*5; j++) 
    { 
      for (i=0; i< strip.numPixels(); i++) 
      {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
      delay(wait);
    }  
  }
}


  uint32_t Wheel(byte WheelPos) 
  {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) 
    {
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else if (WheelPos < 170) 
    {
      WheelPos -= 85;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    } 
    else 
    {
      WheelPos -= 170;
      return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }   
  }
