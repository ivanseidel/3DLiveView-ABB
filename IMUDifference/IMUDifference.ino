// #include <Wire.h>
#include <I2Cdev.h>
#include <Thread.h>
#include <ThreadController.h>
// #include <RHReliableDatagram.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "imu.h"

ThreadController system;

IMUThread mpu1(0x69);
IMUThread mpu2(0x68);

float v1, v2, v3, v4;

long test = 0;
void log(){
  if(!mpu1.newData || !mpu2.newData)
    return;

  mpu1.newData = false;
  mpu2.newData = false;
  // // Serial.print("ypr1\t");
  // // Serial.print(mpu1.ypr[0] * 180/M_PI - mpu2.ypr[0] * 180/M_PI);
  // // Serial.print("\t");
  // // Serial.print(mpu1.ypr[1] * 180/M_PI - mpu2.ypr[1] * 180/M_PI);
  // // Serial.print("\t");
  // // Serial.println(mpu1.ypr[2] * 180/M_PI - mpu2.ypr[2] * 180/M_PI);
  //
  // // Serial.print(mpu1.took);
  // // Serial.print("\t");
  // // Serial.print(mpu2.took);
  // // Serial.print("\t");
  // Serial.print(test++);
  // Serial.print("\t");
  // Serial.print((int)mpu1.newData);
  // Serial.print("\t");
  // Serial.print((int)mpu2.newData);
  // Serial.print("\t");
  // Serial.print(mpu1.fifoCount);
  // Serial.print("\t");
  // Serial.println(mpu2.fifoCount);

  // Relative angle
//  VectorFloat vec1 = VectorFloat(0, 0, 1).getRotated(&mpu1.q);
//  VectorFloat vec2 = VectorFloat(0, 0, 1).getRotated(&mpu2.q);
//  vec1.x = sqrt(vec1.x * vec1.x + vec1.y * vec1.y);
//  vec1.y = 0;
//  vec2.x = sqrt(vec2.x * vec2.x + vec2.y * vec2.y);
//  vec2.y = 0;
//  float angle = (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
//  angle = angle / (vec1.getMagnitude() * vec2.getMagnitude());
//  v3 = acos(angle) * 180 / M_PI;


  v1 = mpu1.ypr[0] * 180/M_PI;
  v2 = mpu1.ypr[1] * 180/M_PI;
  v3 = mpu2.ypr[1] * 180/M_PI - mpu1.ypr[1] * 180/M_PI;
  v4 = mpu2.ypr[2] * 180/M_PI;

  Serial.print(test++ % 100);
  Serial.print(":"); Serial.print(v1);
  Serial.print(":"); Serial.print(v2);
  Serial.print(":"); Serial.print(v3);
  Serial.print(":"); Serial.print(v4);
  Serial.println();
}

Thread logger(log, 1);

void mpu1Interrupt(){
  mpu1.interrupted = true;
}

void mpu2Interrupt(){
  mpu2.interrupted = true;
}

void setup(){
  Serial.begin(115200);
  Serial.println("Starting...");

  unsigned long s;
  unsigned long e;
  s = micros();
  Serial.print(".");
  e = micros();
  Serial.print("took");
  Serial.println(e - s);
  Serial.print("timeout");
  Serial.println(I2Cdev::readTimeout);

  // Fastwire::setup(100, true);
	Wire.begin();
	// TWBR = ((16000000L / 400000L) - 16) / 2;
  TWBR = 12;
  // Fastwire::setup(400, true);
	delay(1);

  mpu1.init();
  mpu2.init();

  // Listen to interrupts
  attachInterrupt(0, mpu1Interrupt, RISING);
  attachInterrupt(1, mpu2Interrupt, RISING);

  system.add(&mpu1);
  system.add(&mpu2);
  system.add(&logger);
}

void loop(){
  system.run();
}
