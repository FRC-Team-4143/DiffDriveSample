#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include <Commands/Command.h>
#include <ctre/Phoenix.h>

// ==========================================================================

class RunMotor : public frc::Command {
public:
	RunMotor(WPI_TalonSRX* motor);

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	WPI_TalonSRX* _motor;
};

// ==========================================================================
