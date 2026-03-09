#include <Arduino.h>
#include <Scheduler.h>
#include "Fan.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// class to control the fan based on humidity reading from the humidity sensor (not implemented yet, just a placeholder for now)

class HumiditySensor {
    public:
    HumiditySensor() {}
    void begin() {
    }
    float readHumidity() {
        // placeholder for reading humidity from the sensor
        return 50; // return a dummy humidity value
    }
    float readTemperature() {
        // placeholder for reading temperature from the sensor
        return 25; // return a dummy temperature value
    }
};


class HygroControl {
    public:
        HygroControl(Fan* fan, HumiditySensor* sensor, int ambientTempPin) : fan_(fan), sensor_(sensor), oneWire(ambientTempPin), ext_temp_sensor(&oneWire) {}

        void begin() {
            sensor_->begin();
            ext_temp_sensor.begin();
        }

        void loop() { 
            // only trigger every 1 minute to avoid too frequent changes
            static uint32_t lastTriggerTime = 0;
            if (!(millis() - lastTriggerTime >= 60000)) 
                return;

            lastTriggerTime = millis();
            float humidity = sensor_->readHumidity();
            float temperature = sensor_->readTemperature();
            float ambientTemp = ext_temp_sensor.getTempCByIndex(0);
            uint8_t dutyCycle = calculateFanDutyCycle(humidity, temperature, ambientTemp);
            fan_->setDutyCycle(dutyCycle);
        }

        uint8_t calculateFanDutyCycle(float humidity, float temperature, float ambientTemp) {
            // simple logic to calculate fan duty cycle based on humidity and temperature
            // this can be improved with more complex logic or machine learning models
            if (humidity > 70 || (temperature > 30 && ambientTemp < 30)) {
                return 100; // max duty cycle
            } else if (humidity > 50 || (temperature > 25 && ambientTemp < 25)) {
                return 50; // medium duty cycle
            } else {
                return 0; // turn off fan
            }
        }

    private:
        Fan* fan_;
        HumiditySensor* sensor_;
        OneWire oneWire;
        DallasTemperature ext_temp_sensor;
};