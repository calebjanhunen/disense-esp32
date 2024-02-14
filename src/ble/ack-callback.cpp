#include "ack-callback.h"

AckCallback::AckCallback() {
    this->deepSleepCallback = deepSleepCallback;
    this->resetAcks();
}

void AckCallback::onWrite(BLECharacteristic *pCharacteristic) {
    std::string val = pCharacteristic->getValue();
    Serial.print("Callback for ackknowledgment");
    Serial.println(val.c_str());

    if (val == "thermistor") {
        this->ack1 = true;
        Serial.println("Thermistor");
    } else if (val == "fsr") {
        this->ack2 = true;
        Serial.println("FSR");
    } else if (val == SPO2_CHARACTERISTIC_UUID) {
        this->ack3 = true;
    }

    // if (this->ack1 && this->ack2) {
    //     this->resetAcks();
    //     this->deepSleepCallback();
    // }
}

void AckCallback::resetAcks() {
    this->ack1 = false;
    this->ack2 = false;
    this->ack3 = false;
}