#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include "Commands/Command.h"

class SpinIndexerCCW : public frc::Command {
public:
	SpinIndexerCCW();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
