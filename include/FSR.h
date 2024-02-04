#ifndef FSR_h
#define FSR_h

#include <Arduino.h>

class FSR {
private:
    byte pin;
    int id;
    float fromVoltageToNewtonsUsing20KResistor(float voltage);
    float fromVoltageToNewtonsUsing10KResistor(float voltage);

public:
    FSR(byte pin, int id);
    void readPressure();
};

#endif