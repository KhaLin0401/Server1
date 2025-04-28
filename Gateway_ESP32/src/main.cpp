#include <Arduino.h>
#include "config.h"
#include "dataPush.h"

void setup() {
  Serial.begin(115200);
  internetInit();
}

void loop() {
  wm.process();
  dataPusher("node001", 31.5, 70.2, 3.72);
  delay(10000);
}

