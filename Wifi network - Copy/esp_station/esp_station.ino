#include <ESP8266WiFi.h>
#include <string.h>

/// Thư viện và một số biến dùng cho cảm biến
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
float humidity = 80;
float temperature = 25;
//------------------------------

// Thiết đặt kết nối đến server trên raspi
WiFiClient myEsp;

const char* ssid = "ESP Soft Access Point";
const char* password = "123456789";

const char* host = "192.168.4.4";   // Địa chỉ của Raspi
int port = 80;                      // Port lắng nghe kết nối của chương trình server chạy trên raspi

// built-in led
int builtinLED = 2;
 
void setup() {
  pinMode(builtinLED, OUTPUT);
  digitalWrite(builtinLED, 1); // Tắt LED

  // Bắt đầu kết nối tới AP
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Đợi kết nối đến AP
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Hiển thị kết quả kết nối đến AP
  Serial.print("\r\nConnected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  /// Bắt đầu tới client - server

  // Kết nối đến server trên raspi
  while (!myEsp.connect(host, port)) {
    Serial.print(".");
    delay(500);
  }
  // Thông báo kết quả kết nối
  Serial.print("Connected to ");
  Serial.println(host);
}
 
 
void loop() {

  // Gửi dữ liệu tới server
  myEsp.printf("%.2f,%.0f\n\r", temperature, humidity);

  // Kiểm tra Server có gửi gì không, có thì xử lý, k thì bỏ qua
  if (myEsp.available())
  {
    String line = myEsp.readStringUntil('\n');
    Serial.println(line);
    digitalWrite(builtinLED, 0);
    delay(1000);
    digitalWrite(builtinLED, 1);
  }

  readSensors();
  delay(1000);
}

void readSensors() {
  static int timeSinceLastRead = 9999;
  if(timeSinceLastRead > 2000) {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C ");

    timeSinceLastRead = 0;
  }
  timeSinceLastRead += 1000;
}

