#include <Arduino.h>
#include "TimeManager.h"
#include "Settings.h"

#pragma once

class Light {
    // class to control light with pwm dimming (from settings lBrightness) and on/off based on time (from settings lOnHour/lOffHour)
    // on off hour is using the TimeManager and are hours of the day in local time

    public:
        Light(int pin, TimeManager* timeprovider, uint8_t pwmChannel = 1) : pin_(pin), timeprovider_(timeprovider), pwmChannel_(pwmChannel) {}

        void begin() {
            pinMode(pin_, OUTPUT);

            //init pwm
            ledcSetup(pwmChannel_, 5000, 8); // 5 kHz PWM frequency, 8-bit resolution
            ledcAttachPin(pin_, pwmChannel_);
        }

        void loop() {
            if(timeprovider_->isInBetween(gSettings.lOnHour, gSettings.lOffHour)){
                // turn on light with brightness from settings
                ledcWrite(pwmChannel_, map(gSettings.lBrightness, 0, 100, 0, 255));
            } else {
                // turn off light
                ledcWrite(pwmChannel_, 0);
            }
        }

    private:
        int pin_;
        TimeManager* timeprovider_;
        const uint8_t pwmChannel_;

};