/*
  Bài 5: ESP Station + Server + Web

  - ESP đóng vai trò là Station trong mô hình mạng wifi.
  - ESP đóng vai trò là Server trong kiến trúc TCP/IP.
  - ESP đóng thể hiện chức năng là Web Server.

  [Mô tả]: ESP kết nối vào mạng wifi. Sau đó mở 1 Web Server
  trên cổng xác định.

  [Giới thiệu về WebSocket]
    WebSocket là một giao thức TCP/IP ở tầng ứng dụng (Tương tự
  như HTTP/HTTPs về vai trò). Nhưng WebSocket được thiết kế để
  gửi nhận thông điệp ngắn gọn hơn => nhanh hơn, tốn ít lưu lượng
  đường truyền hơn. 
    Về hoạt động, WebSocket sử dụng kiến trúc hướng sự kiện. Người
  lập trình có thể đăng ký các hàm xử lý khi các sự kiện xảy ra
  (VD: Sự kiện mở kết nối, đóng kết nối, có thông điệp đến, kết
  nối lỗi...). Nhờ đó người lập trình không cần phải kiểm tra có
  thông điệp gửi đến hay không trong các vòng lặp như ở lập trình
  socket.

  Xem thêm:
   + https://viblo.asia/p/websocket-la-gi-Ljy5VxkbZra
   + https://www.google.com.vn/search?q=websocket+là+gì
*/

/*************************************************
 *        Thêm vào các thư viện cần thiết        *
 *************************************************/

#include <ESP8266WiFi.h>
#include <string.h>
#include <WebSocketsServer.h>



/*************************************************
 *                Thiết đặt Wifi                 *
 *************************************************/

const char* WifiSSID = "IUHYRA";
const char* WifiPassword = "iuhyra@12345";



/*************************************************
 *         Thiết đặt mở WebSocket Server         *
 *************************************************/

WebSocketsServer webSocket = WebSocketsServer(81);



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
   *            2. Mở WebSocket Server             *
   *************************************************/
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}


void loop() {
  // Chuyển lượt xử lý cho WebSocket Server trong mỗi vòng lặp
  webSocket.loop();


  // Ping kiểm tra kết nối tới các Client mỗi 10 giây
  static unsigned long last_10sec = 0;
  static unsigned int counter = 0;
  unsigned long t = millis();
  if((t - last_10sec) > 10 * 1000) {
      counter++;
      bool ping = (counter % 2);
      int i = webSocket.connectedClients(ping);
      Serial.printf("%d Connected websocket clients ping: %d\n", i, ping);
      last_10sec = millis();
  }
}



/*************************************************
 *           Định nghĩa một số hàm khác          *
 *************************************************/

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
      case WStype_DISCONNECTED: // Sự kiện có client ngắt kết nối
        Serial.printf("[%u] Disconnected!\n", num);
        break;

      case WStype_CONNECTED: { // Sự kiện có client kết nối đến
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
        break;

      case WStype_TEXT: // Sự kiện có client gửi thông điệp đến
        Serial.printf("[%u] get Text: %s\n", num, payload);
        break;
    }
}


/** Tóm tắt
 * 1. Kết nối vào mạng wifi.
 * 2. Mở WebSocket Server lắng nghe kết nối trên cổng xác định.
 * 3. Nhận gửi thông điệp với các client kết nối đến.
 **/

