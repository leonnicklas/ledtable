// ------------------------------------------------------------------------------------------------------
// Bibliotheken

#include <Adafruit_NeoPixel.h> // Bibliothek für die RGB-SMD-LEDs von Sparkfun
#include <Wire.h>              // Bibliothek für das Centipede Shield von Macetech
#include <Centipede.h>         // Bibliothek für das Centipede Shield von Macetech
Centipede CS;                  // Bibliothekenzusatz für das Centipede Shield

// ------------------------------------------------------------------------------------------------------
// Grundeinstellung Neopixel

#define PIXEL_PIN 7     // Digitaler Arduino-Pin für Datenübertragung an RGB-SMD-LEDs von Sparkfun
#define PIXEL_COUNT 100 // Anzahl von angeschlossenen RGB-SMD-LEDs von Sparkfun

// Parameter 1 = Number of pixels in strip
// Parameter 2 = Pin number (most are valid)
// Parameter 3 = Pixel type flags, add together as needed:
// NEO_RGB       Pixels are wired for RGB bitstream
// NEO_GRB       Pixels are wired for GRB bitstream, correct for neopixel
// NEO_KHZ400    400 KHz bitstream (e.g. FLORA pixels)
// NEO_KHZ800    800 KHz bitstream (e.g. High Density LED strip), correct for neopixel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int menuFields[9][4] = {{17, 18, 21, 22}, {41, 42, 57, 58}, {77, 78, 81, 82}, {14, 15, 24, 25}, {44, 45, 54, 55}, {74, 75, 84, 85}, {11, 12, 27, 28}, {47, 48, 51, 52}, {71, 72, 87, 88}};

void setup()
{
  // ----------------------------------------------------------------------------------------------------
  // Setup Serielle Schnittstelle

  // Serial.begin(9600);

  // ----------------------------------------------------------------------------------------------------
  // Setup Bibliotheken

  Wire.begin();
  CS.initialize();

  strip.begin();
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();

  // ----------------------------------------------------------------------------------------------------
  // Setup Pinbelegungen

  // Ersten 100 digitalen Pins am Centipede Shield als INPUT deklarieren (Touch-Sensor-Signale)
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    CS.pinMode(i, INPUT);
  }

  // Pin 7 als OUTPUT deklarieren (Daten-Pin der RGB-SMD-LEDs von Sparkfun)
  pinMode(7, OUTPUT);
}

void loop()
{
  show_menu();

  for (byte i = 0; i < sizeof(menuFields) / sizeof(menuFields[0]); i++)
  {
    for (byte j = 0; j < sizeof(menuFields[i]) / sizeof(menuFields[i][0]); j++)
    {
      if (CS.digitalRead(menuFields[i][j]) == 0)
      {
        start(i);
      }
    }
  }
}

void show_menu()
{
  // Alle 100 LED-Felder = 0, 0, 0 (schwarz/aus) setzen
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  // 9 Programm-Menü Felder anzeigen
  for (uint16_t i = 0; i < sizeof(menuFields[0]) / sizeof(int); i++)
  {
    strip.setPixelColor(menuFields[0][i], strip.Color(0, 0, 255));   // Blau
    strip.setPixelColor(menuFields[1][i], strip.Color(0, 155, 255)); // Blaugrün
    strip.setPixelColor(menuFields[2][i], strip.Color(0, 255, 155)); // Grünblau
    strip.setPixelColor(menuFields[3][i], strip.Color(255, 0, 155)); // Pink
    strip.setPixelColor(menuFields[4][i], strip.Color(155, 0, 255)); // Lila
    strip.setPixelColor(menuFields[5][i], strip.Color(0, 255, 0));   // Grün
    strip.setPixelColor(menuFields[6][i], strip.Color(255, 0, 0));   // Rot
    strip.setPixelColor(menuFields[7][i], strip.Color(255, 155, 0)); // Orange
    strip.setPixelColor(menuFields[8][i], strip.Color(155, 255, 0)); // Gelb
  }

  strip.show();
}

void start(int i)
{
  switch (i)
  {
  case 0:
    tictactoe();
    break;
  case 1:
    tictactoe();
    break;
  case 2:
    tictactoe();
    break;
  case 3:
    paint();
    break;
  case 4:
    paint();
    break;
  case 5:
    paint();
    break;
  case 6:
    react();
    break;
  case 7:
    react();
    break;
  case 8:
    react();
    break;
  }
}

