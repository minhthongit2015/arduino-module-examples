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

/// Thiết đặt mở server
#define PORT 80
WiFiServer myServer(PORT);

const char* ssid = "Wall-E";
const char* password = "Wastebasket";

String htmlPage =
    String("HTTP/1.1 200 OK\r\n") +
          "Content-Type: text/html\r\n" +
          "Connection: close\r\n" +  // the connection will be closed after completion of the response
          "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
          "\r\n" +
          "<!DOCTYPE HTML>" +
          "<html><meta charset=utf-8>" +
          "<div style='font-size: 50px; color: #b66e15; font-family:\"Segoe UI\", sans-serif'>Temperature: %.2f°C</div>" +
          "<div style='font-size: 50px; color: #b66e15; font-family:\"Segoe UI\", sans-serif'>Humidity: %.2f %%</div>" +
          "</html>" +
          "\r\n";
char htmlPageBuf[1024];

// Built-in led
int pin = 2;
 
void setup() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  myServer.begin();
  Serial.print("Server is listening on ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(PORT);
}
 
 
void loop() {
  // Đợi kết nối từ client
  WiFiClient client = myServer.available();

  if (client) {
    Serial.println("\r\n[[ Client connected ]]");
    digitalWrite(pin, 0);

    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);

        if (line.length() == 1 && line[0] == '\n') {
          htmlPage.toCharArray(htmlPageBuf, sizeof(htmlPageBuf));
          client.printf(htmlPageBuf, temperature, humidity); // Gửi trả lại nội dung
          break;
        }
      }
    }
  }

  readSensors();
  delay(1000);
}

void readSensors() {
  static int timeSinceLastRead = 0;
  if(timeSinceLastRead > 9999) {
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










