#pragma once

#include <frc/WPILib.h>
using namespace frc;

// ==========================================================================

class OI {
private:
	Joystick* driverJoystick;
	Joystick* driverJoystick2;

public:
	OI();

	float GetRightTrigger();
	float GetLeftTrigger();
	float GetRightTrigger2();
	float GetLeftTrigger2();
	float GetJoystickX();
	float GetJoystickX2();
	float GetJoystickY2();
	float GetJoystickY();
	float GetJoystickZ();
	bool GetButtonStart();
	bool GetButtonA();
	bool GetButtonTrig();
	bool GetButton2();
	bool GetButton6();
	bool GetButton10();
	bool GetButton5();

	Command* spinIndexerCW;
	Command* spinIndexerCCW;
	Command* gearLight;
	Command* fuelLight;
	Command* pickupIn;
	Command* pickupOut;
	Command* unwindWheels;
	//Command* arcade;
	Command* gyroCrab;
	Command* fieldCentric;
	Command* oneBall;
	Command* climb;
	Command* zeroReverse;
	Command* autoShoot;
	Command* gearLineup;
	Command* calculateSpeed;
	Command* boilerLineup;
	Command* testDrive;

	Joystick* GetDriverJoystick() { return driverJoystick; }
	Joystick* GetDriverJoystick2() { return driverJoystick2; }
};

// ==========================================================================
