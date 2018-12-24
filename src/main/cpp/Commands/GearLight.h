#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class GearLight : public frc::Command {
public:
	GearLight();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
