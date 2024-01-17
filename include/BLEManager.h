#ifndef BLEManager_H
#define BLEManager_H

#include "MyServerCallbacks.h"
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class BLEManager {
private:
    BLEServer *server;
    BLEService *service;
    BLECharacteristic *characteristic;
    MyServerCallbacks *serverCallbacks;
    BLEAdvertising *advertising;
    void createBLEServer();
    void createBLEService();
    void createBLECharacteristic();
    void startAdvertising();

public:
    BLEManager();
    void init(std::string deviceName);
    bool getIsDeviceConnected();
    void sendDataToClient(uint32_t &data);
};

#endif