#include <Wire.h>
#include <BH1750.h>

/*** BH1750 ***/
BH1750 lightMeter(0x23);
#define SDA 4
#define SCL 5
/* ---------- */

/*** DHT22 ***/
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
float humidity = 80;
float temperature = 25;
/* --------- */

/*** HC_SR501 ***/
int sensorPin = 12;    // select the input pin for the potentiometer
int ledPin = 2;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
/* ------------ */

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA,SCL);
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

  
  pinMode(ledPin, OUTPUT);

  
  Serial.println("Running…");
  delay(100);
}

void loop() {
  static int i = 9999;

  if (i > 1000) {
    uint16_t lux = lightMeter.readLightLevel(); // Get Lux value
    Serial.printf("Light: %d lux\r\n", lux);
  
    readDHT22();
    i = 0;
  } else {
    i += 100;
  }

  sensorValue = analogRead(sensorPin);
  if (sensorValue) {
    Serial.printf("HC_SR501: %d\r\n", sensorValue);
    digitalWrite(ledPin, LOW); delay(sensorValue); digitalWrite(ledPin, HIGH);
  }
  
  delay(100);
}


void readDHT22() {
  static int timeSinceLastRead = 9999;
  if(timeSinceLastRead >= 2000) {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    Serial.printf("Temperature: %.2f°C | Humidity: %.2f%\r\n", temperature, humidity);

    timeSinceLastRead = 0;
  }
  timeSinceLastRead += 1000;
}


