#include "TinyHlw8032.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

TinyHlw8032::TinyHlw8032()
    : VoltageCoefficient(1.88f), CurrentCoefficient(1.0f)
{
    data_update_flage = 0;
    encodedCharCount = 0;
    energy_update_flage = 0;
}
bool TinyHlw8032::Checksum()
{
    byte check = 0;
    for (byte a = 2; a <= 22; a++)
    {
        check = check + SerialTemps[a];
    }
    if (check == SerialTemps[23])
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool TinyHlw8032::encode(char c)
{
    uint32_t VolPar;
    uint32_t VolData;
    uint32_t CurrentPar;
    uint32_t CurrentData;
    uint32_t PowerPar;
    uint32_t PowerData;
    float a, b;
    struct HLW8032BitFields *HLW8032Pt = (struct HLW8032BitFields *)SerialTemps;
    if (encodedCharCount < _HLW8032_MAX_FIELD_SIZE)
    {
        SerialTemps[encodedCharCount++] = c;
    }
    else
    {
        for (int i = 0; i < _HLW8032_MAX_FIELD_SIZE - 1; i++)
        {
            SerialTemps[i] = SerialTemps[i + 1];
        }
        SerialTemps[_HLW8032_MAX_FIELD_SIZE - 1] = c;
    }

    if (encodedCharCount == 24 && Checksum())
    {
        VolPar = ((uint32_t)SerialTemps[2] << 16) |
                 ((uint32_t)SerialTemps[3] << 8) | SerialTemps[4];
        VolData = ((uint32_t)SerialTemps[5] << 16) |
                  ((uint32_t)SerialTemps[6] << 8) | SerialTemps[7];
        CurrentPar = ((uint32_t)SerialTemps[8] << 16) |
                     ((uint32_t)SerialTemps[9] << 8) | SerialTemps[10];
        CurrentData = ((uint32_t)SerialTemps[11] << 16) |
                      ((uint32_t)SerialTemps[12] << 8) | SerialTemps[13];
        PowerPar = ((uint32_t)SerialTemps[14] << 16) |
                   ((uint32_t)SerialTemps[15] << 8) | SerialTemps[16];
        PowerData = ((uint32_t)SerialTemps[17] << 16) |
                    ((uint32_t)SerialTemps[18] << 8) | SerialTemps[19];

        a = (float)VolPar;
        b = (float)VolData;
        Voltage = a * VoltageCoefficient / b;

        a = (float)CurrentPar;
        b = (float)CurrentData;
        Current = a * CurrentCoefficient / b;

        a = (float)PowerPar;
        b = (float)PowerData;
        Power = VoltageCoefficient * CurrentCoefficient * a / b;

        Power_Factor = Power / (Voltage * Current);

        data_update_flage = true;
        energy_update_flage = true;
        encodedCharCount = 0;
    }
    return 1;
}
bool TinyHlw8032::encode(char c, unsigned long long tims)
{
    static unsigned long long tims_old = 0;
    encode(c);
    if (energy_update_flage)
    {
        if (tims_old != 0)
        {
            Energy += Power * ((tims - tims_old) / 1000);
            tims_old = tims;
        }
        else
        {
            tims_old = tims;
        }
        energy_update_flage = false;
    }
    return 1;
}

//
// internal utilities
//
int TinyHlw8032::fromHex(char a)
{
    if (a >= 'A' && a <= 'F')
        return a - 'A' + 10;
    else if (a >= 'a' && a <= 'f')
        return a - 'a' + 10;
    else
        return a - '0';
}
