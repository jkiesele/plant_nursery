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


DebugLED debugLED;
Scheduler scheduler;
WiFiWrapper wifi(secret::ssid, secret::password);
TimeManager timeManager;
BasicWebInterface webInterface;
WebOTAUpload webOTAUpload;

CirculationPump pump(5); // example pin for pump control
Light light(4, &timeManager); // example pin for light control
Fan fan(18); // example pin for fan 
HumiditySensor humiditySensor; // placeholder for humidity sensor
HygroControl hygroControl(&fan, &humiditySensor, 19); // example pin for ambient temperature sensor

void setup() {

  SettingsBlockBase::kSettingsPassword = secret::hydroPassword;
  debugLED.begin();
  debugLED.setRed(); // indicate setup start


  systemID.begin(); //global
  systemID.setSystemName("plan-nursery"); 

  webLog.begin();
  Serial.begin(115200);
  setLogger(&webLog); 
  webLog.mirrorToSerial = true; // Enable mirroring to Serial
  delay(1000);
  gLogger->println("Plant nursery: " + systemID.systemName());

  wifi.begin();
  timeManager.begin();
  setTimeProvider(&timeManager); // Set the global time provider to our TimeManager instance

  light.begin();

  //
  gSettings.begin(); // load settings from NVS

  //add web displays if needed
  webInterface.addSettings("Settings", &gSettings);
  webInterface.addWebItem(&webOTAUpload);
  // ...
  webInterface.begin();


  pump.begin();
  fan.begin();


  gLogger->println("Setup completed");
  webOTAUpload.markAppValid(); // prevent rollback on next boot

  debugLED.setOff();
}

void loop() {
  scheduler.loop();
  pump.loop();
  light.loop();
  webInterface.loop();
  hygroControl.loop();
}
