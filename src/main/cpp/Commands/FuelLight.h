#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class FuelLight : public frc::Command {
public:
	FuelLight();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
