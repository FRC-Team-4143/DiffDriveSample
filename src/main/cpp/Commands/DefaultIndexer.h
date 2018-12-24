#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class DefaultIndexer : public frc::Command {
public:
	DefaultIndexer();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
