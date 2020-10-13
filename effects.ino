
void ALL_GENERATE()
{
  if (DEBUG_SP)
  {
    R_RED = random(1, 255);
    R_GREEN = random(1, 255);
    R_BLUE = random(1, 255);
    Serial.println(R_RED);
    Serial.println(R_GREEN);
    Serial.println(R_BLUE);
  }
  if (DEBUG_BT)
  {
    R_RED = random(1, 255);
    R_GREEN = random(1, 255);
    R_BLUE = random(1, 255);
    btSerial.println(R_RED);
    btSerial.println(R_GREEN);
    btSerial.println(R_BLUE);
  }
}

// ================================= ЭФФЕКТЫ ====================================







// --------------------------------- ЗАЛИВКА на всю длинну цветом ------------------------------------
void MODE_DEFAULT() {
  for (byte j = 0; j < HEIGHT; j++) {
    for (byte i = 0; i < WIDTH; i++)
      drawPixelXY(i, j, CRGB( COLOR_RED, COLOR_GREEN, COLOR_BLUE));

  }
  LEDS.setBrightness(modes[currentMode].brightness);
}
// --------------------------------- ЗАЛИВКА на всю кроме центральных двух ------------------------------------
void MODE_DEFAULT2() {
  drawPixelXY(0, 0, CRGB( COLOR_RED, COLOR_GREEN, COLOR_BLUE));
  drawPixelXY(1, 0, CRGB( COLOR_RED, COLOR_GREEN, COLOR_BLUE));
  drawPixelXY(2, 0, CRGB( 0, 0, 0));
  drawPixelXY(3, 0, CRGB( 0, 0, 0));
  drawPixelXY(4, 0, CRGB( COLOR_RED, COLOR_GREEN, COLOR_BLUE));
  drawPixelXY(5, 0, CRGB( COLOR_RED, COLOR_GREEN, COLOR_BLUE));
  LEDS.setBrightness(modes[currentMode].brightness);
}
// --------------------------------- ЗАЛИВКА только крайних по одному ------------------------------------
void MODE_DEFAULT3() {
  drawPixelXY(0, 0, CRGB( COLOR_RED, COLOR_GREEN, COLOR_BLUE));
  drawPixelXY(1, 0, CRGB::Black);
  drawPixelXY(2, 0, CRGB::Black);
  drawPixelXY(3, 0, CRGB::Black);
  drawPixelXY(4, 0, CRGB::Black);
  drawPixelXY(5, 0, CRGB( COLOR_RED, COLOR_GREEN, COLOR_BLUE));
  LEDS.setBrightness(modes[currentMode].brightness);
}
// --------------------------------- ЗАЛИВКА рандомная каждый раз кроме центральных двух ------------------------------------

