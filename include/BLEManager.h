#ifndef BLEManager_H
#define BLEManager_H

#include "BLEuuids.h"
#include "MyServerCallbacks.h"
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

class BLEManager {
private:
    BLEServer *server;
    BLEService *service;
    MyServerCallbacks *serverCallbacks;
    void createBLEServer();
    void createBLEService();

public:
    BLEManager(std::string deviceName);
    void startService();
    void startAdvertising();
    BLECharacteristic *createBLECharacteristicForNotify(const char *characteristicUuid);
    bool getIsDeviceConnected();
    void bleShutDown();
};

#endif