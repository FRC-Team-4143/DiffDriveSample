#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class ScriptStopShooter : public frc::Command {
public:
	ScriptStopShooter();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ==========================================================================
