#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class UnwindWheels : public frc::Command {
public:
	UnwindWheels();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ==========================================================================
