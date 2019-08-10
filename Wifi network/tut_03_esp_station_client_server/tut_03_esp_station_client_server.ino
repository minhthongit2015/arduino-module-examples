/*
  Bài 3: ESP Station + Server + Client

  - ESP đóng vai trò là Station trong mô hình mạng wifi.
  - ESP đóng vai trò là Server trong kiến trúc TCP/IP.
  - ESP đóng vai trò là Client trong kiến trúc TCP/IP.

  [Mô tả]: ESP kết nối vào mạng wifi. Sau đó đồng thời
  mở một server song song với mở một kết nối đến máy chủ
  khác trên internet hoặc mạng nội bộ.
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
 *       Thiết đặt kết nối đến server khác       *
 *************************************************/

const char* ServerHost = "192.168.137.1"; // Địa chỉ của server
int ServerPort = 1234; // Port lắng nghe kết nối của chương trình server
WiFiClient myClient; // Đối tượng quản lý kết nối đến server (Này chính là socket)



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
  /*************************************************
   *         3. Xử lý cho vai trò là Server        *
   *************************************************/

  WiFiClient client = myServer.available(); // 3.1. Nhận kết nối từ client

  if (client) {
    Serial.println("\r\n[[ Client connected ]]");

    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\r'); // 3.2. Nhận thông điệp.
        Serial.print(line);
        client.println(line); // 3.3. Gửi thông điệp.
      }
    }
  }



  /*************************************************
   *         4. Xử lý cho vai trò là Client        *
   *************************************************/

  // Kiểm tra nếu mất kết nối thì kết nối lại đến máy chủ
  while (!myClient.connect(ServerHost, ServerPort)) {
    Serial.print("."); delay(500);
  }

  // 4.1. Thử gửi một thông điệp tới máy chủ
  myClient.println("Hello!");
  Serial.println("Send: Hello!"); 
  
  // 4.2. Kiểm tra và nhận thông điệp từ server gửi đến
  if (myClient.available())
  {
    Serial.printf("Available: %d bytes", myClient.available());
    String line = myClient.readStringUntil('\n');  // 4.3 Nhận dữ liệu gửi đến cho đến ký tự \n (ký tự xuống dòng)
    Serial.println(line); // Hiển thị thông điệp
  }

  delay(100);
}

/** Tóm tắt
 * 1. Kết nối vào mạng wifi.
 * 2. Mở Server lắng nghe kết nối trên cổng xác định.
 * 3. Xử lý cho vai trò là Server.
 * 4. Xử lý cho vai trò là Client.
 **/
