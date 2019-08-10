#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include "WebGUI.h"
#include "WebSocketServer.h"

/****************************************
 *              WIFI
 ****************************************/
 
const char *ssid = "IUHYRA";
const char *password = "iuhyra@12345";


void setup ( void ) {
  pinMode ( led, OUTPUT );
  digitalWrite ( led, 0 );
  Serial.begin ( 115200 );
  WiFi.mode ( WIFI_STA );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  setupWebGUI();
  setupWebsocketServer();
}

void loop ( void ) {
  WebGUILoop();
  WebSocketServerLoop();
}





