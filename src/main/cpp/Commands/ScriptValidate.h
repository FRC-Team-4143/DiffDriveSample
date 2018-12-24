#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>
#include <string>

// ==========================================================================

class ScriptValidate : public frc::Command {
public:
	ScriptValidate();
	ScriptValidate(std::string dashboardInput, std::string dashboardOutput);

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

protected:
	void ValidateCommands();

private:
	std::string _dashboardInput;
	std::string _dashboardOutput;
};

// ==========================================================================
