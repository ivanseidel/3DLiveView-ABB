// #include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

#include "imu.h"

IMUThread::IMUThread(uint8_t adr = 0x68): Thread(), mpu(adr){
  q = Quaternion();
  gravity = VectorFloat();
  ypr[0] = 0;
  ypr[1] = 0;
  ypr[2] = 0;
  mpu.devAddr = adr;
}


bool IMUThread::init(){
	bool devStatus;

	Serial.println("IMU::init");
	mpu.initialize();

	if(!mpu.testConnection()){
		Serial.println("MPU6050 connection failed");
		return false;
	}

	mpu.setDMPEnabled(false);
	mpu.resetDMP();
	// Reset IMU
	mpu.reset();
	delay(50);

	// Activate MPU6050
	mpu.setSleepEnabled(false);

	// load and configure the DMP
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(83);
	mpu.setYGyroOffset(-3);
	mpu.setZGyroOffset(48);
	mpu.setZAccelOffset(1974); // 1688 factory default for my test chip

	// make sure it worked (returns 0 if so)
	if (devStatus != 0) {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print("DMP Initialization failed (code ");
		Serial.print(devStatus);
		Serial.println(")");
	}

	dmpEnabled = false;

	return true;
}

bool IMUThread::shouldRun(unsigned long time){
	if(enabled != dmpEnabled){
		Serial.println(enabled ? "IMU ON" : "IMU OFF");
		mpu.setDMPEnabled(enabled);
		dmpEnabled = enabled;
	}

	if(!Thread::shouldRun(time))
		return false;

  // Check for interrupt
  if(!interrupted && fifoCount < 42)
    return false;

  // Clear interrupt flag
  interrupted = false;

	// Check for new Packet
  delayMicroseconds(500);
	interruptStatus = mpu.getIntStatus();
  delayMicroseconds(50);
	fifoCount = mpu.getFIFOCount();

  if ((interruptStatus & 0x10) || fifoCount == 1024 || fifoCount % 42 > 0) {
    // reset so we can continue cleanly
    delayMicroseconds(50);
    mpu.resetFIFO();
    delayMicroseconds(50);
    //Serial.println(F("FIFO overflow!"));
    fifoCount = 0;
    // Serial.println("|");
    return false;
  }

  bool run = (interruptStatus & 0x02);
  // if(run)
    // Serial.print(".");
  // else
    // Serial.println(")");
  return run;
	// return (fifoCount) >= 42;
}

int outOfSyncs = 0;
void IMUThread::run(){
  unsigned long start = millis();
	// if(fifoCount % 42){
	// 	Serial.print("Fifo out sync ");
	// 	Serial.println(fifoCount);
	// }

	// if (interruptStatus & 0x02) {

  // wait for correct available data length, should be a VERY short wait
  // while (fifoCount < 42) fifoCount = mpu.getFIFOCount();
  // Serial.print(".");
  runned();

	// read a packet from FIFO
  delayMicroseconds(50);
	mpu.getFIFOBytes(packet, 42);
  fifoCount -= 42;
  // Serial.print(".");

	// mpu.resetFIFO();

	// Convert and save state to Object
	mpu.dmpGetQuaternion(&q, packet);
	mpu.dmpGetGravity(&gravity, &q);
	mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  // Serial.println(".");

	newData = true;

  took = millis() - start;
	// }

	// if (fifoCount >= 42 * 3 || interruptStatus & 0x10) {
	// 	// reset so we can continue cleanly
	// 	mpu.resetFIFO();
	// }
}
