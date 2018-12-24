#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/CommandGroup.h>

class ScriptShoot : public frc::CommandGroup {
public:
	ScriptShoot(int speed, float timeout);
};
