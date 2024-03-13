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
    if (hlw8032.encode(Serial->read()))
    {
      if (hlw8032.isUpdated())
      {
        // ToDo:
      }
    }
  }
}
