#ifndef __TinyHlw8032_h
#define __TinyHlw8032_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <limits.h>

#define _HLW8032_VERSION "0.0.1" // software version of this library
#define _HLW8032_MAX_FIELD_SIZE 24

class TinyHlw8032
{
public:
    TinyHlw8032();
    bool encode(char c); // process one character received from GPS
    TinyHlw8032 &operator<<(char c)
    {
        encode(c);
        return *this;
    }

    float Voltage;
    float Current;
    float Power;
    float Power_Factor;
    unsigned long long int Energy;

    static const char *libraryVersion() { return _HLW8032_VERSION; }

    bool isUpdated()
    {
        bool tmp = data_update_flage;
        data_update_flage = false;
        return tmp;
    }

    float get_Voltage() { return Voltage; }
    float get_Current() { return Current; }
    float get_Power() { return Power; }
    float get_Power_Factor() { return Power_Factor; }
    unsigned long long int get_Energy() { return Energy; }

private:
    float VoltageCoefficient;
    float CurrentCoefficient;
    bool data_update_flage;
    char SerialTemps[_HLW8032_MAX_FIELD_SIZE];

    int encodedCharCount;
    bool Checksum();

    // internal utilities
    int fromHex(char a);
};

#endif // def(__TinyHlw8032_h)
