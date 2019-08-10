/*
Source code for
*/
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
/*
Each state in the application corresponds to one the integer.
Numbered starting at 0
stateCurrent is a variable that stores the current state of the application.
Welcome: 0
Receive_String: 1
*/
int currentState = 0;
int nextState = -1;
/*--------------------Define--------------------*/
/*Define LiquidCrystal_I2C - myLCD */
LiquidCrystal_I2C myLCD(0x27, 16, 2);
/*Define Wifi ESP8266 - myESP */
#define TIMEOUT      3000
#define RX 2 // pin TX of ESP8266 connect to pin 2 of Arduino
#define TX 3 //pin RX of ESP8266 connect to pin 3 of Arduino
SoftwareSerial myESP(RX,TX);
/*--------------------END Define--------------------*/
/*--------------------Prototype--------------------*/
//Read response from esp. Arduino is a receiver
String readResponse();
//Arduino send command to the esp
String sendCommand(String command);
//Close connection with a link id
void closeConnect();
void sendResponse(String content);
void HandleRequest(String res);
void stateWelcome();
void stateReceive_String();
/*--------------------END Prototype--------------------*/
void setup()
{
  myLCD.begin();
  myLCD.backlight();
  Serial.begin(9600); //Change baud rate according to your ESP
  myESP.begin(9600);
  Serial.println(sendCommand("AT+RST"));
  Serial.println(sendCommand("AT+CWMODE=2"));
  Serial.println(sendCommand("AT+CIFSR"));
  Serial.println(sendCommand("AT+CWSAP=\"My ESP AP\",\"12345678\",5,3"));
  Serial.println(sendCommand("AT+CIPAP=\"192.168.4.1\",\"192.168.4.1\",\"255.255.255.0\""));
  Serial.println(sendCommand("AT+CIPMUX=1"));
  Serial.println(sendCommand("AT+CIPSERVER=1,80"));
  Serial.println(sendCommand("AT+CIFSR"));
  Serial.println("Server is ready.");
  if(currentState == 0){
    stateWelcome();
  }
}
void loop()
{
  //Listening the request from server
  String res = readResponse();
  if(res != ""){
    HandleRequest(res);
  }
  delay(300);
  switch(currentState){
    case 0:
    //<case0>
    break;
    case 1:
    //<case1>
    break;
    default:
    break;
  }
}
/*--------------------Implement--------------------*/
String readResponse()
{
  String res = "";
  long timeout = millis() + TIMEOUT;
  while (millis() < timeout)
  {
    if (myESP.available())
    {
      char ch = myESP.read();
      res += ch;
    }
  }
  return res;
}
//Send command to esp.
String sendCommand(String command)
{
  myESP.println(command);
  return readResponse();
}
//Close connection with a link id
void closeConnect()
{
  Serial.println(sendCommand("AT+CIPCLOSE=5"));
}
void sendResponse(String content)
{
  String cmd = "AT+CIPSEND=0,";
  cmd += content.length();
  Serial.println(sendCommand(cmd));
  Serial.println(sendCommand(content));
  closeConnect();
}
void HandleRequest(String res){
  if (res.indexOf("GET / HTTP/1.1") >= 0)
  {
    String content = "Well come to ESP LAP";
    String cmd = "AT+CIPSEND=0,";
    cmd += content.length();
    Serial.println(sendCommand(cmd));
    Serial.println(sendCommand(content));
    closeConnect();
  }
  else if (res.indexOf("CONNECT") >= 0)
  {
    Serial.println("receive....");
  }
  else if (res.indexOf("+IPD") >= 0)
  {
    //Event: "string" received
    if(res.indexOf("string") != -1)
    {
      stateReceive_String();
    }
    //<requestWifi>
  }
  delay(300);
}
void stateWelcome(){
  myLCD.clear();
  myLCD.print("Hello!");
  currentState = 0;
  delay(200);
}
void stateReceive_String(){
  myLCD.clear();
  myLCD.setCursor(0, 0);
  myLCD.print("Incoming");
  myLCD.setCursor(0, 1);
  myLCD.print("string");
  currentState = 1;
  delay(200);
}
/*--------------------END Implement--------------------*/
