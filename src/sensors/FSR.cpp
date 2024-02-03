#include "FSR.h"

/**
 * Connection steps:
 *  https://www.geeksforgeeks.org/force-sensitive-resistor-fsr-with-arduino/
 */

FSR::FSR(byte pin, int id) {
    this->pin = pin;
    this->id = id;
}

void FSR::readPressure() {
    int fsrADC = analogRead(this->pin);
    float voltage = ((float(fsrADC) * 3.3) / 4095);
    float reistance = ((1000.0 * 3.3) / voltage) - 1000.0;

    Serial.print("FSR adc: ");
    Serial.println(fsrADC);
    Serial.print("Voltage: ");
    Serial.println(voltage);
    Serial.print("FSR resistance: ");
    Serial.println(reistance);
}