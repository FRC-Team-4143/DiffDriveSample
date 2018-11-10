#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include "Commands/Command.h"

class Climb : public frc::Command {
public:
	Climb();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
