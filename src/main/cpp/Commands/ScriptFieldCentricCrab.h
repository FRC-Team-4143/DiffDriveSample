#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/CommandGroup.h>

class ScriptFieldCentricCrab : public frc::Command {
public:
	ScriptFieldCentricCrab(float twist, float y, float x, float timeout);

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	float _twist;
	float _y;
	float _x;
	float _seconds;
};
