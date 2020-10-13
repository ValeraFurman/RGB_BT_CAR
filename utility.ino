void LoadEEPROM()
{
  currentMode = EEPROM.read(0);
  modes[currentMode].brightness = EEPROM.read(1);
  modes[currentMode].speed = EEPROM.read(2);
  modes[currentMode].scale = EEPROM.read(3);
  COLOR_RED = EEPROM.read(4);
  COLOR_GREEN = EEPROM.read(5);
  COLOR_BLUE = EEPROM.read(6);
  BRIGHTNESS = modes[currentMode].brightness;
  if (DEBUG_SP)
  {
    Serial.print(" LOAD START ====    MODE = "); Serial.println(EEPROM.read(0));
    Serial.print("Brightness = "); Serial.println(EEPROM.read(1));
    Serial.print("SPEED = "); Serial.println(EEPROM.read(2));
    Serial.print("SCALE = "); Serial.println(EEPROM.read(3));
    Serial.print("RED = "); Serial.println(EEPROM.read(4));
    Serial.print("GREEN = "); Serial.println(EEPROM.read(5));
    Serial.print("BLUE = "); Serial.println(EEPROM.read(6)); Serial.print(" LOAD END =====");

  }
  if (DEBUG_BT)
  {
    btSerial.print(" LOAD START ====    MODE = "); btSerial.println(EEPROM.read(0));
    btSerial.print("Brightness = "); btSerial.println(EEPROM.read(1));
    btSerial.print("SPEED = "); btSerial.println(EEPROM.read(2));
    btSerial.print("SCALE = "); btSerial.println(EEPROM.read(3));
    btSerial.print("RED = "); btSerial.println(EEPROM.read(4));
    btSerial.print("GREEN = "); btSerial.println(EEPROM.read(5));
    btSerial.print("BLUE = "); btSerial.println(EEPROM.read(6)); btSerial.print(" LOAD END =====");

  }
}
void SaveEEPROM()
{
  EEPROM.write(0, currentMode);
  EEPROM.write(1, modes[currentMode].brightness);
  EEPROM.write(2, modes[currentMode].speed);
  EEPROM.write(3, modes[currentMode].scale);
  EEPROM.write(4, COLOR_RED);
  EEPROM.write(5, COLOR_GREEN);
  EEPROM.write(6, COLOR_BLUE);
  if (DEBUG_SP)
  {
    Serial.print(" TEST SAVE ======  START MODE = "); Serial.println(EEPROM.read(0));
    Serial.print("Brightness = "); Serial.println(EEPROM.read(1));
    Serial.print("SPEED = "); Serial.println(EEPROM.read(2));
    Serial.print("SCALE = "); Serial.println(EEPROM.read(3));
    Serial.print("RED = "); Serial.println(EEPROM.read(4));
    Serial.print("GREEN = "); Serial.println(EEPROM.read(5));
    Serial.print("BLUE = "); Serial.println(EEPROM.read(6)); Serial.print(" TEST SAVE  =====  END");

  }
    if (DEBUG_BT)
  {
    btSerial.print(" TEST SAVE ======  START MODE = "); btSerial.println(EEPROM.read(0));
    btSerial.print("Brightness = "); btSerial.println(EEPROM.read(1));
    btSerial.print("SPEED = "); btSerial.println(EEPROM.read(2));
    btSerial.print("SCALE = "); btSerial.println(EEPROM.read(3));
    btSerial.print("RED = "); btSerial.println(EEPROM.read(4));
    btSerial.print("GREEN = "); btSerial.println(EEPROM.read(5));
    btSerial.print("BLUE = "); btSerial.println(EEPROM.read(6)); btSerial.print(" TEST SAVE  =====  END");

  }
}

// служебные функции
void setPixel(int Pixel, byte red, byte green, byte blue) {
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < WIDTH; i++ ) {
    setPixel(i, red, green, blue);
  }
  FastLED.show();
}
// залить все
void fillAll(CRGB color) {
  for (int i = 0; i < WIDTH; i++) {
    leds[i] = color;
  }
}
//---FIND INDEX OF HORIZONAL OPPOSITE LED
int horizontal_index(int i) {
  //-ONLY WORKS WITH INDEX < TOPINDEX
  if (i == BOTTOM_INDEX) {
    return BOTTOM_INDEX;
  }
  if (i == TOP_INDEX && EVENODD == 1) {
    return TOP_INDEX + 1;
  }
  if (i == TOP_INDEX && EVENODD == 0) {
    return TOP_INDEX;
  }
  return WIDTH - i;
}
void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < WIDTH; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}
// функция отрисовки точки по координатам X Y
void drawPixelXY(int8_t x, int8_t y, CRGB color) {
  if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1) return;
  int thisPixel = getPixelNumber(x, y) * SEGMENTS;
  for (byte i = 0; i < SEGMENTS; i++) {
    leds[thisPixel + i] = color;
  }
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(int thisSegm) {
  int thisPixel = thisSegm * SEGMENTS;
  if (thisPixel < 0 || thisPixel > NUM_LEDS - 1) return 0;
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(int8_t x, int8_t y) {
  return getPixColor(getPixelNumber(x, y));
}

// **************** НАСТРОЙКА МАТРИЦЫ ****************
#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y x

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y x

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y (WIDTH - x - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y y

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y (WIDTH - x - 1)

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif

// получить номер пикселя в ленте по координатам
uint16_t getPixelNumber(int8_t x, int8_t y) {
  if ((THIS_Y % 2 == 0) || MATRIX_TYPE) {               // если чётная строка
    return (THIS_Y * _WIDTH + THIS_X);
  } else {                                              // если нечётная строка
    return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
  }
}
int adjacent_cw(int i) {
  int r;
  if (i < WIDTH - 1) {
    r = i + 1;
  }
  else {
    r = 0;
  }
  return r;
}
//---FIND ADJACENT INDEX COUNTER-CLOCKWISE
int adjacent_ccw(int i) {
  int r;
  if (i > 0) {
    r = i - 1;
  }
  else {
    r = WIDTH - 1;
  }
  return r;
}
int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % WIDTH;
  }
  return iN;
}
