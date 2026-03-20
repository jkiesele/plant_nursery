#include "WebDisplay.h"
#include "HygroControl.h"

#pragma once

class StatusDisplays {
public:
    StatusDisplays(HumiditySensor& sensor) : 
    sensor_(sensor),
    displayTemperature_("temperature", 10, 0), // update every 10s
    displayHumidity_("humidity", 10, 0) {}
    
    void loop() {
        // Update the web display with the latest temperature and humidity readings
        float temperature = sensor_.getTemperature();
        float humidity = sensor_.getHumidity();

        displayTemperature_.update(temperature);
        displayHumidity_.update(humidity);
    }

    //getters for the displays so they can be added to the web interface
    WebDisplay<float>& getTemperatureDisplay() { return displayTemperature_; }
    WebDisplay<float>& getHumidityDisplay() { return displayHumidity_; }
    

private:
    HumiditySensor& sensor_;
    WebDisplay<float> displayTemperature_;
    WebDisplay<float> displayHumidity_;
};