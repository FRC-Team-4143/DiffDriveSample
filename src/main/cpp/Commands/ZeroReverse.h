#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class ZeroReverse : public frc::Command {
public:
	ZeroReverse();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
