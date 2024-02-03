#ifndef thermistor_h
#define thermistor_h

#include "thermistor-lookup-table.h"
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
    float fromResistanceToCelsiusUsingLookupTable(float resistance);
    float linearInterpolation(float resistance, ThermistorTableEntry entry1, ThermistorTableEntry entry2);

public:
    Thermistor(byte pin, int id);
    float readTemperature();
    float readFakeTemperature(); // For testing
    int getId();
    float getTemp();
};

#endif