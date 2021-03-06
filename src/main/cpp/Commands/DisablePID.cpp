#include "Commands/DisablePID.h"
#include "Robot.h"

// ==========================================================================

DisablePID::DisablePID() {
}

// ==========================================================================

void DisablePID::Initialize() {
	Robot::driveTrain->EnablePIDs(false);
}

// ==========================================================================

void DisablePID::Execute() {
}

// ==========================================================================

bool DisablePID::IsFinished() {
	return false;
}

// ==========================================================================

void DisablePID::End() {
	Robot::driveTrain->EnablePIDs(true);
}

// ==========================================================================

void DisablePID::Interrupted() {
	End();
}

// ==========================================================================
