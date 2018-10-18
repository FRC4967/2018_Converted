///*
// * CubeManipulator.cpp
// *
// *  Created on: Jan 27, 2018
// *      Author: UwU it's Braeden
// */
//
#include <Objects/Mechanisms/CubeManipulator/CubeManipulator.h>
#include <stdio.h>
#include "ctre/Phoenix.h"
CubeManipulator::CubeManipulator(ControllerInterface &controllerInterface,
		OperatorInterface &operatorInterface) :
		controllerInterface(controllerInterface), operatorInterface(
				operatorInterface), m_ZeroLimit(0), m_MaxLimit(1), m_Arm420Device(3){
	//controllerInterface.mFrontIntakeFollower->Follow(*controllerInterface.mFrontIntakeMaster);
	//controllerInterface.mLiftOutputFollower->Follow(*controllerInterface.mLiftOutputMaster);

	//TODO setting not false might not be the best way
	bool phase = false;
	controllerInterface.mLiftLeftMaster->SetSensorPhase(!phase);
	controllerInterface.mArmManipulator->SetSensorPhase(phase);
	controllerInterface.mLiftRightFollower->Follow(*controllerInterface.mLiftLeftMaster);
	controllerInterface.mLiftLeftMaster->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 0);

	// TODO Tune PID
	SetPID(controllerInterface.mLiftLeftMaster,1,0,0,0);
	SetPID(controllerInterface.mArmManipulator, 1, 0, 0, 0);

	// TODO let's have some separate bools for each motor, so we can know
	bool leftInvert = false;
	bool rightInvert = !leftInvert;

	// TODO why are these both inverted and running opposite directions, that shouldn't be right
	controllerInterface.mArmManipulator->SetInverted(leftInvert);
	controllerInterface.mLiftLeftMaster->SetInverted(rightInvert);
	controllerInterface.mLiftRightFollower->SetInverted(leftInvert);
	controllerInterface.mLiftOutput->SetInverted(rightInvert);
	controllerInterface.mFrontIntakeLeft->SetInverted(rightInvert);
	controllerInterface.mFrontIntakeRight->SetInverted(leftInvert);



	m_state_lift = 0;
	m_state_output = 0;
	m_state_trim = 0;
	m_state_intake = 0;
	m_state_teleop = RESET;
	m_currentArmAngle = 0;
	m_cubeInLift = false;
}

CubeManipulator::~CubeManipulator() {
	// TODO Auto-generated destructor stub
}

void CubeManipulator::TeleopInit() {
	//ZeroLift();
	m_state_teleop = RESET;
	controllerInterface.mArmManipulator->SetSelectedSensorPosition(0,0,0);
}
void CubeManipulator::SetPID(TalonSRX *motor,double p, double i, double d, double f){
	motor->Config_kP(0,p,0);
	motor->Config_kI(0,i,0);
	motor->Config_kD(0,d,0);
	motor->Config_kF(0,f,0);
}


