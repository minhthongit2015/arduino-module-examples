#include <Wire.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>

WiFiClient myEsp;
const char* ssid = "Wall-E";
const char* password = "Wastebasket";
const char* host = "192.168.1.146"; // Địa chỉ của Raspi
int port = 1234;                    // Port lắng nghe kết nối của chương trình server chạy trên raspi

BH1750 lightMeter;

void setup() {
  Serial.begin(115200);
  
  /// ---------------------------------------- Wifi
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(500);
  }
  Serial.println(String("\r\nConnected to ") + ssid);
  Serial.println(String("IP address: ") + WiFi.localIP());
  
  /// ---------------------------------------- Connect to server
  
  while (!myEsp.connect(host, port)) {
    Serial.print("."); delay(500);
  }
  Serial.println(String("Connected to ") + host + ":" + port);
  
  /// ---------------------------------------- Sensors
  
  Wire.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  
  
  /// ----------------------------------------
  
  Serial.println("Running…");
}

void loop() {
    uint16_t lux = lightMeter.readLightLevel(); // Get Lux value
    Serial.printf("Light: %d lux\r\n", lux);
    
    myEsp.printf("Light: %d lux\r\n", lux);
    
    delay(1000);
}
