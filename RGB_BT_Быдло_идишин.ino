// Для изменения цвета подойдет любое приложения для работы BT терминалом
// Пример изменения цвета   $255 0 255 1;
//                            R  G  B  |
//                                     |
//                                     V
//     0 - не сохраняет цвет, 1 - сохранит цвет в память
#include <Adafruit_NeoPixel.h>
#define BT_TX 3                   // не работает? поменять местами
#define BT_RX 2                   // не работает? поменять местами
#define PIN            6          // Пин ленты
#define NUMPIXELS      6         // Количество диодов в ленте
#define PARSE_AMOUNT 4

int ROM_COLOR_RED = 0;
int ROM_COLOR_GREEN = 1;
int ROM_COLOR_BLUE = 2;

int intData[PARSE_AMOUNT];
boolean recievedFlag;
byte color_RED, color_GREEN, color_BLUE, type_SAVE;

#include <EEPROM.h>
#include <SoftwareSerial.h>
SoftwareSerial btSerial(2, 3);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  pixels.begin();
  Serial.begin(38400);
  btSerial.begin(38400);
  color_RED = EEPROM.read(0);
  color_GREEN = EEPROM.read(1);
  color_BLUE = EEPROM.read(2);
  type_SAVE = EEPROM.read(3);
          if (type_SAVE == 0)
  {
      pixels.setPixelColor(0, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.setPixelColor(1, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      pixels.setPixelColor(4, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.setPixelColor(5, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.show();

  }
  if (type_SAVE == 1)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.show();
    }
  }
  if (type_SAVE == 2)
  {
    pixels.setPixelColor(0, pixels.Color(15, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 0, 15));
    pixels.setPixelColor(3, pixels.Color(0, 0, 15));
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.setPixelColor(5, pixels.Color(15, 0, 0));
    pixels.show();
  }
    if (type_SAVE == 3)
  {
    pixels.setPixelColor(0, pixels.Color(15, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 15, 0));
    pixels.setPixelColor(3, pixels.Color(0, 15, 0));
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.setPixelColor(5, pixels.Color(0, 0, 15));
    pixels.show();
  }
      if (type_SAVE == 4)
  {
    pixels.setPixelColor(0, pixels.Color(10, 10, 10));
    pixels.setPixelColor(1, pixels.Color(10, 10, 10));
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.setPixelColor(4, pixels.Color(10, 10, 10));
    pixels.setPixelColor(5, pixels.Color(10, 10, 10));
    pixels.show();
  }
        if (type_SAVE == 5)
  {
    pixels.setPixelColor(0, pixels.Color(0, 0, 15));
    pixels.setPixelColor(1, pixels.Color(5, 5, 10));
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.setPixelColor(4, pixels.Color(5, 5, 10));
    pixels.setPixelColor(5, pixels.Color(0, 0, 15));
    pixels.show();
  }

}

void loop() {
  parsing();
  if (recievedFlag) {
    recievedFlag = false;
    color_RED = intData[0];
    color_GREEN = intData[1];
    color_BLUE = intData[2];
    type_SAVE = intData[3];
    if (type_SAVE == 0)
    {

      EEPROM.write(0, color_RED);
      EEPROM.write(1, color_GREEN);
      EEPROM.write(2, color_BLUE);
      EEPROM.write(3, type_SAVE);
      pixels.setPixelColor(0, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.setPixelColor(1, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      pixels.setPixelColor(4, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.setPixelColor(5, pixels.Color(color_RED, color_GREEN, color_BLUE));
      pixels.show();
    }
    if (type_SAVE == 1)
    {

      EEPROM.write(0, color_RED);
      EEPROM.write(1, color_GREEN);
      EEPROM.write(2, color_BLUE);
      EEPROM.write(3, type_SAVE);
      char tbs[16];
      sprintf(tbs, "Цвет сохранен: %d %d %d", color_RED, color_GREEN, color_BLUE);
      btSerial.println(tbs);
      Serial.print(tbs);
      for (int i = 0; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255));
        pixels.show();
      }
      delay(5);
      type_SAVE = 1;
    }
    if (type_SAVE == 2)
    {
      EEPROM.write(3, type_SAVE);
    pixels.setPixelColor(0, pixels.Color(15, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 0, 15));
    pixels.setPixelColor(3, pixels.Color(0, 0, 15));
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.setPixelColor(5, pixels.Color(15, 0, 0));
    pixels.show();
      
    }
        if (type_SAVE == 3)
    {
      EEPROM.write(3, type_SAVE);
    pixels.setPixelColor(0, pixels.Color(15, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 15, 0));
    pixels.setPixelColor(3, pixels.Color(0, 15, 0));
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.setPixelColor(5, pixels.Color(0, 0, 15));
    pixels.show();
      
    }
            if (type_SAVE == 4)
    {
      EEPROM.write(3, type_SAVE);
    pixels.setPixelColor(0, pixels.Color(10, 10, 10));
    pixels.setPixelColor(1, pixels.Color(10, 10, 10));
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.setPixelColor(4, pixels.Color(10, 10, 10));
    pixels.setPixelColor(5, pixels.Color(10, 10, 10));
    pixels.show();
      
    }
                if (type_SAVE == 5)
    {
      EEPROM.write(3, type_SAVE);
    pixels.setPixelColor(0, pixels.Color(0, 0, 15));
    pixels.setPixelColor(1, pixels.Color(5, 5, 10));
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.setPixelColor(4, pixels.Color(5, 5, 10));
    pixels.setPixelColor(5, pixels.Color(0, 0, 15));
    pixels.show();
      
    }
  }


}
// this code by AlexGyver
boolean getStarted;
byte index;
String string_convert = "";
void parsing() {
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

