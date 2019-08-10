#include <ESP8266WiFi.h>
#include <string.h>


IPAddress local_IP(192,168,4,4);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

const char* ssid = "Wall-E";
const char* password = "Wastebasket";


#define PORT 1234
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
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  Serial.println("SSID: ESP Soft Access Point\r\nPassword: 123456789");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  myServer.begin();
  Serial.print("Server is listening on ");
  Serial.print(WiFi.softAPIP());
  Serial.print(":");
  Serial.println(PORT);

  int itry = 0;
  while (itry < 10 && WiFi.status() != WL_CONNECTED) {
    itry++;
    Serial.print(".");
    delay(500);
  }
  WiFi.begin(ssid, password);
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
}
 

WiFiClient client;
void loop() {
  // Đợi kết nối từ client
  if (WiFi.softAPgetStationNum() != numClient) {
    numClient = WiFi.softAPgetStationNum();
    Serial.printf("\r\nStations connected = %d\n", numClient);
  }

  client = myServer.available();

  if (client) {
    Serial.println("\r\n[[ Client connected ]]");
    digitalWrite(pin, 0);

    if (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.print(line);
        client.printf("%s\r\n", line.c_str()); // Gửi trả lại nội dung 
      }
      delay(100);
    }
    Serial.println("\r\n[[ Client disconnected ]]");
    
    digitalWrite(pin, 1);
  }

  delay(1000);
}











