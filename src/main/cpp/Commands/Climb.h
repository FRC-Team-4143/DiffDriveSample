#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class Climb : public frc::Command {
public:
	Climb();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
