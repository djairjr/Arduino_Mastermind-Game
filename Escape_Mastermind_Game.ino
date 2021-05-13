#include <FastLED.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include "pRNG.h"

#include "Pitches.h"
#include "Hal.h"


ClickEncoder *encoder;

void timerIsr() {
  encoder->service();
}

void(* resetFunc) (void) = 0;

void resetValues() {
  Serial.println ("Reset");
  int minLedPosition = 4;
  int maxLedPosition = 7;
  int thisLedPosition = 7;

  int minBCPosition = 0;
  int maxBCPosition = 3;
  int thisBCPosition = 0;

  int minPos = 0;
  int maxPos = 3; // Index
  int thisPos = minPos;

  int16_t last, value;
  int ledColor;
  int tryes = 0;
  int reference = 0;
  int allColors = 1;

  getLedColor();
  leds[thisLedPosition] = sevenColors[c];
  leds[thisLedPosition].fadeToBlackBy(192);
  //  FastLED.show();
  last = -1;
  Serial.print ("Posicao do Led ");
  Serial.println (thisLedPosition);
  Serial.print ("Tentativas ");
  Serial.println (tryes);
}

void ledTest() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] =   CRGB::Black;
    FastLED.show();
  }

  for (int row = 0; row < ROWS; row++) {
    int ledBegin = row * bCols;
    int ledEnd = ledBegin + bCols;
    for (int ledPos = ledBegin; ledPos < ledEnd;  ledPos++) {
      if (row % 2 == 0) { // Linhas Pares - Direita
        leds[ledPos] = bullsCows[1];
        leds[ledPos].fadeToBlackBy(192);
      }
      FastLED.show();
    }
  }

  for (int row = 0; row < ROWS; row++) {
    int ledBegin = row * bCols;
    int ledEnd = ledBegin + bCols;
    for (int ledPos = ledBegin; ledPos < ledEnd;  ledPos++) {
      if (row % 2 == 1) { // Linhas Impares - Esquerda
        leds[ledPos] = bullsCows[2];
        leds[ledPos].fadeToBlackBy(192);
      }
      FastLED.show();
    }
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] =   CRGB::Black;
    FastLED.show();
  }
}


void updatePosition() {
  Serial.println ("Update Position");

  reference = tryes * 8;
  minLedPosition = reference + 4;
  maxLedPosition = reference + 7;
  thisLedPosition = maxLedPosition;
  thisPos = minPos; // Posicao do numero e nao da cor
  leds[thisLedPosition] = sevenColors[c];

  Serial.print ("Posicao do Led ");
  Serial.println (thisLedPosition);
  Serial.print ("Tentativas ");
  Serial.println (tryes);
}


void getLedColor() {
  value += encoder->getValue();
  if (value != last) {
    if (value > 80) {
      value = 0;
    }
    if (value < 0) {
      value = 80;
    }
    last = value;
    c = map (value, 0, 80, 1, 8);
    if (c == 8) {
      c = 7;
    }
    Serial.print("Cor: ");
    Serial.println(c);
    leds[thisLedPosition] = sevenColors[c];
    leds[thisLedPosition].fadeToBlackBy(192);
    debugPlayer[thisPos] = c;
    FastLED.show();
  }
}



void computerGuess() {
  for (int i = 0; i < 4; i++) {
    index = map (numeroMaluco.getRndByte(), 0, 255, 1, PINCOLORS);
    debugComp[i] = index;
    computerCode[i] = sevenColors[debugComp[i]];
  }
  Serial.print ("Arduino Escolheu ");
  for  (int i = 0; i < 4; i++) {
    Serial.print (debugComp[i]);
    Serial.print(" ");
  }
}



boolean checkDuplicate(int myArray[4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i != j) {
        if (myArray[i] == myArray[j]) {
          return false;
          break;
        }
      }
    }
  }
  return true;
}

void computerChoice() {
  while (!checkDuplicate(debugComp)) {
    computerGuess();
  }
  Serial.print ("Arduino Escolheu ");
  for  (int i = 0; i < 4; i++) {
    Serial.print (debugComp[i]);
    Serial.print(" ");
  }
  Serial.println("");
}


