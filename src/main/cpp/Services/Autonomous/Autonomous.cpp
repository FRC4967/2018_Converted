/*
 * Autonomous.cpp
 *
 *  Created on: Jan 9, 2018
 *      Author: monke
 */

#include <Services/Autonomous/Autonomous.h>
#include "Objects/TankDrive.h"
#include "Services/Movements/TrapezoidalMove.h"

Autonomous::Autonomous(TankDrive &m_tankdrive, ExecuteMovements &m_execute, CubeManipulator &m_manip, CubeVision &m_cube): m_tank(m_tankdrive), m_execMove(m_execute), m_cubeManip(m_manip), m_cubeVision(m_cube) {
	// TODO Auto-generated constructor stub

}

Autonomous::~Autonomous() {
	// TODO Auto-generated destructor stub
}

void Autonomous::AutonomousPeriodic() {
	switch (m_mode) {
	case AutoMode::DO_NOTHING:
		break;
	case AutoMode::CROSS_BASE_LINE:
		CrossBaseLine();
		break;
	case AutoMode::CUBE_TO_SWITCH:
		CubeToSwitch();
		break;
	case AutoMode::ALONG_THE_SIDE:
		AlongTheSide();
		break;
	case AutoMode::AROUND_THE_BACK:
		AroundTheBackLogic();
		break;
	}
}

void Autonomous::AutonomousInit() {
	//Initialize things
	//cameraServer = CameraServer::GetInstance();
	m_tank.Init();
	m_tank.SetMode(ControlMode::Position);
	m_tank.Zero();
	m_state = 0;
	m_state_temp = AutoStates::INIT;
	//m_cubeManip.ZeroLift();

	//get data from fms
	m_fieldPositions = frc::DriverStation::GetInstance().GetGameSpecificMessage();

	//get input from SmartDashboard
	m_robotPosition = Preferences::GetInstance()->GetString("Starting Position","m")[0];
	m_pCanScale = Preferences::GetInstance()->GetBoolean("Partner can Scale",false);
	m_opponentCanScale = Preferences::GetInstance()->GetBoolean("Opponent can Scale",false);

	//Print positions of switches and scales
	printf("Switches: %c, Scale: %c \n", m_fieldPositions[0], m_fieldPositions[1]);

	//Calculate what auto to do
	AutoLogic();
	printf("\n\n\nAuto Thingy: %i \n\n\n",m_mode);
}

void Autonomous::AutoLogic() {
	if (Preferences::GetInstance()->GetBoolean("Do Nothing",false)){
		m_mode = AutoMode::DO_NOTHING;
	} else if (Preferences::GetInstance()->GetBoolean("Cross Base Line", false)) {
		m_mode = AutoMode::CROSS_BASE_LINE;
		//Overriding position mode for this
		m_tank.SetMode(ControlMode::PercentOutput);
	}
	//if we're in the middle, do the switch auto
	else if(m_robotPosition == 'M'){
		m_mode = AutoMode::CUBE_TO_SWITCH;
	}
	else {
		//determine which field elements are on the same side as us
		m_switchesSameSideAsBot = m_robotPosition == m_fieldPositions[0];
		m_scaleSameSideAsBot = m_robotPosition == m_fieldPositions[1];

		//if we are on the same side as the scale, go along the side
		if (m_scaleSameSideAsBot) {
			m_mode = AutoMode::ALONG_THE_SIDE;
		}

		//if not, go around the back
		else {
			m_mode = AutoMode::AROUND_THE_BACK;
		}
	}
}

void Autonomous::Reset(float &right, float &left){
	m_timer.Stop();
	left=right=0;
	m_tank.Zero();
	m_timer.Reset();
	m_timer.Start();
	m_cubeManip.ResetStates();
}
