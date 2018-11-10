#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include <Commands/Command.h>

// ==========================================================================

class DisablePID : public frc::Command {
public:
	DisablePID();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ==========================================================================
