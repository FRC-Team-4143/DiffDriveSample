#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Command.h>

// ==========================================================================

class DriveTilVision : public frc::Command {
public:
	DriveTilVision(double y, double x, double twist, double seconds);

	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

private:
	double _x;
	double _y;
	double _twist;
	double _seconds;
};

// ==========================================================================
