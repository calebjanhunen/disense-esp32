#include "BLEManager.h"
#include "BLEuuids.h"
#include "LED.h"
#include "SensorIDs.h"
#include "thermistor.h"
#include <Arduino.h>

#define NUM_THERMISTORS 2

// Sensors
Thermistor *thermistor1;
LED *bleLed;

// BLE Objects
BLEManager *bleManager;
BLECharacteristic *thermistorCharacteristic;
BLECharacteristic *forceSensorCharacteristic;
BLECharacteristic *spo2Characteristic;
BLECharacteristic *testCharacteristic;

// thermistor byte array
uint8_t thermistorByteArr[NUM_THERMISTORS * 5]; // 4 bytes per sensor (1 for id, 4 for temperature value)

int val;

void setup() {
    Serial.begin(115200);
    val = 0;

    // Create BLE manager object
    bleManager = new BLEManager("Disense");

    // Create sensor objects
    thermistor1 = new Thermistor(33, METATARSAL_1_THERMISTOR_ID);
    bleLed = new LED(2);

    // Create characteristic objects for sensors (thermistor, force, spo2)
    testCharacteristic = bleManager->createBLECharacteristicForNotify(TEST_CHARACTERISTIC_UUID);
    thermistorCharacteristic = bleManager->createBLECharacteristicForNotify(THERMISTORS_CHARACTERISTIC_UUID);
    // forceSensorCharacteristic = bleManager->createBLECharacteristicForNotify(FORCE_SENSOR_CHARACTERISTIC_UUID);
    // spo2Characteristic = bleManager->createBLECharacteristicForNotify(SPO2_CHARACTERISTIC_UUID);

    // start service and advertising for BLE
    bleManager->startService();
    bleManager->startAdvertising();
}

void encodeThermistorToByteArray(Thermistor *thermistor, uint8_t *byteArr) {
    Serial.println("In encode function");
    int tempVal = thermistor->getCelciusValue() * 10; // multiply by 10 to move decimal to the right
    byteArr[0] = thermistor->getId();
    byteArr[1] = (tempVal >> 24) & 0xFF;
    byteArr[2] = (tempVal >> 16) & 0xFF;
    byteArr[3] = (tempVal >> 8) & 0xFF;
    byteArr[4] = (tempVal) & 0xFF;
}

void loop() {
    float thermistorTemp = thermistor1->getCelciusValue();
    Serial.print("thermistor temp: ");
    Serial.println(thermistorTemp);
    encodeThermistorToByteArray(thermistor1, thermistorByteArr);

    if (bleManager->getIsDeviceConnected()) {
        bleLed->turnOn();
        val++;
        thermistorCharacteristic->setValue(thermistorByteArr, sizeof(thermistorByteArr));
        thermistorCharacteristic->notify();
    } else {
        bleLed->turnOff();
        delay(1000);
        bleLed->turnOn();
    }
    delay(1000);
}