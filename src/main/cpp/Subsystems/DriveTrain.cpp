#include <Commands/BoilerLineup.h>
#include "Subsystems/DriveTrain.h"
#include "Commands/CrabDrive.h"
#include "Modules/Constants.h"
#include "Modules/Logger.h"
#include "Robot.h"
#include "RobotMap.h"
#include "Subsystems/EncoderConstants.h"

#ifdef TESTSWERVE
#define MAXTURNS 3
#else
#define MAXTURNS 3000
#define SOFTTURNLIMIT 2
#endif

const float DIFFSCALE = 0.3; // must tune still
const float DIFFMAX = 0.15;
const float DIFFMIN = 0.001;
const float DIFFDEAD = 0.001;

const float TWISTSCALE = 0.6;

const float DEAD_ZONE = 0.1;

const float driveScale = 950;
// const double AVERAGE_VOLTAGE_BASE = EncoderConstants::HALF_TURN;

#define GYROP .01
#define GYROMAX 1.0

#define MINV .2
#define MAXV 4.5
#define AVEV ((MINV + MAXV) / 2.)
#define DIFFV (MAXV - MINV)
#define SCALEV (5. / DIFFV)

#define MINMAX(x, minval, maxval) (std::min(maxval, std::max(minval, x)))

double DriveTrain::AnalogScale(double input) {
	double scale = ((input - AVEV) * SCALEV) + 2.5;
	scale = MINMAX(scale, 0., 5.);
	scale *= -1.0; // inverse direction
	scale += 5.0;
  	return(scale);
}
// ==========================================================================

DriveTrain::DriveTrain() :
		frc::Subsystem("DriveTrain") {
	LOG("DriveTrain::ctor");

	unwinding = false;
	X = 0;
	Y = 0;

	FLOffset = 0;
	FROffset = 0;
	RLOffset = 0;
	RROffset = 0;

	FLInv = 1;
	FRInv = 1;
	RRInv = 1;
	RLInv = 1;

	lastx = 0.0;
	lasty = 0.0;
	lasttwist = 0.0;

	FLValue = 0.0;
	FRValue = 0.0;
	RLValue = 0.0;
	RRValue = 0.0;

	frontLeftDrive = RobotMap::driveTrainFrontLeftDrive;
	frontLeftDriveSlave = RobotMap::driveTrainFrontLeftDriveSlave;
	frontLeftSteer = RobotMap::driveTrainFrontLeftSteer;
	frontLeftSensor = RobotMap::frontLeftSensor;

	frontRightDrive = RobotMap::driveTrainFrontRightDrive;
	frontRightDriveSlave = RobotMap::driveTrainFrontRightDriveSlave;
	frontRightSteer = RobotMap::driveTrainFrontRightSteer;
	frontRightSensor = RobotMap::frontRightSensor;

	rearLeftDrive = RobotMap::driveTrainRearLeftDrive;
	rearLeftDriveSlave = RobotMap::driveTrainRearLeftDriveSlave;
	rearLeftSteer = RobotMap::driveTrainRearLeftSteer;
	rearLeftSensor = RobotMap::rearLeftSensor;

	rearRightDrive = RobotMap::driveTrainRearRightDrive;
	rearRightDriveSlave = RobotMap::driveTrainRearRightDriveSlave;
	rearRightSteer = RobotMap::driveTrainRearRightSteer;
	rearRightSensor = RobotMap::rearRightSensor;

	//lidar = RobotMap::lidar;
	lidarDistance = 0;
}

// ==========================================================================

void DriveTrain::EnablePIDs(bool enable) {
	if (enable) {
		/*frontLeftSteer->Enable();
		frontRightSteer->Enable();
		rearLeftSteer->Enable();
		rearRightSteer->Enable();*/
	} else {
		/*frontLeftSteer->Disable();
		frontRightSteer->Disable();
		rearLeftSteer->Disable();
		rearRightSteer->Disable();*/
	}
}

// ==========================================================================

void DriveTrain::readLidar() {
	//lidar->ReadOnly(4, (unsigned char*)&lidarDistance);
	//SmartDashboard::PutNumber("lidar distance", lidarDistance);
}

// ==========================================================================

void DriveTrain::InitDefaultCommand() {
	SetDefaultCommand(new CrabDrive());
}

// ==========================================================================

void DriveTrain::SetWheelbase(float w, float x, float y) {
	//W = w;
	X = x;
	Y = y;
}

// ==========================================================================

void DriveTrain::SetOffsets(double FLOff, double FROff, double RLOff,
		double RROff) {
	FLOffset = FLOff;
	FROffset = FROff;
	RLOffset = RLOff;
	RROffset = RROff;
}

// ==========================================================================


void DriveTrain::PositionModeTwist(float desiredangle) {
	//frontLeftSteer->Set(ControlMode::Position, CorrectSteerSetpoint(FLOffset + 0.625, frontLeftSteer));
	//frontRightSteer->Set(ControlMode::Position, CorrectSteerSetpoint(FROffset - 0.625, frontRightSteer));
	//rearLeftSteer->Set(ControlMode::Position, CorrectSteerSetpoint(RLOffset - 0.625, rearLeftSteer));
	//rearRightSteer->Set(ControlMode::Position, CorrectSteerSetpoint(RROffset + 0.625, rearRightSteer));
}

// ==========================================================================

void DriveTrain::TestDriveEncoderZero(){
	RobotMap::driveTrainRearLeftDrive->SetSelectedSensorPosition(0, 0, 10);
	RobotMap::driveTrainRearLeftDriveSlave->SetSelectedSensorPosition(0, 0, 10);
}

//===========================================================================

void DriveTrain::TestDriveEnable(){
	rearLeftDriveSlave->Set(ControlMode::MotionMagic, 0, 0);
	((TalonSRX*)rearLeftDrive)->Follow((ctre::phoenix::motorcontrol::IMotorController&) rearLeftDrive, FollowerType::FollowerType_AuxOutput1);
	//rearLeftDrive->Follow(rearLeftDrive, FollowerType_AuxOutput1);
}

