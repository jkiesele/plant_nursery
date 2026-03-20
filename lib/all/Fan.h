#pragma once

#include <Arduino.h>
#include "Settings.h"
#include "LoggingBase.h"

class Fan {
public:
    Fan(int pin)
        : pin_(pin){}

    void begin() {
        pinMode(pin_, OUTPUT);
    }

    void on(){
        digitalWrite(pin_, HIGH);
    }
    void off(){
        digitalWrite(pin_, LOW);
    }

    void loop() {}

private:
    int pin_;
};