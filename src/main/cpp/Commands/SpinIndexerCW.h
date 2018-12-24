#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class SpinIndexerCW : public frc::Command {
public:
	SpinIndexerCW();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
