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
    float voltage = (float(fsrADC) / float(4095)) * 3.3;
    // float resistance = ((1000.0 * 3.3) / voltage) - 1000.0;
    float resistance = 1000.0 * ((3.3 - voltage) / voltage);
    // float force = this->fromResistanceToNewtonsUsing10kResistor(resistance);
    float force = this->fromResistanceToNewtonsUsing1kResistor(resistance);
    float kPa = (force / 169.6) * 1000;

    Serial.print("FSR adc: ");
    Serial.println(fsrADC);
    Serial.print("FSR resistance: ");
    Serial.println(resistance);
    Serial.print("FSR Voltage: ");
    Serial.println(voltage);
    Serial.print("FSR force: ");
    Serial.println(force);
    Serial.print("FSR pressure (kPa): ");
    Serial.println(kPa);
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
float FSR::fromResistanceToNewtonsUsing10kResistor(float resistance) {
    /**
     * Using calibration curve equation: V = 1.3714ln(F) - 6.4288
     * (V = voltage, F = force in newtons)
     *
     * Solve for V: F=e^(0.729181857955374V)*108.610091681168
     *
     * y=6055.1x^-0.399
     */
    // if (resistance > 38000) {
    //     return 0.0;
    // }

    // return 6055.1 * pow(resistance, -0.399);
    return 31912 - 30634.6 * pow(resistance, 0.0038987);
}

float FSR::fromResistanceToNewtonsUsing1kResistor(float resistance) {
    return 7771.06 - 6435.79 * pow(resistance, 0.0127751);
}