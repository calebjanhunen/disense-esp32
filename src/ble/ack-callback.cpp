#include "ack-callback.h"

AckCallback::AckCallback(Callback deepSleepCallback) {
    this->deepSleepCallback = deepSleepCallback;
    this->resetAcks();
}

void AckCallback::onWrite(BLECharacteristic *pCharacteristic) {
    std::string base64Val = pCharacteristic->getValue();
    Serial.print("Base64 val: ");
    Serial.println(base64Val.data());

    int base64Len = sizeof(base64Val);

    int decodedLen = Base64.decodedLength(&base64Val[0], base64Len);
    char decodedStr[decodedLen];
    Base64.decode(decodedStr, &base64Val[0], base64Len);
    Serial.print("decoded val: ");
    Serial.println(decodedStr);

    if (decodedStr == THERMISTORS_CHARACTERISTIC_UUID) {
        this->ack1 = true;
    } else if (decodedStr == FSR_CHARACTERISTIC_UUID) {
        this->ack2 = true;
    } else if (decodedStr == SPO2_CHARACTERISTIC_UUID) {
        this->ack3 = true;
    }

    if (this->ack1 && this->ack2 && this->ack3) {
        this->resetAcks();
        this->deepSleepCallback();
    }
}

void AckCallback::resetAcks() {
    this->ack1 = false;
    this->ack2 = false;
    this->ack3 = false;
}