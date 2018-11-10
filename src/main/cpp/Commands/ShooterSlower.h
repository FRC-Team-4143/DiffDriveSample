#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include <Commands/Command.h>

// ==========================================================================

class ShooterSlower : public frc::Command {
public:
	ShooterSlower();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

// ==========================================================================
