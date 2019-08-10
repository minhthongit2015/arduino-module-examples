/*
  Bài 4: ESP Station + Server + Web

  - ESP đóng vai trò là Station trong mô hình mạng wifi.
  - ESP đóng vai trò là Server trong kiến trúc TCP/IP.
  - ESP đóng thể hiện chức năng là Web Server.

  [Mô tả]: ESP kết nối vào mạng wifi. Sau đó mở 1 Web Server
  trên cổng xác định.
*/

/*************************************************
 *        Thêm vào các thư viện cần thiết        *
 *************************************************/

#include <ESP8266WiFi.h>
#include <string.h>
#include <ESP8266WebServer.h>



/*************************************************
 *                Thiết đặt Wifi                 *
 *************************************************/

const char* WifiSSID = "IUHYRA";
const char* WifiPassword = "iuhyra@12345";



/*************************************************
 *            Thiết đặt mở Web Server            *
 *************************************************/

const unsigned int myPort = 80;
ESP8266WebServer myWebServer(myPort);

void handleRoot();
void drawGraph();
void handleNotFound();



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
   *      2. Mở Web Server trên cổng xác định      *
   *************************************************/
  myWebServer.on("/", handleRoot);  // Phản hồi web page trên đường dẫn "/"
  myWebServer.on("/test.svg", drawGraph); // Phản hổi hình ảnh, media...
  myWebServer.on("/inline",
    []() { // Cú pháp khai báo "hàm vô danh trong C++ 11" (Lambda expression)
      myWebServer.send(200, "text/plain", "this works as well");
    }
  );
  myWebServer.onNotFound(handleNotFound); // Xử lý cho các đường dẫn không hợp lệ
  myWebServer.begin(); // Bắt đầu mở Server
  Serial.println("HTTP server started");
}
 
 
void loop() {
  // Chuyển lượt xử lý cho Web Server trong mỗi vòng lặp
  myWebServer.handleClient();
}



/*************************************************
 *           Định nghĩa một số hàm khác          *
 *************************************************/

void handleRoot() {
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,
    "<html>\
      <head>\
        <meta http-equiv='refresh' content='5'/>\
        <title>ESP8266 Demo</title>\
        <style>\
          body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
        </style>\
      </head>\
      <body>\
        <h1>Hello from ESP8266!</h1>\
        <p>Uptime: %02d:%02d:%02d</p>\
        <img src=\"/test.svg\" />\
      </body>\
    </html>",
    hr, min % 60, sec % 60
  );
  myWebServer.send(200, "text/html", temp);
}

void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x+= 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  myWebServer.send(200, "image/svg+xml", out);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += myWebServer.uri();
  message += "\nMethod: ";
  message += (myWebServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += myWebServer.args();
  message += "\n";

  for (uint8_t i = 0; i < myWebServer.args(); i++) {
    message += " " + myWebServer.argName(i) + ": " + myWebServer.arg(i) + "\n";
  }

  myWebServer.send(404, "text/plain", message);
}

/** Tóm tắt
 * 1. Kết nối vào mạng wifi.
 * 2. Mở Web Server lắng nghe kết nối trên cổng xác định.
 * 3. Xử lý các yêu cầu Request từ client.
 *    + Lấy nội dung web page.
 *    + Lấy nội dung file.
 *    + Xử lý khi client truy cập đường dẫn không hợp lệ.
 **/






