#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include "Commands/Command.h"

class SpinIndexerCW : public frc::Command {
public:
	SpinIndexerCW();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
