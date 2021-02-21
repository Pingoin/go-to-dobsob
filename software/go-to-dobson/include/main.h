#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <ESP_FlexyStepper.h>

#define stepPinAz 15
#define dirPinAz 2

#define STEPS_PER_REVOLVE 200
#define SPEED_IN_STEPS_PER_SECOND 1500
#define ACCELERATION_IN_STEPS_PER_SECOND 800
#define DECELERATION_IN_STEPS_PER_SECOND 800
#define STEPS_PER_DEGREE_AZIMUTH STEPS_PER_REVOLVE*10

ESP_FlexyStepper azAxis;
double azimuth=0.0, altitude=0.0;


#endif