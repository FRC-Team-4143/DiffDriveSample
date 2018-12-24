#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class SetWheelOffsets : public frc::Command {
public:
	SetWheelOffsets();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ==========================================================================