//===========================================================================

// keeps controls consistent regardless of rotation of robot
void DriveTrain::FieldCentricCrab(float twist, float y, float x,
		bool operatorControl) {
	float FWD = y;
	float STR = x;

	auto robotangle = Robot::gyroSub->PIDGet() * pi / 180;

	FWD = y * cos(robotangle) + x * sin(robotangle);
	STR = -y * sin(robotangle) + x * cos(robotangle);

	Crab(twist, FWD, STR, operatorControl);
}

// ==========================================================================

// attempts to keep robot square to the field as it drives
void DriveTrain::GyroCrab(float desiredangle, float y, float x,
		bool operatorControl) {
	auto robotangle = Robot::gyroSub->PIDGet();

	float twist = desiredangle - robotangle;
	while (twist > 180.0)
		twist -= 360.0;
	while (twist < -180.0)
		twist += 360.0;

	twist = std::min(GYROMAX, std::max(-GYROMAX, twist * GYROP));
	Crab(twist, y, x, operatorControl);
}

// ==========================================================================

void DriveTrain::GyroRotate(float desiredangle, double power) {
	auto robotangle = Robot::gyroSub->PIDGet();

	float twist = desiredangle - robotangle;
	while (twist > 180.0)
		twist -= 360.0;
	while (twist < -180.0)
		twist += 360.0;

	twist = std::min(power, std::max(-power, twist * (0.2/10)));
	Crab(twist, 0, 0, false);
}

// ==========================================================================

void DriveTrain::Crab(float twist, float y, float x, bool operatorControl) {
	//LOG("DriveTrain::Crab1");
	// stop PID loop if wires wrap.
	/*
	 if (unwinding || abs(frontRightPos->GetTurns()) > MAXTURNS ||
	 abs(rearRightPos->GetTurns()) > MAXTURNS ||
	 abs(frontLeftPos->GetTurns()) > MAXTURNS ||
	 abs(rearLeftPos->GetTurns()) > MAXTURNS) {
		 SetDriveSpeed(0, 0, 0, 0);
		 return;
	 }
	 */
	// this stores the direction of joystick when all axis last crossed into the
	// deadzone and keeps the wheels pointed that direction
	// this .1 should be kept the same as the deadzone in oi.cpp
	if (operatorControl && x == 0.0 && y == 0.0 && twist == 0.0) {
		if (fabs(lasty) > DEAD_ZONE || fabs(lastx) > DEAD_ZONE
				|| fabs(lasttwist) > DEAD_ZONE) {
			y = std::min(std::max(lasty, -DEAD_ZONE), DEAD_ZONE);
			x = std::min(std::max(lastx, -DEAD_ZONE), DEAD_ZONE);
			twist = std::min(std::max(lasttwist, -DEAD_ZONE), DEAD_ZONE);
		} else {
			y = .05;
			// default wheel position
		}
	} else if (Robot::oi->GetButtonA() && !operatorControl && twist == 0.0) {
		twist = 0.05;
	}

	lastx = x;
	lasty = y;
	lasttwist = twist;

	if (operatorControl) {
		// scale for operator control
		x *= 1;
		y *= 1;
		float avg = (std::abs(x) + std::abs(y)) / 2;
		float scale = 1 - avg / 2;
		twist *= scale; // TWISTSCALE;
	}

	float FWD = y;
	float STR = x;

	auto radius = sqrt(pow(Y, 2) + pow(X, 2));

	auto AP = STR - twist * X / radius;
	auto BP = STR + twist * X / radius;
	auto CP = FWD - twist * Y / radius;
	auto DP = FWD + twist * Y / radius;

	float FLSetPoint = EncoderConstants::HALF_TURN;
	float FRSetPoint = EncoderConstants::HALF_TURN;
	float RLSetPoint = EncoderConstants::HALF_TURN;
	float RRSetPoint = EncoderConstants::HALF_TURN;

	if (DP != 0 || BP != 0) {
		FLSetPoint = (EncoderConstants::HALF_TURN
				+ EncoderConstants::HALF_TURN / pi * atan2(BP, DP));
	}
	if (BP != 0 || CP != 0) {
		FRSetPoint = (EncoderConstants::HALF_TURN
				+ EncoderConstants::HALF_TURN / pi * atan2(BP, CP));
	}
	if (AP != 0 || DP != 0) {
		RLSetPoint = (EncoderConstants::HALF_TURN
				+ EncoderConstants::HALF_TURN / pi * atan2(AP, DP));
	}
	if (AP != 0 || CP != 0) {
		RRSetPoint = (EncoderConstants::HALF_TURN
				+ EncoderConstants::HALF_TURN / pi * atan2(AP, CP));
	}
	//LOG("DriveTrain::Crab2");

	SetSteer(FLSetPoint, FRSetPoint, RLSetPoint, RRSetPoint);
	//LOG("DriveTrain::Crab3");

	double FL; // FL, distance from Front Left Wheel to the center of rotation
	double FR; // FR, distance from Front Right Wheel to the center of rotation
	double RL; // RL, distance from Rear Left Wheel to the center of rotation
	double RR; // RR, distance from Rear Right Wheel to the center of rotation

	FL = sqrt(pow(BP, 2) + pow(DP, 2));
	FR = sqrt(pow(BP, 2) + pow(CP, 2));
	RL = sqrt(pow(AP, 2) + pow(DP, 2));
	RR = sqrt(pow(AP, 2) + pow(CP, 2));

	// Solve for fastest wheel speed
	double speedarray[] = { fabs(FL), fabs(FR), fabs(RL), fabs(RR) };

	int length = 4;
	double maxspeed = speedarray[0];
	for (int i = 1; i < length; i++) {
		if (speedarray[i] > maxspeed) {
			maxspeed = speedarray[i];
		}
	}

	double FRRatio; // Ratio of Speed of Front Right wheel
	double FLRatio; // Ratio of Speed of Front Left wheel
	double RRRatio; // Ratio of Speed of Rear Right wheel
	double RLRatio; // Ratio of Speed of Rear Left wheel

	// Set ratios based on maximum wheel speed
	if (maxspeed > 1 || maxspeed < -1) {
		FLRatio = FL / maxspeed;
		FRRatio = FR / maxspeed;
		RLRatio = RL / maxspeed;
		RRRatio = RR / maxspeed;
	} else {
		FLRatio = FL;
		FRRatio = FR;
		RLRatio = RL;
		RRRatio = RR;
	}

	if (operatorControl && fabs(x) <= DEAD_ZONE && fabs(y) <= DEAD_ZONE
			&& fabs(twist) <= DEAD_ZONE) {
		FLRatio = 0.0;
		FRRatio = 0.0;
		RLRatio = 0.0;
		RRRatio = 0.0;
	}
	//LOG("DriveTrain::Crab4");

	// Set drive speeds
	SetDriveSpeed(FLRatio, -FRRatio, RLRatio, -RRRatio);
	//LOG("DriveTrain::Crab5");
}

