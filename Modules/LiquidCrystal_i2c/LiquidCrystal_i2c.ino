/*
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 * YWROBOT
 *Compatible with the Arduino IDE 1.0
 *Library version:1.1
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x20 for a 16 chars and 2 line display

#define SCL 5
#define SDA 4

void i2cScanner();
void scrollText(int row, String message, int delayTime, int lcdColumns);

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Hello!");

  // LCD scan
  i2cScanner();

  // LCD 20x4
  lcd.init();
  lcd.backlight(); delay(1000);
  lcd.noBacklight(); delay(1000);
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("Hello!");
  lcd.cursor();
  lcd.blink();
}

void loop()
{
  static int rowIndex = 0;
  if (Serial.available()) {
    lcd.clear();
    lcd.setCursor(0, rowIndex % 4);
    while (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '\n') rowIndex++;
      if (c != '\n' && c != '\r')
        lcd.write(c);
    }
  }
}

void i2cScanner()
{
  Serial.println("Start scanning I2C...");
  Wire.begin(SDA, SCL);
  byte error, address;
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  Serial.println("Scanning I2C done!");
}

void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}
