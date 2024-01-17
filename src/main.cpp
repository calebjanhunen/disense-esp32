#include "BLEManager.h"
#include "thermistor.h"
#include <Arduino.h>

uint32_t value = 0;
BLEManager *bleManager = new BLEManager();
Thermistor *thermistor1 = new Thermistor(33);

void setup() {
    Serial.begin(115200);
    bleManager->init("Disense");
}

void loop() {
    // notify changed value
    // if (bleManager->getIsDeviceConnected()) {
    //     Serial.println("device connected");
    //     bleManager->sendDataToClient(value);
    //     value++;
    //     delay(1000);
    // }
    float resistance = thermistor1->getCelciusValue();
    // Serial.println(resistance);
    delay(1000);
}