// ==========================================================================

float DriveTrain::SetSteerSetpoint(float setpoint, WPI_TalonSRX* talon, double *inverse, AnalogInput* sensor){
	float currentPosition = AnalogScale(sensor->GetVoltage());
	int turns = 0;
	int currentAngle = currentPosition;

	//currentPosition *= EncoderConstants::FULL_TURN;
	//turns *= EncoderConstants::FULL_TURN;
	//currentAngle *= EncoderConstants::FULL_TURN;

	float angleOptions[6];
	angleOptions[0] = turns - EncoderConstants::FULL_TURN + setpoint;
	angleOptions[1] = turns - EncoderConstants::FULL_TURN + setpoint + EncoderConstants::HALF_TURN;
	angleOptions[2] = turns + setpoint;
	angleOptions[3] = turns + setpoint + EncoderConstants::HALF_TURN;
	angleOptions[4] = turns + EncoderConstants::FULL_TURN + setpoint;
	angleOptions[5] = turns + EncoderConstants::FULL_TURN + setpoint + EncoderConstants::HALF_TURN;

	float minMove = fabs(currentPosition - angleOptions[0]);
	int minI = 0;
	for (int i = 1; i < 6; i++){
		if (fabs(currentPosition - angleOptions[i]) < minMove){
			minMove = fabs(currentPosition - angleOptions[i]);
			minI = i;
		}
	}
	//talon->Set(ControlMode::Position, angleOptions[minI]/EncoderConstants::FULL_TURN);

	// add back later RJS
	//*inverse = cos((minMove / EncoderConstants::FULL_TURN) * 2 * 3.141);
	*inverse = 1;
	if (minI % 2)
		*inverse *= -1;
	else
		*inverse *= 1;
    return angleOptions[minI];
}

// ==========================================================================

void DriveTrain::SetSteer(float FLSetPoint, float FRSetPoint,
		float RLSetPoint, float RRSetPoint) {
	//DFLSetPoint = -FLSetPoint;
	//FRSetPoint = -FRSetPoint;
	//RLSetPoint = -RLSetPoint;
	//RRSetPoint = -RRSetPoint;

	SmartDashboard::PutNumber("FLSetpointValue", FLSetPoint);
	SmartDashboard::PutNumber("FRSetpointValue", FRSetPoint);
	SmartDashboard::PutNumber("RLSetpointValue", RLSetPoint);
	SmartDashboard::PutNumber("RRSetpointValue", RRSetPoint);


	FLSetpointValue = SetSteerSetpoint(FLSetPoint + FLOffset, frontLeftSteer, &FLInv, frontLeftSensor);
	FRSetpointValue = SetSteerSetpoint(FRSetPoint + FROffset, frontRightSteer, &FRInv, frontLeftSensor);
	RLSetpointValue = SetSteerSetpoint(RLSetPoint + RLOffset, rearLeftSteer, &RLInv, rearLeftSensor);
	RRSetpointValue = SetSteerSetpoint(RRSetPoint + RROffset, rearRightSteer, &RRInv, rearRightSensor);
}

// ==========================================================================
void DriveTrain::GetDiffSensorValues(){
	
	flsensorval = AnalogScale(frontLeftSensor->GetVoltage());
	frsensorval = AnalogScale(frontRightSensor->GetVoltage());
	rlsensorval = AnalogScale(rearLeftSensor->GetVoltage());
	rrsensorval = AnalogScale(rearRightSensor->GetVoltage());
}

