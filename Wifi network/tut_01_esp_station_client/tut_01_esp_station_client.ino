/*
  Bài 1: ESP Station + Client

  - ESP đóng vai trò là Station trong mô hình mạng wifi.
  - ESP đóng vai trò là Client trong kiến trúc TCP/IP.

  [Mô tả]: ESP kết nối vào mạng wifi và thông qua mạng wifi
  để gửi thông điệp tới server trên internet hoặc server
  nào đó trong mạng nội bộ.
*/


/*************************************************
 *        Thêm vào các thư viện cần thiết        *
 *************************************************/

#include <ESP8266WiFi.h>
#include <string.h> // Để xử lý chuỗi đơn giản hơn



/*************************************************
 *                Thiết đặt Wifi                 *
 *************************************************/

const char* WifiSSID = "IUHYRA";
const char* WifiPassword = "iuhyra@12345";



/*************************************************
 *          Thiết đặt kết nối đến server         *
 *************************************************/

const char* ServerHost = "169.254.86.158"; // Địa chỉ của server
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

  // Hiển thị thông tin mạng wifi vừa kết nối vào
  Serial.printf("\r\nConnected to %s\r\n", WifiSSID); // Hiển thị tên mạng wifi
  Serial.printf("IP address: %s\r\n", WiFi.localIP().toString().c_str());  // IP của ESP sau khi kết nối vào mạng


  
  /*************************************************
   *          2. Kết nối đến một máy chủ           *
   *************************************************/

  while (!myClient.connect(ServerHost, ServerPort)) { // Vòng lặp đợi đến khi kết nối thành công đến máy chủ
    Serial.print("."); delay(500);
  }
  Serial.printf("\r\nConnected to: %s:%d\r\n", ServerHost, ServerPort);
}

void loop() {
  // Kiểm tra nếu mất kết nối thì kết nối lại đến máy chủ
  // (*) Ở đây ta không cần kiểm tra kết nối tới Wifi Access Point
  //     vì này đã được thiết lập tự động kết nối lại vào mạng.
  while (!myClient.connect(ServerHost, ServerPort)) {
    Serial.print("."); delay(500);
  }

  // 3. Thử gửi một thông điệp tới máy chủ
  myClient.println("Hello!");
  Serial.println("Send: Hello!"); 
  
  // 4. Kiểm tra và nhận thông điệp từ server gửi đến
  if (myClient.available())
  {
    Serial.printf("Available: %d bytes", myClient.available());
    String line = myClient.readStringUntil('\n');  // 4. Nhận dữ liệu gửi đến cho đến ký tự \n (ký tự xuống dòng)
    Serial.println(line); // Hiển thị thông điệp
  }

  delay(1000);
}

/** Tóm tắt
 * 1. Kết nối vào mạng wifi.
 * 2. Kết nối đến server.
 * 3. Gửi thông điệp.
 * 4. Nhận thông điệp.
 * 
 * (*) Lưu ý: Gửi nhận này có thể tùy ý, không nhất thiết phải gửi trước rồi mới nhận hay nhận trước rồi mới gửi.
 **/
