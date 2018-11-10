#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include "Commands/Command.h"

class OneBall : public frc::Command {
public:
	OneBall();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
