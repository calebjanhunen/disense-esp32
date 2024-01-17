#include "LED.h"

LED::LED(byte pin) {
    this->pin = pin;
}

void LED::init() {
    pinMode(this->pin, OUTPUT);
}

void LED::turnOn() {
    digitalWrite(this->pin, HIGH);
}

void LED::turnOff() {
    digitalWrite(this->pin, LOW);
}