#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class DefaultTurret : public frc::Command {
public:
	DefaultTurret();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
