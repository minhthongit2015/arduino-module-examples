#pragma once
#ifndef _WEBGUI_H
#define _WEBGUI_H
extern "C" {
  #include  <user_interface.h>
}

#include <ESP8266WebServer.h>


/****************************************
 *                Web GUI
 ****************************************/

ESP8266WebServer server ( 80 );

const int led = 13;



/****************************************
 *          Khai báo các hàm
 ****************************************/

void setupWebGUI();
void WebGUILoop();

/* Hàm xử lý request chính */
void handleRoot();

/* Hàm xử lý request tới file */
void drawGraph();

/* Hàm xử lý không tìm thấy file*/
void handleNotFound();



/****************************************
 *          Định Nghĩa Các Hàm
 ****************************************/

void setupWebGUI() {
  server.on ( "/", handleRoot );
  server.on ( "/test.svg", drawGraph );
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void WebGUILoop() {
  server.handleClient();
}

/* Hàm xử lý request chính */
void handleRoot() {
  digitalWrite ( led, 1 );
  char temp[1024];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( temp, 1024,
"<head>\n  <meta charset=\"utf-8\">\n  <style>\n    label {\n      user-select: none;\n    }\n  </style>\n</head>\n\n<body>\n  <label>\n    <input type=\"checkbox\" id=\"d1\">\n    <span>Đèn 1</span>\n  </label>\n\n  <label>\n    <input type=\"checkbox\" id=\"d2\">\n    <span>Đèn 2</span>\n  </label>\n\n  <label>\n    <input type=\"checkbox\" id=\"d3\">\n    <span>Đèn 3</span>\n  </label>\n\n  <label>\n    <input type=\"checkbox\" id=\"d4\">\n    <span>Đèn 4</span>\n  </label>\n\n  <script>\n    let ws = new WebSocket(\"ws://192.168.1.118:81\");\n    ws.onopen = () => ws.send(\"Hello\");\n    ws.onmessage = (e) => console.log(e);\n\n    let d = document;\n    let btns = d.getElementsByTagName(\"input\");\n\n    for (let btn of btns) {\n      btn.addEventListener(\"change\", (e) => {\n        let tg = e.target;\n        let state = tg.checked;\n        ws.send(`${tg.id}_${+state}`);\n      });\n    }\n  </script>\n</body>" );
  server.send ( 200, "text/html", temp );
  digitalWrite ( led, 0 );
}


/* Hàm xử lý request tới file */
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

  server.send ( 200, "image/svg+xml", out);
}

/* Hàm xử lý không tìm thấy file*/
void handleNotFound() {
  digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}





#endif






