/*
 * ExecuteMovements.cpp
 *
 *  Created on: Feb 15, 2018
 *      Author: monke
 */

#include <Services/Movements/ExecuteMovements.h>

ExecuteMovements::ExecuteMovements(TankDrive &m_tankdrive) : m_tank(m_tankdrive) {
	// TODO Auto-generated constructor stub

}

ExecuteMovements::~ExecuteMovements() {
	// TODO Auto-generated destructor stub
}

bool ExecuteMovements::TurnXDegrees(double dblAngle) {
	static float rightVal;
	static float leftVal;
	static float fltTurnParam;
	static float fltRate = 10;
	float t = m_timer.Get();
	switch(m_turnState){
	case 0:
		m_tank.SetMode(ControlMode::Position);
		m_tank.Zero();
		m_timer.Reset();
		m_timer.Start();
		fltTurnParam = m_movement.Spin(dblAngle);
		m_trapMove.SetAll(fltRate, fltRate, fltRate*4, fabs(fltTurnParam));
		m_turnState++;
		break;
	case 1:
		rightVal = m_trapMove.Position(t);
		if(fltTurnParam<0) rightVal*=-1;
		leftVal = -rightVal;
		if(t>m_trapMove.GetTotalTime()) {
			m_turnState++;
		}
		break;
	case 2:
		m_tank.Zero();
		leftVal = rightVal = 0;
		m_timer.Stop();
		m_timer.Reset();
		break;
	}
	printf("State: %i, Turn Distance: %f, leftVal: %f, rightVal: %f, Time: %f, TrapMoveTime: %f \n",m_turnState,fltTurnParam,leftVal,rightVal,t,m_trapMove.GetTotalTime());
	m_tank.Drive(leftVal, rightVal);
	if(m_turnState >= 2) {
		m_turnState = 0;
		return true;
	} else {
		return false;
	}

}
bool ExecuteMovements::MoveForwardXInch(double dblDistance) {
	static float leftVal;
	static float rightVal;
	static float fltRate = 12;
	float fltTime = m_timer.Get();
	switch(m_driveState) {
	case 0:
		m_tank.SetMode(ControlMode::Position);
		m_tank.Zero();
		m_timer.Reset();
		m_timer.Start();
		m_trapMove.SetAll(fltRate, fltRate, fltRate*4, dblDistance);
		m_driveState++;
		break;
	case 1:
		leftVal = rightVal = m_trapMove.Position(fltTime);
		if (fltTime > m_trapMove.GetTotalTime()) {
			m_driveState++;
			break;
		}

		break;
	case 2:
		m_tank.Zero();
		leftVal = rightVal = 0;
		m_timer.Stop();
		m_timer.Reset();
		break;
	}

	m_tank.Drive(leftVal, rightVal);
	if (m_driveState >= 2) {
		m_driveState = 0;
		return true;
	} else {
		return false;
	}
}

bool ExecuteMovements::ArcMove(double dblAngle, double dblDistance) {
	static float leftVal;
	static float rightVal;
	static float secondTime;
	static float fltRate = 7;
	static ArcReturnStruct arcInfo;

	switch (m_arcState) {
	case 0:
		m_tank.SetMode(ControlMode::Position);
		m_tank.Zero();

		arcInfo = m_movement.Arc(dblAngle, dblDistance/4, false);
		m_trapMove.SetAll(fltRate, fltRate, fltRate*4, arcInfo.distance);

		m_timer.Reset();
		m_timer.Start();

		//TODO Increment m_arcState
			
		break;
	case 1:
		rightVal = m_trapMove.Position(m_timer.Get());
		leftVal = rightVal * arcInfo.ratio;

		if (secondTime) {
			std::swap(rightVal, leftVal);
		}

		if (m_timer.Get() > m_trapMove.GetTotalTime() && !secondTime) {
			secondTime = true;
			m_arcState = 0;
		} else if (m_timer.Get() > m_trapMove.GetTotalTime() && secondTime) {
			m_arcState++;
		}
		break;
	case 2:
		m_tank.Zero();
		leftVal = rightVal = 0;
		m_timer.Stop();
		m_timer.Reset();
		m_arcState++;
		break;
	}

	m_tank.Drive(leftVal, rightVal);
	if (m_arcState > 2) {
		m_arcState = 0;
		secondTime = false;
		return true;
	} else {
		return false;
	}
}


bool ExecuteMovements::AdTrapArc(float angle, float radius, bool clockwise, float accelTo, float accelTime, float decelTo, float decelTime) {
	static float leftVal;
	static float rightVal;
	static ArcReturnStruct arcInfo;

	switch(m_adArcState) {
	case 0:
		arcInfo = m_movement.Arc(angle, radius, clockwise);
		m_adTrap.SetAll(accelTo, accelTime, decelTo, decelTime, arcInfo.distance);
		Reset(leftVal, rightVal);
		m_adArcState++;
		break;
	case 1:
		rightVal = m_adTrap.Position(m_timer.Get());
		leftVal = rightVal * arcInfo.ratio;
		if (m_timer.Get() > m_adTrap.GetTotalTime()) {
			m_adArcState++;
		}
		break;
	}
	m_tank.Drive(leftVal, rightVal);

	if (m_adArcState > 1) {
		m_arcState = 0;
		return true;
	} else {
		return false;
	}


}

bool ExecuteMovements::AdTrapDrive(float accelTo, float accelTime, float decelTo, float decelTime, float distance) {
	static float leftVal;
	static float rightVal;

	switch (m_adMoveState) {
	case 0:
		m_adTrap.SetAll(accelTo, accelTime, decelTo, decelTime, distance);
		Reset(leftVal, rightVal);
		m_adMoveState++;
		break;
	case 1:
		rightVal = leftVal = m_adTrap.Position(m_timer.Get());
		if (m_timer.Get() > m_adTrap.GetTotalTime()) {
			m_adMoveState++;
		}
		break;
	}
	m_tank.Drive(leftVal, rightVal);

	if (m_adMoveState > 1) {
		m_adMoveState = 1;
		return true;
	} else  {
		return false;
	}
}

void ExecuteMovements::Reset(float &leftVal, float &rightVal) {
	m_timer.Stop();
	m_tank.Zero();
	rightVal = leftVal = 0;
	m_timer.Reset();
	m_timer.Start();
}
