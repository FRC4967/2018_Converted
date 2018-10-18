/*
 * ArmManipulator.cpp
 *
 *  Created on: Mar 13, 2018
 *      Author: harsh
 */

#include "Objects/Mechanisms/CubeManipulator/CubeManipulator.h"
void CubeManipulator::ArmTeleop(){
//	if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::BUTTON_ARM_UP)){
//				PositionArms(1);
//	}else if(operatorInterface.joystick_manipulator->GetRawButton(ManipButtonMap::BUTTON_ARM_DOWN)){
//	PositionArms(2);
//	}else{
//		PositionArms(0);
//	}
}
void CubeManipulator::PositionArms(int state) {
	int sensorPosition = controllerInterface.mArmManipulator->GetSelectedSensorPosition(CANBUSMap::MOTOR_ARM_MANIPULATOR);
	printf("Stuff: %i",sensorPosition);
	switch(state){
	case 0:
		controllerInterface.mArmManipulator->Set(ControlMode::PercentOutput, 0);
		break;
	case 1:
		controllerInterface.mArmManipulator->Set(ControlMode::PercentOutput, .7);
		break;
	case 2:
		controllerInterface.mArmManipulator->Set(ControlMode::PercentOutput, -.7);
		break;
	}
}

bool CubeManipulator::MoveArmsDown(){
	static int state;
	switch(state){
	case 0:
	m_timer_arms.Reset();
	m_timer_arms.Start();
	PositionArms(2);
	state++;
	break;
	case 1:
		if(m_timer_arms.Get()>.5){
			PositionArms(0);
			m_timer_arms.Stop();
			return true;
		}
	}
	return false;
}