void RANDOM_COLOR() {

  drawPixelXY(0, 0, CRGB( R_RED, R_GREEN, R_BLUE));
  drawPixelXY(1, 0, CRGB( R_RED, R_GREEN, R_BLUE));
  drawPixelXY(2, 0, CRGB( 0, 0, 0));
  drawPixelXY(3, 0, CRGB( 0, 0, 0));
  drawPixelXY(4, 0, CRGB( R_RED, R_GREEN, R_BLUE));
  drawPixelXY(5, 0, CRGB( R_RED, R_GREEN, R_BLUE));
  LEDS.setBrightness(modes[currentMode].brightness);
}
void Fire(int Cooling, int Sparking) {
  static byte heat[WIDTH];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < WIDTH; i++) {
    cooldown = random(0, ((Cooling * 10) / WIDTH) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = WIDTH - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < WIDTH; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  FastLED.show();
  delay(modes[currentMode].speed);
}
void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}
void rainbowCycle() {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 1; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < WIDTH; i++) {
      c = Wheel(((i * 256 / WIDTH) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    FastLED.show();
    delay(1);
  }
}
byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}
void rainbow_loop() {                        //-m3-LOOP HSV RAINBOW
  l_idex++;
  l_ihue = l_ihue + l_thisstep;
  if (l_idex >= WIDTH) {
    l_idex = 0;
  }
  if (l_ihue > 255) {
    l_ihue = 0;
  }
  leds[l_idex] = CHSV(l_ihue, l_thissat, 255);
  LEDS.show();
  delay(modes[currentMode].speed);
}
void random_burst() {                         //-m4-RANDOM INDEX/COLOR
  l_idex = random(0, WIDTH);
  l_ihue = random(0, 255);
  leds[l_idex] = CHSV(l_ihue, l_thissat, 255);
  LEDS.show();
  delay(modes[currentMode].speed);
}
void color_bounceFADE() {                    //-m6-BOUNCE COLOR (SIMPLE MULTI-LED FADE)
  if (bouncedirection == 0) {
    l_idex = l_idex + 1;
    if (l_idex == WIDTH) {
      bouncedirection = 1;
      l_idex = l_idex - 1;
    }
  }
  if (bouncedirection == 1) {
    l_idex = l_idex - 1;
    if (l_idex == 0) {
      bouncedirection = 0;
    }
  }
  int iL1 = adjacent_cw(l_idex);
  int iL2 = adjacent_cw(iL1);
  int iL3 = adjacent_cw(iL2);
  int iR1 = adjacent_ccw(l_idex);
  int iR2 = adjacent_ccw(iR1);
  int iR3 = adjacent_ccw(iR2);
  for (int i = 0; i < WIDTH; i++ ) {
    if (i == l_idex) {
      leds[i] = CHSV(l_thishue, l_thissat, 255);
    }
    else if (i == iL1) {
      leds[i] = CHSV(l_thishue, l_thissat, 150);
    }
    else if (i == iL2) {
      leds[i] = CHSV(l_thishue, l_thissat, 80);
    }
    else if (i == iL3) {
      leds[i] = CHSV(l_thishue, l_thissat, 20);
    }
    else if (i == iR1) {
      leds[i] = CHSV(l_thishue, l_thissat, 150);
    }
    else if (i == iR2) {
      leds[i] = CHSV(l_thishue, l_thissat, 80);
    }
    else if (i == iR3) {
      leds[i] = CHSV(l_thishue, l_thissat, 20);
    }
    else {
      leds[i] = CHSV(0, 0, 0);
    }
  }
  LEDS.show();
  delay(modes[currentMode].speed);
}
void ems_lightsONE() {                    //-m7-EMERGENCY LIGHTS (TWO COLOR SINGLE LED)
  l_idex++;
  if (l_idex >= WIDTH) {
    l_idex = 0;
  }
  int idexR = l_idex;
  int idexB = antipodal_index(idexR);
  int thathue = (l_thishue + 160) % 255;
  for (int i = 0; i < WIDTH; i++ ) {
    if (i == idexR) {
      leds[i] = CHSV(l_thishue, l_thissat, 255);
    }
    else if (i == idexB) {
      leds[i] = CHSV(thathue, l_thissat, 255);
    }
    else {
      leds[i] = CHSV(0, 0, 0);
    }
  }
  LEDS.show();
  delay(modes[currentMode].speed);
}
void ems_lightsALL() {                  //-m8-EMERGENCY LIGHTS (TWO COLOR SOLID)
  l_idex++;
  if (l_idex >= WIDTH) {
    l_idex = 0;
  }
  int idexR = l_idex;
  int idexB = antipodal_index(idexR);
  int thathue = (l_thishue + 160) % 255;
  leds[idexR] = CHSV(l_thishue, l_thissat, 255);
  leds[idexB] = CHSV(thathue, l_thissat, 255);
  LEDS.show();
  delay(modes[currentMode].speed);
}

