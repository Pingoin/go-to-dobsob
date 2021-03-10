#pragma once
#include <ESP_FlexyStepper.h>

#define stepPinAz 5
#define dirPinAz 18
#define enPinAz 4

#define stepPinAlt 2
#define dirPinAlt 0
#define enPinAlt 15

#define STEPS_PER_REVOLVE 200
#define SPEED_IN_STEPS_PER_SECOND 1500
#define ACCELERATION_IN_STEPS_PER_SECOND 800
#define DECELERATION_IN_STEPS_PER_SECOND 800

#define STEPS_PER_DEGREE_AZIMUTH STEPS_PER_REVOLVE * 10
#define STEPS_PER_DEGREE_ALTITUDE STEPS_PER_REVOLVE * 10

class altAzController
{
public:
    altAzController();
    ~altAzController();
    void init();
    bool targetReached();
    void setTarget(double azimuth,double altitude);
    double getAltitude();
    double getAzimuth();
    void setPosition(double azimuth, double altitude);
private:
    ESP_FlexyStepper azAxis;
    ESP_FlexyStepper altAxis;
};