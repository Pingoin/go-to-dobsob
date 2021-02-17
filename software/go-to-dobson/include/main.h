#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>


void readAccGyro();

uint16_t accX, accY, accZ, gyroX, gyroY, gyroZ, tRaw; // Raw register values (accelaration, gyroscope, temperature)
char result[7];     
#endif