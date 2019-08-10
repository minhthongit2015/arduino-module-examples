#include <ESP8266WiFi.h>
#include <string.h>


IPAddress local_IP(192,168,4,4);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

const char* ssid = "IUHYRA";
const char* password = "iuhyra@12345";


#define PORT 80
WiFiServer myServer(PORT);

String htmlPage =
    String("HTTP/1.1 200 OK\r\n") +
          "Content-Type: text/html\r\n" +
          "Connection: close\r\n" +  // the connection will be closed after completion of the response
          "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
          "\r\n" +
          "<!DOCTYPE HTML>" +
          "<html><meta charset=utf-8>" +
          "<div style='font-size: 30px; color: #b66e15; font-family:\"Segoe UI\", sans-serif'>Temperature: 27.5°C</div>" +
          "</html>" +
          "\r\n";

int pin = 2;
int numClient = 0;

void setup() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 1);

  Serial.begin(115200);

  // Thiết đặt access point
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESP Soft Access Point", "123456789") ? "Ready" : "Failed!");
  Serial.println("SSID: ESP Soft Access Point\r\nPassword: 123456789");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  myServer.begin();
  Serial.print("Server is listening on ");
  Serial.print(WiFi.softAPIP());
  Serial.print(":");
  Serial.println(PORT);

  int itry = 0;
  WiFi.begin(ssid, password);
  while (itry < 10 && WiFi.status() != WL_CONNECTED) {
    itry++;
    Serial.print(".");
    delay(500);
  }
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
}
 
 
void loop() {
  // Đợi kết nối từ client
  if (WiFi.softAPgetStationNum() != numClient) {
    numClient = WiFi.softAPgetStationNum();
    Serial.printf("\r\nStations connected = %d\n", numClient);
  }

  WiFiClient client = myServer.available();

  if (client) {
    Serial.println("\r\n[[ Client connected ]]");
    digitalWrite(pin, 0);

    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);

        if (line.length() == 1 && line[0] == '\n') {
          client.println(htmlPage); // Gửi trả lại nội dung
          break;
        }
      }
    }
  }

  delay(1000);
}










