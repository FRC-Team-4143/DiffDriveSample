#pragma once
#include <frc/WPILib.h>
using namespace frc;
#include <frc/commands/Subsystem.h>
#include <ctre/Phoenix.h>
#include <frc/Timer.h>

class Indexer : public Subsystem {
private:
	WPI_TalonSRX *indexMotor;
	PowerDistributionPanel *pdp;
	Timer *timer;
	bool indexJammed;
	double reverseTime;
	double jamPosition;
	bool _loadingOne;

public:
	Indexer();
	void InitDefaultCommand();
	void SpinCW();
	void SpinCW(float speed);
	void SpinCCW();
	void SpinBall();
	void SetLoadingOne(bool loadingOne);
	void Stop();
	bool IsJammed();
	void TestJamShooter();
	void ReadPDP();
	void SetSpeed(double speed);
	void ZeroTimer();
	double GetPosition() const;
	double GetReverseTime() const;
	double GetTimer() const;
	bool IsIndexJammed() const;
};
