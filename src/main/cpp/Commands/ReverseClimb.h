#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include "Commands/Command.h"

class ReverseClimb : public frc::Command {
public:
	ReverseClimb();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
