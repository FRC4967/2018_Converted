/*
 * AroundTheBack.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: Seveloper
 */

#include <Services/Autonomous/Autonomous.h>
#include <fstream>
#include <ctime>
void Autonomous::AroundTheBackLogic() {
	if (!m_pCanScale) {
		ATBFullPath();
	} else {
		if (m_switchesSameSideAsBot) {
			ATBStopBetween();
		} else {
			ATBStopBetween();
		}
	}
}
string Autonomous::CurrentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}
void Autonomous::ATBFullPath() {
	static ArcReturnStruct arcInfo;
	static bool onTheRight;
	static float rightPos;
	static float leftPos;
	static float speed = 80;
	float t = m_timer.Get();
	bool liftMoveDone;
	switch (m_state) {
	case 0:
		//initialization

		//NOTE DEFAULT PATH IS ON THE LEFT

		//determine which side we are on
		onTheRight = m_robotPosition == 'R';
		m_adTrap.SetAll(speed, 2, speed / 2, .5, 160);
		Reset(rightPos, leftPos);
		m_state++;
		break;
	case 1:
		rightPos = m_adTrap.Position(t);
		leftPos = rightPos;
		printf("First Straight MOVE\n");
		if (t > m_adTrap.GetTotalTime()) {
			arcInfo = m_calcMove.Arc(90, 51.25, true);
			m_adTrap.SetAll(speed / 2, .01, speed/1.2, .75, arcInfo.distance);
			Reset(rightPos, leftPos);
			m_state++;
		}
		break;
	case 2:
		m_cubeManip.MoveArmsDown();
		rightPos = m_adTrap.Position(t);
		leftPos = rightPos * arcInfo.ratio;

		//Check if we have reached the platform
		if (t > m_adTrap.GetTotalTime()) {
			Reset(rightPos, leftPos);
			m_adTrap.SetAll(speed/1.2, 0.01, speed / 2, 0.75, 50);
			m_state++;
		}
		break;
	case 3:
		rightPos = leftPos = m_adTrap.Position(t);

		//Check if we have reached the platform
		if (t > m_adTrap.GetTotalTime()) {
			Reset(rightPos, leftPos);
			m_adTrap.SetAll(speed / 2, 0.01, 0, 1, 88);
			m_state++;
		}
		break;
	case 4:
		rightPos = leftPos = m_adTrap.Position(t);
		liftMoveDone = m_cubeManip.MoveArmToAngle(
				robotDimensions::scaleLiftHeight);
		if (t > m_adTrap.GetTotalTime()) {
			arcInfo = m_calcMove.Arc(100, 15, false);
			m_trap.SetAll(1000 * speed, 1000 * speed, speed / 2,
					arcInfo.distance);
			Reset(rightPos, leftPos);
			m_state++;
		}
		break;
	case 5:
		rightPos = m_trap.Position(t);
		leftPos = rightPos * arcInfo.ratio;
		liftMoveDone = m_cubeManip.MoveArmToAngle(
				robotDimensions::scaleLiftHeight);
		if (t > m_trap.GetTotalTime()) {
			Reset(rightPos, leftPos);
			m_trap.SetAll(1000 * speed, speed / 2, speed / 2, 34);
			m_state++;
		}
		break;
	case 6:
		rightPos = leftPos = m_trap.Position(t);
		liftMoveDone = m_cubeManip.MoveArmToAngle(
				robotDimensions::scaleLiftHeight);
		if (t > m_trap.GetTotalTime() && liftMoveDone) {
			if (m_cubeManip.OutputCube(true)) {
				m_state++;
			}
		}
		break;
	}
	if (onTheRight) {
		std::swap(leftPos, rightPos);
	}

	printf("Case: %i, Time: %f\n ", m_state, t);
	printf("L:%f, R:%f \n", leftPos, rightPos);
	float leftDrive = m_tank.InvertDrive(leftPos, rightPos).left;
	float rightDrive = m_tank.InvertDrive(leftPos, rightPos).right;
	m_tank.Drive(leftDrive, rightDrive);
}

