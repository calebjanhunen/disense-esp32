#include "my-characteristic-callbacks.h"

MyCharacteristicCallbacks::MyCharacteristicCallbacks(BLEManager *bleManager) {
    this->bleManager = bleManager;
    this->resetAcknowledgments();
}

void MyCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
    if (pCharacteristic->getUUID().toString() == THERMISTORS_CHARACTERISTIC_UUID) {
        this->ack1 = true;
    } else if (pCharacteristic->getUUID().toString() == FSR_CHARACTERISTIC_UUID) {
        this->ack2 = true;
    } else if (pCharacteristic->getUUID().toString() == FSR_CHARACTERISTIC_UUID) {
        this->ack3 = true;
    }

    if (this->ack1 && this->ack2 && this->ack3) {
        this->resetAcknowledgments();
        // enter sleep mode
    }
}

void MyCharacteristicCallbacks::resetAcknowledgments() {
    this->ack1 = false;
    this->ack2 = false;
    this->ack3 = false;
}