void flicker() {                          //-m9-FLICKER EFFECT
  int random_bright = random(0, 255);
  int random_delay = random(10, 100);
  int random_bool = random(0, random_bright);
  if (random_bool < 10) {
    for (int i = 0 ; i < WIDTH; i++ ) {
      leds[i] = CHSV(l_thishue, l_thissat, random_bright);
    }
    LEDS.show();
    delay(modes[currentMode].speed);
  }
}
void pulse_one_color_all() {              //-m10-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR
  if (bouncedirection == 0) {
    ibright++;
    if (ibright >= 255) {
      bouncedirection = 1;
    }
  }
  if (bouncedirection == 1) {
    ibright = ibright - 1;
    if (ibright <= 1) {
      bouncedirection = 0;
    }
  }
  for (int idex = 0 ; idex < WIDTH; idex++ ) {
    leds[idex] = CHSV(l_thishue, l_thissat, ibright);
  }
  LEDS.show();
  delay(modes[currentMode].speed);
}
void flame() {                                    //-m22-FLAMEISH EFFECT
  int idelay = random(0, 35);
  float hmin = 0.1; float hmax = 45.0;
  float hdif = hmax - hmin;
  int randtemp = random(0, 3);
  float hinc = (hdif / float(TOP_INDEX)) + randtemp;
  int l_ihue = hmin;
  for (int i = 0; i <= TOP_INDEX; i++ ) {
    l_ihue = l_ihue + hinc;
    leds[i] = CHSV(l_ihue, l_thissat, 255);
    int ih = horizontal_index(i);
    leds[ih] = CHSV(l_ihue, l_thissat, 255);
    leds[TOP_INDEX].r = 255; leds[TOP_INDEX].g = 255; leds[TOP_INDEX].b = 255;
    LEDS.show();
    delay(modes[currentMode].speed);
  }
}
void ems_lightsSTROBE() {                  //-m26-EMERGENCY LIGHTS (STROBE LEFT/RIGHT)
  int thishue = 0;
  int thathue = (thishue + 160) % 255;
  for (int x = 0 ; x < 5; x++ ) {
    for (int i = 0 ; i < TOP_INDEX; i++ ) {
      leds[i] = CHSV(thishue, l_thissat, 255);
    }
    LEDS.show(); delay(modes[currentMode].speed);
    one_color_all(0, 0, 0);
    LEDS.show(); delay(modes[currentMode].speed);
  }
  for (int x = 0 ; x < 5; x++ ) {
    for (int i = TOP_INDEX ; i < WIDTH; i++ ) {
      leds[i] = CHSV(thathue, l_thissat, 255);
    }
    LEDS.show(); delay(modes[currentMode].speed);
    one_color_all(0, 0, 0);
    LEDS.show(); delay(modes[currentMode].speed);
  }
}
void ems_lightsSTROBEX() {                  //-m26-EMERGENCY LIGHTS (STROBE LEFT/RIGHT ORANGE) 2
  int thishue = 0;
  int thathue = (thishue + 160) % 255;
  for (int x = 0 ; x < 5; x++ ) {
    for (int i = 0 ; i < TOP_INDEX; i++ ) {
      leds[i] = CHSV(15, l_thissat, 255);
    }
    LEDS.show(); delay(modes[currentMode].speed);
    one_color_all(0, 0, 0);
    LEDS.show(); delay(modes[currentMode].speed);
  }
  for (int x = 0 ; x < 5; x++ ) {
    for (int i = TOP_INDEX ; i < WIDTH; i++ ) {
      leds[i] = CHSV(15, l_thissat, 255);
    }
    LEDS.show(); delay(modes[currentMode].speed);
    one_color_all(0, 0, 0);
    LEDS.show(); delay(modes[currentMode].speed);
  }
}
void rgb_propeller() {                           //-m27-RGB PROPELLER
  l_idex++;
  int ghue = (l_thishue + 80) % 255;
  int bhue = (l_thishue + 160) % 255;
  int N3  = int(WIDTH / 3);
  int N6  = int(WIDTH / 6);
  int N12 = int(WIDTH / 12);
  for (int i = 0; i < N3; i++ ) {
    int j0 = (l_idex + i + WIDTH - N12) % WIDTH;
    int j1 = (j0 + N3) % WIDTH;
    int j2 = (j1 + N3) % WIDTH;
    leds[j0] = CHSV(l_thishue, l_thissat, 255);
    leds[j1] = CHSV(ghue, l_thissat, 255);
    leds[j2] = CHSV(bhue, l_thissat, 255);
  }
  LEDS.show();
  delay(modes[currentMode].speed);
}
//-------------------------------Sparkle---------------------------------------
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(WIDTH);
  setPixel(Pixel, red, green, blue);
  FastLED.show();
  delay(SpeedDelay);
  setPixel(Pixel, 0, 0, 0);
}
//-------------------------------SnowSparkle---------------------------------------
void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red, green, blue);

  int Pixel = random(WIDTH);
  setPixel(Pixel, 0xff, 0xff, 0xff);
  FastLED.show();
  delay(SparkleDelay);
  setPixel(Pixel, red, green, blue);
  FastLED.show();
  delay(SpeedDelay);
}
//-------------------------------Strobe---------------------------------------
void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause) {
  for (int j = 0; j < StrobeCount; j++) {
    setAll(red, green, blue);
    FastLED.show();
    delay(FlashDelay);
    setAll(0, 0, 0);
    FastLED.show();
    delay(FlashDelay);
  }

  delay(EndPause);
}
// --------------------------------- конфетти ------------------------------------
void sparklesRoutine() {
  for (byte i = 0; i < modes[0].scale; i++) {
    byte x = random(0, WIDTH);
    byte y = random(0, HEIGHT);
    if (getPixColorXY(x, y) == 0)
      leds[getPixelNumber(x, y)] = CHSV(random(0, 255), 255, 255);
  }
  fader(70);
}

