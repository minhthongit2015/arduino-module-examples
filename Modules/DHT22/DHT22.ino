/* Thông số kỹ thuật

- Điện áp hoạt động: 5VDC
- Dãi độ ẩm hoạt động: 0% - 100% RH, sai số ±2%RH
- Dãi nhiệt độ hoạt động: -40°C ~ 80°C, sai số ±0.5°C
- Không cần thêm linh kiện ngoài
- Hoạt động với độ ổn định lâu dài
*/

#include "DHT.h"

// #define D6 12
#define DHTPIN D6
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
float humidity = 80;
float temperature = 25;

 
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  dht.begin();
}

void loop() {
  readSensors();
  delay(1000);
}

void readSensors() {
  static int last = 9999;
  if (last >= 2000) {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      last = 0;
      return;
    }

    Serial.printf("Temperature: %.2f*C | Humidity: %.2f%\r\n", temperature, humidity);

    last = 0;
  }
  last += 1000;
}


