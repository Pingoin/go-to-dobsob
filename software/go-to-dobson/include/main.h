#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include "altAzController.h"

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

altAzController altAZ;
double azimuth=0.0, altitude=0.0;
BluetoothSerial SerialBT;

#endif