// ===========================================================================
void DriveTrain::SetDriveSpeed(float FLSpeed, float FRSpeed, float RLSpeed,
		float RRSpeed) {

	flsdiff = (FLSetpointValue - flsensorval);
	frsdiff = (FRSetpointValue - frsensorval);
	rlsdiff = (RLSetpointValue - rlsensorval);
	rrsdiff = (RRSetpointValue - rrsensorval);
	 
	if(flsdiff < -2.5) flsdiff += 5.0;
	if(frsdiff < -2.5) frsdiff += 5.0;
	if(rlsdiff < -2.5) rlsdiff += 5.0;
	if(rrsdiff < -2.5) rrsdiff += 5.0;

	if(flsdiff > 2.5) flsdiff -= 5.0;
	if(frsdiff > 2.5) frsdiff -= 5.0;
	if(rlsdiff > 2.5) rlsdiff -= 5.0;
	if(rrsdiff > 2.5) rrsdiff -= 5.0;

	flsdiff *= -DIFFSCALE;
	frsdiff *= -DIFFSCALE;
	rlsdiff *= -DIFFSCALE;
	rrsdiff *= -DIFFSCALE;


	//char sz[256];
	//sprintf(sz, "diffs: FL %f, FR %f, RL %f, RR %f", flsdiff, frsdiff, rlsdiff, rrsdiff);
	//LOG(sz);

	if(flsdiff > DIFFDEAD) flsdiff += DIFFMIN;
	if(flsdiff < -DIFFDEAD) flsdiff -= DIFFMIN;
	if(frsdiff > DIFFDEAD) frsdiff += DIFFMIN;
	if(frsdiff < -DIFFDEAD) frsdiff -= DIFFMIN;
	if(rlsdiff > DIFFDEAD) rlsdiff += DIFFMIN;
	if(rlsdiff < -DIFFDEAD) rlsdiff -= DIFFMIN;
	if(rrsdiff > DIFFDEAD) rrsdiff += DIFFMIN;
	if(rrsdiff < -DIFFDEAD) rrsdiff -= DIFFMIN;

	flsdiff = std::min(DIFFMAX, std::max(-DIFFMAX, flsdiff));
	frsdiff = std::min(DIFFMAX, std::max(-DIFFMAX, frsdiff));
	rlsdiff = std::min(DIFFMAX, std::max(-DIFFMAX, rlsdiff));
	rrsdiff = std::min(DIFFMAX, std::max(-DIFFMAX, rrsdiff));

	if (RobotMap::SpeedControl) { //this mode not implemented for diff swerve yet
		frontLeftDrive->Set(FLSpeed * FLInv * driveScale);
		frontRightDrive->Set(FRSpeed * FRInv * driveScale);
		SmartDashboard::PutNumber("Front Right Drive", (FRSpeed * FRInv * driveScale));
		rearLeftDrive->Set(ControlMode::PercentOutput, RLSpeed * RLInv * driveScale);
		rearRightDrive->Set(RRSpeed * RRInv * driveScale);
		frontLeftDriveSlave->Set(FLSpeed * -FLInv * driveScale);
		frontRightDriveSlave->Set(FRSpeed * -FRInv * driveScale);
		SmartDashboard::PutNumber("Front Right Slave", (FRSpeed * -FRInv * driveScale));
		rearLeftDriveSlave->Set(ControlMode::PercentOutput, RLSpeed * -RLInv * driveScale);
		rearRightDriveSlave->Set(RRSpeed * -RRInv * driveScale);
	} else {
		frontLeftDrive->Set((FLSpeed * FLInv) - flsdiff);
		frontRightDrive->Set((FRSpeed * FRInv) - frsdiff);
		SmartDashboard::PutNumber("Front Right Drive", ((FRSpeed * FRInv) - frsdiff));
		rearLeftDrive->Set(ControlMode::PercentOutput, (RLSpeed * RLInv) - rlsdiff);
		SmartDashboard::PutNumber("Rear Left Drive", ((RLSpeed * RLInv) - rlsdiff));
		rearRightDrive->Set((RRSpeed * RRInv) - rrsdiff);
		frontLeftDriveSlave->Set((FLSpeed * -FLInv) - flsdiff);
		frontRightDriveSlave->Set((FRSpeed * -FRInv) - frsdiff);
		SmartDashboard::PutNumber("Front Right Slave", ((FRSpeed * -FRInv) - frsdiff));
		rearLeftDriveSlave->Set(ControlMode::PercentOutput, (RLSpeed * -RLInv) - rlsdiff);
		SmartDashboard::PutNumber("Rear Left Slave", ((RLSpeed * -RLInv) - rlsdiff));
		rearRightDriveSlave->Set((RRSpeed * -RRInv) - rrsdiff);
	}
}

// ==========================================================================

void DriveTrain::Lock() {
	SetSteer(3.0, 1.5, 3.0, 1.5);
	SetDriveSpeed(0, 0, 0, 0);
}

// ==========================================================================

void DriveTrain::SideLock() {
	SetSteer(2.0, 0.75, 3.25, 4.5);
	SetDriveSpeed(0, 0, 0, 0);
}

// ==========================================================================

void DriveTrain::updateDistanceEncoders() {
	FLValue = (frontLeftDrive->GetSelectedSensorPosition() + frontLeftDriveSlave->GetSelectedSensorPosition()) / 2.0;
	FRValue = (frontRightDrive->GetSelectedSensorPosition() + frontRightDriveSlave->GetSelectedSensorPosition()) / 2.0;
	RLValue = (rearLeftDrive->GetSelectedSensorPosition() + rearLeftDriveSlave->GetSelectedSensorPosition()) / 2.0;
	RRValue = (rearRightDrive->GetSelectedSensorPosition() + rearRightDriveSlave->GetSelectedSensorPosition()) / 2.0;
}

// ==========================================================================

double DriveTrain::getDistanceEncodersValues() {
	double average = (FLValue + FRValue + RLValue + RRValue) / 4.0;
	return average;
}

// ==========================================================================

void DriveTrain::zeroDistanceEncoders() {
	frontLeftDrive->SetSelectedSensorPosition(0, 0, 10);
	frontRightDrive->SetSelectedSensorPosition(0, 0, 10);
	rearLeftDrive->SetSelectedSensorPosition(0, 0, 10);
	rearRightDrive->SetSelectedSensorPosition(0, 0, 10);
	frontLeftDriveSlave->SetSelectedSensorPosition(0, 0, 10);
	frontRightDriveSlave->SetSelectedSensorPosition(0, 0, 10);
	rearLeftDriveSlave->SetSelectedSensorPosition(0, 0, 10);
	rearRightDriveSlave->SetSelectedSensorPosition(0, 0, 10);
}

// ==========================================================================

void DriveTrain::zeroSteeringEncoders() {
	frontLeftSteer->SetSelectedSensorPosition(0);
	frontRightSteer->SetSelectedSensorPosition(0);
	rearLeftSteer->SetSelectedSensorPosition(0);
	rearRightSteer->SetSelectedSensorPosition(0);
}

// ==========================================================================

