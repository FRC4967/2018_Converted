/*
 * Lift.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: monke
 */
#include <Objects/Mechanisms/CubeManipulator/CubeManipulator.h>

void CubeManipulator::SetArmAngleTo(float height){
//	if(height>robotDimensions::liftMaxHeight) height = robotDimensions::liftMaxHeight;
//	if(height<0) height = 0;
	m_currentArmAngle = height;
	height *= robotDimensions::encoderTicksPerDegree;
	printf("SetPoint: %f Angle: %f\n", height, m_currentArmAngle);
	controllerInterface.mLiftLeftMaster->Set(ControlMode::Position,height);
}
bool CubeManipulator::MoveArmToAngle(float height){
	static float startingHeight;
	float t=m_timer.Get();
	float setPoint;
	static float heightChange;
	static double maxT = 4;
	switch(m_state_lift){
	case 0:
		//Init
		//ZeroLift();
		startingHeight = GetArmAngle();
		heightChange = height - startingHeight;
		printf("StartingHeight: %f, HeightChange: %f \n",startingHeight, heightChange);
		m_trap.SetAll(400,250,500,fabs(heightChange));
		m_timer.Reset();
		m_timer.Start();
		m_state_lift++;
		break;
	case 1:
		setPoint = m_trap.Position(t);
		if(heightChange<0){
			setPoint *=-1;
		}
		printf("SetPoint: %f \n",setPoint);
		setPoint+=startingHeight;
		SetArmAngleTo(setPoint);
		printf("SetPoint: %f", setPoint);
		if(t>m_trap.GetTotalTime()){
			m_state_lift++;
		}
		break;
	case 2:
		m_timer.Stop();
		m_state_lift++;
		break;
	case 3:
		return true;
//		if (height == 0) {
//			if (!m_ZeroLimit.Get()) {
//				controllerInterface.mLiftLeftMaster->Set(ControlMode::PercentOutput, -.2);
//			} else {
//				controllerInterface.mLiftLeftMaster->Set(ControlMode::PercentOutput, 0);
//				ZeroLift();
//				return true;
//			}
//		} else if (height == m_MaxArmAngle) {
//			if (!m_MaxLimit.Get()) {
//				controllerInterface.mLiftLeftMaster->Set(ControlMode::PercentOutput, .2);
//			} else {
//				controllerInterface.mLiftLeftMaster->Set(ControlMode::PercentOutput, 0);
//				MaxLift();
//				return true;
//			}
//		} else {
//			return true;
//		}
	}
	return false;
}
bool CubeManipulator::OutputCube(bool back){
	bool usingBack = !(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::BUTTON_REVERSE_OUTPUT));
	float t = m_timer.Get();
	float adjust = operatorInterface.joystick_manipulator->GetRawAxis(1);
	float speed = 1 - adjust;
	if(back)speed*=-1;
	bool atGround = m_currentArmAngle < 5;

	switch(m_state_output){
	case 0:
		m_timer.Reset();
		m_timer.Start();
		RunOutput(speed);
		if (atGround) {
			if(usingBack) ReverseIntake();
			if(!usingBack) StartIntake();
		}
		m_state_output++;
		break;
	case 1:
		if (t>1){
			RunOutput(0);
			StopIntake();
			m_timer.Stop();
			return true;
		}
		break;
	}
	return false;
}

//void CubeManipulator::ZeroLift(){
//	double armAngle = GetArmAngle();
//	controllerInterface.mLiftLeftMaster->SetSelectedSensorPosition(0, 0, 0);
//	controllerInterface.mLiftLeftMaster->Set(ControlMode::PercentOutput, 0);
//}
//
//void CubeManipulator::MaxLift() {
//	int sensorPosition = m_MaxArmAngle * robotDimensions::encoderTicksPerDegree;
//	controllerInterface.mLiftLeftMaster->SetSelectedSensorPosition(sensorPosition,0,0);
//	controllerInterface.mLiftLeftMaster->Set(ControlMode::PercentOutput, 0);
//}

void CubeManipulator::TrimScaleLift(float trim){
	float setPoint = robotDimensions::scaleLiftHeight + trim;
	switch(m_state_trim){
	case 0:
		if(MoveArmToAngle(setPoint)){
			m_state_lift = 0;
			break;
		}
		break;
	}
}
void CubeManipulator::RunOutput(float speed){
	controllerInterface.mFrontIntakeLeft->Set(ControlMode::PercentOutput,speed);
	controllerInterface.mFrontIntakeRight->Set(ControlMode::PercentOutput,speed);
}

