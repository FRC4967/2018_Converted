/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <Services/Autonomous/Autonomous.h>
#include <iostream>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <Services/Vision/CubeVision.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

#include "Services/Teleop.h"
#include "Objects/TankDrive.h"


class Robot : public frc::IterativeRobot {
	Autonomous *m_autonomous = nullptr;
	Teleop *m_teleop = nullptr;
	ControllerInterface controllerInterface;
	OperatorInterface operatorInterface;
	CubeManipulator m_cubeManipulator;
	TankDrive m_tank;
	CubeVision m_cube;
	ExecuteMovements m_executeMovements;
public:
	Robot() :
			m_tank(controllerInterface, operatorInterface, m_cubeManipulator), m_cube(m_tank, m_executeMovements, m_cubeManipulator),
			m_cubeManipulator(controllerInterface, operatorInterface), m_executeMovements(m_tank) {
			m_teleop = new Teleop(m_tank, m_cube, m_cubeManipulator);
			m_autonomous = new Autonomous(m_tank, m_executeMovements, m_cubeManipulator,m_cube);

	}
	void RobotInit() {
//		m_cubeManipulator.ZeroLift();

	}


	void AutonomousInit() override {
		m_autonomous->AutonomousInit();
	}

	void AutonomousPeriodic() {
		m_autonomous->AutonomousPeriodic();
		controllerInterface.pdp->ClearError();
	}

	void TeleopInit() {
		m_teleop->TeleopInit();
		controllerInterface.mLiftLeftMaster->Set(ControlMode::Position, m_cubeManipulator.m_setPoint);
	}

	void TeleopPeriodic() {
		m_teleop->TeleopPeriodic();
		controllerInterface.pdp->ClearError();
	}

	void DisabledPeriodic() override {
		controllerInterface.pdp->ClearError();
		m_cubeManipulator.ArmInit();
	}

	void TestPeriodic() {}

private:
};

START_ROBOT_CLASS(Robot)
