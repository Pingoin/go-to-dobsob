#include "altAzController.h"

altAzController::altAzController(){
}

altAzController::~altAzController()
{}

void altAzController::init(){
    azAxis.connectToPins(stepPinAz, dirPinAz);
    altAxis.connectToPins(stepPinAlt, dirPinAlt);

    azAxis.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
    altAxis.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);

    azAxis.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
    altAxis.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);

    azAxis.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);
    altAxis.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

    azAxis.setStepsPerMillimeter(STEPS_PER_DEGREE_AZIMUTH);
    altAxis.setStepsPerMillimeter(STEPS_PER_DEGREE_ALTITUDE);

    azAxis.setStepsPerRevolution(STEPS_PER_REVOLVE);
    altAxis.setStepsPerRevolution(STEPS_PER_REVOLVE);

    azAxis.setCurrentPositionInMillimeters(0.0);
    altAxis.setCurrentPositionInMillimeters(0.0);

    azAxis.startAsService();
    altAxis.startAsService();
}

bool altAzController::targetReached()
{
    return altAxis.motionComplete() && azAxis.motionComplete();
}

void altAzController::setTarget(double azimuth, double altitude){
    azAxis.setTargetPositionInMillimeters(azimuth);
    altAxis.setTargetPositionInMillimeters(altitude);
}
void altAzController::setPosition(double azimuth, double altitude)
{
    azAxis.setCurrentPositionInMillimeters(azimuth);
    altAxis.setCurrentPositionInMillimeters(altitude);
}

double altAzController::getAzimuth(){
    return azAxis.getCurrentPositionInMillimeters();
}
double altAzController::getAltitude(){
    return altAxis.getCurrentPositionInMillimeters();
}