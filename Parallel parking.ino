#include <Wire.h>
#include <MPU6050_tockn.h>
#include <NewPing.h>

#define I2C_SLAVE_ADDR 0x04

NewPing sonar(19, 33, 400);

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(9600);  
  Wire.begin();
  mpu6050.begin();
  delay(100);
  mpu6050.calcGyroOffsets(true);
  delay(100);
}

void loop() { 
  displayAngleZ();
  displayDistance();
  sendtoSlave(160, 160, 87);
  delay(1000);

  long HalfTurn = mpu6050.getAngleZ() + 180;

  do {
    sendtoSlave(160, 160, 40);
    displayAngleZ();
    displayDistance();
    delay(500);
  }
  while (mpu6050.getAngleZ() < HalfTurn);

  while (sonar.ping_cm() > 10 || sonar.ping_cm() == 0) {
    sendtoSlave(-160, -160, 90);
    displayAngleZ();
    displayDistance();
    delay(500);
  }

    sendtoSlave(0, 0, 90);
    long QuarterTurn = mpu6050.getAngleZ() - 90;
    displayAngleZ();
    displayDistance();

    do {
      sendtoSlave(160, 160, 140);
      displayAngleZ();
      displayDistance();
      Serial.print(QuarterTurn);
    }
    while (mpu6050.getAngleZ() > QuarterTurn);

  while (sonar.ping_cm() > 10 || sonar.ping_cm() == 0) {
    sendtoSlave(-160, -160, 90);
    displayAngleZ();
    displayDistance();
    delay(500);
  }
 
  while (1) {
    sendtoSlave(0, 0, 90);
    displayAngleZ();
    displayDistance();
    delay(500);
  }
}

void sendtoSlave(int leftMotor, int rightMotor, int steeringAngle) {
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write((byte)((leftMotor & 0x0000FF00) >> 8));
  Wire.write((byte)(leftMotor & 0x000000FF));
  Wire.write((byte)((rightMotor & 0x0000FF00) >> 8));
  Wire.write((byte)(rightMotor & 0x000000FF));
  Wire.write((byte)((steeringAngle & 0x0000FF00) >> 8));
  Wire.write((byte)(steeringAngle & 0x000000FF));
  Wire.endTransmission();
}

void displayAngleZ () {
  mpu6050.update();
  delay(100);
  Serial.print("\nZ-axis rotation: ");
  Serial.print(mpu6050.getAngleZ());
}

void displayDistance () {
  Serial.print("Distance = ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");
}