void bullOrCow() {
  minBCPosition = reference + 0;
  maxBCPosition = reference + 3;
  thisBCPosition = minBCPosition;

  Serial.print ("Jogador:  ");
  for (int i = 0; i < 4; i++) {
    Serial.print (debugPlayer[i]);
    Serial.print(" ");
  }
  int bull = 0;
  int cow = 0;
  int count = 0;
  Serial.print ("Computador: ");
  for (int b = 0; b < 4; b++) {
    Serial.print (debugComp[b]);
    Serial.print (" ");
    if (debugPlayer[b] == debugComp[b]) {
      bull ++;
    } else {
      for (int c = 0; c < 4; c++) {
        if (b != c) {
          if (debugPlayer[b] == debugComp[c]) {
            cow++;
          }
        }
      }
    }
  }
  Serial.println ("");
  if (bull == 4) {
    for (int i=minBCPosition; i<=maxBCPosition; i++) {
      leds[i] = bullsCows[1];
      leds[i].fadeToBlackBy(192);
    }
    FastLED.show();
    delay(1000);
    victory();
    resetFunc();
  } else {

    // Display Bull Or Cow
    int bullEnd = bull + minBCPosition;
    int cowEnd = bullEnd + cow;

    for (int i = minBCPosition; i < bullEnd; i++) {
      leds[i] =  bullsCows[1];
      leds[i].fadeToBlackBy(192);
      tone (8, NOTE_B5, 100);
      delay(100);
      noTone(8);
    }

    for (int c = bullEnd; c < cowEnd; c++) {
      leds[c] = bullsCows[2];
      leds[c].fadeToBlackBy(192);
      tone (8, NOTE_F5, 100);
      delay(100);
      noTone(8);
    }
    FastLED.show();
  }
}



void gameOver() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] =   CRGB::Black;
    FastLED.show();
  }
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int defeatDuration = 1000 / defeatDurations[thisNote];
    tone(8, defeat[thisNote], defeatDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = defeatDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

void victory() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] =   CRGB::Green;
    leds[i].fadeToBlackBy(192);
    FastLED.show();
  }
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / 150;
  int divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(8, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(8);
  }
    for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] =   CRGB::Black;
    FastLED.show();
  }
}



void setup() {
  Serial.begin (115200);
  Serial.println ("Sistema Iniciado");
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  encoder = new ClickEncoder(ROTARY_CLK, ROTARY_DT, ROTARY_SW);
  leds[thisLedPosition] = sevenColors[1];
  leds[thisLedPosition].fadeToBlackBy(192);
  FastLED.show();
  last = -1;
  computerChoice();
}


void loop() {

  while (tryes < 16) {
    reference = tryes * 8;
    minLedPosition = reference + 4;
    maxLedPosition = reference + 7;

    getLedColor();

    ClickEncoder::Button b = encoder->getButton();
    if (b != ClickEncoder::Open) {
      Serial.print("Button: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;

      switch (b) {
          VERBOSECASE(ClickEncoder::Pressed);
          VERBOSECASE(ClickEncoder::Held)
          VERBOSECASE(ClickEncoder::Released)
        case ClickEncoder::Clicked:

          tone (8, NOTE_A6, 100);
          delay(100);
          noTone(8);

          Serial.println("Muda o Led");

          if (thisLedPosition > minLedPosition) {
            --thisLedPosition;
            thisPos ++;
            leds[thisLedPosition] = sevenColors[c];

          } else {

            bullOrCow();
            tryes ++;
            updatePosition();
          }
          leds[thisLedPosition].fadeToBlackBy(192);
          FastLED.show();
          break;

        case ClickEncoder::DoubleClicked:
          tone (8, NOTE_A6, 100);
          delay(100);
          noTone(8);
          if (thisLedPosition == minLedPosition) {
            bullOrCow();
            tryes ++;
            updatePosition();

            leds[thisLedPosition].fadeToBlackBy(192);
            FastLED.show();
          }
          break;
      }
    }
  }
  gameOver();
  resetFunc();
}
