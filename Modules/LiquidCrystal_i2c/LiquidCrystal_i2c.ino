/*
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 * YWROBOT
 *Compatible with the Arduino IDE 1.0
 *Library version:1.1
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd2(0x27,16,2);  // 

void i2cScanner();

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello!");

  // LCD scan
  i2cScanner();

  // LCD 20x4
  lcd.begin();
  lcd.backlight(); delay(1000);
  lcd.noBacklight(); delay(1000);
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("Hello!");
  lcd.cursor();
  lcd.blink();

  // LCD 16x2
  lcd2.begin();
  lcd2.backlight(); delay(500);
  lcd2.noBacklight(); delay(500);
  lcd2.backlight();
  
  lcd2.setCursor(0,0);
  lcd2.print("Hello!");
  lcd2.cursor();
  lcd2.blink();
}

void loop()
{
  if (Serial.available()) {
    delay(100);
    lcd.clear();
//    lcd2.clear();
    while (Serial.available() > 0) {
      char c = Serial.read();
      if (c != '\n' && c != '\r')
        lcd.write(c);
      Serial.print(c);
    }
  }
}

void i2cScanner()
{
  Wire.begin();
  byte error, address;
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
    }  else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address,HEX);
    }
  }
}

