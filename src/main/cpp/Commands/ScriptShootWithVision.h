#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include <Commands/CommandGroup.h>

class ScriptShootWithVision : public CommandGroup {
public:
	ScriptShootWithVision(float timeout);
	ScriptShootWithVision(int speed, float timeout);
};
