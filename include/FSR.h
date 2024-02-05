#ifndef FSR_h
#define FSR_h

#include <Arduino.h>

class FSR {
private:
    byte pin;
    int id;
    float force;
    int totalADCValue;
    int numReadings;
    int id;
    float fromADCValToVoltage(float adcVal);
    float fromVoltageToResistance(float voltage);
    float fromVoltageToNewtonsUsing20KResistor(float voltage);
    float fromResistanceToNewtonsUsing10kResistor(float resistance);
    float fromResistanceToNewtonsUsing1kResistor(float resistance);

public:
    FSR(byte pin, int id);
    void readInstantaneousPressure();
    void readData();
    float getAveragePressure();
    int getId();
};

#endif