#include <Bounce2.h>
#include <Keyboard.h>

const int bounceDelay = 90;
const int rows[8] = {10, 11, 12, 13, 14, 16, 44, 43};
const int cols[7] = {15, 17, 45, 42, 41, 40, 39};
const char keymap[8][7] = {
  {' ',  'L',  'K',  'J',  'H',  ';', 0},
  {' ',  'O',  'I',  'U',  'Y',  'P', 0},
  {0,    '9',  '8',  '7',  '6',  '0', 0},
  {'S',  'D',  'F',  'G',  'A',  0,   0},
  {'W',  'E',  'R',  'T',  'Q',  0,   0},
  {'X',  'C',  'V',  'B',  'Z',  0,   0},
  {'2',  '3',  '4',  '5',  '1',  0,   0},
  {0,    '.',  ',',  'M',  'N',  '/', 0}
};

Bounce wires[15];

bool fctn;
bool shft;
bool ctrl;
bool lock;

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
  }
  for (int i = 0; i < 7; i++) {
    pinMode(cols[i], INPUT);
  }

  for (int i = 0; i < 15; i++) {
    wires[i] = Bounce(cols[i % 7], bounceDelay);
  }

  Keyboard.begin();
}

void checkModifiers() {
  ctrl = digitalRead(rows[7]);
  shft = digitalRead(rows[2]);
  fctn = digitalRead(rows[6]);
  lock = digitalRead(rows[1]);
}

void loop() {
  checkModifiers();
  
  for (int r = 0; r < 8; r++) {
    digitalWrite(rows[r], HIGH);
    for (int c = 0; c < 7; c++) {
      wires[c + r * 7].update();
      if (wires[c + r * 7].risingEdge()) {
        char ch = keymap[r][c];
        if (ch == 0) {
          continue;
        }
        if (lock) {
          ch = toupper(ch);
          lock = false;
        } else if (shft) {
          ch = toupper(ch);
          shft = false;
        } else if (fctn) {
          if (ch == '_') {
            ch = '-';
          } else if (ch == '?') {
            ch = '=';
          } else if (ch == '"') {
            ch = '\'';
          }
          fctn = false;
        }
        Keyboard.write(ch);
      }
    }
    digitalWrite(rows[r], LOW);
  }
}