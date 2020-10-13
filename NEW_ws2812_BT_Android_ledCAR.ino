/*
   Спарвка по данным
   MODE BRIG SPEED SCALE R G B SAVE
*/
#define LED_PIN        6          // Пин ленты
#define PARSE_AMOUNT   8          // Сколько параметров принимаем от блюпука
int BRIGHTNESS;
#define CURRENT_LIMIT 500    // лимит по току в миллиамперах, автоматически управляет яркостью. 0 - выключить лимит
#define WIDTH 6              // ширина матрицы
#define HEIGHT 1             // высота матрицы
#define COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB
#define BT_TX 3                   // не работает? поменять местами
#define BT_RX 2                   // не работает? поменять местами
#define MATRIX_TYPE 0         // тип матрицы: 0 - зигзаг, 1 - параллельная                                                          DEF: 0
#define CONNECTION_ANGLE 0    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний       DEF: 0
#define STRIP_DIRECTION 0     // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз                              DEF: 0
#define MODE_AMOUNT  21       // Модов
#define NUM_LEDS WIDTH * HEIGHT
#define SEGMENTS 1            // диодов в одном "пикселе" (для создания матрицы из кусков ленты)

#include <EEPROM.h>
#include <FastLED.h>
#include <SoftwareSerial.h>

////////// DEBUG ///////////
boolean DEBUG_SP = true;
boolean DEBUG_BT = false;
///////////////////////////
boolean loadingFlag = true;
CRGB leds[NUM_LEDS];


int8_t currentMode = 3;
boolean ONflag = true;
uint32_t effTimer;
struct {
  byte brightness = 50;
  byte speed = 30;
  byte scale = 40;
} modes[30];
int COLOR_RED; int COLOR_GREEN; int COLOR_BLUE; int SAVE;
int R_RED;
int R_GREEN;
int R_BLUE;
int intData[PARSE_AMOUNT];
boolean recievedFlag;
unsigned char matrixValue[8][16];
int l_idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int l_ihue = 0;                //-HUE (0-255)
int l_thissat = 255;           //-FX LOOPS DELAY VAR
int l_thisdelay = 20;          //-FX LOOPS DELAY VAR
int l_thisstep = 10;           //-FX LOOPS DELAY VAR
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
int l_thishue = 0;             //-FX LOOPS DELAY VAR
int ibright = 0;             //-BRIGHTNESS (0-255)
int TOP_INDEX = int(WIDTH / 2);
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int EVENODD = WIDTH % 2;
SoftwareSerial btSerial(2, 3);
void setup() {
  LoadEEPROM();
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection( TypicalLEDStrip )*/;
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  FastLED.show();
  Serial.begin(9600);
  btSerial.begin(38400);
  randomSeed(analogRead(0));    // ON генератор случайных чисел
  ALL_GENERATE();

}

void loop() {
  //parsingSP(); // По проводу управление
  parsingBT(); // По блюпуку
  if (recievedFlag) {
    recievedFlag = false;
    //   MODE BRIG SPEED SCALE R G B SAVE
    //   $1 1 1 1 1 1 1 1;
    currentMode = intData[0];
    modes[currentMode].brightness = intData[1];
    LEDS.setBrightness(intData[1]);
    modes[currentMode].speed = intData[2];
    modes[currentMode].scale = intData[3];
    COLOR_RED = intData[4];
    COLOR_GREEN = intData[5];
    COLOR_BLUE = intData[6];
    SAVE = intData[7];
    if (SAVE == 1)
    {
      LEDS.setBrightness(1);
      SaveEEPROM();
      LEDS.setBrightness(255);
      SAVE = 0;
      LEDS.setBrightness(modes[currentMode].brightness);
    }
    if (DEBUG_SP)
    {
      Serial.print("MODE = "); Serial.println(currentMode);
      Serial.print("Brightness = "); Serial.println(modes[currentMode].brightness);
      Serial.print("SPEED = "); Serial.println(modes[currentMode].speed);
      Serial.print("SCALE = "); Serial.println(modes[currentMode].scale);
      Serial.print("RED = "); Serial.println(COLOR_RED);
      Serial.print("GREEN = "); Serial.println(COLOR_GREEN);
      Serial.print("BLUE = "); Serial.println(COLOR_BLUE);
      Serial.print("SAVE = "); Serial.println(SAVE);
    }
    if (DEBUG_SP)
    {
      btSerial.print("MODE = "); btSerial.println(currentMode);
      btSerial.print("Brightness = "); btSerial.println(modes[currentMode].brightness);
      btSerial.print("SPEED = "); btSerial.println(modes[currentMode].speed);
      btSerial.print("SCALE = "); btSerial.println(modes[currentMode].scale);
      btSerial.print("RED = "); btSerial.println(COLOR_RED);
      btSerial.print("GREEN = "); btSerial.println(COLOR_GREEN);
      btSerial.print("BLUE = "); btSerial.println(COLOR_BLUE);
      btSerial.print("SAVE = "); btSerial.println(SAVE);
    }
  }
  effectsTick();

}