void DriveTrain::setWheelOffsets() {
	// Get the current steering positions from the drive train.
	/*
	 auto FLPosition = Robot::driveTrain->frontLeftPos->GetRawAngle();
	 auto FRPosition = Robot::driveTrain->frontRightPos->GetRawAngle();
	 auto RLPosition = Robot::driveTrain->rearLeftPos->GetRawAngle();
	 auto RRPosition = Robot::driveTrain->rearRightPos->GetRawAngle();
	 auto ArmPosition = Robot::armSub->GetRawPosition();
	 */
	auto FLPosition = 0.;//(-2.5 + AnalogScale(frontLeftSensor->GetVoltage()));
	auto FRPosition = 0.;//(-2.5 + AnalogScale(frontRightSensor->GetVoltage()));
	auto RLPosition = 0.;//(-2.5 + AnalogScale(rearLeftSensor->GetVoltage()));
	auto RRPosition = 0.;//(-2.5 + AnalogScale(rearRightSensor->GetVoltage()));
	LogSettings(FLPosition, FRPosition, RLPosition, RRPosition);

	// Save the positions to Preferences.
	auto prefs = Preferences::GetInstance();
	prefs->PutDouble(Constants::FL_POS_NAME, FLPosition);
	prefs->PutDouble(Constants::FR_POS_NAME, FRPosition);
	prefs->PutDouble(Constants::RL_POS_NAME, RLPosition);
	prefs->PutDouble(Constants::RR_POS_NAME, RRPosition);
	//prefs->PutDouble(Constants::ARM_POSITION_NAME, ArmPosition);

	// Set the drive train positions.
	SetOffsets(FLPosition, FRPosition, RLPosition, RRPosition);
	//Robot::armSub->SetOffset(ArmPosition);
}

// ==========================================================================

void DriveTrain::loadWheelOffsets() {
	LOG("DriveTrainSettings::LoadSettings");

	// Load the positions from Preferences.
	auto prefs = Preferences::GetInstance();
	auto FLPosition = prefs->GetDouble(Constants::FL_POS_NAME,
			Constants::FL_POS_DEFAULT);
	auto FRPosition = prefs->GetDouble(Constants::FR_POS_NAME,
			Constants::FR_POS_DEFAULT);
	auto RLPosition = prefs->GetDouble(Constants::RL_POS_NAME,
			Constants::RL_POS_DEFAULT);
	auto RRPosition = prefs->GetDouble(Constants::RR_POS_NAME,
			Constants::RR_POS_DEFAULT);

	LogSettings(FLPosition, FRPosition, RLPosition, RRPosition);

	// Set the drive train positions.
	SetOffsets(FLPosition, FRPosition, RLPosition, RRPosition);
}

// ==========================================================================

void DriveTrain::LogSettings(double fl, double fr, double rl, double rr) {
	char sz[256];
	sprintf(sz, "Wheel Positions: FL %f, FR %f, RL %f, RR %f", fl, fr, rl, rr);
	LOG(sz);
}

// ==========================================================================

void DriveTrain::Dashboard() {
	//SmartDashboard::PutNumber("Steering Motor Encoder FL",
	//		frontLeftSteer->GetSelectedSensorPosition());
	//SmartDashboard::PutNumber("Steering Motor Encoder FR",
	//		frontRightSteer->GetSelectedSensorPosition());
	//SmartDashboard::PutNumber("Steering Motor Encoder RL",
	//		rearLeftSteer->GetSelectedSensorPosition());
	//SmartDashboard::PutNumber("Steering Motor Encoder RR",
	//		rearRightSteer->GetSelectedSensorPosition());

	return;
	
}

// ==========================================================================

void DriveTrain::CrabInit() {
	/*frontLeftSteer->Enable();
	frontRightSteer->Enable();
	rearLeftSteer->Enable();
	rearRightSteer->Enable();
	frontLeftSteer->SetControlMode(CANSpeedController::kPosition);
	frontRightSteer->SetControlMode(CANSpeedController::kPosition);
	rearLeftSteer->SetControlMode(CANSpeedController::kPosition);
	rearRightSteer->SetControlMode(CANSpeedController::kPosition);
	*/
	LOG("DriveTrain::CrabInit");

}

// ==========================================================================

void DriveTrain::SetWheelsStraight() {
	//frontLeftSteer->Set(ControlMode::Position, FLOffset);
	//frontRightSteer->Set(ControlMode::Position, FROffset);
	//rearLeftSteer->Set(ControlMode::Position, RLOffset);
	//rearRightSteer->Set(ControlMode::Position, RROffset);
}

// ==========================================================================

void DriveTrain::ArcadeDriveMode(float x, float y) {
	float leftMotorOutput;
	float rightMotorOutput;

	if (y > 0.0) {
		if (x > 0.0) {
			leftMotorOutput = y - x;
			rightMotorOutput = std::max(y, x);
		} else {
			leftMotorOutput = std::max(y, -x);
			rightMotorOutput = y + x;
		}
	} else {
		if (x > 0.0) {
			leftMotorOutput = -std::max(-y, x);
			rightMotorOutput = y + x;
		} else {
			leftMotorOutput = y - x;
			rightMotorOutput = -std::max(-y, -x);
		}
	}

	SetDriveSpeed(leftMotorOutput, rightMotorOutput, leftMotorOutput,
			rightMotorOutput);
}

// ==========================================================================

void DriveTrain::disableSpeedControl() {
	/*frontLeftDrive->SetControlMode(CANSpeedController::kPercentVbus);
	frontRightDrive->SetControlMode(CANSpeedController::kPercentVbus);
	rearLeftDrive->SetControlMode(CANSpeedController::kPercentVbus);
	rearRightDrive->SetControlMode(CANSpeedController::kPercentVbus);
	frontLeftDriveSlave->SetControlMode(CANSpeedController::kPercentVbus);
	frontRightDriveSlave->SetControlMode(CANSpeedController::kPercentVbus);
	rearLeftDriveSlave->SetControlMode(CANSpeedController::kPercentVbus);
	rearRightDriveSlave->SetControlMode(CANSpeedController::kPercentVbus);*/
	RobotMap::SpeedControl = 0;
}

// ==========================================================================