void react()
{
  // ----------------------------------------------------------------------------------------------------
  // Variablen

  byte slowdown_factor = 5; // Reduziert die Geschwindigkeit des Regenbogen-Verlaufs im Farbmenü (Mitte)

  int wheelPos = 0;
  int color_selected = -1; // Nichts ausgewählt zum Anfang (-1)

  int colors[16][3] = {{255, 0, 0}, {255, 64, 0}, {255, 127, 0}, {0, 255, 0}, {127, 255, 0}, {255, 255, 0}, {255, 0, 127}, {255, 0, 255}, {127, 0, 255}, {0, 255, 127}, {0, 200, 255}, {0, 0, 255}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}};
  int colorGrid[16] = {18, 22, 36, 81, 77, 63, 88, 72, 66, 11, 27, 33, 44, 45, 54, 55};

  // ----------------------------------------------------------------------------------------------------
  // Setup

  // Alle 100 LED-Felder = 0, 0, 0 (schwarz/aus) setzen
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  // Farben-Auswahl-Menü anzeigen
  for (byte i = 0; i < sizeof(colorGrid) / sizeof(int); i++)
  {
    strip.setPixelColor(colorGrid[i], strip.Color(colors[i][0], colors[i][1], colors[i][2]));
  }

  strip.show();

  waitUntilTouchReleased();

  // ----------------------------------------------------------------------------------------------------
  // Menu

  while (1)
  {
    strip.setPixelColor(44, Wheel((wheelPos / slowdown_factor) % 255));
    strip.setPixelColor(45, Wheel((wheelPos / slowdown_factor) % 255));
    strip.setPixelColor(54, Wheel((wheelPos / slowdown_factor) % 255));
    strip.setPixelColor(55, Wheel((wheelPos / slowdown_factor) % 255));

    strip.show();

    for (byte i = 0; i < sizeof(colorGrid) / sizeof(int); i++)
    {
      if (CS.digitalRead(colorGrid[i]) == 0)
      {
        color_selected = i;
      }
    }

    if (color_selected < 0)
    {
      wheelPos++;

      if (wheelPos > (255 * slowdown_factor))
      {
        wheelPos = 0;
      }
    }
    else if (color_selected < 12)
    {
      react_color(strip.Color(colors[color_selected][0], colors[color_selected][1], colors[color_selected][2]));
    }
    else
    {
      react_rainbow(wheelPos / slowdown_factor);
    }
  }
}

void react_color(uint32_t color)
{
  while (1)
  {
    // Alle 100 Touch-Sensor-Signale einlesen und entsprechende Felder einfärben/ entfärben
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
      if (CS.digitalRead(i) == 0)
      {
        strip.setPixelColor(i, color);
      }
      else
      {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }

    strip.show();
  }
}

void react_rainbow(byte i_start)
{
  byte i = i_start;

  while (1)
  {
    // Regenbogen-Farbverlauf durch alle Farben (0-255)
    while (i < 256)
    {
      i++;

      // Alle 100 Touch-Sensor-Signale einlesen und entsprechende Felder einfärben/ entfärben
      for (byte j = 0; j < 100; j++)
      {
        if (CS.digitalRead(j) == 0)
        {
          strip.setPixelColor(j, Wheel(i % 255));
        }
        else
        {
          strip.setPixelColor(j, strip.Color(0, 0, 0));
        }
      }

      strip.show();
    }

    i = 0;
  }
}

