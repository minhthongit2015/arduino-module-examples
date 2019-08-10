#include <Arduino.h>
#line 1 "k:\\Arrows\\Raspberry Pi Tutorial\\Modules\\TTP229\\TTP229.ino"
#line 1 "k:\\Arrows\\Raspberry Pi Tutorial\\Modules\\TTP229\\TTP229.ino"
#include <TTP229.h>

const int SCL_PIN = 10; // The pin number of the clock pin.
const int SDO_PIN = 9;  // The pin number of the data pin.

TTP229 ttp229(SCL_PIN, SDO_PIN); // TTP229(sclPin, sdoPin)

#line 8 "k:\\Arrows\\Raspberry Pi Tutorial\\Modules\\TTP229\\TTP229.ino"
void setup();
#line 14 "k:\\Arrows\\Raspberry Pi Tutorial\\Modules\\TTP229\\TTP229.ino"
void loop();
#line 8 "k:\\Arrows\\Raspberry Pi Tutorial\\Modules\\TTP229\\TTP229.ino"
void setup()
{
  Serial.begin(9600);
  Serial.println("Start Touching One Key At a Time!");
}

void loop()
{
  // uint8_t key = ttp229.ReadKey16(); // Blocking
  // if (key)
  //   Serial.println(key);

  static uint8_t oldKey = 0;
  static unsigned long last = millis();
  uint8_t key = ttp229.GetKey16(); // Non Blocking
  unsigned long dif = millis() - last;
  if (key && dif > 100 && (key != oldKey || dif > 200))
  {
    last = millis();
    Serial.println(key);
  }
}
