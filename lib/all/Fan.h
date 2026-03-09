#include <Arduino.h>
//simple fan class with duty cycle via pwm; will be controlled externally

#pragma once

class Fan {
    public:
        Fan(int pin, uint8_t pwmChannel = 2) : pin_(pin), pwmChannel_(pwmChannel) {}

        void begin() {
            pinMode(pin_, OUTPUT);

            //init pwm
            ledcSetup(pwmChannel_, 5000, 8); // 5 kHz PWM frequency, 8-bit resolution
            ledcAttachPin(pin_, pwmChannel_);
        }

        void setDutyCycle(int duty) {
            dutyCycle_ = constrain(duty, 0, 100);
            ledcWrite(pwmChannel_, map(dutyCycle_, 0, 100, 0, 255));
        }

    private:
        int pin_;
        int dutyCycle_ = 50; // default to 50%
        const uint8_t pwmChannel_;
};