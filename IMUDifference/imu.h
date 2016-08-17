// #include <Wire.h>
#include <Thread.h>
#include <MPU6050_6Axis_MotionApps20.h>

#ifndef IMUThread_H
#define IMUThread_H

class IMUThread: public Thread{

protected:

	// Status of DMP (Enabled/Not enabled)
	bool dmpEnabled;

	// Last packed received
	uint8_t packet[84];


public:
  MPU6050 mpu;
  uint8_t fifoCount;
  int interruptStatus;
  unsigned long took;
  bool interrupted;
	bool newData;

	IMUThread(uint8_t adr);

	bool init();

	// Return if the Thread should be runned or not
	bool shouldRun(unsigned long time);

	// run() Method is overrided
	void run();

	Quaternion q;
	VectorFloat gravity;
	float ypr[3];
};

extern IMUThread imu;

#endif