void DriveTrain::enableSpeedControl() {
	frontLeftDrive->SelectProfileSlot(0,0);
	frontRightDrive->SelectProfileSlot(0,0);
	rearLeftDrive->SelectProfileSlot(0,0);
	rearRightDrive->SelectProfileSlot(0,0);
	frontLeftDrive->ConfigPeakOutputForward(1,10);
	frontLeftDrive->ConfigPeakOutputReverse(-1,10);
	frontRightDrive->ConfigPeakOutputForward(1,10);
	frontRightDrive->ConfigPeakOutputReverse(-1,10);
	rearLeftDrive->ConfigPeakOutputForward(1,10);
	rearLeftDrive->ConfigPeakOutputReverse(-1,10);
	rearRightDrive->ConfigPeakOutputForward(1,10);
	rearRightDrive->ConfigPeakOutputReverse(-1,10);
	//frontLeftDrive->SetControlMode(CANSpeedController::kSpeed);
	//frontRightDrive->SetControlMode(CANSpeedController::kSpeed);
	//rearLeftDrive->SetControlMode(CANSpeedController::kSpeed);
	//rearRightDrive->SetControlMode(CANSpeedController::kSpeed);
	frontLeftDriveSlave->SelectProfileSlot(0,0);
	frontRightDriveSlave->SelectProfileSlot(0,0);
	rearLeftDriveSlave->SelectProfileSlot(0,0);
	rearRightDriveSlave->SelectProfileSlot(0,0);
	frontLeftDriveSlave->ConfigPeakOutputForward(1,10);
	frontLeftDriveSlave->ConfigPeakOutputReverse(-1,10);
	frontRightDriveSlave->ConfigPeakOutputForward(1,10);
	frontRightDriveSlave->ConfigPeakOutputReverse(-1,10);
	rearLeftDriveSlave->ConfigPeakOutputForward(1,10);
	rearLeftDriveSlave->ConfigPeakOutputReverse(-1,10);
	rearRightDriveSlave->ConfigPeakOutputForward(1,10);
	rearRightDriveSlave->ConfigPeakOutputReverse(-1,10);
	//frontLeftDrive->SetControlMode(CANSpeedController::kSpeed);
	//frontRightDrive->SetControlMode(CANSpeedController::kSpeed);
	//rearLeftDrive->SetControlMode(CANSpeedController::kSpeed);
	//rearRightDrive->SetControlMode(CANSpeedController::kSpeed);
	RobotMap::SpeedControl = 1;
}

// ==========================================================================

void DriveTrain::enablePositionControl() {
	frontLeftDrive->SelectProfileSlot(1,0);
	frontRightDrive->SelectProfileSlot(1,0);
	rearLeftDrive->SelectProfileSlot(1,0);
	rearRightDrive->SelectProfileSlot(1,0);
	frontLeftDrive->ConfigPeakOutputForward(.33,10);
	frontLeftDrive->ConfigPeakOutputReverse(-.33,10);
	frontRightDrive->ConfigPeakOutputForward(.33,10);
	frontRightDrive->ConfigPeakOutputReverse(-.33,10);
	rearLeftDrive->ConfigPeakOutputForward(.33,10);
	rearLeftDrive->ConfigPeakOutputReverse(-.33,10);
	rearRightDrive->ConfigPeakOutputForward(.33,10);
	rearRightDrive->ConfigPeakOutputReverse(-.33,10);
	//frontLeftDrive->SetControlMode(CANSpeedController::kPosition);
	//frontRightDrive->SetControlMode(CANSpeedController::kPosition);
	//rearLeftDrive->SetControlMode(CANSpeedController::kPosition);
	//rearRightDrive->SetControlMode(CANSpeedController::kPosition);
	frontLeftDriveSlave->SelectProfileSlot(1,0);
	frontRightDriveSlave->SelectProfileSlot(1,0);
	rearLeftDriveSlave->SelectProfileSlot(1,0);
	rearRightDriveSlave->SelectProfileSlot(1,0);
	frontLeftDriveSlave->ConfigPeakOutputForward(.33,10);
	frontLeftDriveSlave->ConfigPeakOutputReverse(-.33,10);
	frontRightDriveSlave->ConfigPeakOutputForward(.33,10);
	frontRightDriveSlave->ConfigPeakOutputReverse(-.33,10);
	rearLeftDriveSlave->ConfigPeakOutputForward(.33,10);
	rearLeftDriveSlave->ConfigPeakOutputReverse(-.33,10);
	rearRightDriveSlave->ConfigPeakOutputForward(.33,10);
	rearRightDriveSlave->ConfigPeakOutputReverse(-.33,10);
	//frontLeftDrive->SetControlMode(CANSpeedController::kPosition);
	//frontRightDrive->SetControlMode(CANSpeedController::kPosition);
	//rearLeftDrive->SetControlMode(CANSpeedController::kPosition);
	//rearRightDrive->SetControlMode(CANSpeedController::kPosition);
	RobotMap::SpeedControl = 2;
}

// ==========================================================================

void DriveTrain::enableSteeringPID() {
	/*frontLeftSteer->Enable();
	frontRightSteer->Enable();
	rearLeftSteer->Enable();
	rearRightSteer->Enable();*/
}

/* ==========================================================================

double DriveTrain::getTalonPosition(WPI_TalonSRX* talon) {
	return (talon->GetSelectedSensorPosition() - trunc(talon->GetSelectedSensorPosition()))*EncoderConstants::FULL_TURN;
}

*/ // ==========================================================================

bool DriveTrain::unwind() { // float y, float x){
//frontLeftSteer->Disable();
	//frontRightSteer->Disable();
	//rearLeftSteer->Disable();
	//rearRightSteer->Disable();

	frontLeftSteer->Config_kP(0,0.4,10);
	frontRightSteer->Config_kP(0,0.4,10);
	rearLeftSteer->Config_kP(0,0.4,10);
	rearRightSteer->Config_kP(0,0.4,10);

	frontLeftSteer->ConfigPeakOutputForward(.5,10);
	frontLeftSteer->ConfigPeakOutputReverse(-.5,10);
	frontRightSteer->ConfigPeakOutputForward(.5,10);
	frontRightSteer->ConfigPeakOutputReverse(-.5,10);
	rearLeftSteer->ConfigPeakOutputForward(.5,10);
	rearLeftSteer->ConfigPeakOutputReverse(-.5,10);
	rearRightSteer->ConfigPeakOutputForward(.5,10);
	rearRightSteer->ConfigPeakOutputReverse(-.5,10);

	frontLeftSteer->Set(ControlMode::Position,FLOffset / EncoderConstants::FULL_TURN);
	frontRightSteer->Set(ControlMode::Position,FROffset / EncoderConstants::FULL_TURN);
	rearLeftSteer->Set(ControlMode::Position,RLOffset / EncoderConstants::FULL_TURN);
	rearRightSteer->Set(ControlMode::Position,RROffset / EncoderConstants::FULL_TURN);

	unwinding = true;
	return true;
}

