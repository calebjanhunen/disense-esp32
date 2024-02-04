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
    // float reistance = ((1000.0 * 3.3) / voltage) - 1000.0;
    float force = this->fromVoltageToNewtonsUsing10KResistor(voltage);

    // Serial.print("FSR adc: ");
    // Serial.println(fsrADC);
    // Serial.print("Voltage: ");
    // Serial.println(voltage);
    Serial.print("FSR Voltage: ");
    Serial.println(voltage);
    Serial.print("FSR force: ");
    Serial.println(force);
    Serial.println(" ");
    Serial.println(" ");
}

/**
 * @brief Converts voltage to newtons using calibration curve (using 20kOhm resistor)
 * @param [in] voltage - The voltage of the fsr sensor
 * @return The force in newtons
 */
float FSR::fromVoltageToNewtonsUsing20KResistor(float voltage) {
    /**
     * Using calibration curve equation: V = 0.9152ln(F) - 2.5498
     * (V = voltage, F = force in newtons)
     *
     * Solve for V: F=e^(1.09265734265734V)*16.2169613641861
     */
    return exp(1.09265734265734 * voltage) * 16.2169613641861;
}

/**
 * @brief Converts voltage to newtons using calibration curve (using 10kOhm resistor)
 * @param [in] voltage - The voltage of the fsr sensor
 * @return The force in newtons
 */
float FSR::fromVoltageToNewtonsUsing10KResistor(float voltage) {
    /**
     * Using calibration curve equation: V = 1.3714ln(F) - 6.4288
     * (V = voltage, F = force in newtons)
     *
     * Solve for V: F=e^(0.729181857955374V)*108.610091681168
     */
    return exp(0.729181857955374 * voltage) * 108.610091681168;
}