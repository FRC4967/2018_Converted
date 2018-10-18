/*
 * CubeToSwitch.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: monke
 */

#include <Services/Autonomous/Autonomous.h>

void Autonomous::CubeToSwitch() {
	static ArcReturnStruct arcInfo;
	static bool toTheRight;
	static bool secondTime;
	static bool liftDone;
	static bool drive = true;
	float rightPos;
	float leftPos;
	float leftDrive, rightDrive;
	float t = m_timer.Get();
	//TODO Make m_state an enumerated thingy
	switch(m_state_temp) {
	case AutoStates::INIT: //0
		//initialization
		arcInfo = m_calcMove.Arc(56,62,false);
		secondTime = false;

		//NOTE DEFAULT PATH IS TO THE LEFT

		m_trap.SetAll(30,1200,30,arcInfo.distance);
		Reset(rightPos,leftPos);



		//TODO Basic init code

		//determine which side our platform is on
		toTheRight = m_fieldPositions[0]=='R';
		m_state_temp = DRIVE_TO_PLAT;
		break;
	case AutoStates::DRIVE_TO_PLAT:
		rightPos = m_trap.Position(t);
		leftPos = rightPos*arcInfo.ratio;
		//MIRROR ARC SECOND TIME
		if(secondTime){
			swap(rightPos,leftPos);
			liftDone = m_cubeManip.MoveArmToAngle(robotDimensions::switchLiftHeight);
		}
		//TODO Code to drive to platform

		//Check if we have reached the platform
		if(t>m_trap.GetTotalTime()&&!secondTime){
			printf(" \n Second Arc! \n");
			Reset(rightPos,leftPos);
			m_trap.SetAll(1200,30,30,arcInfo.distance);
			secondTime = true;
		}else if (t>m_trap.GetTotalTime()&&secondTime&&liftDone){
			Reset(rightPos,leftPos);
			m_cubeManip.m_state_lift = 0;
			m_state_temp = PLACE_CUBE_AUX;
		}
		break;
	case AutoStates::PLACE_CUBE_AUX:
		m_cubeManip.MoveArmsDown();
		//Place Cube and/or other auxiliary tasks
		printf("I put a cube somewhere or something idk \n");
		//TODO Code to place cube Method to call, see below.
		if(m_cubeManip.OutputCube(true)){
			m_trap.SetAll(30,30,30,20);
			Reset(rightPos,leftPos);
			m_cubeManip.m_state_output = 0;
			m_state_temp = DRIVE_TO_CUBE_PILE;
		}
		break;
	case AutoStates::DRIVE_TO_CUBE_PILE:
		//Drive to Cube Pile
		//negative because we are driving backwards.
		rightPos = -m_trap.Position(t);
		leftPos = rightPos;
		liftDone = m_cubeManip.MoveArmToAngle(0);
		printf("I'm Moving to The Cube Pile! I've gone %f Inches! \n", rightPos);
		//TODO Code to drive to cube pile

		//Check if we have reached Cube Pile
		if(t>m_trap.GetTotalTime()&&liftDone){


			printf("Turning\n");
			float angle = -80;
			if(toTheRight) angle*=-1;
			if(m_execMove.TurnXDegrees(angle)){
			printf("I'm There! \n");
			Reset(rightPos,leftPos);
			m_cubeManip.m_state_lift = 0;
			m_state_temp = PICK_UP_CUBE_CTS;
			drive = true;
			}
			else{
				drive = false;
			}
		}
		break;
	case AutoStates::PICK_UP_CUBE_CTS:
		//pick up cube. MAY TAKE VISION

		printf("I picked up a cube! \n");
		//TODO Code to Pick up Cube
		drive = false;
		if(/*m_cubeVision.AcquireCube()*/true){
			/* again, We should have some sort of return for a bool here*/
			drive = true;
			if (toTheRight){
				arcInfo=m_calcMove.Arc(90,60,false);
				secondTime=true;
			}else{
				arcInfo=m_calcMove.Arc(90,24.5,false);
				secondTime=false;
			}
			m_trap.SetAll(30,30,30,arcInfo.distance);
			Reset(rightPos,leftPos);
			m_state_temp = DRIVE_TO_EXCHANGE;
		}

		break;
	case AutoStates::DRIVE_TO_EXCHANGE:
		//drive to exchange zone MAY TAKE VISION
		rightPos = -m_trap.Position(t);
		if(!toTheRight) rightPos*=-1;
		leftPos = rightPos*arcInfo.ratio;
		if(secondTime&&!toTheRight) leftPos=rightPos;
		if(t>m_trap.GetTotalTime()&&!secondTime){
			m_trap.SetAll(15,10,20,44);
			Reset(rightPos,leftPos);
			secondTime=true;
		}
		printf("I'm Moving To the Exchange Zone! I've gone %f Inches! \n", rightPos);
		//check if we are at exchange zone
		if(t>m_trap.GetTotalTime()&&secondTime){
			printf("I'm There");
			Reset(rightPos,leftPos);
			m_state_temp = PLACE_CUBE_IN_EXCHANGE;
		}
		break;
	case AutoStates::PLACE_CUBE_IN_EXCHANGE:
		//put cube in exchange zone
		printf("I Put a cube in the Exchange Zone! \n");

		//TODO Code to put cube in exchange zone
		if(m_cubeManip.OutputCube(false)) {
			//see above
			m_state_temp = DONE;
		}
		break;
	default:
		break;
	}
	if(toTheRight){
		std::swap(leftPos,rightPos);
	}
	leftDrive = m_tank.InvertDrive(leftPos,rightPos).left;
	rightDrive = m_tank.InvertDrive(leftPos,rightPos).right;
	printf("Time: %f ",t);
	printf("L:%f, R:%f \n",leftDrive,rightDrive);
	if(drive){ m_tank.Drive(leftDrive,rightDrive); printf("driving\n");}
}
