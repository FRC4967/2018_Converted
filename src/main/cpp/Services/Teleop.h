/*
 * Teleop.h
 *
 *  Created on: Jan 22, 2018
 *      Author: developer
 */
#include <WPILib.h>
#include "Objects/TankDrive.h"
#include "Services/Vision/CubeVision.h"
#include "Objects/Mechanisms/CubeManipulator/CubeManipulator.h"

#ifndef SRC_TELEOP_H_
#define SRC_TELEOP_H_

class Teleop {
	TankDrive &m_tank;
	CubeVision &m_cubeVision;
	CubeManipulator &m_cubeManipulator;


public:
	Teleop(TankDrive&, CubeVision&, CubeManipulator&);
	virtual ~Teleop();
	void TeleopPeriodic();
	void TeleopInit();
};

#endif /* SRC_TELEOP_H_ */
