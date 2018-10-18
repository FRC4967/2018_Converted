/*
 * TankDrive.cpp
 *
 *  Created on: Jan 8, 2018
 *      Author: Harsh Sinha
 */

#include "TankDrive.h"

TankDrive::TankDrive(ControllerInterface &controllerInterface,
		OperatorInterface &operatorInterface, CubeManipulator &cubeManipulator) :
		controllerInterface(controllerInterface), operatorInterface(
				operatorInterface), cubeManipulator(cubeManipulator){
	// TODO Auto-generated constructor stub
	controllerInterface.mLeftDriveFollower1->Follow(
			*controllerInterface.mLeftDriveMaster);
	controllerInterface.mRightDriveFollower1->Follow(
			*controllerInterface.mRightDriveMaster);
	controllerInterface.mLeftDriveFollower2->Follow(
			*controllerInterface.mLeftDriveMaster);
	controllerInterface.mRightDriveFollower2->Follow(
			*controllerInterface.mRightDriveMaster);
	bool leftInvert = false;
	bool rightInvert = !leftInvert;
	controllerInterface.mLeftDriveMaster->SetSensorPhase(true);
	controllerInterface.mRightDriveMaster->SetSensorPhase(true);
	controllerInterface.mLeftDriveMaster->SetInverted(leftInvert);
	controllerInterface.mLeftDriveFollower1->SetInverted(leftInvert);
	controllerInterface.mLeftDriveFollower2->SetInverted(leftInvert);
	controllerInterface.mRightDriveMaster->SetInverted(rightInvert);
	controllerInterface.mRightDriveFollower1->SetInverted(rightInvert);
	controllerInterface.mRightDriveFollower2->SetInverted(rightInvert);
}

TankDrive::~TankDrive() {
	// TODO Auto-generated destructor stub
}

void TankDrive::Init() {
	//Do Whatever is Needed
	SetMode(ControlMode::PercentOutput);
	Zero();
}

void TankDrive::SetMode(ControlMode controlMode) {
	//Grab values for PIDF Gains from the ShuffleBoard to allow for easier fine tuning.
	double P = Preferences::GetInstance()->GetDouble("P",1);
	double I = Preferences::GetInstance()->GetDouble("I",0);
	double D = Preferences::GetInstance()->GetDouble("D",0);
	double F = Preferences::GetInstance()->GetDouble("F",0);
	switch(controlMode){
	case ControlMode::PercentOutput: {
		m_driveMode = ControlMode::PercentOutput;
		break;
	}
	case ControlMode::Position: {
		m_driveMode = ControlMode::Position;
		SetPID(controllerInterface.mLeftDriveMaster, P, I, D, F);
		SetPID(controllerInterface.mRightDriveMaster, P, I, D, F);
		break;
	}
	}
}

void TankDrive::SetPID(TalonSRX *motor, double p, double i, double d, double f){
	motor->Config_kP(0,p,0);
	motor->Config_kI(0,i,0);
	motor->Config_kD(0,d,0);
	motor->Config_kF(0,f,0);
}

void TankDrive::TeleopPeriodic() {
	//INPUT FROM JOYSTICKS IS NEGATIVE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	float leftSpeed = operatorInterface.joystick_driverLeft->GetRawAxis(5);
	float rightSpeed = operatorInterface.joystick_driverLeft->GetRawAxis(1);
	//determine if right or left speed is negative
	if (rightSpeed<0) {
		//if it is, square it but keep it negative
		rightSpeed*=-rightSpeed;

	}
	else {
		//if not, just square it
		rightSpeed*=rightSpeed;

	}
	// see above
	if (leftSpeed<0) {

			leftSpeed*=-leftSpeed;
		}
		else {

			leftSpeed*=leftSpeed;
		}

	//m_liftHeightPercentDeduction = (1 - (.55 * (cubeManipulator.GetArmAngle()/robotDimensions::liftMaxHeight)));

	//leftSpeed *= m_liftHeightPercentDeduction;
	//rightSpeed *= m_liftHeightPercentDeduction;



	//cap values to 1 for safety precautions
	if(leftSpeed > 1) leftSpeed = 1;
	else if(leftSpeed < -1) leftSpeed = -1;
	if(rightSpeed > 1) rightSpeed = 1;
	else if(rightSpeed < -1) rightSpeed = -1;

	// button held to reverse drive (or at least that's what it's supposed to do)
	if (operatorInterface.joystick_driverLeft->GetRawAxis(3) > .2) {
		printf("Hold button to reverse controls");
			std::swap(leftSpeed, rightSpeed);
			rightSpeed *= -1;
			leftSpeed *= -1;
		}

	if (operatorInterface.joystick_driverRight->GetRawAxis(2) > .2) {
		leftSpeed = rightSpeed = ((leftSpeed+rightSpeed)/2);
	}


	if (operatorInterface.joystick_driverLeft->GetRawButton(LeftButtonMap::BUTTON_VISION)) {

	} else {
		Drive(leftSpeed, rightSpeed);
	}

}
void TankDrive::Drive(float left_speed, float right_speed){
	if(m_driveMode == ControlMode::Position){
		right_speed *= robotDimensions::encoderTicksPerInch;
		left_speed  *= robotDimensions::encoderTicksPerInch;
		double rightError =
				controllerInterface.mRightDriveMaster->GetClosedLoopError(0);
		double leftError =
				controllerInterface.mLeftDriveMaster->GetClosedLoopError(0);
		double avgError = (rightError+leftError)/2;
		frc::SmartDashboard::PutNumber("Right Error",rightError);
		frc::SmartDashboard::PutNumber("Left Error",leftError);
		frc::SmartDashboard::PutNumber("Average Error",avgError);
	}
	controllerInterface.mRightDriveMaster->Set(m_driveMode, right_speed);
	controllerInterface.mLeftDriveMaster->Set(m_driveMode, left_speed);
}

void TankDrive::Zero(){
	controllerInterface.mRightDriveMaster->Set(m_driveMode, 0);
	controllerInterface.mLeftDriveMaster->Set(m_driveMode, 0);
	controllerInterface.mRightDriveMaster->SetSelectedSensorPosition(0, 0, 0);
	controllerInterface.mLeftDriveMaster->SetSelectedSensorPosition(0, 0, 0);
}
driveVal TankDrive::InvertDrive(float left, float right){
	driveVal drive;
	right *= -1;
	left *= -1;
	std::swap(left,right);
	drive.right = right;
	drive.left = left;
	return drive;

}