// ==========================================================================

void DriveTrain::doneunwind() {
	unwinding = 0;
}

// ==========================================================================

double DriveTrain::CorrectSteerSetpoint(double setpoint, WPI_TalonSRX* talon) {
	if (setpoint < 0) {
		return (setpoint + EncoderConstants::FULL_TURN)/EncoderConstants::FULL_TURN;
	}
	else if (setpoint > EncoderConstants::FULL_TURN) {
		return (setpoint - EncoderConstants::FULL_TURN)/EncoderConstants::FULL_TURN;
	}
	else if (setpoint == EncoderConstants::FULL_TURN) {
		return 0;
	}
	else {
		return setpoint/EncoderConstants::FULL_TURN;
	}

	/*
	if (setpoint < 0) {
		setpoint = (setpoint + EncoderConstants::FULL_TURN) / EncoderConstants::FULL_TURN;
	} else if (setpoint > EncoderConstants::FULL_TURN) {
		setpoint = (setpoint - EncoderConstants::FULL_TURN) / EncoderConstants::FULL_TURN;
	}

	float currentPosition = talon->GetPosition();

	if (setpoint < 0.25 && currentPosition - trunc(currentPosition) > 0.75) {
		return (trunc(currentPosition) + setpoint + 1);
	} else if (setpoint > 0.75 && currentPosition - trunc(currentPosition) < 0.25) {
		return (trunc(currentPosition) + setpoint - 1);
	} else {
		return trunc(currentPosition) + setpoint;
	}
	*/
	/*
	 int newSetpoint = (trunc(currentPosition)+setpoint);

	 while (newSetpoint - currentPosition < -EncoderConstants::FULL_TURN){
	 newSetpoint -= EncoderConstants::FULL_TURN;
	 }
	 while (newSetpoint - currentPosition > EncoderConstants::FULL_TURN){
	 newSetpoint += EncoderConstants::FULL_TURN;
	 }

	 return newSetpoint / EncoderConstants::FULL_TURN;


	 if (setpoint < 0) {
	 return (setpoint + EncoderConstants::FULL_TURN)/EncoderConstants::FULL_TURN;
	 }
	 else if (setpoint > EncoderConstants::FULL_TURN) {
	 return (setpoint - EncoderConstants::FULL_TURN)/EncoderConstants::FULL_TURN;
	 }
	 else if (setpoint == EncoderConstants::FULL_TURN) {
	 return 0;
	 }
	 else {
	 return setpoint/5;
	 }*/
}

// ==========================================================================

