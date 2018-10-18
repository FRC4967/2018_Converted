/*
 * ExecuteMovements.h
 *
 *  Created on: Feb 15, 2018
 *      Author: monke
 */
#include <WPILib.h>
#include "Objects/TankDrive.h"
#include "Services/Movements/CalculateMovement.h"
#include "Services/Movements/AdvancedTrapMove.h"
#ifndef SRC_SERVICES_MOVEMENTS_EXECUTEMOVEMENTS_H_
#define SRC_SERVICES_MOVEMENTS_EXECUTEMOVEMENTS_H_

class ExecuteMovements {
private:
	TankDrive &m_tank;
	Timer m_timer;
	TrapezoidalMove m_trapMove;
	AdvancedTrapMove m_adTrap;
	CalculateMovement m_movement;
	int m_turnState = 0;
	int m_driveState = 0;
	int m_arcState = 0;
	int m_adArcState = 0;
	int m_adMoveState = 0;
	void Reset(float &leftVal, float &rightVal);
public:
	ExecuteMovements(TankDrive&);
	virtual ~ExecuteMovements();
	bool TurnXDegrees(double dblAngle);
	bool MoveForwardXInch(double dblDistance);
	bool ArcMove(double dblAngle, double dblDistance);
	bool AdTrapArc(float angle, float radius, bool clockwise, float accelTo, float accelTime, float decelTo, float decelTime);
	bool AdTrapDrive(float accelTo, float accelTime, float decelTo, float decelTime, float distance);
};

#endif /* SRC_SERVICES_MOVEMENTS_EXECUTEMOVEMENTS_H_ */
