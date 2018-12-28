#include "RobotMap.h"
#include <frc/LiveWindow/LiveWindow.h>

bool RobotMap::SpeedControl = 0;

frc::PowerDistributionPanel* RobotMap::pdp = nullptr;

WPI_TalonSRX*          RobotMap::driveTrainFrontLeftDrive = nullptr;
WPI_TalonSRX*          RobotMap::driveTrainFrontLeftDriveSlave = nullptr;
WPI_TalonSRX*          RobotMap::driveTrainFrontLeftSteer = nullptr;
frc::AnalogInput*		RobotMap::frontLeftSensor = nullptr;

WPI_TalonSRX*          RobotMap::driveTrainFrontRightDrive = nullptr;
WPI_TalonSRX*          RobotMap::driveTrainFrontRightDriveSlave = nullptr;
WPI_TalonSRX*          RobotMap::driveTrainFrontRightSteer = nullptr;
frc::AnalogInput*		RobotMap::frontRightSensor = nullptr;

WPI_TalonSRX*          RobotMap::driveTrainRearLeftDrive = nullptr;
WPI_TalonSRX*          RobotMap::driveTrainRearLeftDriveSlave = nullptr;
WPI_TalonSRX*          RobotMap::driveTrainRearLeftSteer = nullptr;
frc::AnalogInput*		RobotMap::rearLeftSensor = nullptr;

WPI_TalonSRX*          RobotMap::driveTrainRearRightDrive = nullptr;
WPI_TalonSRX*          RobotMap::driveTrainRearRightDriveSlave = nullptr;
WPI_TalonSRX*          RobotMap::driveTrainRearRightSteer = nullptr;
frc::AnalogInput*		RobotMap::rearRightSensor = nullptr;

//frc::I2C* RobotMap::i2c = nullptr;
//frc::I2C* RobotMap::lidar = nullptr;

//frc::AnalogInput* RobotMap::sonar = nullptr;

//WPI_TalonSRX* RobotMap::climbingMotor = nullptr;
//WPI_TalonSRX* RobotMap::climbingMotor2 = nullptr;

//SerialPort* RobotMap::serialPort = nullptr;
//SerialPort* RobotMap::serialPort1 = nullptr;
//SerialPort* RobotMap::serialPort2 = nullptr;

//WPI_TalonSRX* RobotMap::shooterMotor1;
//WPI_TalonSRX* RobotMap::shooterMotor2;

//WPI_TalonSRX* RobotMap::turretMotor;
//WPI_TalonSRX* RobotMap::indexMotor;
//WPI_TalonSRX* RobotMap::pickup;

AHRS* RobotMap::imu = nullptr;

#define CONTINUOUS true
#define P 1.1 // 0.7
#define I 0.0
#define D 0.1 // 0.0
#define F 0.0
#define IZone 100
#define driveP 0.3
#define driveI 0.0
#define driveD 0.0
//#define driveF 0.1 //14 tooth gear
#define driveF 0.108 // 13 tooth gear
#define pdriveP 1.5
#define pdriveI 0.01
#define pdriveIZone 100.0
#define pdriveD 0.0
#define pdriveF 0.0
#define POTMIN 0.0
#define POTMAX 5.0
#define STEERPOW 1.0
#define TOLERANCE 0.1
#define PERIOD .02
#define RATIO 1

#define FLD 5
#define FLDS 9
#define FLS 1

#define FRD 6
#define FRDS 10
#define FRS 2

#define RLD 7
#define RLDS 11
#define RLS 3

#define RRD 8
#define RRDS 12
#define RRS 4

