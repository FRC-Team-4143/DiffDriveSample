/*
 * Fake AHRS.h
 *
 *  Created on: Jul 30, 2015
 *      Author: Scott
 */

#ifndef SRC_AHRS_H_
#define SRC_AHRS_H_

#include "WPILib.h"
#include "frc/I2C.h"

class IIOProvider;
class ContinuousAngleTracker;
class InertialDataIntegrator;
class OffsetTracker;
class AHRSInternal;

class AHRS {//public SensorBase,
             //public frc::PIDSource 
public:

    enum BoardAxis {
        kBoardAxisX = 0,
        kBoardAxisY = 1,
        kBoardAxisZ = 2,
    };

    struct BoardYawAxis
    {
        /* Identifies one of the board axes */
        BoardAxis board_axis;
        /* true if axis is pointing up (with respect to gravity); false if pointing down. */
        bool up;
    };

    enum SerialDataType {
    /**
     * (default):  6 and 9-axis processed data
     */
    kProcessedData = 0,
    /**
     * unprocessed data from each individual sensor
     */
    kRawData = 1
    };

public:
    AHRS(frc::SPI::Port spi_port_id);
    AHRS(frc::I2C::Port i2c_port_id);
    AHRS(frc::SerialPort::Port serial_port_id);

    AHRS(frc::SPI::Port spi_port_id, uint8_t update_rate_hz);
    AHRS(frc::SPI::Port spi_port_id, uint32_t spi_bitrate, uint8_t update_rate_hz);

    AHRS(frc::I2C::Port i2c_port_id, uint8_t update_rate_hz);

    AHRS(frc::SerialPort::Port serial_port_id, AHRS::SerialDataType data_type, uint8_t update_rate_hz);

    float  GetPitch();
    float  GetRoll();
    float  GetYaw();
    float  GetCompassHeading();
    void   ZeroYaw();
    bool   IsCalibrating();
    bool   IsConnected();
    double GetByteCount();
    double GetUpdateCount();
    long   GetLastSensorTimestamp();
    float  GetWorldLinearAccelX();
    float  GetWorldLinearAccelY();
    float  GetWorldLinearAccelZ();
    bool   IsMoving();
    bool   IsRotating();
    float  GetBarometricPressure();
    float  GetAltitude();
    bool   IsAltitudeValid();
    float  GetFusedHeading();
    bool   IsMagneticDisturbance();
    bool   IsMagnetometerCalibrated();
    float  GetQuaternionW();
    float  GetQuaternionX();
    float  GetQuaternionY();
    float  GetQuaternionZ();
    void   ResetDisplacement();
    void   UpdateDisplacement( float accel_x_g, float accel_y_g,
                               int update_rate_hz, bool is_moving );
    float  GetVelocityX();
    float  GetVelocityY();
    float  GetVelocityZ();
    float  GetDisplacementX();
    float  GetDisplacementY();
    float  GetDisplacementZ();
    double GetAngle();
    double GetRate();
    void   SetAngleAdjustment(double angle);
    double GetAngleAdjustment();
    void   Reset();
    float  GetRawGyroX();
    float  GetRawGyroY();
    float  GetRawGyroZ();
    float  GetRawAccelX();
    float  GetRawAccelY();
    float  GetRawAccelZ();
    float  GetRawMagX();
    float  GetRawMagY();
    float  GetRawMagZ();
    float  GetPressure();
    float  GetTempC();
    AHRS::BoardYawAxis GetBoardYawAxis();
    std::string GetFirmwareVersion();

    int GetActualUpdateRate();
    int GetRequestedUpdateRate();

    void EnableLogging(bool enable);

    int16_t GetGyroFullScaleRangeDPS();
    int16_t GetAccelFullScaleRangeG();

};

#endif /* SRC_AHRS_H_ */
