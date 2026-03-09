#include <Arduino.h>
#include <WebSettings.h>

#pragma once

class Settings: public SettingsBlockBase {
    public:
        Settings() : SettingsBlockBase("plant_nursery", "/settings") {}

        /* light settings */
        DEF_SETTING(int, lOnHour, "Light on hour (0-23)", 6, 1);
        DEF_SETTING(int, lOffHour, "Light off hour (0-23)", 23, 1);
        DEF_SETTING(int, lBrightness, "Light brightness (%)", 80, 1);

        /* circulation pump settings */
        DEF_SETTING(int, pOnSec, "Pump on seconds", 60, 1);
        DEF_SETTING(int, pOffSec, "Pump off seconds", 60, 1);
        DEF_SETTING(int, pDuty, "Pump duty cycle (%)", 50, 1);

};

extern Settings gSettings;