void RobotMap::Initialize() {
	LiveWindow* lw = LiveWindow::GetInstance();
	lw->DisableAllTelemetry();

	//serialPort = new SerialPort(9600, SerialPort::kUSB);
	//serialPort1 = new SerialPort(9600, SerialPort::kUSB1);
	//serialPort2 = new SerialPort(9600, SerialPort::kUSB2);

	imu = new AHRS(frc::I2C::Port::kOnboard); // SPI::kOnboardCS0);//serialPort, 100);
	//imu = new AHRS(SerialPort::kUSB);
	pdp = new frc::PowerDistributionPanel();

	////////////////////////////////////
	////Front Left Wheel////////////////
	////////////////////////////////////
	//Drive Motor
	driveTrainFrontLeftDrive = new WPI_TalonSRX(FLD);
	driveTrainFrontLeftDrive->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,10);
	driveTrainFrontLeftDrive->ConfigPeakOutputForward(1,10);
	driveTrainFrontLeftDrive->ConfigPeakOutputReverse(-1,10);
	driveTrainFrontLeftDrive->Config_kP(0,driveP,10);
	driveTrainFrontLeftDrive->Config_kI(0,driveI,10);
	driveTrainFrontLeftDrive->Config_kD(0,driveD,10);
	driveTrainFrontLeftDrive->Config_kF(0,driveF,10);
	driveTrainFrontLeftDrive->SetSensorPhase(false);
	driveTrainFrontLeftDrive->SelectProfileSlot(0,0);
	driveTrainFrontLeftDrive->Config_kP(1,pdriveP,10);
	driveTrainFrontLeftDrive->Config_kI(1,pdriveI,10);
	driveTrainFrontLeftDrive->Config_IntegralZone(1,pdriveIZone,10);
	driveTrainFrontLeftDrive->Config_kD(1,pdriveD,10);
	driveTrainFrontLeftDrive->Config_kF(1,pdriveF,10);

	//Drive Motor Slave
	driveTrainFrontLeftDriveSlave = new WPI_TalonSRX(FLDS);
	driveTrainFrontLeftDriveSlave->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,10);
	driveTrainFrontLeftDriveSlave->ConfigPeakOutputForward(1,10);
	driveTrainFrontLeftDriveSlave->ConfigPeakOutputReverse(-1,10);
	driveTrainFrontLeftDriveSlave->Config_kP(0,driveP,10);
	driveTrainFrontLeftDriveSlave->Config_kI(0,driveI,10);
	driveTrainFrontLeftDriveSlave->Config_kD(0,driveD,10);
	driveTrainFrontLeftDriveSlave->Config_kF(0,driveF,10);
	driveTrainFrontLeftDriveSlave->SetSensorPhase(false);
	driveTrainFrontLeftDriveSlave->SelectProfileSlot(0,0);
	driveTrainFrontLeftDriveSlave->Config_kP(1,pdriveP,10);
	driveTrainFrontLeftDriveSlave->Config_kI(1,pdriveI,10);
	driveTrainFrontLeftDriveSlave->Config_IntegralZone(1,pdriveIZone,10);
	driveTrainFrontLeftDriveSlave->Config_kD(1,pdriveD,10);
	driveTrainFrontLeftDriveSlave->Config_kF(1,pdriveF,10);

	//Steering Motor
	driveTrainFrontLeftSteer = new WPI_TalonSRX(FLS);
	//driveTrainFrontLeftSteer->SetControlMode(CANTalon::kPosition);
	driveTrainFrontLeftSteer->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute,0,10);
	driveTrainFrontLeftSteer->SelectProfileSlot(0, 0);
	driveTrainFrontLeftSteer->Config_kP(0,P,10);
	driveTrainFrontLeftSteer->Config_kI(0,I,10);
	driveTrainFrontLeftSteer->Config_kD(0,D,10);
	driveTrainFrontLeftSteer->Config_kF(0,F,10);
	driveTrainFrontLeftSteer->Config_IntegralZone(0, IZone,10);
	driveTrainFrontLeftSteer->SetSensorPhase(true);
	driveTrainFrontLeftSteer->ConfigNominalOutputForward(0,10);
	driveTrainFrontLeftSteer->ConfigNominalOutputReverse(0,10);
	driveTrainFrontLeftSteer->ConfigPeakOutputForward(STEERPOW,10);
	driveTrainFrontLeftSteer->ConfigPeakOutputReverse(-STEERPOW,10);

	////////////////////////////////////
	////Front Right Wheel///////////////
	////////////////////////////////////
	//Driving Motor
	driveTrainFrontRightDrive = new WPI_TalonSRX(FRD);
	driveTrainFrontRightDrive->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,10);
	driveTrainFrontRightDrive->ConfigPeakOutputForward(1,10);
	driveTrainFrontRightDrive->ConfigPeakOutputReverse(-1,10);
	driveTrainFrontRightDrive->Config_kP(0,driveP,10);
	driveTrainFrontRightDrive->Config_kI(0,driveI,10);
	driveTrainFrontRightDrive->Config_kD(0,driveD,10);
	driveTrainFrontRightDrive->Config_kF(0,driveF,10);
	driveTrainFrontRightDrive->SetSensorPhase(false);
	driveTrainFrontRightDrive->SelectProfileSlot(0,0);
	driveTrainFrontRightDrive->Config_kP(1,pdriveP,10);
	driveTrainFrontRightDrive->Config_kI(1,pdriveI,10);
	driveTrainFrontRightDrive->Config_IntegralZone(1,pdriveIZone,100);
	driveTrainFrontRightDrive->Config_kD(1,pdriveD,10);
	driveTrainFrontRightDrive->Config_kF(1,pdriveF,10);
	//driveTrainFrontRightDrive->ConfigMotionCruiseVelocity(3500, 10);
	//driveTrainFrontRightDrive->ConfigMotionAcceleration(2000, 10);

	//Slave Motor
	driveTrainFrontRightDriveSlave = new WPI_TalonSRX(FRDS);
	driveTrainFrontRightDriveSlave->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,10);
	driveTrainFrontRightDriveSlave->ConfigPeakOutputForward(1,10);
	driveTrainFrontRightDriveSlave->ConfigPeakOutputReverse(-1,10);
	driveTrainFrontRightDriveSlave->Config_kP(0,driveP,10);
	driveTrainFrontRightDriveSlave->Config_kI(0,driveI,10);
	driveTrainFrontRightDriveSlave->Config_kD(0,driveD,10);
	driveTrainFrontRightDriveSlave->Config_kF(0,driveF,10);
	driveTrainFrontRightDriveSlave->SetSensorPhase(false);
	driveTrainFrontRightDriveSlave->SelectProfileSlot(0,0);
	driveTrainFrontRightDriveSlave->Config_kP(1,pdriveP,10);
	driveTrainFrontRightDriveSlave->Config_kI(1,pdriveI,10);
	driveTrainFrontRightDriveSlave->Config_IntegralZone(1,pdriveIZone,100);
	driveTrainFrontRightDriveSlave->Config_kD(1,pdriveD,10);
	driveTrainFrontRightDriveSlave->Config_kF(1,pdriveF,10);
	//driveTrainFrontRightDrive->ConfigMotionCruiseVelocity(3500, 10);
	//driveTrainFrontRightDrive->ConfigMotionAcceleration(2000, 10);

	//Steering Motor
	driveTrainFrontRightSteer = new WPI_TalonSRX(FRS);
	//driveTrainFrontRightSteer->SetControlMode(CANTalon::kPosition);
	driveTrainFrontRightSteer->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute,0,10);
	driveTrainFrontRightSteer->SelectProfileSlot(0, 0);
	driveTrainFrontRightSteer->Config_kP(0,P,10);
	driveTrainFrontRightSteer->Config_kI(0,I,10);
	driveTrainFrontRightSteer->Config_kD(0,D,10);
	driveTrainFrontRightSteer->Config_kF(0,F,10);
	driveTrainFrontRightSteer->Config_IntegralZone(0,IZone,10);
	driveTrainFrontRightSteer->SetSensorPhase(true);
	driveTrainFrontRightSteer->ConfigNominalOutputForward(0,10);
	driveTrainFrontRightSteer->ConfigNominalOutputReverse(0,10);
	driveTrainFrontRightSteer->ConfigPeakOutputForward(STEERPOW,10);
	driveTrainFrontRightSteer->ConfigPeakOutputReverse(-STEERPOW,10);

	////////////////////////////////////
	////Rear Left Wheel/////////////////
	////////////////////////////////////
	//Driving Motor
	driveTrainRearLeftDrive = new WPI_TalonSRX(RLD);
	driveTrainRearLeftDrive->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,10);
	driveTrainRearLeftDrive->ConfigPeakOutputForward(1,10);
	driveTrainRearLeftDrive->ConfigPeakOutputReverse(-1,10);
	driveTrainRearLeftDrive->Config_kP(0,driveP,10);
	driveTrainRearLeftDrive->Config_kI(0,driveI,10);
	driveTrainRearLeftDrive->Config_kD(0,driveD,10);
	driveTrainRearLeftDrive->Config_kF(0,driveF+.00,10);  //  take out .05 for comp bot
	driveTrainRearLeftDrive->SetInverted(false);
	driveTrainRearLeftDrive->SetSensorPhase(false);
	driveTrainRearLeftDrive->SelectProfileSlot(0,0);
	driveTrainRearLeftDrive->Config_kP(1,pdriveP,10);
	driveTrainRearLeftDrive->Config_kI(1,pdriveI,10);
	driveTrainRearLeftDrive->Config_IntegralZone(1,pdriveIZone,10);
	driveTrainRearLeftDrive->Config_kD(1,pdriveD,10);
	driveTrainRearLeftDrive->Config_kF(1,pdriveF,10);
	//driveTrainRearLeftDrive->ConfigMotionCruiseVelocity(3500, 10);
	//driveTrainRearLeftDrive->ConfigMotionAcceleration(2000, 10);

	//Slave Motor
	driveTrainRearLeftDriveSlave = new WPI_TalonSRX(RLDS);
	driveTrainRearLeftDriveSlave->ConfigRemoteFeedbackFilter(driveTrainRearLeftDrive->GetDeviceID(), RemoteSensorSource::RemoteSensorSource_TalonSRX_SelectedSensor, 0, 10);
	driveTrainRearLeftDriveSlave->ConfigSensorTerm(SensorTerm::SensorTerm_Sum0, FeedbackDevice::RemoteSensor0, 10);
	driveTrainRearLeftDriveSlave->ConfigSensorTerm(SensorTerm::SensorTerm_Sum1, FeedbackDevice::QuadEncoder, 10);
	driveTrainRearLeftDriveSlave->ConfigSelectedFeedbackSensor(FeedbackDevice::SensorSum, 0, 10);
	driveTrainRearLeftDriveSlave->ConfigPeakOutputForward(1,10);
	driveTrainRearLeftDriveSlave->ConfigPeakOutputReverse(-1,10);
	driveTrainRearLeftDriveSlave->Config_kP(0,driveP,10);
	driveTrainRearLeftDriveSlave->Config_kI(0,driveI,10);
	driveTrainRearLeftDriveSlave->Config_kD(0,driveD,10);
	driveTrainRearLeftDriveSlave->Config_kF(0,driveF+.00,10);  //  take out .05 for comp bot
	driveTrainRearLeftDriveSlave->SetInverted(false);
	driveTrainRearLeftDriveSlave->SetSensorPhase(false);
	driveTrainRearLeftDriveSlave->SelectProfileSlot(0,0);
	driveTrainRearLeftDriveSlave->Config_kP(1,pdriveP,10);
	driveTrainRearLeftDriveSlave->Config_kI(1,pdriveI,10);
	driveTrainRearLeftDriveSlave->Config_IntegralZone(1,pdriveIZone,10);
	driveTrainRearLeftDriveSlave->Config_kD(1,pdriveD,10);
	driveTrainRearLeftDriveSlave->Config_kF(1,pdriveF,10);
	driveTrainRearLeftDrive->ConfigMotionCruiseVelocity(3500, 10);
	driveTrainRearLeftDrive->ConfigMotionAcceleration(2000, 10);

	//Steering Motor
	driveTrainRearLeftSteer = new WPI_TalonSRX(RLS);
	//driveTrainRearLeftSteer->SetControlMode(CANTalon::kPosition);
	driveTrainRearLeftSteer->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute,0,10);
	driveTrainRearLeftSteer->SelectProfileSlot(0, 0);
	driveTrainRearLeftSteer->Config_kP(0,P,10);
	driveTrainRearLeftSteer->Config_kI(0,I,10);
	driveTrainRearLeftSteer->Config_kD(0,D,10);
	driveTrainRearLeftSteer->Config_kF(0,F,10);
	driveTrainRearLeftSteer->Config_IntegralZone(0,IZone,10);
	driveTrainRearLeftSteer->SetSensorPhase(true);
	driveTrainRearLeftSteer->ConfigNominalOutputForward(0,10);
	driveTrainRearLeftSteer->ConfigNominalOutputReverse(0,10);
	driveTrainRearLeftSteer->ConfigPeakOutputForward(STEERPOW,10);
	driveTrainRearLeftSteer->ConfigPeakOutputReverse(-STEERPOW,10);

	////////////////////////////////////
	////Rear Right Wheel////////////////
	////////////////////////////////////
	//Driving Motor
	driveTrainRearRightDrive = new WPI_TalonSRX(RRD);
	driveTrainRearRightDrive->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,10);
	driveTrainRearRightDrive->ConfigPeakOutputForward(1,10);
	driveTrainRearRightDrive->ConfigPeakOutputReverse(-1,10);
	driveTrainRearRightDrive->Config_kP(0,driveP,10);
	driveTrainRearRightDrive->Config_kI(0,driveI,10);
	driveTrainRearRightDrive->Config_kD(0,driveD,10);
	driveTrainRearRightDrive->Config_kF(0,driveF,10);
	driveTrainRearRightDrive->SetSensorPhase(false);
	driveTrainRearRightDrive->SelectProfileSlot(0, 0);
	driveTrainRearRightDrive->Config_kP(1,pdriveP,10);
	driveTrainRearRightDrive->Config_kI(1,pdriveI,10);
	driveTrainRearRightDrive->Config_IntegralZone(1,pdriveIZone,10);
	driveTrainRearRightDrive->Config_kD(1,pdriveD,10);
	driveTrainRearRightDrive->Config_kF(1,pdriveF,10);
	//driveTrainRearRightDrive->ConfigMotionCruiseVelocity(3500, 10);
	//driveTrainRearRightDrive->ConfigMotionAcceleration(2000, 10);

	//Slave Motor
	driveTrainRearRightDriveSlave = new WPI_TalonSRX(RRDS);
	driveTrainRearRightDriveSlave->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,10);
	driveTrainRearRightDriveSlave->ConfigPeakOutputForward(1,10);
	driveTrainRearRightDriveSlave->ConfigPeakOutputReverse(-1,10);
	driveTrainRearRightDriveSlave->Config_kP(0,driveP,10);
	driveTrainRearRightDriveSlave->Config_kI(0,driveI,10);
	driveTrainRearRightDriveSlave->Config_kD(0,driveD,10);
	driveTrainRearRightDriveSlave->Config_kF(0,driveF,10);
	driveTrainRearRightDriveSlave->SetSensorPhase(false);
	driveTrainRearRightDriveSlave->SelectProfileSlot(0, 0);
	driveTrainRearRightDriveSlave->Config_kP(1,pdriveP,10);
	driveTrainRearRightDriveSlave->Config_kI(1,pdriveI,10);
	driveTrainRearRightDriveSlave->Config_IntegralZone(1,pdriveIZone,10);
	driveTrainRearRightDriveSlave->Config_kD(1,pdriveD,10);
	driveTrainRearRightDriveSlave->Config_kF(1,pdriveF,10);
	//driveTrainRearRightDrive->ConfigMotionCruiseVelocity(3500, 10);
	//driveTrainRearRightDrive->ConfigMotionAcceleration(2000, 10);

	//Steering Motor
	driveTrainRearRightSteer = new WPI_TalonSRX(RRS);
	//driveTrainRearRightSteer->SetControlMode(CANTalon::kPosition);
	driveTrainRearRightSteer->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute,0,10);
	driveTrainRearRightSteer->SelectProfileSlot(0,0);
	driveTrainRearRightSteer->Config_kP(0,P,10);
	driveTrainRearRightSteer->Config_kI(0,I,10);
	driveTrainRearRightSteer->Config_kD(0,D,10);
	driveTrainRearRightSteer->Config_kF(0,F,10);
	driveTrainRearRightSteer->Config_IntegralZone(0,IZone,10);
	driveTrainRearRightSteer->SetSensorPhase(true);
	driveTrainRearRightSteer->ConfigNominalOutputForward(0,10);
	driveTrainRearRightSteer->ConfigNominalOutputReverse(0,10);
	driveTrainRearRightSteer->ConfigPeakOutputForward(STEERPOW,10);
	driveTrainRearRightSteer->ConfigPeakOutputReverse(-STEERPOW,10);

	//i2c = new frc::I2C(frc::I2C::Port::kMXP, 0x04);
	//lidar = new frc::I2C(frc::I2C::Port::kOnboard, 0x05);

	frontLeftSensor = new frc::AnalogInput(0);
	frontRightSensor = new frc::AnalogInput(1);
	rearLeftSensor = new frc::AnalogInput(2);
	rearRightSensor = new frc::AnalogInput(3);

	//climbingMotor = new WPI_TalonSRX(63); //13
	//climbingMotor2 = new WPI_TalonSRX(66); //9


    //shooterMotor1 = new WPI_TalonSRX(61); //11
    //shooterMotor2 = new WPI_TalonSRX(62); //12

	//pickup = new WPI_TalonSRX(60); //10

	//indexMotor = new WPI_TalonSRX(64); //14
	
	//turretMotor = new WPI_TalonSRX(65); //Unused
	
}
