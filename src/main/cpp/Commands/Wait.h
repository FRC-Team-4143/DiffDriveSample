#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class Wait : public frc::Command {
public:
	Wait(double timeoutSeconds);

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	double _timeoutSeconds;
};

// ==========================================================================
