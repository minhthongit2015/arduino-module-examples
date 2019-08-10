#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter(0x23);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  Serial.println("Running…");
  delay(100);
}

void loop() {
    uint16_t lux = lightMeter.readLightLevel(); // Get Lux value
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println("lux");
    delay(1000);
}

