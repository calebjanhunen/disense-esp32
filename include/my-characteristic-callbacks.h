#ifndef MyCharacteristicCallbacks_H
#define MyCharacteristicCallbacks_H

#include "BLEManager.h"
#include <Arduino.h>
#include <BLECharacteristic.h>

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
private:
    bool ack1;
    bool ack2;
    bool ack3;
    BLEManager *bleManager;
    void resetAcknowledgments();

public:
    MyCharacteristicCallbacks(BLEManager *bleManager);
    void onWrite(BLECharacteristic *pCharacteristic);
};

#endif