#pragma once
#include "frc/WPILib.h"
using namespace frc;
#include <Commands/Subsystem.h>

// ==========================================================================

class BasicCameraSub : public frc::Subsystem {
public:
	BasicCameraSub(const std::string& cameraName);
	~BasicCameraSub();

	void Enable();
	bool IsEnabled() const;

private:
	bool m_enabled;

	void _InitializeCamera();
};

// ==========================================================================