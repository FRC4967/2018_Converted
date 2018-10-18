/*
 * Autonomous.h
 *
 *  Created on: Jan 9, 2018
 *      Author: monke
 */
#include <Services/Movements/CalculateMovement.h>
#include <WPILib.h>
#include "Objects/TankDrive.h"
#include "Services/Movements/TrapezoidalMove.h"
#include "Services/Movements/AdvancedTrapMove.h"
#include "Services/Movements/ExecuteMovements.h"
#include "Services/Vision/CubeVision.h"
#include "Objects/Mechanisms/CubeManipulator/CubeManipulator.h"
#include <string>

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_
using namespace std;
enum AutoMode {
	DO_NOTHING,
	CROSS_BASE_LINE,
	CUBE_TO_SWITCH,
	ALONG_THE_SIDE,
	AROUND_THE_BACK
};

enum AutoStates {
	INIT,
	VBUS_FORWARD,
	STOP,
	DONE,

	MOVE_DRIVE,
	MOVE_DRIVE_CONTINUE,
	PLACE_CUBE,
	DRIVE_TO_2ND_CUBE,
	PICK_UP_CUBE_ATS,	//"ATS" stands for "AlongTheSide"
	GET_2ND_CUBE,
	PICK_UP_CUBE,
	DETERMINE_WHAT_TO_DO,
	PLACE_CUBE_SWITCH,
	RETURN_TO_SCALE,
	PLACE_2ND_ON_SCALE,

	DRIVE_TO_PLAT,
	PLACE_CUBE_AUX,
	DRIVE_TO_CUBE_PILE,
	PICK_UP_CUBE_CTS,	//"CTS" stands for "CubeToSwitch"
	DRIVE_TO_EXCHANGE,
	PLACE_CUBE_IN_EXCHANGE,

};

class Autonomous {
	int m_state;
	AutoStates m_state_temp;
	Timer m_timer;
	AutoMode m_mode;
	std::string m_fieldPositions;
	char m_robotPosition;
	TankDrive &m_tank;
	ExecuteMovements &m_execMove;
	CubeManipulator &m_cubeManip;
	CubeVision &m_cubeVision;
	TrapezoidalMove m_trap;
	AdvancedTrapMove m_adTrap;
	CalculateMovement m_calcMove;
	bool m_switchesSameSideAsBot, m_scaleSameSideAsBot, m_opponentCanScale, m_pCanScale;


public:
	Autonomous(TankDrive&, ExecuteMovements&, CubeManipulator&, CubeVision&);
	virtual ~Autonomous();
	void AutonomousPeriodic();
	void AutonomousInit();
	void AutoLogic();
	string CurrentDateTime();

	void AroundTheBackLogic();
	void ATBFullPath();
	void ATBStopBetween();
	void ATBStopAtSwitch();

	void AlongTheSide();
	void CubeToSwitch();
	void CrossBaseLine();


	void Reset(float &right, float &left);
};

#endif /* SRC_AUTONOMOUS_H_ */
//useless line so this thing will let me push.  delete later
