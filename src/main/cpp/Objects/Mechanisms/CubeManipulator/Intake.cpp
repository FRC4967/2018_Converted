/*
 * Intake.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: monke
 */
#include <Objects/Mechanisms/CubeManipulator/CubeManipulator.h>
bool CubeManipulator::IntakeCube(float timeout){
	//TODO Do the thing
	if (timeout==0) timeout = 100000;
	float currentThreshold = 6;
	float secondsAfter1stSpike = .75;
	float outputSpeed = -.75;
	//TODO it is not getting the current spike
	double current = ((controllerInterface.pdp->GetCurrent(5) +controllerInterface.pdp->GetCurrent(4))/2);
	printf("OutputCurrent: %f\n", current);
	float t = m_timer.Get();
	switch(m_state_intake){
	case 0:
		//Init
		StartIntake();
		RunOutput(outputSpeed);
		m_timer.Reset();
		m_timer.Start();
		m_state_intake++;
		break;
	case 1:
		if((current>currentThreshold && m_timer.Get() > .5)){
			m_timer.Reset();
			m_timer.Start();
			printf("Current Threshold Reached\n");
			m_state_intake++;
			break;
		} else if (m_timer.Get()>timeout) {
			return true;
		}

		break;
	case 2:
		if(m_timer.Get()>secondsAfter1stSpike){
			//m_timer.Stop();
			StopIntake();
			RunOutput(0);
			//return true;
			if (m_timer.Get() > 1.5) {
				m_state_intake++;
			}
		}
		break;
	case 3:
		StartIntake();
		RunOutput(outputSpeed);
		m_timer.Reset();
		m_timer.Start();
		m_state_intake++;
		break;
	case 4:
		if(m_timer.Get() > 1) {
			m_timer.Stop();
			StopIntake();
			RunOutput(0);
			m_cubeInLift = true;
			return true;
		}
		break;
	}
	return false;
}

void CubeManipulator::ReverseIntake(){
	float speed = -.75;
	RunIntake(speed,speed);
}

void CubeManipulator::StartIntake(){
	float speed = 1;
	RunIntake(speed,speed);
}

void CubeManipulator::StopIntake() {
	RunIntake(0,0);
}

void CubeManipulator::RunIntake(float right, float left){
		controllerInterface.mFrontIntakeLeft->Set(ControlMode::PercentOutput, right);
		controllerInterface.mFrontIntakeRight->Set(ControlMode::PercentOutput, left);
}
bool CubeManipulator::IntakeJiggle(float timeout){
	//TODO Do the thing
	if (timeout==0) timeout = 100000;
	float currentThreshold = 6;
	float secondsAfter1stSpike = .75;
	float outputSpeed = -.75;
	//TODO it is not getting the current spike
	double current = ((controllerInterface.pdp->GetCurrent(5) +controllerInterface.pdp->GetCurrent(4))/2);
	printf("OutputCurrent: %f\n", current);
	float t = m_timer.Get();
	switch(m_state_intake){
	case 0:
		//Init
		StartIntake();
		RunOutput(outputSpeed);
		m_timer.Reset();
		m_timer.Start();
		m_state_intake++;
		break;
	case 1:
		if((m_timer.Get() > 1)){
			m_timer.Reset();
			m_timer.Start();
			printf("Current Threshold Reached\n");
			m_state_intake++;
			break;
		}
		break;
	case 2:
		if(m_timer.Get()>secondsAfter1stSpike){
			//m_timer.Stop();
			RunIntake(-.5,1);
			//return true;
			if (m_timer.Get() > 1) {
				m_state_intake++;
			}
		}
		break;
	case 3:
		StartIntake();
		RunOutput(outputSpeed);
		m_timer.Reset();
		m_timer.Start();
		m_state_intake++;
		break;
	case 4:
		if(m_timer.Get() > 1.5) {
			m_timer.Stop();
			StopIntake();
			RunOutput(0);
			m_cubeInLift = true;
			return true;
		}
		break;
	}
	return false;
}
