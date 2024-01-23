#include "thermistor.h"

Thermistor::Thermistor(byte pin, const int id) {
    this->pin = pin;
    this->id = id;
}

int Thermistor::getId() {
    return this->id;
}

/**
 * @brief Reads voltage value from analog pin and converts it to a temperature value (in celcius).
 * @return The temperature of the thermistor in celcius.
 */
float Thermistor::getCelciusValue() {
    int adcVoltage = this->readFromADCPin();
    float thermistorVoltage = this->fromADCReadingToVoltage(adcVoltage);
    float thermistorResistance = this->fromVoltageToResistance(thermistorVoltage);
    float celsiusValue = this->fromResistanceToCelsius(thermistorResistance);

    // round to nearest 10th decimal place
    celsiusValue = round(celsiusValue * 10) / 10;

    this->tempVal = celsiusValue;
    return this->tempVal;
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
    return (this->supplyVoltage / float(adcMaxVal)) * float(adcVoltage);
}

/**
 * @brief Converts the thermistor voltage to resistance
 * @param [in] voltage - The voltage read from the analog pin
 * @return The resistance of the thermistor.
 */
float Thermistor::fromVoltageToResistance(float voltage) {
    // Using voltage divider formula
    float r_fixed = 10000.0; // resistor used in voltage divider circuit
    return r_fixed * voltage / (this->supplyVoltage - voltage);
}

/**
 * @brief Converts the thermistor resistance to temperature in celsius
 * @param [in] resistance - The restiance of the thermistor
 * @return The temperature in celsius.
 */
float Thermistor::fromResistanceToCelsius(float resistance) {
    // Using steinhart-hart equation
    float T0 = 298.15; // reference temperature in Kelvin (25C = 298.15K)
    float B = 3950;    // b parameter found in datasheet of thermistor
    float R0 = 10000;  // thermistor resistance at T0 (usually 10k ohms)

    // Get kelvin temp value
    float tempInKelvin = 1 / ((1 / T0) + ((1 / B) * log(resistance / R0)));

    // convert kelvin to celsius
    return tempInKelvin - 273.15;
}