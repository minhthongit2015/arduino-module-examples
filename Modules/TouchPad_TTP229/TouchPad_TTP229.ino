
#include <Arduino.h>
#include <HardwareSerial.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define SCL 5
#define SDA 4
#define CHECK_BYTE '\xff'
#include <SocketIoClient.h>
SocketIoClient webSocket;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include <TTP229.h>

const int SCL_PIN = D3;
const int SDO_PIN = D4;

TTP229 ttp229(SCL_PIN, SDO_PIN); // TTP229(sclPin, sdoPin)

void setup()
{
  Serial.begin(115200);
  Serial.println("Start Touching One Key At a Time!");
  #ifdef ARDUINO
    Serial.println(ARDUINO);
  #endif
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

  // Serial.print("state: "); Serial.println(key);

  if (key && dif > 100 && (key != oldKey || dif > 200))
  {
    last = millis();
    Serial.println(key);
  }
  oldKey = key;
}