#ifndef MyCharacteristicCallbacks_H
#define MyCharacteristicCallbacks_H

#include "BLEuuids.h"
#include <Arduino.h>
#include <BLECharacteristic.h>
#include <Base64.h>
#include <Wstring.h>

typedef void (*Callback)();

class AckCallback : public BLECharacteristicCallbacks {
private:
    bool ack1;
    bool ack2;
    bool ack3;
    Callback deepSleepCallback;
    void resetAcks();

public:
    AckCallback(Callback deepSleepCallback);
    void onWrite(BLECharacteristic *pCharacteristic);
};

#endif