#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/CommandGroup.h>

class ScriptShootWithVision : public CommandGroup {
public:
	ScriptShootWithVision(float timeout);
	ScriptShootWithVision(int speed, float timeout);
};
