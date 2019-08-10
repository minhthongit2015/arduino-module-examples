#pragma once
#ifndef _WEBSOCKETSERVER_H
#define _WEBSOCKETSERVER_H
extern "C" {
  #include  <user_interface.h>
}

#include <WebSocketsServer.h>


/****************************************
 *            WebSocket Server
 ****************************************/

WebSocketsServer webSocket = WebSocketsServer(81);

byte myLed[4] = {D3, D5, D6, D7};



/****************************************
 *           Khai báo các hàm
 ****************************************/

void setupWebsocketServer();
void WebSocketServerLoop();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);



/****************************************
 *          Định Nghĩa Các Hàm
 ****************************************/

void setupWebsocketServer() {
    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    for (byte i=0; i<sizeof(myLed); i++) {
      pinMode(myLed[i], OUTPUT);
    }
}

void WebSocketServerLoop() {
  webSocket.loop();
}

/* Hàm xử lý sự kiện Websocket */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
            if (payload[0] == 'd') {
              byte led = myLed[payload[1] - '1'];
              bool state = payload[3] != '0';
              digitalWrite(led, state);
            }
            break;
    }

}




#endif








