#pragma once

#include <Arduino.h>
#include <Scheduler.h>
#include "Fan.h"
#include <DHTesp.h>
#include "LoggingBase.h"

// class to control the fan based on humidity reading from the humidity sensor (not implemented yet, just a placeholder for now)

class HumiditySensor {
    public:
    HumiditySensor(uint8_t pin) : pin_(pin) {}

    void begin() {
        dht_.setup(pin_, DHTesp::AM2302);
    }
    void loop() {
        //update every 10s, that's enough.
        if (millis() - lastReadTime_ >= 10000) {
            lastReadValues_ = dht_.getTempAndHumidity();
            lastReadTime_ = millis();
        }
    }
    float getHumidity() {
        // placeholder for reading humidity from the sensor
        return lastReadValues_.humidity;
    }
    float getTemperature() {
        // placeholder for reading temperature from the sensor
        return lastReadValues_.temperature;
    }
    private:
    uint8_t pin_;
    DHTesp dht_;
    TempAndHumidity lastReadValues_;
    uint32_t lastReadTime_ = 0;
};


class HygroControl {
    public:
        HygroControl(Fan& fan, HumiditySensor& sensor, int ambientTempPin) : fan_(fan), sensor_(sensor) {}

        void begin() {}

        void loop() { 
            // only trigger every 30s only - enough
            static uint32_t lastTriggerTime = 0;
            if (!(millis() - lastTriggerTime >= 30000)) 
                return;

            lastTriggerTime = millis();
            float humidity = sensor_.getHumidity();
            float temperature = sensor_.getTemperature();
            float ambientTemp = 25; // FIXME 
            if(calculateFanHysteresis(humidity, temperature, ambientTemp))
                fan_.on();
            else
                fan_.off();
        }

        bool calculateFanHysteresis(float humidity, float temperature, float ambientTemp) {
            const float th = gSettings.fTh;
            const float hTh = gSettings.fHTh;
            //turns on at threshold, but runs a bit longer below
            if (fanOn_) {
                if (humidity < th - hTh) {
                    fanOn_ = false;
                }
            } else {
                if (humidity > th) { 
                    fanOn_ = true;
                }
            }
            //some hystersis logic here
            return fanOn_;
        }

    private:
        Fan& fan_;
        bool fanOn_ = false;
        HumiditySensor& sensor_;
};