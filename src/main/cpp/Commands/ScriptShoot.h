#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include <Commands/CommandGroup.h>

class ScriptShoot : public frc::CommandGroup {
public:
	ScriptShoot(int speed, float timeout);
};
