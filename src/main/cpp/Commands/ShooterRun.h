#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class ShooterRun : public frc::Command {
public:
	ShooterRun();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ==========================================================================
