#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class CalculateSpeed : public frc::Command {
public:
	CalculateSpeed();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
