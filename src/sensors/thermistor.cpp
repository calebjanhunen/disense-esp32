#include "thermistor.h"

Thermistor::Thermistor(byte pin) {
    this->pin = pin;
}

/**
 * @brief Reads voltage value from analog pin and converts it to a temperature value (in celcius).
 * @return The temperature of the thermistor in celcius.
 */
float Thermistor::getCelciusValue() {
    int adcVoltage = this->readFromADCPin();
    float thermistorVoltage = this->fromADCReadingToVoltage(adcVoltage);
    Serial.print("sup Voltage: ");
    Serial.println(this->supplyVoltage);
    Serial.print("Voltage: ");
    Serial.println(thermistorVoltage);
    float thermistorResistance = this->fromVoltageToResistance(thermistorVoltage);
    Serial.print("Resistance: ");
    Serial.println(thermistorResistance);
    return thermistorResistance;
}

/**
 * @brief Reads from ADC pin to get adc reading
 * @return The voltage reading as an ADC reading (0-4095).
 */
int Thermistor::readFromADCPin() {
    return analogRead(this->pin);
}

/**
 * @brief Converts the adc voltage read from analog pin to actual voltage of thermistor
 * @param [in] adcVoltage - The voltage value read from adc
 * @return The actual voltage value of the thermistor
 */
float Thermistor::fromADCReadingToVoltage(int adcVoltage) {
    int adcMaxVal = 4095; // For 12-bit ADC, max value is 4095
    return (float(adcVoltage) / float(adcMaxVal)) * this->supplyVoltage;
}

/**
 * @brief Converts the thermistor voltage to resistance
 * @param [in] voltage - The voltage read from the analog pin
 * @return The resistance of the thermistor.
 */
float Thermistor::fromVoltageToResistance(float voltage) {
    // Using voltage divider formula
    int r_fixed = 10000; // resistor used in voltage divider circuit
    return r_fixed * ((this->supplyVoltage / voltage) - 1);
}