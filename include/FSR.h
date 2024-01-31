#ifndef FSR_h
#define FSR_h

#include <Arduino.h>

class FSR {
private:
    byte pin;
    int id;

public:
    FSR(byte pin, int id);
    void readPressure();
};

#endif