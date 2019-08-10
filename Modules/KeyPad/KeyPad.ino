#include <Keypad.h>

#define numRows 4
#define numCols 4

byte keymap[numRows][numCols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {9,8,7,6}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columns 0 to 3

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

int myLed = A0;

void setup() {
  Serial.begin(9600);

  pinMode(myLed, OUTPUT);
}

int brightness = 0;
int fadeAmount = 5;

void loop() {
//  static byte val = 0;
//  char keypressed = myKeypad.getKey();
//  
//  if (keypressed != NO_KEY) {
//    val = (keypressed - '1') *(255/9);
//    Serial.print(keypressed); Serial.print(" -> "); Serial.println(val);
//  }
//  analogWrite(myLed, val);
//
//  delay(30);

  analogWrite(myLed, brightness);
  brightness = brightness + fadeAmount;
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  delay(30); 
}
