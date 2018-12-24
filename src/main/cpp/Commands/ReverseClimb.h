#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class ReverseClimb : public frc::Command {
public:
	ReverseClimb();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