void CubeManipulator::TeleopPeriodic() {
	ArmTeleop();
	printf("PID Error: %i, Set Point: %f, Encoder Position: %i\n", controllerInterface.mLiftLeftMaster->GetClosedLoopError(0), m_setPoint, controllerInterface.mLiftLeftMaster->GetSelectedSensorPosition(0));
	GetArmAngle();
	float speed =1 - .8*operatorInterface.joystick_manipulator->GetRawAxis(1);
	if(speed>1) speed = 1;
	if (m_ZeroLimit.Get()) {
		//ZeroLift();
	} else if (m_MaxLimit.Get()) {
		//MaxLift();
	}
	bool back =  operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::BUTTON_REVERSE_OUTPUT);
	bool atGround = m_currentArmAngle == 0;
	//TODO change to axis back to 3
	float trim = -operatorInterface.joystick_manipulator->GetRawAxis(3)*robotDimensions::scaleTrimDistance;
	if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::BUTTON_ENABLE_ZERO)){
		printf("Manual Lift \n");
		double dblAxis = -operatorInterface.joystick_manipulator->GetRawAxis(1)/1;
		controllerInterface.mLiftLeftMaster->Set(ControlMode::PercentOutput, dblAxis);
	}else{
	switch(m_state_teleop){
	case GET_COMMAND:
		if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::ARM_FORWARD_45)){
			m_state_teleop=ARM_FORWARD_45;
		}else if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::ARM_BACKWARD_45)){
			m_state_teleop=ARM_BACKWARD_45;
		}else if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::ARM_FORWARD_DOWN)){
			m_state_teleop=ARM_FORWARD_DOWN;
		}else if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::ARM_BACKWARD_DOWN)){
			m_state_teleop=ARM_BACKWARD_DOWN;
		}else if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::ARM_UP)){
			m_state_teleop=ARM_UP;
		}else if(operatorInterface.joystick_manipulator->GetRawButton(30)){
			m_state_teleop=LIFT_TO_SCALE;
		}else if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::BUTTON_OUTPUT_CUBE)){
			m_state_teleop= OUTPUT;
		}
			else {
			m_state_teleop = RESET;
		}
		break;
	case OUTPUT:
		if(back) speed *=-1;
		RunOutput(speed);
		m_state_teleop = GET_COMMAND;
		break;
	case ARM_FORWARD_45:
		if(MoveArmToAngle(45)){
			m_state_teleop = RESET;
		}
		break;
	case ARM_BACKWARD_45:
		if(MoveArmToAngle(-45)){
			m_state_teleop= RESET;
		}
		break;
	case ARM_FORWARD_DOWN:
		if(MoveArmToAngle(120)) {
			m_state_teleop= GET_COMMAND;


		}
		break;
	case ARM_BACKWARD_DOWN:
		if (MoveArmToAngle(-110)){
			m_state_teleop = RESET;
		}
		break;
	case ARM_UP:
		if(MoveArmToAngle(0)){
			m_state_teleop = RESET;
		}
		break;
	case RESET:
		m_state_trim = 0;
		m_state_lift = 0;
		if (!operatorInterface.joystick_driverLeft->GetRawButton(BUTTON_VISION)) {
			StopIntake();
			m_state_intake = 0;
			RunOutput(0);
			m_state_output = 0;
		}

		m_state_teleop = GET_COMMAND;
		break;

	}
	}

}

float CubeManipulator::GetArmAngle() {
//	int EncoderPosition = controllerInterface.mLiftLeftMaster->GetSelectedSensorPosition(0);
//	m_currentArmAngle = (EncoderPosition) / (robotDimensions::encoderTicksPerDegree);
//
//	printf("Lift Encoder Position = %i\f", EncoderPosition);
	//printf("Arm Angle = %f\n", m_currentArmAngle);

//	printf("Pot: %f\n", m_Arm420Device.GetVoltage());
//
//	m_currentArmAngle = (m_Arm420Device.GetVoltage()) / (robotDimensions::voltPerDegree - robotDimensions::zeroAngleVolt);
//	printf("Arm Angle: %f\n", m_currentArmAngle);

	return m_currentArmAngle;
}

void CubeManipulator::ResetStates(){
	m_state_intake = 0;
	m_state_lift = 0;
	m_state_trim=0;
	m_state_output = 0;
}

void CubeManipulator::ArmInit() {
	double potRead = m_Arm420Device.GetVoltage();


	if (potRead > 4.8 || potRead < .2) {
		m_currentArmAngle = 90;
	} else {
		m_currentArmAngle = (2.6-potRead) / robotDimensions::voltPerDegree;
	}
	double setPoint = m_setPoint = m_currentArmAngle*robotDimensions::encoderTicksPerDegree;

	controllerInterface.mLiftLeftMaster->SetSelectedSensorPosition(setPoint, 0, 0);

	SetPID(controllerInterface.mLiftLeftMaster,1,0,0,0);
	printf("Arm Angle: %f, Voltage: %f, Set Point: %f\n", m_currentArmAngle, potRead, setPoint);
}

void CubeManipulator::TunePID() {
	if (m_tunePID) {
		double setPoint = Preferences::GetInstance()->GetDouble("PID Setpoint", 0);
		controllerInterface.mLiftLeftMaster->Set(ControlMode::Position, setPoint);

	}
}
