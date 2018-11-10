#pragma once
#include "frc/WPIlib.h"
using namespace frc;
#include <Commands/Command.h>

// ==========================================================================

class FieldCentric : public frc::Command {
public:
	FieldCentric();

	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

// ==========================================================================
