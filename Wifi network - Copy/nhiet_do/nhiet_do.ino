//Include thư viện
#include <OneWire.h>
#include <DallasTemperature.h>

// Chân nối với ESP
#define ONE_WIRE_BUS 16
//Thiết đặt thư viện onewire
OneWire oneWire(ONE_WIRE_BUS);
//Mình dùng thư viện DallasTemperature để đọc cho nhanh
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(115200);
  sensors.begin();
}

void loop(void)
{
  Serial.println("Start Loop");
  sensors.requestTemperatures();  
  Serial.print("Nhiet do");
  Serial.println(sensors.getTempCByIndex(0)); // vì 1 ic nên dùng 0
 
  //chờ 1 s rồi đọc để bạn kịp thấy sự thay đổi
  delay(1000);
}
