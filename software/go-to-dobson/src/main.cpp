#include <Arduino.h>
#include "Wire.h"
#include "main.h"

#define MPU6050_ADDR 0x68 // Alternatively set AD0 to HIGH  --> Address = 0x69
#define HMC5883L_Addr 0x0d

void setup()
{
  Serial.begin(9600);
  Wire.begin(21, 22);
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1a);
  Wire.write(6);
  Wire.write(0x1c);
  Wire.write(0);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // wake up!
  Wire.endTransmission(true);
}
void loop()
{
  readAccGyro();
  double alt = 90 - atan(accZ / sqrt(accY * accY + accX * accX + 0.0)) / PI * 180;
  Serial.printf("AcX = %5d", accX);
  Serial.printf(" | AcY = %5d", accY);
  Serial.printf(" | AcZ = %5d", accZ);
  // from data sheet:
  Serial.printf(" | tmp = %2.2f", (tRaw + 12412.0) / 340.0);
  Serial.printf(" | GyX = %5d", gyroX);
  Serial.printf(" | GyY = %5d", gyroY);
  Serial.printf(" | GyZ = %5d", gyroZ);
  Serial.printf(" | alt = %3d deg %2d ' %2d ''", int(alt), int(alt * 60) % 60, int(alt * 3600) % 60);
  Serial.println();

  delay(1000);
}

void readAccGyro()
{
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);                         // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);              // the parameter indicates that the Arduino will send a restart.
                                            // As a result, the connection is kept active.
  Wire.requestFrom((uint16_t) MPU6050_ADDR,(uint8_t) 14, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same int16_t variable
  accX = Wire.read() << 8 | Wire.read();  // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accY = Wire.read() << 8 | Wire.read();  // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accZ = Wire.read() << 8 | Wire.read();  // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  tRaw = Wire.read() << 8 | Wire.read();  // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyroX = Wire.read() << 8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyroY = Wire.read() << 8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyroZ = Wire.read() << 8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
}