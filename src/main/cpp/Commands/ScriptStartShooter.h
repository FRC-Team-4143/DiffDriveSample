#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

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