void Autonomous::ATBStopBetween() {
	static ArcReturnStruct arcInfo;
	static bool onTheRight;
	static float rightPos;
	static float leftPos;
	static float speed = 80;
	float t = m_timer.Get();
	switch (m_state) {
	case 0:
		//initialization

		//NOTE DEFAULT PATH IS ON THE LEFT

		//determine which side we are on
		onTheRight = m_robotPosition == 'R';
		m_adTrap.SetAll(speed, 2, speed / 2, .5, 160);
		Reset(rightPos, leftPos);
		m_state++;
		break;
	case 1:
		rightPos = m_adTrap.Position(t);
		leftPos = rightPos;
		printf("First Straight MOVE\n");
		if (t > m_adTrap.GetTotalTime()) {
			arcInfo = m_calcMove.Arc(90, 51.25, true);
			m_adTrap.SetAll(speed / 2, .01, speed/1.2, .75, arcInfo.distance);
			Reset(rightPos, leftPos);
			m_state++;
		}
		break;
	case 2:
		m_cubeManip.MoveArmsDown();
		rightPos = m_adTrap.Position(t);
		leftPos = rightPos * arcInfo.ratio;
		if (t > m_adTrap.GetTotalTime()) {
			Reset(rightPos, leftPos);
			m_adTrap.SetAll(speed/1.2, 0.01, speed / 2, 0.75, 50);
			m_state++;
		}
		break;
	case 3:
		rightPos = leftPos = m_adTrap.Position(t);

		//Check if we have reached the platform
		if (t > m_adTrap.GetTotalTime()) {
			Reset(rightPos, leftPos);
			m_adTrap.SetAll(speed / 2, 0.01, 0, 1, 55);
			m_state++;
		}
		break;
	case 4:
		rightPos = leftPos = m_adTrap.Position(t);
		if (t > m_adTrap.GetTotalTime()) {
			m_state++;
		}
		break;
	}
	if (onTheRight) {
		std::swap(leftPos, rightPos);
	}
	printf("Case: %i, Time: %f\n ", m_state, t);
	printf("L:%f, R:%f \n", leftPos, rightPos);
	float leftDrive = m_tank.InvertDrive(leftPos, rightPos).left;
	float rightDrive = m_tank.InvertDrive(leftPos, rightPos).right;
	m_tank.Drive(leftDrive, rightDrive);
}
void Autonomous::ATBStopAtSwitch() {
	static ArcReturnStruct arcInfo;
	static bool onTheRight;
	static float rightPos;
	static float leftPos;
	static float speed = 20;
	float t = m_timer.Get();
	bool liftMoveDone;
	string path = "/home/lvuser/outputs/AdvanceTrapTest.csv";
	static std::ofstream stream;
	switch (m_state) {
	case 0:
		//initialization

		//NOTE DEFAULT PATH IS ON THE LEFT

		//determine which side we are on
		onTheRight = m_robotPosition == 'R';
		stream.open(path);
		stream << "Start of Log at " << CurrentDateTime() << "\n";
		stream << "t" << ", " << "leftPos" << ", " << "rightPos" << std::endl;
		m_adTrap.SetAll(speed, 1, speed / 2, .5, 95);
		Reset(rightPos, leftPos);
		m_state++;
		break;
	case 1:
		rightPos = m_adTrap.Position(t);
		leftPos = rightPos;
		printf("First Straight MOVE\n");
		if (t > m_adTrap.GetTotalTime()) {
			arcInfo = m_calcMove.Arc(120, 25, true);
			m_adTrap.SetAll(speed / 2, .01, 0, .5, arcInfo.distance);
			Reset(rightPos, leftPos);
			m_state++;
		}
		break;
	case 2:
		m_cubeManip.MoveArmsDown();
		rightPos = m_adTrap.Position(t);
		leftPos = rightPos * arcInfo.ratio;
		liftMoveDone = m_cubeManip.MoveArmToAngle(
				robotDimensions::switchLiftHeight);
		//Check if we have reached the platform
		if ((t > m_adTrap.GetTotalTime()||t>1.5 )) {
			Reset(rightPos, leftPos);
			leftPos=rightPos=0;
			if(liftMoveDone){
			if (m_cubeManip.OutputCube(true)) {
				arcInfo = m_calcMove.Arc(270, 41, false);
				m_adTrap.SetAll(speed / 2, 1, 0, .75, arcInfo.distance);
				Reset(rightPos, leftPos);
				m_state++;
				}
			}
		}
		break;
	case 3:
		rightPos = -m_adTrap.Position(t);
		leftPos = rightPos * arcInfo.ratio;
		liftMoveDone = m_cubeManip.MoveArmToAngle(0);
		//Check if we have reached the platform
		if (t > m_adTrap.GetTotalTime() && liftMoveDone) {
			if (/*m_cubeVision.AcquireCube()*/true) {
				if (m_opponentCanScale) {
					arcInfo = m_calcMove.Arc(90, 23.5, true);
					Reset(rightPos, leftPos);
					m_adTrap.SetAll(speed / 6, 1, speed/3, .75, arcInfo.distance);
					m_state++;
				} else {
					m_state = 6;
				}
			}
		}
		break;
	case 4:
		rightPos = m_adTrap.Position(t);
		leftPos = rightPos * arcInfo.ratio;
		//Check if we have reached the platform
		if (t > m_adTrap.GetTotalTime()) {
			Reset(rightPos, leftPos);
			m_adTrap.SetAll(speed/3, 0.001, 0, .75, 60);
			m_state++;
		}
		break;
	case 5:
		rightPos = m_adTrap.Position(t);
		leftPos = rightPos;
		if (t > m_adTrap.GetTotalTime()) {
			m_state = 7;
		}
		break;
	case 6:
		liftMoveDone = m_cubeManip.MoveArmToAngle(
				robotDimensions::switchLiftHeight);
		if (liftMoveDone) {
			if (m_cubeManip.OutputCube(false)) {
				m_state++;
			}
			break;
		}
	}
	if (onTheRight) {
		std::swap(leftPos, rightPos);
	}

	stream << t << ", " << leftPos << ", " << rightPos << std::endl;

	printf("Case: %i, Time: %f\n ", m_state, t);
	printf("L:%f, R:%f \n", leftPos, rightPos);
	float leftDrive = m_tank.InvertDrive(leftPos, rightPos).left;
	float rightDrive = m_tank.InvertDrive(leftPos, rightPos).right;
	m_tank.Drive(leftDrive, rightDrive);
}
