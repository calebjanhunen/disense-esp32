#ifndef thermistor_h
#define thermistor_h

#include <Arduino.h>

class Thermistor {
private:
    const float supplyVoltage = 3.3; // supply voltage from esp32
    byte pin;
    int id;
    float tempVal;
    float fromVoltageToResistance(float voltage);
    float fromResistanceToCelsius(float resistance);
    int readFromADCPin();
    float fromADCReadingToVoltage(int adcVoltage);

public:
    Thermistor(byte pin, int id);
    float getCelciusValue();
    int getId();
};

#endif