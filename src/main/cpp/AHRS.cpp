#include "AHRS.h"

AHRS::AHRS(frc::I2C::Port port){

}

float AHRS::GetYaw(){
    return 0;
}

void AHRS::ZeroYaw(){
    return;
}

bool AHRS::IsCalibrating(){
    return false;
}

float AHRS::GetCompassHeading(){
    return 0;
}

float AHRS::GetDisplacementX(){
    return 0;
}

float AHRS::GetDisplacementY(){
    return 0;
}