void effectsTick() {
  if (!recievedFlag) {
    if (ONflag && millis() - effTimer >= ((currentMode < 5) ? modes[currentMode].speed : 50) ) {
      effTimer = millis();
      switch (currentMode) {
        case 0: MODE_DEFAULT2(); //ЗАЛИВКА на всю кроме центральных двух цветом
          break;
        case 1: MODE_DEFAULT3(); //ЗАЛИВКА только крайних по одному цветом
          break;
        case 2: MODE_DEFAULT(); // ЗАЛИВКА на всю длинну цветом
          break;
        case 3: RANDOM_COLOR(); // ЗАЛИВКА рандомная каждый раз кроме центральных двух
          break;
        case 4: sparklesRoutine(); // конфетти
          break;
        case 5: random_burst(); //-m4-RANDOM INDEX/COLOR
          break;
        case 6: color_bounceFADE(); //-m6-BOUNCE COLOR (SIMPLE MULTI-LED FADE)
          break;
        case 7: ems_lightsONE();  //-m7-EMERGENCY LIGHTS (TWO COLOR SINGLE LED)
          break;
        case 8: ems_lightsALL(); //-m8-EMERGENCY LIGHTS (TWO COLOR SOLID)
          break;
        case 9: flicker(); //-m9-FLICKER EFFECT
          break;
        case 10: pulse_one_color_all();              //-m10-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR
          break;
        case 11: ems_lightsSTROBEX();                                    //-m26-EMERGENCY LIGHTS (STROBE LEFT/RIGHT ORANGE)
          break;
        case 12: ems_lightsSTROBE();                 //-m26-EMERGENCY LIGHTS (STROBE LEFT/RIGHT)
          break;
        case 13: rgb_propeller();                 //-m27-RGB PROPELLER
          break;
        case 14: Sparkle(0xff, 0xff, 0xff, l_thisdelay);
          break;
        case 15: SnowSparkle(0x10, 0x10, 0x10, l_thisdelay, random(100, 1000));
          break;
        case 16: Strobe(0xff, 0xff, 0xff, 10, l_thisdelay, 1000);
          break;
        case 17:  rainbow_loop();
          break;
        case 18: Fire(55, 120);
          break;
        case 19:  rainbowVertical();
          break;
        case 20: rainbowHorizontal();
          break;
      }
      FastLED.show();
    }
  }
}
/*
        case 7: rainbowVertical();
          break;
        case 8: rainbowHorizontal();
          break;
        case 9: colorsRoutine();
          break;
*/
boolean getStarted;
byte index;
String string_convert = "";

void parsingSP() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (getStarted) {
      if (incomingByte != ' ' && incomingByte != ';') {
        string_convert += incomingByte;
      } else {
        intData[index] = string_convert.toInt();
        string_convert = "";
        index++;
      }
    }
    if (incomingByte == '$') {
      getStarted = true;
      index = 0;
      string_convert = "";
    }
    if (incomingByte == ';') {
      getStarted = false;
      recievedFlag = true;
    }
  }
}
void parsingBT() {
  if (btSerial.available() > 0) {
    char incomingByte = btSerial.read();
    if (getStarted) {
      if (incomingByte != ' ' && incomingByte != ';') {
        string_convert += incomingByte;
      } else {
        intData[index] = string_convert.toInt();
        string_convert = "";
        index++;
      }
    }
    if (incomingByte == '$') {
      getStarted = true;
      index = 0;
      string_convert = "";
    }
    if (incomingByte == ';') {
      getStarted = false;
      recievedFlag = true;
    }
  }
}
