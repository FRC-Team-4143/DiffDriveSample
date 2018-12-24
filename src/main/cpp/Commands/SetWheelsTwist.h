#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class SetWheelsTwist : public frc::Command {
public:
	SetWheelsTwist(double seconds);

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	double _seconds;
};

// ==========================================================================
