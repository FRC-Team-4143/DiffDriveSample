#include "Commands/TestDrive.h"
#include "Modules/Logger.h"
#include "Robot.h"

// ==========================================================================

TestDrive::TestDrive()
: frc::Command("TestDrive") {
	LOG(GetName() + "::ctor");

	Requires(Robot::driveTrain);
}

// ==========================================================================

void TestDrive::Initialize() {
	Robot::driveTrain->TestDriveEncoderZero();
}

// ==========================================================================

void TestDrive::Execute() {
	float x = 0;
	float y = 0;
	float z = 0;
	if (!DriverStation::GetInstance().IsAutonomous()) {
		x = Robot::oi->GetJoystickX();
		y = Robot::oi->GetJoystickY();
		z = Robot::oi->GetJoystickZ();
	}

	x *= (x < 0 ? -x: x);
	z *= (z < 0 ? -z: z);
	if (fabs(y) > 0.5)
		z *= 0.75;
	y *= (y < 0 ? -y: y);



	Robot::driveTrain->Crab(z, -y, x, true); //ENABLE THIS
}

// ==========================================================================

bool TestDrive::IsFinished() {
	return false;
}

// ==========================================================================

void TestDrive::End() {
}

// ==========================================================================

void TestDrive::Interrupted() {
	End();
}

// ==========================================================================
