#ifndef thermistor_h
#define thermistor_h

#include "thermistor-lookup-table.h"
#include <Arduino.h>

class Thermistor {
private:
    const float supplyVoltage = 3.3; // supply voltage from esp32
    byte pin;
    int id;
    int totalADCValue;
    int numReadings;
    float fromVoltageToResistance(float voltage);
    float fromResistanceToCelsius(float resistance);
    int readFromADC();
    float fromADCReadingToVoltage(float adcVoltage);
    float fromResistanceToCelsiusUsingLookupTable(float resistance);
    float linearInterpolation(float resistance, ThermistorTableEntry entry1, ThermistorTableEntry entry2);

    // New
    float fromADCToTemp(float adcVal);

public:
    Thermistor(byte pin, int id);
    float readTemperature();
    int getId();

    // NEW
    void readData();
    float getAverageTemp();
};

#endif