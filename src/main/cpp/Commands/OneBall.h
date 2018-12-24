#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class OneBall : public frc::Command {
public:
	OneBall();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
