#include "main.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek einbinden

LiquidCrystal_I2C lcd(0x27, 16, 2); //Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Für ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)"

double xPos = 0, yPos = 0, headingVel = 0;
uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; //how often to read data from the board
uint16_t PRINT_DELAY_MS = 500;            // how often to print the data
uint16_t printCount = 0;                  //counter to avoid printing every 10MS sample

//velocity = accel*dt (dt in seconds)
//position = 0.5*accel*dt^2
double ACCEL_VEL_TRANSITION = (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0;
double ACCEL_POS_TRANSITION = 0.5 * ACCEL_VEL_TRANSITION * ACCEL_VEL_TRANSITION;
double DEG_2_RAD = 0.01745329251; //trig functions require radians, BNO055 outputs degrees

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

float target = 1.0;
void printEvent(sensors_event_t *event);

void setup()
{
  Serial.begin(115200);
  azAxis.connectToPins(stepPinAz, dirPinAz);
  azAxis.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  azAxis.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  azAxis.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);
  azAxis.setStepsPerMillimeter(STEPS_PER_DEGREE_AZIMUTH);
  azAxis.setStepsPerRevolution(STEPS_PER_REVOLVE);
  azAxis.setCurrentPositionInMillimeters(0.0);
  azAxis.startAsService();
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1)
      ;
  }

  lcd.init(); //Im Setup wird der LCD gestartet
  lcd.backlight();
  lcd.setContrast(255); //Hintergrundbeleuchtung einschalten (lcd.noBacklight(); schaltet die Beleuchtung aus).
  delay(1000);
}

void loop()
{
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  printEvent(&orientationData);
  //Serial.printf("Moving with %f /sec at %f\n",azAxis.getCurrentVelocityInRevolutionsPerSecond(),azAxis.getCurrentPositionInMillimeters());

  if (azAxis.getDistanceToTargetSigned() == 0)
  {
    //Serial.printf("Moving stepper to %2.2f deg\n", target);
    azAxis.setTargetPositionInMillimeters(target);
    if (target >= 10)
    {
      target = 0;
    }
    else
    {
      target++;
    }
  }

  delay(1000);
}

void printEvent(sensors_event_t *event)
{
  double x = -1000000, y = -1000000, z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ORIENTATION)
  {
    x = event->orientation.x;
    y = event->orientation.y;
    z = 180 + event->orientation.z;

    if (z >= 45)
    {
      x -= 180;
    }

    if (z>180){
      z-=360;
    }
    Serial.printf("01;%3.15f;%2.16f\n", x, z);
  }

  lcd.setCursor(0, 0);
  lcd.printf("Azimuth=%3.5f", x);
  lcd.setCursor(0, 1);
  lcd.printf("Altitude=%3.5f", z);
}
