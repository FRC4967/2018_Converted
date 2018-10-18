/*
 * OI.h
 *
 *  Created on: Jan 10, 2018
 *      Author: monke
 */

#ifndef SRC_OI_H_
#define SRC_OI_H_
#include <WPILib.h>
#include "RobotMap.h"
class OperatorInterface {

	double dblXRes;
	double dblYRes;

	public:
		// Joysticks
		frc::Joystick* joystick_driverLeft = new frc::Joystick(DriverStationUsbMap::STICK_LEFT);
		frc::Joystick* joystick_driverRight = new frc::Joystick(DriverStationUsbMap::STICK_RIGHT);
		frc::Joystick* joystick_manipulator = new frc::Joystick(DriverStationUsbMap::STICK_MANIPULATOR);

	OperatorInterface() {
	}
	virtual ~OperatorInterface() {
	}

};


#endif /* SRC_OI_H_ */