void tictactoe()
{
  // ----------------------------------------------------------------------------------------------------
  // Variablen

  byte player = 0;
  byte winningFlashRepeats = 5;
  byte winningFlashDelay = 50;

  int newRoundDelay = 500;
  int victories[2] = {0, 0};

  uint32_t gridColor = strip.Color(0, 0, 200);
  uint32_t playerColors[2] = {strip.Color(255, 0, 0), strip.Color(0, 255, 0)};
  uint32_t playerScoreColors[2] = {strip.Color(22, 0, 0), strip.Color(0, 22, 0)};
  uint32_t playerIndicatorColors[2] = {strip.Color(5, 0, 0), strip.Color(0, 5, 0)};

  int tictactoeGrid[28] = {38, 37, 36, 35, 34, 33, 32, 31, 61, 62, 63, 64, 65, 66, 67, 68, 16, 23, 43, 56, 76, 83, 13, 26, 46, 53, 73, 86};
  int tictactoeFrame[36] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 19, 20, 29, 30, 39, 40, 49, 50, 59, 60, 69, 70, 79, 80, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};
  int tictactoeFields[9][4] = {{17, 18, 21, 22}, {41, 42, 57, 58}, {77, 78, 81, 82}, {14, 15, 24, 25}, {44, 45, 54, 55}, {74, 75, 84, 85}, {11, 12, 27, 28}, {47, 48, 51, 52}, {71, 72, 87, 88}};
  int tictactoeFieldStates[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
  int tictactoePlayerIndicators[2][10] = {{19, 20, 39, 40, 59, 60, 79, 80}, {10, 29, 30, 49, 50, 69, 70, 89}};
  int tictactoeScoreDisplays[2][10] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, {90, 91, 92, 93, 94, 95, 96, 97, 98, 99}};
  int tictactoeWinningRows[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

  // ----------------------------------------------------------------------------------------------------
  // Setup

  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  for (byte i = 0; i < 28; i++)
  {
    strip.setPixelColor(tictactoeGrid[i], gridColor);
  }

  strip.show();
  waitUntilTouchReleased();

  // ----------------------------------------------------------------------------------------------------
  // Loop

  while (1)
  {
    // search through every 4x4 field for active touch input
    for (byte i = 0; i < 9; i++)
    {
      // search throug ever single field of the current 4x4 field for active touch input
      for (byte j = 0; j < 4; j++)
      {
        // check if single field has active touch input
        if (tictactoeFieldStates[i] == -1 && CS.digitalRead(tictactoeFields[i][j]) == 0)
        {
          // log corresponding 4x4 field to current player id
          tictactoeFieldStates[i] = player;
          // set corresponding 4x4 field to current player color
          for (byte k = 0; k < 4; k++)
          {
            strip.setPixelColor(tictactoeFields[i][k], playerColors[player]);
          }
          strip.show();

          // check if current player has won current round - check for every possible winning graphic
          for (byte i = 0; i < 8; i++)
          {
            // check if current player has won the current round with the current winning graphic
            if (tictactoeFieldStates[tictactoeWinningRows[i][0]] == player && tictactoeFieldStates[tictactoeWinningRows[i][1]] == player && tictactoeFieldStates[tictactoeWinningRows[i][2]] == player)
            {
              // count up victory for current player
              victories[player] = victories[player] + 1;
              // play winning flash animation in current players color
              for (byte j = 0; j < winningFlashRepeats; j++)
              {
                // set frame to current players color
                for (byte k = 0; k < 36; k++)
                {
                  strip.setPixelColor(tictactoeFrame[k], playerColors[player]);
                }
                strip.show();
                delay(winningFlashDelay);
                // set frame to black
                for (byte k = 0; k < 36; k++)
                {
                  strip.setPixelColor(tictactoeFrame[k], strip.Color(0, 0, 0));
                }
                strip.show();
                delay(winningFlashDelay);
              }
              // check if current player has won entirely with 10 round victories
              if (victories[player] == 10)
              {
                // set frame to current players color
                for (byte j = 0; j < 36; j++)
                {
                  strip.setPixelColor(tictactoeFrame[j], playerColors[player]);
                }
                // set all 4x4 fields to current players color
                for (byte i = 0; i < 9; i++)
                {
                  for (byte j = 0; j < 4; j++)
                  {
                    strip.setPixelColor(tictactoeFields[i][j], playerColors[player]);
                  }
                }
                strip.show();
                delay(3000);
                // reset both players victories to 0
                victories[0] = 0;
                victories[1] = 0;
                // set frame to black
                for (byte j = 0; j < 36; j++)
                {
                  strip.setPixelColor(tictactoeFrame[j], strip.Color(0, 0, 0));
                }
                // reset all 4x4 fields && reset log
                for (byte i = 0; i < 9; i++)
                {
                  // reset log of all 4x4 fields to -1
                  tictactoeFieldStates[i] = -1;
                  // set corresponding 4x4 field to black
                  for (byte j = 0; j < 4; j++)
                  {
                    strip.setPixelColor(tictactoeFields[i][j], strip.Color(0, 0, 0));
                  }
                }
                strip.show();
                waitUntilTouchReleased();
              }
              else
              {
                // reset all 4x4 fields && reset log
                for (byte i = 0; i < 9; i++)
                {
                  // reset log of all 4x4 fields to -1
                  tictactoeFieldStates[i] = -1;
                  // set corresponding 4x4 field to black
                  for (byte j = 0; j < 4; j++)
                  {
                    strip.setPixelColor(tictactoeFields[i][j], strip.Color(0, 0, 0));
                  }
                }
                strip.show();
                // display current victories count per player
                for (byte i = 0; i < 10; i++)
                {
                  // check if current victory count was already won of player 0
                  if (victories[0] > i)
                  {
                    strip.setPixelColor(tictactoeScoreDisplays[0][i], playerScoreColors[0]);
                  }
                  // check if current victory count was already won of player 1
                  if (victories[1] > i)
                  {
                    strip.setPixelColor(tictactoeScoreDisplays[1][i], playerScoreColors[1]);
                  }
                  // check if current update was last -> then exit fro loop early
                  if (victories[0] <= i && victories[1] <= i)
                  {
                    break;
                  }
                  strip.show();
                  delay(newRoundDelay / 10);
                }
                waitUntilTouchReleased();
              }
            }
          }
          // check if nobody has wo current round
          if (tictactoeFieldStates[0] != -1 && tictactoeFieldStates[1] != -1 && tictactoeFieldStates[2] != -1 && tictactoeFieldStates[3] != -1 && tictactoeFieldStates[4] != -1 && tictactoeFieldStates[5] != -1 && tictactoeFieldStates[6] != -1 && tictactoeFieldStates[7] != -1 && tictactoeFieldStates[8] != -1)
          {
            delay(newRoundDelay + winningFlashRepeats * winningFlashDelay);
            // reset all 4x4 fields && reset log
            for (byte i = 0; i < 9; i++)
            {
              // reset log of all 4x4 fields to -1
              tictactoeFieldStates[i] = -1;
              // set corresponding 4x4 field to black
              for (byte j = 0; j < 4; j++)
              {
                strip.setPixelColor(tictactoeFields[i][j], strip.Color(0, 0, 0));
              }
            }
            strip.show();
            waitUntilTouchReleased();
          }
          // switch current player id
          if (player == 0)
          {
            player = 1;
          }
          else
          {
            player = 0;
          }
        }
        // update current player indicator
        if (player == 0)
        {
          // set player indicator of player 0 to corresponding color
          for (byte i = 0; i < 8; i++)
          {
            strip.setPixelColor(tictactoePlayerIndicators[0][i], playerIndicatorColors[0]);
          }
          // set other player indicator to black
          for (byte i = 0; i < 8; i++)
          {
            strip.setPixelColor(tictactoePlayerIndicators[1][i], strip.Color(0, 0, 0));
          }
        }
        else
        {
          // set player indicator of player 1 to corresponding color
          for (byte i = 0; i < 8; i++)
          {
            strip.setPixelColor(tictactoePlayerIndicators[1][i], playerIndicatorColors[1]);
          }
          // set other player indicator to black
          for (byte i = 0; i < 8; i++)
          {
            strip.setPixelColor(tictactoePlayerIndicators[0][i], strip.Color(0, 0, 0));
          }
        }
        strip.show();
      }
    }
  }
}

