
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
float humidity = 80;
float temperature = 25;
//------------------------------
 
void setup() {
  Serial.begin(115200);
}
 
 
void loop() {
  readSensors();
  delay(1000);
}

void readSensors() {
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

    Serial.printf("Temperature: %.2f*C | Humidity: %.2f%\r\n", temperature, humidity);

    timeSinceLastRead = 0;
  }
  timeSinceLastRead += 1000;
}


