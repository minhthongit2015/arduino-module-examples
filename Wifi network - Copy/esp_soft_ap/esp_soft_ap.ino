#include <ESP8266WiFi.h>
#include <string.h>


IPAddress local_IP(192,168,4,4);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

#define PORT 80
WiFiServer myServer(PORT);

String htmlPage = "";

int builtinLED = 2;
int numClient = 0;

void setup() {
  pinMode(builtinLED, OUTPUT);
  digitalWrite(builtinLED, 1); // tat

  Serial.begin(115200);


  // Thiết đặt access point
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!"); // Thiet dat mang.

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESP Soft Access Point", "123456789") ? "Ready" : "Failed!"); // Bat dau phat wifi
  Serial.println("SSID: ESP Soft Access Point\r\nPassword: 123456789");


  // Kiem tra lai thong tin

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());


  // Tao server

  myServer.begin();
  Serial.print("Server is listening on ");
  Serial.print(WiFi.softAPIP());
  Serial.print(":");
  Serial.println(PORT);

}
 
 
void loop() {
  // Hiện xem bao nhiêu client kết nối vào AP
  if (WiFi.softAPgetStationNum() != numClient) {
    numClient = WiFi.softAPgetStationNum();
    Serial.printf("\r\nStations connected = %d\n", numClient);
  }

  // Đợi kết nối đến server
  WiFiClient client = myServer.available();

  if (client) {
    Serial.println("\r\n[[ Client connected ]]");
    digitalWrite(builtinLED, 0);

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










