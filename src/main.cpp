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
BLECharacteristic *fsrCharacteristic;
BLECharacteristic *spo2Characteristic;

// byte arrays
uint8_t thermistorByteArr[NUM_THERMISTORS * BYTES_PER_SENSOR]; // 4 bytes per sensor (1 for id, 4 for temperature value -- int = 4 bytes)
uint8_t fsrByteArr[NUM_FSR * BYTES_PER_SENSOR];

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
    fsrCharacteristic = bleManager->createBLECharacteristicForNotify(FSR_CHARACTERISTIC_UUID);

    bleManager->startService();
    bleManager->startAdvertising();
    // spo2->init(Wire);
}

void encodeDataToByteArr(float val, int id, uint8_t *byteArr) {
    int valInteger = val * 10; // multiply by 10 to move decimal to the right
    byteArr[0] = id;
    byteArr[1] = (valInteger >> 24) & 0xFF;
    byteArr[2] = (valInteger >> 16) & 0xFF;
    byteArr[3] = (valInteger >> 8) & 0xFF;
    byteArr[4] = (valInteger) & 0xFF;
}

void encodeThermistorData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_THERMISTORS; i++) {
        float temp = thermistorArr[i]->readTemperature();
        Serial.print("thermistor temp ");
        Serial.print(thermistorArr[i]->getId());
        Serial.print(": ");
        Serial.println(temp);
        encodeDataToByteArr(temp, thermistorArr[i]->getId(), &thermistorByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
    Serial.println(" ");
}

void encodeFSRData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_FSR; i++) {
        float force = fsrArr[i]->readForce();
        Serial.print("FSR force ");
        Serial.print(fsrArr[i]->getId());
        Serial.print(": ");
        Serial.println(force);
        encodeDataToByteArr(force, fsrArr[i]->getId(), &fsrByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
    Serial.println(" ");
}

void loop() {
    if (bleManager->getIsDeviceConnected()) {
        bleLed->turnOn();

        encodeThermistorData();
        encodeFSRData();
        Serial.println(" ");
        Serial.println(" ");
        thermistorCharacteristic->setValue(thermistorByteArr, sizeof(thermistorByteArr));
        thermistorCharacteristic->notify();
        fsrCharacteristic->setValue(fsrByteArr, sizeof(fsrByteArr));
        fsrCharacteristic->notify();
    } else {
        bleLed->turnOff();
        delay(500);
        bleLed->turnOn();
        delay(300);
    }
    delay(1000);
}