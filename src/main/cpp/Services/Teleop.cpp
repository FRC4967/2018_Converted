/*
 * Teleop.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: developer
 */

#include <Services/Teleop.h>
#include "Objects/TankDrive.h"
#include "Movements/TrapezoidalMove.h"

Teleop::Teleop(TankDrive &m_tankdrive, CubeVision &m_cube, CubeManipulator &m_manipulator)
				: m_tank(m_tankdrive), m_cubeVision(m_cube), m_cubeManipulator(m_manipulator) {
	// TODO Auto-generated constructor stub

}

Teleop::~Teleop() {
	// TODO Auto-generated destructor stub
}

void Teleop::TeleopPeriodic() {
	m_tank.TeleopPeriodic();
	m_cubeVision.TeleopPeriodic();
	m_cubeManipulator.TeleopPeriodic();
}

void Teleop::TeleopInit(){
	m_tank.Init();
	m_cubeManipulator.TeleopInit();

}
