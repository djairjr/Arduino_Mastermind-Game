#define NUM_LEDS 128
#define ROWS 32
#define DATA_PIN 7

#define TONE 8

#define BUTTON 3         // Laranja

#define ROTARY_SW  6 // fio branco
#define ROTARY_DT   5 // fio violeta
#define ROTARY_CLK 4 // fio cinza

#define ROTARYSTEPS 1
#define ROTARYMIN 0
#define ROTARYMAX 6 // Seven Colors Maximum

#define cCols 4
#define cRows 16
#define bCols 4
#define bRows 16

#define levels 16

#define PINCOLORS 8
#define BULLCOWCOLORS 3

CRGB leds [NUM_LEDS];
CRGB gameBoard [NUM_LEDS / 2];
CRGB gameClue[NUM_LEDS / 2];

pRNG numeroMaluco;
byte index;

int gameLevel [levels];
int currentLevel;
int gameIsOver;

int computerIndex;
int playerIndex;

bool win = false;

int debugComp[bCols];
int debugPlayer[bCols];

int boardCols [bCols];
int boardRow [bRows];

int clueCols[cCols];
int clueRows[cRows];

CRGB computerCode[bCols];
CRGB playerCode[bCols];

enum colors {BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, CYAN, MAGENTA}; // 0 to 6
CRGB sevenColors [PINCOLORS] = {CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White, CRGB::Green, CRGB::Navy, CRGB::OrangeRed, CRGB::Magenta};

enum hintColors {OFF, BULL, COW}; // Bull Right Color and Right Position - Cow Right Color
CRGB bullsCows [BULLCOWCOLORS] = {CRGB::Black, CRGB::Lime, CRGB::OrangeRed};

// notes in the melody:
int defeat[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int defeatDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


int melody[] = {
  // Pulo da gaita - Auto da Compadecida
  // Score available at https://musescore.com/user/196039/scores/250206

  NOTE_G4, 16, NOTE_C4, 8, NOTE_C4, 16, NOTE_G4, 16, NOTE_G4, 8, NOTE_G4, 16,
  NOTE_C5, 4, NOTE_G4, 8, NOTE_AS4, 4, NOTE_A4, 8,
  NOTE_G4, 2,
  NOTE_C5, 4, NOTE_G4, 8, NOTE_AS4, 4, NOTE_A4, 8,

  NOTE_G4, 16, NOTE_C4, 8, NOTE_C4, 16, NOTE_G4, 16, NOTE_G4, 8, NOTE_G4, 16,
  NOTE_F4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_C4, -2,
  NOTE_C5, 4, NOTE_G4, 8, NOTE_AS4, 4, NOTE_A4, 8,

  NOTE_G4, 16, NOTE_C4, 8, NOTE_C4, 16, NOTE_G4, 16, NOTE_G4, 8, NOTE_G4, 16,
  NOTE_C5, 4, NOTE_G4, 8, NOTE_AS4, 4, NOTE_A4, 8,
  NOTE_G4, 2,
  NOTE_C5, 4, NOTE_G4, 8, NOTE_AS4, 4, NOTE_A4, 8,

  NOTE_G4, 16, NOTE_C4, 8, NOTE_C4, 16, NOTE_G4, 16, NOTE_G4, 8, NOTE_G4, 16,
  NOTE_F4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_C4, -2,
  NOTE_C4, 16, NOTE_C4, 8, NOTE_C4, 16, NOTE_E4, 16, NOTE_E4, 8, NOTE_E4, 16,
  NOTE_F4, 16, NOTE_F4, 8, NOTE_F4, 16, NOTE_FS4, 16, NOTE_FS4, 8, NOTE_FS4, 16,

  NOTE_G4, 8, REST, 8, NOTE_AS4, 8, NOTE_C5, 1,
};

int count;
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

int c = 1;
