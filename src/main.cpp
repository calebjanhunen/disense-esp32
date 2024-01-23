#include "BLEManager.h"
#include "BLEuuids.h"
#include "LED.h"
#include "SensorIDs.h"
#include "thermistor.h"
#include <Arduino.h>

#define NUM_THERMISTORS 4
#define BYTES_PER_SENSOR 5 // For byte array (1 byte for sensor id, 4 bytes for temp value)

// Thermistors
Thermistor *thermistor1;
Thermistor *testThermistor;
Thermistor *thermistorArr[NUM_THERMISTORS];

// LEDs
LED *bleLed;

// BLE Objects
BLEManager *bleManager;
BLECharacteristic *thermistorCharacteristic;
BLECharacteristic *forceSensorCharacteristic;
BLECharacteristic *spo2Characteristic;
BLECharacteristic *testCharacteristic;

// thermistor byte array
uint8_t thermistorByteArr[NUM_THERMISTORS * BYTES_PER_SENSOR]; // 4 bytes per sensor (1 for id, 4 for temperature value)

float val;

void setup() {
    Serial.begin(115200);

    // Create BLE manager object
    bleManager = new BLEManager("Disense");

    // Create thermistor objects
    thermistorArr[0] = new Thermistor(33, METATARSAL_1_THERMISTOR_ID);
    thermistorArr[1] = new Thermistor(34, METATARSAL_5_THERMISTOR_ID);
    thermistorArr[2] = new Thermistor(35, HALLUX_THERMISTOR_ID);
    thermistorArr[3] = new Thermistor(36, CALCANEUS_THERMISTOR_ID);

    // Create LED objects
    bleLed = new LED(2);

    // Create characteristic objects for sensors (thermistor, force, spo2)
    testCharacteristic = bleManager->createBLECharacteristicForNotify(TEST_CHARACTERISTIC_UUID);
    thermistorCharacteristic = bleManager->createBLECharacteristicForNotify(THERMISTORS_CHARACTERISTIC_UUID);

    // start service and advertising for BLE
    bleManager->startService();
    bleManager->startAdvertising();
}

void encodeThermistorToByteArray(Thermistor *thermistor, uint8_t *byteArr) {
    int tempVal = thermistor->getTemp() * 10; // multiply by 10 to move decimal to the right
    byteArr[0] = thermistor->getId();
    byteArr[1] = (tempVal >> 24) & 0xFF;
    byteArr[2] = (tempVal >> 16) & 0xFF;
    byteArr[3] = (tempVal >> 8) & 0xFF;
    byteArr[4] = (tempVal) & 0xFF;
}

void readAndEncodeThermistorData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_THERMISTORS; i++) {
        if (i == 0) {
            thermistorArr[i]->readTemperature();
        } else {
            thermistorArr[i]->readFakeTemperature();
        }
        Serial.print("thermistor temp: ");
        Serial.print(i);
        Serial.print(", ");
        Serial.println(thermistorArr[i]->getTemp());
        encodeThermistorToByteArray(thermistorArr[i], &thermistorByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
    Serial.println(" ");
}

void loop() {
    // Serial.print("thermistor temp: ");
    // Serial.println(thermistorTemp);
    // encodeThermistorToByteArray(thermistor1, thermistorByteArr);

    if (bleManager->getIsDeviceConnected()) {
        bleLed->turnOn();
        readAndEncodeThermistorData();
        thermistorCharacteristic->setValue(thermistorByteArr, sizeof(thermistorByteArr));
        thermistorCharacteristic->notify();
    } else {
        bleLed->turnOff();
        delay(1000);
        bleLed->turnOn();
    }
    delay(1000);
}