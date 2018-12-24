#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class SpinIndexerCCW : public frc::Command {
public:
	SpinIndexerCCW();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
