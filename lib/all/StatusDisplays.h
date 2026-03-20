
#pragma once

#include "WebDisplay.h"
#include "HygroControl.h"
#include "CirculationPump.h"


class StatusDisplays {
public:
    StatusDisplays(HumiditySensor& sensor, const CirculationPump& pump, uint8_t connectedPin) : 
    sensor_(sensor), pump_(pump), connectedPin_(connectedPin),
    displayTemperature_("temperature", 10, 0), // update every 10s
    displayHumidity_("humidity", 10, 0) {}

    void begin() {
        //init connected pin as input with pullup
        pinMode(connectedPin_, INPUT_PULLUP);
    }   
    
    void loop() {
        // Update the web display with the latest temperature and humidity readings
        float temperature = sensor_.getTemperature();
        float humidity = sensor_.getHumidity();

        displayTemperature_.update(temperature);
        displayHumidity_.update(humidity);
        // Update connected status based on failsafe pin (e.g. if sensor is disconnected or reading is invalid)
        bool connected = digitalRead(connectedPin_) == LOW; 
        displayConnected_.update(connected);

        // Update pump safe status based on failsafe pin
        bool pumpSafe = !pump_.unsafe();
        displayPumpSafe_.update(pumpSafe);
    }

    //getters for the displays so they can be added to the web interface
    WebDisplay<float>& getTemperatureDisplay() { return displayTemperature_; }
    WebDisplay<float>& getHumidityDisplay() { return displayHumidity_; }
    WebDisplay<bool>& getConnectedDisplay() { return displayConnected_; } // example display for whether sensor is connected (e.g. based on reading validity or failsafe pin)
    WebDisplay<bool>& getPumpSafeDisplay() { return displayPumpSafe_; } // example display for whether pump is in unsafe state (e.g. based on failsafe pin)

private:
    HumiditySensor& sensor_;
    const CirculationPump& pump_;
    uint8_t connectedPin_;
    WebDisplay<float> displayTemperature_;
    WebDisplay<float> displayHumidity_;
    WebDisplay<bool> displayConnected_{"bottom_connected", 10, false}; // example display for whether sensor is connected (e.g. based on reading validity or failsafe pin)
    WebDisplay<bool> displayPumpSafe_{"pump_safe", 10, false}; // example display for whether pump is in unsafe state (e.g. based on failsafe pin)
};