// функция плавного угасания цвета для всех пикселей
void fader(byte step) {
  for (byte i = 0; i < WIDTH; i++) {
    for (byte j = 0; j < HEIGHT; j++) {
      fadePixel(i, j, step);
    }
  }
}
void fadePixel(byte i, byte j, byte step) {     // новый фейдер
  int pixelNum = getPixelNumber(i, j);
  if (getPixColor(pixelNum) == 0) return;

  if (leds[pixelNum].r >= 30 ||
      leds[pixelNum].g >= 30 ||
      leds[pixelNum].b >= 30) {
    leds[pixelNum].fadeToBlackBy(step);
  } else {
    leds[pixelNum] = 0;
  }
}

// -------------------------------------- огонь ---------------------------------------------
// эффект "огонь"
#define SPARKLES 1        // вылетающие угольки вкл выкл
unsigned char line[WIDTH];
int pcnt = 0;

//these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM = {
  {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
  {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
};

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
const unsigned char hueMask[8][16] PROGMEM = {
  {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 },
  {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 },
  {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};

void fireRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    //FastLED.clear();
    generateLine();
  }
  if (pcnt >= 100) {
    shiftUp();
    generateLine();
    pcnt = 0;
  }
  drawFrame(pcnt);
  pcnt += 30;
}

// Randomly generate the next line (matrix row)

void generateLine() {
  for (uint8_t x = 0; x < WIDTH; x++) {
    line[x] = random(64, 255);
  }
}

void shiftUp() {
  for (uint8_t y = HEIGHT - 1; y > 0; y--) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y > 7) continue;
      matrixValue[y][newX] = matrixValue[y - 1][newX];
    }
  }

  for (uint8_t x = 0; x < WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    matrixValue[0][newX] = line[newX];
  }
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

void drawFrame(int pcnt) {
  int nextv;

  //each row interpolates with the one before it
  for (unsigned char y = HEIGHT - 1; y > 0; y--) {
    for (unsigned char x = 0; x < WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y < 8) {
        nextv =
          (((100.0 - pcnt) * matrixValue[y][newX]
            + pcnt * matrixValue[y - 1][newX]) / 100.0)
          - pgm_read_byte(&(valueMask[y][newX]));

        CRGB color = CHSV(
                       modes[1].scale * 2.5 + pgm_read_byte(&(hueMask[y][newX])), // H
                       255, // S
                       (uint8_t)max(0, nextv) // V
                     );

        leds[getPixelNumber(x, y)] = color;
      } else if (y == 8 && SPARKLES) {
        if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0) drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else drawPixelXY(x, y, 0);
      } else if (SPARKLES) {

        // старая версия для яркости
        if (getPixColorXY(x, y - 1) > 0)
          drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else drawPixelXY(x, y, 0);

      }
    }
  }

  //first row interpolates with the "next" line
  for (unsigned char x = 0; x < WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    CRGB color = CHSV(
                   modes[1].scale * 2.5 + pgm_read_byte(&(hueMask[0][newX])), // H
                   255,           // S
                   (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
                 );
    leds[getPixelNumber(newX, 0)] = color;
  }
}

byte hue;
// ---------------------------------------- радуга ------------------------------------------
void rainbowVertical() {
  hue += 2;
  for (byte j = 0; j < HEIGHT; j++) {
    CHSV thisColor = CHSV((byte)(hue + j * modes[2].scale), 255, 255);
    for (byte i = 0; i < WIDTH; i++)
      drawPixelXY(i, j, thisColor);
  }
}
void rainbowHorizontal() {
  hue += 2;
  for (byte i = 0; i < WIDTH; i++) {
    CHSV thisColor = CHSV((byte)(hue + i * modes[3].scale), 255, 255);
    for (byte j = 0; j < HEIGHT; j++)
      drawPixelXY(i, j, thisColor);   //leds[getPixelNumber(i, j)] = thisColor;
  }
}

// ---------------------------------------- ЦВЕТА ------------------------------------------
void colorsRoutine() {
  hue += modes[4].scale;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
  }
}