void paint()
{
  // TODO: paint() NOT OPTIMISED, BUT WORKING - code refactoring pending...
  // TODO: paint() NOT OPTIMISED, BUT WORKING - code refactoring pending...
  // TODO: paint() NOT OPTIMISED, BUT WORKING - code refactoring pending...

  // ----------------------------------------------------------------------------------------------------
  // Setup
  
  byte rotVal     = 0;                // Value für "Rot" zum kompletten Ausfüllen bei "Malen"
  byte gelbVal    = 0;                // Value für "Gelb" zum kompletten Ausfüllen bei "Malen"
  byte gruenVal   = 0;                // Value für "Grün" zum kompletten Ausfüllen bei "Malen"
  byte tuerkisVal = 0;                // Value für "Türkis" zum kompletten Ausfüllen bei "Malen"
  byte blauVal    = 0;                // Value für "Blau" zum kompletten Ausfüllen bei "Malen"
  byte lilaVal    = 0;                // Value für "Lila" zum kompletten Ausfüllen bei "Malen"
  byte weissVal   = 0;                // Value für "Weiß" zum kompletten Ausfüllen bei "Malen"
  byte schwarzVal = 0;                // Value für "Schwarz" zum kompletten Ausfüllen bei "Malen"

  byte resetVal   = 0;                // Value für "Reset" zum Löschen bei "Malen"
  int sperrenVal  = 0;                // Value für "Sperren" zum Sperren bei "Malen"

  byte resetDelayVal        = 1;      // Abbau-Delay-Zeit für Löschen wie z.B. Löschen bei "Malen"; 
  byte aufbauDelayVal       = 25;     // Aufbau-Delay-Zeit für Menüs wie z.B. Farbauswahl-Menü bei "Live-Malen"; Farbauswahl-Leiste bei "Malen"
  byte musterAufbauDelayVal = 10;     // Aufbau-Delay-Zeit für Muster wie z.B. Spirale bei Ausfüllen in "Malen"
  int menu_delay            = 750;    // Wartezeit in ms nach Programm-Auswahl bis zur nächsten Interaktion
  
  byte fuellenVal = 35;               // Programm-Durchlauf-Anzahl als Zeit für komplettes Füllen mit einer Farbe bei "Malen"

  int touchStateArray[100];
  int malenTouchSensorArray[100] = {4, 15, 24, 35, 44, 55, 64, 75, 84, 95, 5, 14, 25, 34, 45, 54, 65, 74, 85, 94, 6, 13, 26, 33, 46, 53, 66, 73, 86, 93, 0, 19, 20, 39, 40, 59, 60, 79, 80, 99, 1, 18, 21, 38, 41, 58, 61, 78, 81, 98, 2, 17, 22, 37, 42, 57, 62, 77, 82, 97, 3, 16, 23, 36, 43, 56, 63, 76, 83, 96, 7, 12, 27, 32, 47, 52, 67, 72, 87, 92, 8, 11, 28, 31, 48, 51, 68, 71, 88, 91, 9, 10, 29, 30, 49, 50, 69, 70, 89, 90};
  int malenLedArray[100] = {4, 15, 24, 35, 44, 55, 64, 75, 84, 95, 5, 14, 25, 34, 45, 54, 65, 74, 85, 94, 6, 13, 26, 33, 46, 53, 66, 73, 86, 93, 0, 19, 20, 39, 40, 59, 60, 79, 80, 99, 1, 18, 21, 38, 41, 58, 61, 78, 81, 98, 2, 17, 22, 37, 42, 57, 62, 77, 82, 97, 3, 16, 23, 36, 43, 56, 63, 76, 83, 96, 7, 12, 27, 32, 47, 52, 67, 72, 87, 92, 8, 11, 28, 31, 48, 51, 68, 71, 88, 91, 9, 10, 29, 30, 49, 50, 69, 70, 89, 90};
  int musterLedArray1[100] = {55, 44, 35, 36, 43, 56, 63, 64, 65, 54, 45, 34, 25, 24, 23, 22, 37, 42, 57, 62, 77, 76, 75, 74, 73, 66, 53, 46, 33, 26, 13, 14, 15, 16, 17, 18, 21, 38, 41, 58, 61, 78, 81, 82, 83, 84, 85, 86, 87, 72, 67, 52, 47, 32, 27, 12, 7, 6, 5, 4, 3, 2, 1, 0, 19, 20, 39, 40, 59, 60, 79, 80, 99, 98, 97, 96, 95, 94, 93, 92, 8, 11, 28, 31, 48, 51, 68, 71, 88, 91, 90, 89, 70, 69, 50, 49, 30, 29, 10, 9};

  int colorArray[3];                  // Mal-Farben-Array für z.B. "Malen"; "RGB-Live-Malen"; "RGB-Komplimentär-Live-Malen"; "Löschen ("Malen")
  
  int bild[100][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};                   // Matrix-Speicher eines 10x10 Pixel Bildes von z.B. "Malen"

  colorArray[0] = 0; // Farbe Rot = 0 setzen
  colorArray[1] = 0; // Farbe Grün = 0 setzen
  colorArray[2] = 0; // Farbe Blau = 0 setzen

  for (byte i = 0; i < 100; i++) // Alle LED-Felder = 0, 0, 0 setzen
  {
    strip.setPixelColor(i, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
  }
  strip.show();

  // ----------------------------------------------------------------------------------------------------
  // Loop

  while (1) // Endlosschleife Programm4 (1/2)
  {
    start4:

    // Farb-Auswahl-Menü anzeigen inkl. aktuell gewählter Farbe und schwarzem Balken

    for (byte i = 80; i < 90; i++)
    {
      strip.setPixelColor(malenLedArray[i], strip.Color(0, 0, 0));
    }

    strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
    strip.setPixelColor(10, strip.Color(0, 0, 0));
    strip.setPixelColor(29, strip.Color(255, 0, 0));
    strip.setPixelColor(30, strip.Color(255, 255, 0));
    strip.setPixelColor(49, strip.Color(0, 255, 0));
    strip.setPixelColor(50, strip.Color(0, 255, 255));
    strip.setPixelColor(69, strip.Color(0, 0, 255));
    strip.setPixelColor(70, strip.Color(255, 0, 255));
    strip.setPixelColor(89, strip.Color(255, 255, 255));
    strip.setPixelColor(90, strip.Color(0, 0, 0));

    strip.show();

    delay(menu_delay);

    start4_1:

    for (byte i = 0; i < 100; i++) // Alle 100 Touch-Sensor-Signale einlesen und speichern (in "Malen" Reihenfolge -> Array)
    {
      touchStateArray[i] = CS.digitalRead(malenTouchSensorArray[i]);
    }

    for (byte i = 0; i < 80; i++) // Alle 80 Touch-Status-Signale der Mal-Felder der ausgewählten Farbe gleichsetzen + Gemaltes Bild speichern bzw. updaten in bild[x1][x2,1/x2,2/x2,3] (x1 = PixelId. ; x2,1/x2,2/x2,3 = Farbe des jeweiligen Pixels x,x,x)
    {
      if (touchStateArray[i] == 0)
      {
        strip.setPixelColor(malenLedArray[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
        strip.show();
        bild[i][0] = colorArray[0];
        bild[i][1] = colorArray[1];
        bild[i][2] = colorArray[2];
      }
    }

    if (touchStateArray[92] == 0) // Wenn Rot ausgewählt wird -> colorArray = 255,0,0 setzen und aktuelle Farbe aktualisieren + rotVal um 1 erhöhen für komplettes Füllen bei rotVal = fuellenVal + wenn komplett ausgefüllt -> rotVal = 0
    {
      rotVal++;

      colorArray[0] = 255;
      colorArray[1] = 0;
      colorArray[2] = 0;

      strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (rotVal == fuellenVal)
      {
        for (byte i = 0; i < 80; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        rotVal = 0;
      }
    }

    else if (touchStateArray[93] == 0) // Wenn Gelb ausgewählt wird -> colorArray = 255,255,0 setzen und aktuelle Farbe aktualisieren + gelbVal um 1 erhöhen für komplettes Füllen bei gelbVal = fuellenVal + wenn komplett ausgefüllt -> gelbVal = 0
    {
      gelbVal++;

      colorArray[0] = 255;
      colorArray[1] = 255;
      colorArray[2] = 0;

      strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (gelbVal == fuellenVal)
      {
        for (byte i = 0; i < 80; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        gelbVal = 0;
      }
    }

    else if (touchStateArray[94] == 0) // Wenn Grün ausgewählt wird -> colorArray = 0,255,0 setzen und aktuelle Farbe aktualisieren + gruenVal um 1 erhöhen für komplettes Füllen bei gruenVal = fuellenVal + wenn komplett ausgefüllt -> gruenVal = 0
    {
      gruenVal++;

      colorArray[0] = 0;
      colorArray[1] = 255;
      colorArray[2] = 0;

      strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (gruenVal == fuellenVal)
      {
        for (byte i = 0; i < 80; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        gruenVal = 0;
      }
    }

    else if (touchStateArray[95] == 0) // Wenn Türkis ausgewählt wird -> colorArray = 0,255,255 setzen und aktuelle Farbe aktualisieren + tuerkisVal um 1 erhöhen für komplettes Füllen bei tuerkisVal = fuellenVal + wenn komplett ausgefüllt -> tuerkisVal = 0
    {
      tuerkisVal++;

      colorArray[0] = 0;
      colorArray[1] = 255;
      colorArray[2] = 255;

      strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (tuerkisVal == fuellenVal)
      {
        for (byte i = 0; i < 80; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        tuerkisVal = 0;
      }
    }

    else if (touchStateArray[96] == 0) // Wenn Blau ausgewählt wird -> colorArray = 0,0,255 setzen und aktuelle Farbe aktualisieren + blauVal um 1 erhöhen für komplettes Füllen bei blauVal = fuellenVal + wenn komplett ausgefüllt -> blauVal = 0
    {
      blauVal++;

      colorArray[0] = 0;
      colorArray[1] = 0;
      colorArray[2] = 255;

      strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (blauVal == fuellenVal)
      {
        for (byte i = 0; i < 80; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        blauVal = 0;
      }
    }

    else if (touchStateArray[97] == 0) // Wenn Lila ausgewählt wird -> colorArray = 255,0,255 setzen und aktuelle Farbe aktualisieren + lilaVal um 1 erhöhen für komplettes Füllen bei lilaVal = fuellenVal + wenn komplett ausgefüllt -> lilaVal = 0
    {
      lilaVal++;

      colorArray[0] = 255;
      colorArray[1] = 0;
      colorArray[2] = 255;

      strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (lilaVal == fuellenVal)
      {
        for (byte i = 0; i < 80; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        lilaVal = 0;
      }
    }

    else if (touchStateArray[98] == 0) // Wenn Weiß ausgewählt wird -> colorArray = 255,255,255 setzen und aktuelle Farbe aktualisieren + weissVal um 1 erhöhen für komplettes Füllen bei weissVal = fuellenVal + wenn komplett ausgefüllt -> weissVal = 0
    {
      weissVal++;

      colorArray[0] = 255;
      colorArray[1] = 255;
      colorArray[2] = 255;

      strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (weissVal == fuellenVal)
      {
        for (byte i = 0; i < 80; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        weissVal = 0;
      }
    }

    else if (touchStateArray[99] == 0) // Wenn Schwarz ausgewählt wird -> colorArray = 0, 0, 0 setzen und aktuelle Farbe aktualisieren + schwarzVal um 1 erhöhen für komplettes Füllen bei schwarzVal = fuellenVal + wenn komplett ausgefüllt -> schwarzVal = 0
    {
      schwarzVal++;

      colorArray[0] = 0;
      colorArray[1] = 0;
      colorArray[2] = 0;

      strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (schwarzVal == fuellenVal)
      {
        for (byte i = 0; i < 80; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        schwarzVal = 0;
      }
    }

    else if (touchStateArray[90] == 0) // Wenn Sperren ausgewählt wird -> gehe zu sperren1
    {
      goto sperren1;
    }

    else if (touchStateArray[91] == 0) // Wenn Reset ausgewählt wird -> gehe zu reset1
    {
      goto reset1;
    }

    else // sonst setze Value aller Farben xVal = 0 + wiederhole ganzen Vorgang
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

  while (1) // Endlosschleife Programm4 (2/2)
  {
    start4_5:

    // Farb-Auswahl-Menü anzeigen inkl. aktuell gewählter Farbe und schwarzem Balken

    for (byte i = 0; i < 30; i++)
    {
      strip.setPixelColor(malenLedArray[i], strip.Color(0, 0, 0));
    }

    strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
    strip.setPixelColor(14, strip.Color(0, 0, 0));
    strip.setPixelColor(25, strip.Color(255, 0, 0));
    strip.setPixelColor(34, strip.Color(255, 255, 0));
    strip.setPixelColor(45, strip.Color(0, 255, 0));
    strip.setPixelColor(54, strip.Color(0, 255, 255));
    strip.setPixelColor(65, strip.Color(0, 0, 255));
    strip.setPixelColor(74, strip.Color(255, 0, 255));
    strip.setPixelColor(85, strip.Color(255, 255, 255));
    strip.setPixelColor(94, strip.Color(0, 0, 0));

    strip.show();

    delay(menu_delay);

    start4_6:

    for (byte i = 0; i < 100; i++) // Alle 100 Touch-Sensor-Signale einlesen und speichern (in "Malen" Reihenfolge -> Array)
    {
      touchStateArray[i] = CS.digitalRead(malenTouchSensorArray[i]);
    }

    for (byte i = 30; i < 100; i++) // Alle 70 Touch-Status-Signale der Mal-Felder der ausgewählten Farbe gleichsetzen + Gemaltes Bild speichern bzw. updaten in bild[x1][x2,1/x2,2/x2,3] (x1 = PixelId. ; x2,1/x2,2/x2,3 = Farbe des jeweiligen Pixels x,x,x)
    {
      if (touchStateArray[i] == 0)
      {
        strip.setPixelColor(malenLedArray[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
        strip.show();
        bild[i][0] = colorArray[0];
        bild[i][1] = colorArray[1];
        bild[i][2] = colorArray[2];
      }
    }

    if (touchStateArray[12] == 0) // Wenn Rot ausgewählt wird -> colorArray = 255,0,0 setzen und aktuelle Farbe aktualisieren + rotVal um 1 erhöhen für komplettes Füllen bei rotVal = fuellenVal + wenn komplett ausgefüllt -> rotVal = 0
    {
      rotVal++;

      colorArray[0] = 255;
      colorArray[1] = 0;
      colorArray[2] = 0;

      strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (rotVal == fuellenVal)
      {
        for (byte i = 80; i < 100; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        rotVal = 0;
      }
    }

    else if (touchStateArray[13] == 0) // Wenn Gelb ausgewählt wird -> colorArray = 255,255,0 setzen und aktuelle Farbe aktualisieren + gelbVal um 1 erhöhen für komplettes Füllen bei gelbVal = fuellenVal + wenn komplett ausgefüllt -> gelbVal = 0
    {
      gelbVal++;

      colorArray[0] = 255;
      colorArray[1] = 255;
      colorArray[2] = 0;

      strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (gelbVal == fuellenVal)
      {
        for (byte i = 80; i < 100; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        gelbVal = 0;
      }
    }

    else if (touchStateArray[14] == 0) // Wenn Grün ausgewählt wird -> colorArray = 0,255,0 setzen und aktuelle Farbe aktualisieren + gruenVal um 1 erhöhen für komplettes Füllen bei gruenVal = fuellenVal + wenn komplett ausgefüllt -> gruenVal = 0
    {
      gruenVal++;

      colorArray[0] = 0;
      colorArray[1] = 255;
      colorArray[2] = 0;

      strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (gruenVal == fuellenVal)
      {
        for (byte i = 80; i < 100; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        gruenVal = 0;
      }
    }

    else if (touchStateArray[15] == 0) // Wenn Türkis ausgewählt wird -> colorArray = 0,255,255 setzen und aktuelle Farbe aktualisieren + tuerkisVal um 1 erhöhen für komplettes Füllen bei tuerkisVal = fuellenVal + wenn komplett ausgefüllt -> tuerkisVal = 0
    {
      tuerkisVal++;

      colorArray[0] = 0;
      colorArray[1] = 255;
      colorArray[2] = 255;

      strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (tuerkisVal == fuellenVal)
      {
        for (byte i = 80; i < 100; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        tuerkisVal = 0;
      }
    }

    else if (touchStateArray[16] == 0) // Wenn Blau ausgewählt wird -> colorArray = 0,0,255 setzen und aktuelle Farbe aktualisieren + blauVal um 1 erhöhen für komplettes Füllen bei blauVal = fuellenVal + wenn komplett ausgefüllt -> blauVal = 0
    {
      blauVal++;

      colorArray[0] = 0;
      colorArray[1] = 0;
      colorArray[2] = 255;

      strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (blauVal == fuellenVal)
      {
        for (byte i = 80; i < 100; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        blauVal = 0;
      }
    }

    else if (touchStateArray[17] == 0) // Wenn Lila ausgewählt wird -> colorArray = 255,0,255 setzen und aktuelle Farbe aktualisieren + lilaVal um 1 erhöhen für komplettes Füllen bei lilaVal = fuellenVal + wenn komplett ausgefüllt -> lilaVal = 0
    {
      lilaVal++;

      colorArray[0] = 255;
      colorArray[1] = 0;
      colorArray[2] = 255;

      strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (lilaVal == fuellenVal)
      {
        for (byte i = 80; i < 100; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        lilaVal = 0;
      }
    }

    else if (touchStateArray[18] == 0) // Wenn Weiß ausgewählt wird -> colorArray = 255,255,255 setzen und aktuelle Farbe aktualisieren + weissVal um 1 erhöhen für komplettes Füllen bei weissVal = fuellenVal + wenn komplett ausgefüllt -> weissVal = 0
    {
      weissVal++;

      colorArray[0] = 255;
      colorArray[1] = 255;
      colorArray[2] = 255;

      strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (weissVal == fuellenVal)
      {
        for (byte i = 80; i < 100; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        weissVal = 0;
      }
    }

    else if (touchStateArray[19] == 0) // Wenn Schwarz ausgewählt wird -> colorArray = 0, 0, 0 setzen und aktuelle Farbe aktualisieren + schwarzVal um 1 erhöhen für komplettes Füllen bei schwarzVal = fuellenVal + wenn komplett ausgefüllt -> schwarzVal = 0
    {
      schwarzVal++;

      colorArray[0] = 0;
      colorArray[1] = 0;
      colorArray[2] = 0;

      strip.setPixelColor(5, strip.Color(colorArray[0], colorArray[1], colorArray[2]));
      strip.show();

      if (schwarzVal == fuellenVal)
      {
        for (byte i = 80; i < 100; i++)
        {
          strip.setPixelColor(musterLedArray1[i], strip.Color(colorArray[0], colorArray[1], colorArray[2]));
          strip.show();
          bild[i][0] = colorArray[0];
          bild[i][1] = colorArray[1];
          bild[i][2] = colorArray[2];
          delay(musterAufbauDelayVal);
        }
        schwarzVal = 0;
      }
    }

    else if (touchStateArray[10] == 0) // Wenn Sperren ausgewählt wird -> gehe zu sperren1
    {
      goto sperren2;
    }

    else if (touchStateArray[11] == 0) // Wenn Reset ausgewählt wird -> gehe zu reset1
    {
      goto reset2;
    }

    else // sonst setze Value aller Farben xVal = 0 + wiederhole ganzen Vorgang
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

  resetVal++; // resetVall um 1 erhöhen

  if (resetVal == 50) // Wenn restVal = 50 dann lösche Bild und setze aktuelle Farbe auf 0, 0, 0
  {
    for (byte i = 0; i < 80; i++)
    {
      strip.setPixelColor(malenLedArray[i], strip.Color(0, 0, 0));
    }
    strip.show();

    resetVal = 0; // Setze resteVal = 0

    colorArray[0] = 0; // Farbe Rot = 0 setzen
    colorArray[1] = 0; // Farbe Grün = 0 setzen
    colorArray[2] = 0; // Farbe Blau = 0 setzen

    strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2])); // Anzeige aktueller Farbe auf 0, 0, 0 setzen
    strip.show();

    for (byte i = 0; i < 100; i++)
    {
      bild[i][0] = colorArray[0];
      bild[i][1] = colorArray[1];
      bild[i][2] = colorArray[2];
    }

    goto start4; // Wiederhole Mal-Vorgang ab start4
  }

  else
  {
    goto start4_1; // Wiederhole Mal-Vorgang ab start4_1
  }

  // ....................................................................................................

  reset2:

  resetVal++; // resetVall um 1 erhöhen

  if (resetVal == 50) // Wenn restVal = 50 dann lösche Bild und setze aktuelle Farbe auf 0, 0, 0
  {
    for (byte i = 0; i < 80; i++)
    {
      strip.setPixelColor(malenLedArray[i], strip.Color(0, 0, 0));
    }
    strip.show();

    resetVal = 0; // Setze resteVal = 0

    colorArray[0] = 0; // Farbe Rot = 0 setzen
    colorArray[1] = 0; // Farbe Grün = 0 setzen
    colorArray[2] = 0; // Farbe Blau = 0 setzen

    strip.setPixelColor(9, strip.Color(colorArray[0], colorArray[1], colorArray[2])); // Anzeige aktueller Farbe auf 0, 0, 0 setzen
    strip.show();

    for (byte i = 0; i < 100; i++)
    {
      bild[i][0] = colorArray[0];
      bild[i][1] = colorArray[1];
      bild[i][2] = colorArray[2];
    }

    goto start4; // Wiederhole Mal-Vorgang ab start4
  }

  else
  {
    goto start4_6; // Wiederhole Mal-Vorgang ab start4_1
  }

  // ....................................................................................................

  sperren1:

  sperrenVal++; // sperrenVal um 1 erhöhen

  if (sperrenVal == 10) // Wenn sperrenVal = 40 dann blende Farben-Auswahl aus und färbe diese Reihe in aktueller Farbe + wenn sperrenVal erneut 4000 dann blende Farben-Auswahl wieder ein und gehe zurück in Mal-Programm (gehe zu start 1_5)
  {
    for (byte i = 80; i < 100; i++)
    {
      strip.setPixelColor(malenLedArray[i], strip.Color(bild[i][0], bild[i][1], bild[i][2]));
    }

    sperrenVal = 0;

    colorArray[0] = 0;
    colorArray[1] = 0;
    colorArray[2] = 0;

    goto start4_5;
  }

  else
  {
    goto start4_1; // Wiederhole Mal-Vorgang ab start4_1
  }

  // ....................................................................................................

  sperren2:

  sperrenVal++; // sperrenVal um 1 erhöhen

  if (sperrenVal == 10) // Wenn sperrenVal = 40 dann blende Farben-Auswahl aus und färbe diese Reihe in aktueller Farbe + wenn sperrenVal erneut 4000 dann blende Farben-Auswahl wieder ein und gehe zurück in Mal-Programm (gehe zu start 1_5)
  {
    for (byte i = 0; i < 100; i++)
    {
      strip.setPixelColor(malenLedArray[i], strip.Color(bild[i][0], bild[i][1], bild[i][2]));
    }
    strip.show();

    sperrenVal = 0;

    while (1) // Endlosschleife im gesperrten Zusatand
    {
      touchStateArray[90] = CS.digitalRead(malenTouchSensorArray[90]);

      if (touchStateArray[90] == 0)
      {
        sperrenVal++;

        if (sperrenVal == 4000)
        {
          for (byte i = 0; i < 80; i++)
          {
            strip.setPixelColor(malenLedArray[i], strip.Color(bild[i][0], bild[i][1], bild[i][2]));
          }

          sperrenVal = 0;

          colorArray[0] = 0;
          colorArray[1] = 0;
          colorArray[2] = 0;

          goto start4; // Wiederhole Mal-Vorgang ab start4
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
    goto start4_6; // Wiederhole Mal-Vorgang ab start4_1
  }
}

void waitUntilTouchReleased()
{
  bool triggered = true;
  byte i_release = 0;

  while (triggered || i_release < 10)
  {
    triggered = false;

    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
      if (CS.digitalRead(i) == 0)
      {
        triggered = true;
      }
    }

    if (triggered == false)
    {
      i_release++;
    }
    else
    {
      i_release = 0;
    }
  }
}

// strip.Color(0, 0, 255), 50
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// 15
void rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// 3
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// strip.Color(0, 0, 127), 50
// Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
  for (int j = 0; j < 10; j++)
  { // do 10 cycles of chasing
    for (int q = 0; q < 3; q++)
    {
      for (int i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, c); // turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// 50
// Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
  for (int j = 0; j < 256; j++)
  { // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++)
    {
      for (int i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, Wheel((i + j) % 255)); // turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3)
      {
        strip.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;

  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }

  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }

  WheelPos -= 170;

  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
