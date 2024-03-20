#include <TinyHlw8032.h>

TinyHlw8032 hlw8032;

void setup()
{
  Serial.begin(4800);
}

void loop()
{
  while (Serial->available())
  {
    if (hlw8032.encode(Serial->read(), millis()))
    {
      if (hlw8032.isUpdated())
      {
        // ToDo:
        float V =  hlw8032.get_Voltage();
        float A = hlw8032.get_Current();
        float P = hlw8032.get_Power();
        double E = hlw8032.get_Energy();
      }
    }
  }
}
