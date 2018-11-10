#pragma once
#include "frc/WPILib.h"
using namespace frc;
#include <Commands/Subsystem.h>
#include <ctre/Phoenix.h>

class Pickup : public Subsystem {
	WPI_TalonSRX *pickup;

private:

public:
	Pickup();
	void InitDefaultCommand();
	void In();
	void Out();
	void Stop();
};
