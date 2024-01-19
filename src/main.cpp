#include "BLEManager.h"
#include "BLEuuids.h"
#include "LED.h"
#include "thermistor.h"
#include <Arduino.h>

// Sensors
Thermistor *thermistor1;
LED *bleLed;

// BLE Objects
BLEManager *bleManager;
BLECharacteristic *thermistorCharacteristic;
BLECharacteristic *forceSensorCharacteristic;
BLECharacteristic *spo2Characteristic;

void setup() {
    Serial.begin(115200);

    // Create BLE manager object
    bleManager = new BLEManager("Disense");

    // Create sensor objects
    thermistor1 = new Thermistor(33);
    bleLed = new LED(2);

    // Create characteristic objects for sensors (thermistor, force, spo2)
    thermistorCharacteristic = bleManager->createBLECharacteristicForNotify(THERMISTORS_CHARACTERISTIC_UUID);
    forceSensorCharacteristic = bleManager->createBLECharacteristicForNotify(FORCE_SENSOR_CHARACTERISTIC_UUID);
    spo2Characteristic = bleManager->createBLECharacteristicForNotify(SPO2_CHARACTERISTIC_UUID);

    // start service and advertising for BLE
    bleManager->startService();
    bleManager->startAdvertising();
}

void loop() {
    if (bleManager->getIsDeviceConnected()) {
        bleLed->turnOn();
        thermistorCharacteristic->setValue("Hello World");
        thermistorCharacteristic->notify();
        delay(1000);
    } else {
        bleLed->turnOff();
        delay(500);
        bleLed->turnOn();
        delay(500);
    }
}