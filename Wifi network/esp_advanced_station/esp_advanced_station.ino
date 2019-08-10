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
WiFiClient client;

const char* ssid = "Wall-E";
const char* password = "Wastebasket";

const char* server = "www.insys.vn"; // Địa chỉ của Cloud server

// built-in led
int pin = 2;
 
void setup() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Kết nối vào mạng wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
 
void loop() {
  static char buffer[100];

  static int last_sync = 9999;
  if (last_sync > 5000) {
    sprintf(buffer, "GET /cloud?temp=%.2f&humi=%.0f HTTP/1.1", temperature, humidity);
    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      // Make a HTTP request:
      client.println(buffer);
      client.println("Host: www.insys.vn");
      client.println();
    }
    last_sync = 0;
  } else {
    last_sync += 1000;
  }
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
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
    Serial.print(" *C\n");

    timeSinceLastRead = 0;
  }
  timeSinceLastRead += 1000;
}



