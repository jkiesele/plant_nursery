#include <Arduino.h>
#include "Settings.h"
// simple pump on pin with PWM duty cycle and on/off times, using simple state machine in main loop

#pragma once

class CirculationPump {
    public:
        CirculationPump(int pin, uint8_t pwmChannel = 0) : pin_(pin), pwmChannel_(pwmChannel) {}

        void begin() {
            pinMode(pin_, OUTPUT);

            //init pwm
            ledcSetup(pwmChannel_, 5000, 8); // 5 kHz PWM frequency, 8-bit resolution
            ledcAttachPin(pin_, pwmChannel_);
        }

        void setDutyCycle(int duty) {
            dutyCycle_ = constrain(duty, 0, 100);
        }

        void turnOn() {
            ledcWrite(pwmChannel_, map(dutyCycle_, 0, 100, 0, 255));
            isOn_ = true;
        }

        void turnOff() {
            ledcWrite(pwmChannel_, 0);
            isOn_ = false;
        }

        void loop() {
            uint32_t currentTime = millis();
            //simple state machine logic to turn pump on/off based on time in gSettings (ok if in main loop and sync web server since it's fast and non-blocking)
            uint32_t onDuration = gSettings.pOnSec * 1000;
            uint32_t offDuration = gSettings.pOffSec * 1000;
            if (isOn_) {
                if (currentTime - lastToggleTime_ >= onDuration) {
                    turnOff();
                    lastToggleTime_ = currentTime;
                }
            } else {
                if (currentTime - lastToggleTime_ >= offDuration) {
                    turnOn();
                    lastToggleTime_ = currentTime;
                }
            }
        }

    private:
        int pin_;
        int dutyCycle_ = 50; // default to 50%
        const uint8_t pwmChannel_;
        bool isOn_ = false;
        uint32_t lastToggleTime_ = 0;
};