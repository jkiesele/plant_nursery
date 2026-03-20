#include <Arduino.h>
#include <Scheduler.h>
#include <WebOTAUpload.h>
#include <BasicWebInterface.h>
#include <SystemID.h>
#include <WiFiWrapper.h>
#include <passwords.h>
#include <WebLog.h>
#include <TimeManager.h>
#include "Settings.h"
#include "DebugLED.h"
#include "CirculationPump.h"
#include "Light.h"
#include "Fan.h"
#include "HygroControl.h"
#include "StatusDisplays.h"


WiFiWrapper wifi(secret::ssid, secret::password);
TimeManager timeManager;
BasicWebInterface webInterface;
WebOTAUpload webOTAUpload;

CirculationPump pump(4, 16, 0); //  pwn 0 ; pin for pump control, and failsafe pin (e.g. water level sensor)
Light light(3, &timeManager, 1); // pwm 1 example pin for light control
Fan fan(10); // pin 10 for fan 
HumiditySensor humiditySensor(12); // placeholder for humidity sensor

HygroControl hygroControl(fan, humiditySensor, 18); // example pin for ambient temperature sensor

StatusDisplays statusDisplays(humiditySensor);

void setup() {

  SettingsBlockBase::kSettingsPassword = secret::hydroPassword;

  delay(100); // small delay to make sure power is stable
  systemID.begin(); //global
  systemID.setSystemName("plan-nursery"); 

  webLog.begin();
  setLogger(&webLog); 
  gLogger->println("Plant nursery: " + systemID.systemName());

  wifi.begin();
  timeManager.begin();
  setTimeProvider(&timeManager); // Set the global time provider to our TimeManager instance

  light.begin();

  // Initialize settings
  gSettings.begin(); // load settings from NVS

  //add web displays if needed
  webInterface.addSettings("Settings", &gSettings);
  webInterface.addWebItem(&webOTAUpload);
  webInterface.addDisplay("Temperature", &statusDisplays.getTemperatureDisplay());
  webInterface.addDisplay("Humidity", &statusDisplays.getHumidityDisplay());
  // ...
  webInterface.begin();

  
  pump.begin();
  fan.begin();
  humiditySensor.begin();
  hygroControl.begin();

  gLogger->println("Setup completed");
  webOTAUpload.markAppValid(); // prevent rollback on next boot

}

void loop() {
  wifi.loop();
  timeManager.loop(); //empty but for compatibility
  pump.loop();
  light.loop();
  fan.loop();
  statusDisplays.loop();
  webInterface.loop();
  humiditySensor.loop();
  hygroControl.loop();
}
