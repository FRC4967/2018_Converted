/*
 * AlongTheSide.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: Seveloper
 */

#include <Services/Autonomous/Autonomous.h>

void Autonomous::AlongTheSide() {
	static ArcReturnStruct arcInfo;
	static bool onTheRight;
	static bool secondTime;
	static bool liftDone;
	static bool drive = true;
	static float speed = 60;
	float rightPos;
	float leftPos;
	float t = m_timer.Get();
	switch (m_state_temp) {
	case AutoStates::INIT:
		//initialization

		//NOTE DEFAULT PATH IS ON THE LEFT

		m_trap.SetAll(speed, 1000*speed, speed, 150);
		Reset(rightPos, leftPos);

		//determine which side we are on
		onTheRight = m_robotPosition == 'R';

		m_state_temp = MOVE_DRIVE;
		break;
	case AutoStates::MOVE_DRIVE:
		//Movement/Driving
		rightPos = leftPos = m_trap.Position(t);
		if (t>1.5) liftDone=m_cubeManip.MoveArmToAngle(robotDimensions::scaleLiftHeight);
		if (t > m_trap.GetTotalTime()) {
			arcInfo = m_calcMove.Arc(40-2, 90+6, true);
			secondTime = false;
			m_trap.SetAll(1000*speed, 1000*speed, speed, arcInfo.distance);
			Reset(rightPos, leftPos);
			m_state_temp = MOVE_DRIVE_CONTINUE;
		}
		break;
	case AutoStates::MOVE_DRIVE_CONTINUE:
		//Movement/Driving continued
		liftDone=m_cubeManip.MoveArmToAngle(robotDimensions::scaleLiftHeight);
		rightPos = m_trap.Position(t);
		leftPos = rightPos * arcInfo.ratio;
		m_cubeManip.MoveArmsDown();
		//MIRROR ARC SECOND TIME
		if (secondTime) {
			swap(rightPos, leftPos);

		}

		//Check if we have reached the platform
		if (t > m_trap.GetTotalTime() && !secondTime) {
			printf(" \n Second Arc! \n");
			Reset(rightPos, leftPos);
			m_trap.SetAll(1000*speed, speed/1.4, speed, arcInfo.distance);
			secondTime = true;
		} else if (t > m_trap.GetTotalTime() && secondTime&&liftDone) {
			Reset(rightPos, leftPos);
			m_cubeManip.m_state_lift = 0;
			m_state_temp = PLACE_CUBE;
		}
		break;
	case AutoStates::PLACE_CUBE:
		//Place Cube
		printf("I put a cube somewhere or something idk \n");
		if (m_cubeManip.OutputCube(true)) {
			m_cubeManip.m_state_output = 0;
			arcInfo = m_calcMove.Arc(23, 66, true);
			m_trap.SetAll(speed, speed, speed, arcInfo.distance);
			secondTime = false;
			Reset(rightPos, leftPos);
			m_state_temp = DRIVE_TO_2ND_CUBE;
		}
		break;
	case AutoStates::DRIVE_TO_2ND_CUBE:
		//Drive to Switch to get second Cube
		liftDone = m_cubeManip.MoveArmToAngle(0);
		rightPos = -m_trap.Position(t);
		leftPos = rightPos * arcInfo.ratio;
		//MIRROR ARC SECOND TIME
		if (secondTime) {
			swap(rightPos, leftPos);
		}

		//Check if we have reached the platform
		if (t > m_trap.GetTotalTime() && !secondTime) {
			printf(" \n Second Arc! \n");
			Reset(rightPos, leftPos);
			m_trap.SetAll(speed, speed, speed, arcInfo.distance);
			secondTime = true;
		} else if (t > m_trap.GetTotalTime() && secondTime&&liftDone) {
			Reset(rightPos, leftPos);
			m_cubeManip.m_state_lift = 0;
			m_state_temp = PICK_UP_CUBE_ATS;
		}
		break;
	case AutoStates::PICK_UP_CUBE_ATS:
		//pick up Cube. MAY TAKE VISION
		drive = false;
		if (/*m_cubeVision.AcquireCube()*/true) {
			m_state_temp = DETERMINE_WHAT_TO_DO;
			drive = true;
		}
		break;
	case AutoStates::DETERMINE_WHAT_TO_DO:
		//check what to do with cube and then start doing that

		//check if switch is on same side as scale
		if (m_switchesSameSideAsBot) {
			//check input and/or vision to see if we should put cube on switch instead of scale
			if (!m_opponentCanScale) {
				//drive remaining distance to switch if necessary
				m_state_temp = PLACE_CUBE_SWITCH;
				break;
			} 
			else {
				//initialize code to drive back to scale

				arcInfo = m_calcMove.Arc(23, 66, true);
				m_trap.SetAll(speed, speed, speed, arcInfo.distance);
				secondTime = false;
				Reset(rightPos, leftPos);
				m_state_temp = RETURN_TO_SCALE;
				break;
			}

		} else {
			//initialize code to drive back to scale

			arcInfo = m_calcMove.Arc(23, 66, true);
			m_trap.SetAll(speed, speed, speed, arcInfo.distance);
			secondTime = false;
			Reset(rightPos, leftPos);
			m_state_temp = RETURN_TO_SCALE;
			break;
		}
	case AutoStates::PLACE_CUBE_SWITCH:
		//place cube on Switch
		if(m_cubeManip.MoveArmToAngle(robotDimensions::switchLiftHeight)){
			if(m_cubeManip.OutputCube(false)){
				m_state_temp = DONE;
				m_cubeManip.m_state_output = 0;
				m_cubeManip.m_state_lift = 0;
			}
		}
		break;
	case AutoStates::RETURN_TO_SCALE:
		//return to scale
		liftDone = m_cubeManip.MoveArmToAngle(robotDimensions::scaleLiftHeight);
		rightPos = m_trap.Position(t);
		leftPos = rightPos * arcInfo.ratio;
		//MIRROR ARC SECOND TIME
		if (secondTime) {
			swap(rightPos, leftPos);
		}

		//Check if we have reached the platform
		if (t > m_trap.GetTotalTime() && !secondTime) {
			printf(" \n Second Arc! \n");
			Reset(rightPos, leftPos);
			m_trap.SetAll(speed, speed, speed, arcInfo.distance);
			secondTime = true;
		} else if (t > m_trap.GetTotalTime() && secondTime && liftDone) {
			Reset(rightPos, leftPos);
			m_cubeManip.m_state_lift = 0;
			m_state_temp = PLACE_2ND_ON_SCALE;
		}
		break;
	case AutoStates::PLACE_2ND_ON_SCALE:
		//place second cube on scale
		if(m_cubeManip.OutputCube(true)){
			m_cubeManip.m_state_output = 0;
			m_state_temp = DONE;
		}
		break;
	case AutoStates::DONE:
		break;
	default:
		break;
	}

	if (onTheRight) {
		std::swap(leftPos, rightPos);
	}
	printf("Time: %f ", t);
	printf("L:%f, R:%f \n", leftPos, rightPos);
	float leftDrive = m_tank.InvertDrive(leftPos,rightPos).left;
	float rightDrive = m_tank.InvertDrive(leftPos,rightPos).right;
	if (drive) m_tank.Drive(leftDrive, rightDrive);

}
