#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include <Commands/Command.h>

// ==========================================================================

class WaitForVision : public frc::Command {
public:
	WaitForVision();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ==========================================================================
