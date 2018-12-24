#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class DriveDistance : public frc::Command {
public:
	DriveDistance(float driveAngle, float speed, float twistAngle, float distance);

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	float _driveAngle;
	float _speed;
	float _twistAngle;
	float _distance;
};

// ==========================================================================
