#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class ZeroYaw : public frc::Command {
public:
	ZeroYaw();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ==========================================================================
