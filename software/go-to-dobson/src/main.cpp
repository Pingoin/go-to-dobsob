#include "main.h"
float target = 1.0;
void setup()
{
  Serial.begin(9600);
  azAxis.connectToPins(stepPinAz, dirPinAz);
  azAxis.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  azAxis.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  azAxis.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);
  azAxis.setStepsPerMillimeter(STEPS_PER_DEGREE_AZIMUTH);
  azAxis.setStepsPerRevolution(STEPS_PER_REVOLVE);
  azAxis.setCurrentPositionInMillimeters(0.0);
  azAxis.startAsService();
  Wire.begin();
}

void loop()
{
  Serial.printf("Moving with %f /sec at %f\n",azAxis.getCurrentVelocityInRevolutionsPerSecond(),azAxis.getCurrentPositionInMillimeters());

  if (azAxis.getDistanceToTargetSigned() == 0)
  {
    Serial.printf("Moving stepper to %2.2f deg\n", target);
    azAxis.setTargetPositionInMillimeters(target);
    if (target>=10){
      target=0;
    }else{
      target++;
    }
  }

  delay(1000);
}