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

unsigned long sendTime;

float sensorAlt, sensorAzimuth;
uint8_t calSys, calMag, calAcc, calGyro;
bool stepperInit = false;
void setup()
{
  Serial.begin(115200);
  altAZ.init();
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1)
      ;
  }
  delay(100);
  SerialBT.begin("ESP32test");
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  printEvent(&orientationData);

  lcd.init(); //Im Setup wird der LCD gestartet
  lcd.backlight();
  lcd.setContrast(255); //Hintergrundbeleuchtung einschalten (lcd.noBacklight(); schaltet die Beleuchtung aus).
  delay(1000);
}

void loop()
{

  if (millis() - sendTime >= 500)
  {
    sensors_event_t orientationData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    printEvent(&orientationData);
    sendTime = millis();
  }
  if (Serial.available())
  {
    String tmp = Serial.readStringUntil('\n');
    interpretComData(tmp);
  }

  if (SerialBT.available())
  {
    String tmp = SerialBT.readStringUntil('\n');
    interpretComData(tmp);
  }
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

    if (z > 180)
    {
      z -= 360;
    }
  }
  sensorAzimuth = x;
  sensorAlt = z;
  if (!stepperInit)
  {
    altAZ.setPosition(sensorAzimuth, sensorAlt);
    altAZ.setTarget(sensorAzimuth, sensorAlt);
    stepperInit = true;
  }
  bno.getCalibration(&calSys, &calMag, &calAcc, &calGyro);
}

void interpretComData(String input)
{
  int command = input.substring(0, 2).toInt();
  switch (command)
  {
  case 0:
    Serial.printf("00;%d;%d;%d;%d\n", calSys, calMag, calAcc, calGyro);
    SerialBT.printf("00;%d;%d;%d;%d\n", calSys, calMag, calAcc, calGyro);
    break;
  case 1:
    Serial.printf("01;%+018.13f;%+018.14f\n", altAZ.getAzimuth(), altAZ.getAltitude());
    SerialBT.printf("01;%+018.13f;%+018.14f\n", altAZ.getAzimuth(), altAZ.getAltitude());
    break;
  case 2:
  {
    double azNew = input.substring(3, 21).toDouble();
    double altNew = input.substring(22, 41).toDouble();
    altAZ.setTarget(azNew, altNew);
    Serial.printf("02\n");
    SerialBT.printf("02\n");
    break;
  }
  case 3:
    Serial.printf("03;%+018.13f;%+018.14f\n", sensorAzimuth, sensorAlt);
    SerialBT.printf("03;%+018.13f;%+018.14f\n", sensorAzimuth, sensorAlt);
    break;
  case 4:
    altAZ.setPosition(sensorAzimuth, sensorAlt);
    altAZ.setTarget(sensorAzimuth, sensorAlt);
    Serial.printf("04\n");
    SerialBT.printf("04\n");
    break;
  default:
    break;
  }
}