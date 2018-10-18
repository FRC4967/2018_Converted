/*
 * CrossBaseLine.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: developer
 */
#include <Services/Autonomous/Autonomous.h>

void Autonomous::CrossBaseLine() {

	static frc::Timer AutoTimer;
	switch (m_state_temp) {
	case AutoStates::INIT:
		printf("Starting Cross Base Line\n");
		AutoTimer.Reset();
		AutoTimer.Start();
		m_tank.Drive(.3, .3);
		m_state_temp = VBUS_FORWARD;
		break;
	case AutoStates::VBUS_FORWARD:
		printf("Driving forward, Timer is %f \n", AutoTimer.Get());
		if (AutoTimer.Get() >= 3) {
			m_state_temp = STOP;
		}
		break;
	case AutoStates::STOP:
		printf("Stopping\n");
		m_tank.Drive(0, 0);
		m_state_temp = DONE;
		break;
	case AutoStates::DONE:
		break;
	default:
		break;
	}
}
