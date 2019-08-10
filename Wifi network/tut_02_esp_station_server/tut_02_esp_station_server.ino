/*
  Bài 2: ESP Station + Server

  - ESP đóng vai trò là Station trong mô hình mạng wifi.
  - ESP đóng vai trò là Server trong kiến trúc TCP/IP.

  [Mô tả]: ESP kết nối vào mạng wifi và khởi tạo một server
  lắng nghe kết nối tại địa chỉ của mình. Các máy trong mạng
  có thể kết nối đến để gửi thông điệp cho ESP hoặc nhận dữ
  liệu nào đó từ ESP.
*/


/*************************************************
 *        Thêm vào các thư viện cần thiết        *
 *************************************************/

#include <ESP8266WiFi.h>
#include <string.h>



/*************************************************
 *                Thiết đặt Wifi                 *
 *************************************************/

const char* WifiSSID = "IUHYRA";
const char* WifiPassword = "iuhyra@12345";



/*************************************************
 *              Thiết đặt mở server              *
 *************************************************/

const unsigned int myPort = 80;
WiFiServer myServer(myPort);



/*************************************************
 *           Bắt đầu chương trình chính          *
 *************************************************/

void setup() {
  Serial.begin(115200);


  /*************************************************
   *          1. Kết nối đến mạng wifi             *
   *************************************************/
  WiFi.mode(WIFI_STA); // Đặt chế độ hoạt động là Station
  WiFi.begin(WifiSSID, WifiPassword); // Kết nối đến mạng wifi khác

  while (WiFi.status() != WL_CONNECTED) { // Vòng lặp đợi đến khi kết nối thành công vào mạng wifi
    Serial.print("."); delay(500);
  }
  Serial.print("\r\nConnected to: "); Serial.println(WifiSSID);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());



  /*************************************************
   *    2. Khởi tạo máy chủ trên cổng xác định     *
   *************************************************/
  myServer.begin();
  Serial.printf("Server is listening on %s:%d\r\n",
    WiFi.localIP().toString().c_str(),
    myPort
  );
}

void loop() {
  WiFiClient client = myServer.available(); // 3. Nhận kết nối từ client

  if (client) {
    Serial.println("\r\n[[ Client connected ]]");

    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\r'); // 4. Nhận thông điệp.
        Serial.print(line);
        client.println(line); // 5. Gửi thông điệp.
      }
    }
  }

  delay(100);
}

/** Tóm tắt
 * 1. Kết nối vào mạng wifi.
 * 2. Mở Server lắng nghe kết nối trên cổng xác định.
 * 3. Nhận kết nối từ client.
 * 4. Nhận thông điệp.
 * 5. Gửi thông điệp.
 * 
 * (*) Lưu ý: Gửi nhận này có thể tùy ý, không nhất thiết phải nhận trước rồi mới gửi hay gửi trước rồi mới nhận.
 **/