//Old SetSteer function
/*
void DriveTrain::SetSteer(float FLSetPoint, float FRSetPoint,
		float RLSetPoint, float RRSetPoint) {
	FLSetPoint = -FLSetPoint;
	FRSetPoint = -FRSetPoint;
	RLSetPoint = -RLSetPoint;
	RRSetPoint = -RRSetPoint;

	SetSteerSetpoint(FLSetPoint + FLOffset, frontLeftSteer, &FLInv);
	SetSteerSetpoint(FRSetPoint + FROffset, frontRightSteer, &FRInv);
	SetSteerSetpoint(RLSetPoint + RLOffset, rearLeftSteer, &RLInv);
	SetSteerSetpoint(RRSetPoint + RROffset, rearRightSteer, &RRInv);

	return;

	//////////////////////////////////
	// Front Left Wheel
	//////////////////////////////////
		if (frontLeftPos->GetTurns() > SOFTTURNLIMIT) {
	 if (CorrectSteerSetpoint(FLSetPoint + FLOffset - frontLeftPos->GetAngle()) > EncoderConstants::HALF_TURN) {
	 frontLeft->SetSetpoint(CorrectSteerSetpoint(FLSetPoint + FLOffset));
	 FLInv = 1;
	 }
	 else {
	 frontLeft->SetSetpoint(CorrectSteerSetpoint(FLSetPoint + FLOffset - EncoderConstants::HALF_TURN));
	 FLInv = -1;
	 }
	 }
	 else if (frontLeftPos->GetTurns() < -SOFTTURNLIMIT) {
	 if (CorrectSteerSetpoint(FLSetPoint + FLOffset - frontLeftPos->GetAngle()) < EncoderConstants::HALF_TURN) {
	 frontLeft->SetSetpoint(CorrectSteerSetpoint(FLSetPoint + FLOffset));
	 FLInv = 1;
	 }
	 else {
	 frontLeft->SetSetpoint(CorrectSteerSetpoint(FLSetPoint + FLOffset - EncoderConstants::HALF_TURN));
	 FLInv = -1;
	 }
	 }
	 else {
	// Default rotation logic
	if (fabs(FLSetPoint + FLOffset - getTalonPosition(frontLeftSteer))
			< EncoderConstants::QUARTER_TURN
			|| fabs(FLSetPoint + FLOffset - getTalonPosition(frontLeftSteer))
					> EncoderConstants::THREEQUARTER_TURN) {
		frontLeftSteer->SetSetpoint(
				CorrectSteerSetpoint(FLSetPoint + FLOffset, frontLeftSteer));
		FLInv = 1;
	} else {
		frontLeftSteer->SetSetpoint(
				CorrectSteerSetpoint(
						FLSetPoint + FLOffset - EncoderConstants::HALF_TURN,
						frontLeftSteer));
		FLInv = -1;
	}
	//}

	//////////////////////////////////
	// Front Right Wheel
	//////////////////////////////////
	if (frontRightPos->GetTurns() > SOFTTURNLIMIT) {
	 if (CorrectSteerSetpoint(FRSetPoint + FROffset - frontRightPos->GetAngle()) > EncoderConstants::HALF_TURN) {
	 frontRight->SetSetpoint(CorrectSteerSetpoint(FRSetPoint + FROffset));
	 FRInv = 1;
	 }
	 else {
	 frontRight->SetSetpoint(CorrectSteerSetpoint(FRSetPoint + FROffset - EncoderConstants::HALF_TURN));
	 FRInv = -1;
	 }
	 }
	 else if (frontRightPos->GetTurns() < -SOFTTURNLIMIT) {
	 if (CorrectSteerSetpoint(FRSetPoint + FROffset - frontRightPos->GetAngle()) < EncoderConstants::HALF_TURN) {
	 frontRight->SetSetpoint(CorrectSteerSetpoint(FRSetPoint + FROffset));
	 FRInv = 1;
	 }
	 else {
	 frontRight->SetSetpoint(CorrectSteerSetpoint(FRSetPoint + FROffset - EncoderConstants::HALF_TURN));
	 FRInv = -1;
	 }
	 }
	 else {
	// default rotation logic
	if (fabs(FRSetPoint + FROffset - getTalonPosition(frontRightSteer))
			< EncoderConstants::QUARTER_TURN
			|| fabs(FRSetPoint + FROffset - getTalonPosition(frontRightSteer))
					> EncoderConstants::THREEQUARTER_TURN) {
		frontRightSteer->SetSetpoint(
				CorrectSteerSetpoint(FRSetPoint + FROffset, frontRightSteer));
		FRInv = 1;
	} else {
		frontRightSteer->SetSetpoint(
				CorrectSteerSetpoint(
						FRSetPoint + FROffset - EncoderConstants::HALF_TURN,
						frontRightSteer));
		FRInv = -1;
	}
	//}

	//////////////////////////////////
	// Rear Left Wheel
	//////////////////////////////////
	if (rearLeftPos->GetTurns() > SOFTTURNLIMIT) {
	 if (CorrectSteerSetpoint(RLSetPoint + RLOffset - rearLeftPos->GetAngle()) > EncoderConstants::HALF_TURN) {
	 rearLeft->SetSetpoint(CorrectSteerSetpoint(RLSetPoint + RLOffset));
	 RLInv = 1;
	 }
	 else {
	 rearLeft->SetSetpoint(CorrectSteerSetpoint(RLSetPoint + RLOffset - EncoderConstants::HALF_TURN));
	 RLInv = -1;
	 }
	 }
	 else if (rearLeftPos->GetTurns() < -SOFTTURNLIMIT) {
	 if (CorrectSteerSetpoint(RLSetPoint + RLOffset - rearLeftPos->GetAngle()) < EncoderConstants::HALF_TURN) {
	 rearLeft->SetSetpoint(CorrectSteerSetpoint(RLSetPoint + RLOffset));
	 RLInv = 1;
	 }
	 else {
	 rearLeft->SetSetpoint(CorrectSteerSetpoint(RLSetPoint + RLOffset - EncoderConstants::HALF_TURN));
	 RLInv = -1;
	 }
	 }
	 else {
	// default rotation logic
	if (fabs(RLSetPoint + RLOffset - getTalonPosition(rearLeftSteer))
			< EncoderConstants::QUARTER_TURN
			|| fabs(RLSetPoint + RLOffset - getTalonPosition(rearLeftSteer))
					> EncoderConstants::THREEQUARTER_TURN) {
		rearLeftSteer->SetSetpoint(
				CorrectSteerSetpoint(RLSetPoint + RLOffset, rearLeftSteer));
		RLInv = 1;
	} else {
		rearLeftSteer->SetSetpoint(
				CorrectSteerSetpoint(
						RLSetPoint + RLOffset - EncoderConstants::HALF_TURN,
						rearLeftSteer));
		RLInv = -1;
	}
	//}

	//////////////////////////////////
	// Rear Right Wheel
	//////////////////////////////////
	if (rearRightPos->GetTurns() > SOFTTURNLIMIT) {
	 if (CorrectSteerSetpoint(RRSetPoint + RROffset - rearRightPos->GetAngle()) > EncoderConstants::HALF_TURN) {
	 rearRight->SetSetpoint(CorrectSteerSetpoint(RRSetPoint + RROffset));
	 RRInv = 1;
	 }
	 else {
	 rearRight->SetSetpoint(CorrectSteerSetpoint(RRSetPoint + RROffset - EncoderConstants::HALF_TURN));
	 RRInv = -1;
	 }
	 }
	 else if (rearRightPos->GetTurns() < -SOFTTURNLIMIT) {
	 if (CorrectSteerSetpoint(RRSetPoint + RROffset - rearRightPos->GetAngle()) < EncoderConstants::HALF_TURN) {
	 rearRight->SetSetpoint(CorrectSteerSetpoint(RRSetPoint + RROffset));
	 RRInv = 1;
	 }
	 else {
	 rearRight->SetSetpoint(CorrectSteerSetpoint(RRSetPoint + RROffset - EncoderConstants::HALF_TURN));
	 RRInv = -1;
	 }
	 }
	 else {
	// default rotation logic
	if (fabs(RRSetPoint + RROffset - getTalonPosition(rearRightSteer))
			< EncoderConstants::QUARTER_TURN
			|| fabs(RRSetPoint + RROffset - getTalonPosition(rearRightSteer))
					> EncoderConstants::THREEQUARTER_TURN) {
		rearRightSteer->SetSetpoint(
				CorrectSteerSetpoint(RRSetPoint + RROffset, rearRightSteer));
		RRInv = 1;
	} else {
		rearRightSteer->SetSetpoint(
				CorrectSteerSetpoint(
						RRSetPoint + RROffset - EncoderConstants::HALF_TURN,
						rearRightSteer));
		RRInv = -1;
	}
	//}
}
*/

