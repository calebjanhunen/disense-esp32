#include "BLEManager.h"
#include "BLEuuids.h"
#include "FSR.h"
#include "LED.h"
#include "SensorIDs.h"
#include "spo2.h"
#include "thermistor.h"
#include <Arduino.h>
#include <Wire.h>

#define NUM_THERMISTORS 3
#define NUM_FSR 2
#define BYTES_PER_SENSOR 5 // For byte array (1 byte for sensor id, 4 bytes for temp value)

// Sensors
Thermistor *thermistorArr[NUM_THERMISTORS];
FSR *fsrArr[NUM_FSR];
SPO2 *spo2;

// LEDs
LED *bleLed;

// BLE Objects
BLEManager *bleManager;
BLECharacteristic *thermistorCharacteristic;
BLECharacteristic *forceSensorCharacteristic;
BLECharacteristic *spo2Characteristic;

// thermistor byte array
uint8_t thermistorByteArr[NUM_THERMISTORS * BYTES_PER_SENSOR]; // 4 bytes per sensor (1 for id, 4 for temperature value)

const long bleTransmissionInterval = 2000; // Interval for sending data over BLE (2 seconds)
unsigned long prevMillis = 0;              // Stores last time ble tranmission was executed

void setup() {
    Wire.begin();
    Serial.begin(115200);

    // Create BLE manager object
    bleManager = new BLEManager("Disense");

    // Create thermistor objects
    thermistorArr[0] = new Thermistor(13, METATARSAL_1_THERMISTOR_ID);
    thermistorArr[1] = new Thermistor(14, METATARSAL_5_THERMISTOR_ID);
    thermistorArr[2] = new Thermistor(33, HALLUX_THERMISTOR_ID);

    // Create FSR objects
    fsrArr[0] = new FSR(32, METATARSAL_1_FSR_ID);
    fsrArr[1] = new FSR(34, METATARSAL_2_FSR_ID);

    // Create SPO2 objects
    spo2 = new SPO2(4, 5);

    // Create LED objects
    bleLed = new LED(2);

    // Create characteristic objects for sensors (thermistor, force, spo2)
    thermistorCharacteristic = bleManager->createBLECharacteristicForNotify(THERMISTORS_CHARACTERISTIC_UUID);

    bleManager->startService();
    bleManager->startAdvertising();
    // spo2->init(Wire);
}

void encodeThermistorToByteArray(float temp, int id, uint8_t *byteArr) {
    int tempVal = temp * 10; // multiply by 10 to move decimal to the right
    byteArr[0] = id;
    byteArr[1] = (tempVal >> 24) & 0xFF;
    byteArr[2] = (tempVal >> 16) & 0xFF;
    byteArr[3] = (tempVal >> 8) & 0xFF;
    byteArr[4] = (tempVal) & 0xFF;
}

void encodeThermistorData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_THERMISTORS; i++) {
        float temp = thermistorArr[i]->getAverageTemp();
        Serial.print("thermistor temp ");
        Serial.print(thermistorArr[i]->getId());
        Serial.print(": ");
        Serial.println(temp);
        encodeThermistorToByteArray(temp, thermistorArr[i]->getId(), &thermistorByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
    Serial.println(" ");
}

void encodeFSRData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_FSR; i++) {
        fsrArr[i]->getAveragePressure();
        Serial.print("FSR force ");
        Serial.print(fsrArr[i]->getId());
        Serial.print(": ");
        Serial.println(fsrArr[i]->getAveragePressure());
    }
    Serial.println(" ");
}

void loop() {
    // encodeThermistorData();
    // readFSRData();
    for (int i = 0; i < NUM_THERMISTORS; i++) {
        thermistorArr[i]->readData();
    }
    for (int i = 0; i < NUM_FSR; i++) {
        fsrArr[i]->readData();
    }

    unsigned long currMillis = millis();
    if (currMillis - prevMillis >= 2000) {
        prevMillis = currMillis;
        encodeThermistorData();
        encodeFSRData();
        Serial.println(" ");
        Serial.println(" ");
    }
    // spo2->readSensor();

    // if (bleManager->getIsDeviceConnected()) {
    //     bleLed->turnOn();
    //     // readAndEncodeThermistorData();
    //     thermistorCharacteristic->setValue(thermistorByteArr, sizeof(thermistorByteArr));
    //     thermistorCharacteristic->notify();
    // } else {
    //     bleLed->turnOff();
    //     delay(10);
    //     bleLed->turnOn();
    //     delay(10);
    // }
    delay(20);
}