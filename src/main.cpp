#include "BLEManager.h"
#include "BLEuuids.h"
#include "FSR.h"
#include "LED.h"
#include "SensorIDs.h"
#include "byte-array-encoder.h"
#include "spo2.h"
#include "thermistor.h"
#include <Arduino.h>
#include <Wire.h>

#define NUM_THERMISTORS 3
#define NUM_FSR 2
#define NUM_SPO2 1
#define BYTES_PER_SENSOR 5 // For byte array (1 byte for sensor id, 4 bytes for temp value)

// Sensors
Thermistor *thermistorArr[NUM_THERMISTORS];
FSR *fsrArr[NUM_FSR];
SPO2 *spo2Arr[NUM_SPO2];

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
uint8_t spo2ByteArr[NUM_SPO2 * BYTES_PER_SENSOR];

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

    // Initialize SPO2 objects
    for (int i = 0; i < NUM_SPO2; i++) {
        spo2Arr[i] = new SPO2(SPO2_ID, 4, 5);
        spo2Arr[i]->init(Wire);
    }

    // Create LED objects
    bleLed = new LED(2);

    // Create characteristic objects for sensors (thermistor, force, spo2)
    thermistorCharacteristic = bleManager->createBLECharacteristicForNotify(THERMISTORS_CHARACTERISTIC_UUID);
    fsrCharacteristic = bleManager->createBLECharacteristicForNotify(FSR_CHARACTERISTIC_UUID);
    spo2Characteristic = bleManager->createBLECharacteristicForNotify(SPO2_CHARACTERISTIC_UUID);

    bleManager->startService();
    bleManager->startAdvertising();
}

void readAndEncodeThermistorData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_THERMISTORS; i++) {
        float temp = thermistorArr[i]->readTemperature();
        encodeDataToByteArr(temp, thermistorArr[i]->getId(), &thermistorByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
    Serial.println(" ");
}

void readAndEncodeFSRData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_FSR; i++) {
        float force = fsrArr[i]->readForce();
        encodeDataToByteArr(force, fsrArr[i]->getId(), &fsrByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
    Serial.println(" ");
}

void readAndEncodeSPO2Data() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_SPO2; i++) {
        bioData data = spo2Arr[i]->readSensor();
        encodeSPO2DataToByteArr(spo2Arr[i]->getId(), data, &spo2ByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
}

void loop() {
    if (bleManager->getIsDeviceConnected()) {
        bleLed->turnOn();

        readAndEncodeThermistorData();
        readAndEncodeFSRData();
        readAndEncodeSPO2Data();
        for (int i = 0; i < sizeof(spo2ByteArr); i++) {
            Serial.print(spo2ByteArr[i]);
        }
        Serial.println(" ");
        Serial.println(" ");
        thermistorCharacteristic->setValue(thermistorByteArr, sizeof(thermistorByteArr));
        thermistorCharacteristic->notify();
        fsrCharacteristic->setValue(fsrByteArr, sizeof(fsrByteArr));
        fsrCharacteristic->notify();
        spo2Characteristic->setValue(spo2ByteArr, sizeof(spo2ByteArr));
        spo2Characteristic->notify();
    } else {
        bleLed->turnOff();
        delay(500);
        bleLed->turnOn();
        delay(300);
    }
    delay(1000);
}