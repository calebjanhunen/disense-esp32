#include "BLEManager.h"
#include "BLEuuids.h"
#include "FSR.h"
#include "LED.h"
#include "SensorIDs.h"
#include "spo2.h"
#include "thermistor.h"
#include <Arduino.h>
#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <Wire.h>

#define NUM_THERMISTORS 3
#define NUM_FSR 1
#define BYTES_PER_SENSOR 5 // For byte array (1 byte for sensor id, 4 bytes for temp value)

// Sensors
Thermistor *thermistorArr[NUM_THERMISTORS];
FSR *fsr[NUM_FSR];
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

SparkFun_Bio_Sensor_Hub bioHub;
bioData body;

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
    fsr[0] = new FSR(34, FSR_ID);

    // Create SPO2 objects
    spo2 = new SPO2(4, 5);

    // Create LED objects
    bleLed = new LED(2);

    // Create characteristic objects for sensors (thermistor, force, spo2)
    thermistorCharacteristic = bleManager->createBLECharacteristicForNotify(THERMISTORS_CHARACTERISTIC_UUID);

    bleManager->startService();
    bleManager->startAdvertising();
    spo2->init(Wire);
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
        thermistorArr[i]->readTemperature();
        Serial.print("thermistor temp ");
        Serial.print(thermistorArr[i]->getId());
        Serial.print(": ");
        Serial.println(thermistorArr[i]->getTemp());
        // encodeThermistorToByteArray(thermistorArr[i], &thermistorByteArr[byteArrIndex]);
        // byteArrIndex += BYTES_PER_SENSOR;
    }
    Serial.println(" ");
}

void loop() {
    // readAndEncodeThermistorData();
    // fsr[0]->readPressure();
    spo2->readSensor();

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
    delay(250);
}