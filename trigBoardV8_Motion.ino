#include "includes.h"

const char fwVersion[] = "8/06/20_Motion";
const int PIRpin = 39;


void setup() {
  pinMode(ESPlatchPin, OUTPUT);
  digitalWrite(ESPlatchPin, HIGH);
  pinMode(LEDpin, OUTPUT);

  Serial.begin(115200);
  checkWakeupPins();



  loadConfiguration(filename, config);
  rtcInit(config.timerCountDown, false);
  Serial.println(getBattery(), 2);



  if (pushLogic()) { //decide if push will occur or nt and what message will be
    if (wiFiNeeded) {
      if (connectWiFi()) {
        pushOver();
        pushSafer();
        ifttt();
        mqtt();
      }
    }
    udp();
  }


  killPower();
  adcAttachPin(PIRpin);
  while (analogRead(PIRpin) > 500) { //wait for PIR to settle
    int PIRread = analogRead(PIRpin);
    Serial.println(PIRread);
    delay(10);
  }
  waitForButton();
  initBluetooth();
}

void loop() {
  if (!OTAsetup)
    serviceBluetooth();
  else
    checkOTA();
}
