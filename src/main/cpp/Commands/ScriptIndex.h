#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

class ScriptIndex : public frc::Command {
public:
	ScriptIndex(float speed, float timeout);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
private:
	float _timeout;
	float _speed;
};
