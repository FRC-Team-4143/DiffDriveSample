#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>
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
