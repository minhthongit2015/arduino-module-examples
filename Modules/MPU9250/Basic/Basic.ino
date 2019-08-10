//@[IoT Maker Viet Nam](https://iotmaker.vn)

#include "MPU9250.h"
#include <ESP8266WiFi.h>
#include <string.h>

/* WIFI config */
const char ssid[32] = "IUHYRA";
const char password[32] = "iuhyra@12345";
//const char ssid[32] = "CPH1801";
//const char password[32] = "123456780";
//const char ssid[32] = "moidoiten";
//const char password[32] = "passla123";

String raspiServer = "192.168.1.122";
uint16_t raspiPort = 1412;
WiFiClient raspi;


// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;

void setup() {
  // serial to display data
  Serial.begin(9600);
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(500);
  }
  Serial.println("Connected!");
}

char buffer[100] = "";
void loop() {
  if (!raspi.connected()) {
    while (!raspi.connect(raspiServer, raspiPort)) {
      Serial.println("Connect to Server..."); delay(500);
    }
  }
  
  IMU.readSensor();
  sprintf(buffer, "Acc: [%s, %s, %s]\t", String(IMU.getAccelX_mss()).c_str(), String(IMU.getAccelY_mss()).c_str(), String(IMU.getAccelZ_mss()).c_str()); Serial.print(buffer);
  sprintf(buffer, "Gyro: [%s, %s, %s]\t", String(IMU.getGyroX_rads()).c_str(), String(IMU.getGyroY_rads()).c_str(), String(IMU.getGyroZ_rads()).c_str()); Serial.print(buffer);
  sprintf(buffer, "Mag: [%s, %s, %s]\t", String(IMU.getMagX_uT()).c_str(), String(IMU.getMagY_uT()).c_str(), String(IMU.getMagZ_uT()).c_str()); Serial.print(buffer);
  sprintf(buffer, "T: %s\n", String(IMU.getTemperature_C()).c_str()); Serial.print(buffer);

  
  if (IMU.getAccelX_mss() < -140) {
    raspi.print("L\n");
  }
  if (IMU.getAccelY_mss() < -140) {
    raspi.print("O\n");
  }
  
  if (IMU.getGyroZ_rads() > 4) {
    raspi.print("Pause\n");
  }
  delay(100);
}
