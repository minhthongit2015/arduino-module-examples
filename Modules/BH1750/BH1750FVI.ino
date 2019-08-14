
#include <BH1750FVI.h>

BH1750FVI LightSensor(BH1750FVI::k_DevModeContHighRes);

void setup() 
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  LightSensor.begin();  
}

void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light: ");
  Serial.println(lux);
  delay(250);
}
