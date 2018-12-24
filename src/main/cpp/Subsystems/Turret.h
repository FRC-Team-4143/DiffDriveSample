#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include "ctre/Phoenix.h"
#include <frc/commands/Subsystem.h>

class Turret : public Subsystem {
private:
	//WPI_TalonSRX *indexMotor;
	WPI_TalonSRX *turretMotor;

public:
	Turret();
	void InitDefaultCommand();
	void SetSpeed(double speed);
	void Stop();

	float startingPosition;
};
