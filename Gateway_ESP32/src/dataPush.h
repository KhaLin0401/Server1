#ifndef DATAPUSH_H
#define DATAPUSH_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

extern WiFiManager wm;

void internetInit();

void dataPusher(const String &sensorId, float temp, float humi, float voltage);

#endif