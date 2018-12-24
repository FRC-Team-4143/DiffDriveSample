#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class PickupIn : public frc::Command {
public:
	PickupIn();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
