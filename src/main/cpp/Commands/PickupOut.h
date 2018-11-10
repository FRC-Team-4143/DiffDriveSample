#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include "Commands/Command.h"

class PickupOut : public frc::Command {
public:
	PickupOut();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
