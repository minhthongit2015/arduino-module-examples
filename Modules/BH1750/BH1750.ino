#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter(0x23);

#define SDA 4
#define SCL 5
void setup() {
  Serial.begin(115200);
  Wire.begin(SDA,SCL);
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  Serial.println("Running…");
  delay(100);
}

void loop() {
    uint16_t lux = lightMeter.readLightLevel(); // Get Lux value
    Serial.printf("Light: %d lux\r\n", lux);
    delay(1000);
}

