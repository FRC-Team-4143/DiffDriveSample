#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include <Commands/Command.h>

// ==========================================================================

class ScriptStartShooter : public frc::Command {
public:
	ScriptStartShooter(int speed);
	ScriptStartShooter();

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
private:
	int _speed;
};

// ==========================================================================
