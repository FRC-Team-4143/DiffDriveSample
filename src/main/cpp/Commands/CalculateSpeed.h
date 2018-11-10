#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include "Commands/Command.h"

class CalculateSpeed : public frc::Command {
public:
	CalculateSpeed();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
