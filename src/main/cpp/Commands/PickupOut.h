#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class PickupOut : public frc::Command {
public:
	PickupOut();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
