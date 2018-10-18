/*
 * TankDrive.h
 *
 *  Created on: Jan 8, 2018
 *      Author: harsh
 */

#ifndef SRC_TANKDRIVE_H_
#define SRC_TANKDRIVE_H_

#include <WPILib.h>
#include <ctre/Phoenix.h>
#include <Configuration/ControllerInterface.h>
#include <Configuration/OperatorInterface.h>
#include "Configuration/Information.h"
#include "Objects/Mechanisms/CubeManipulator/CubeManipulator.h"
	struct driveVal {
		float right;
		float left;
	};
class TankDrive {
	Information m_info;

private:
	float m_liftHeightPercentDeduction;
	ControllerInterface &controllerInterface;
	OperatorInterface &operatorInterface;
	CubeManipulator &cubeManipulator;
	double m_AngleError = 0.0;
public:
	int intState = 0;
	ControlMode m_driveMode = ControlMode::PercentOutput;
	TankDrive(ControllerInterface &controllerInterface,
			OperatorInterface &operatorInterface, CubeManipulator &cubeManipulator);
	virtual ~TankDrive();
	void SetPID(TalonSRX *motor, double p, double i, double d, double f);
	void Init();
	void TeleopPeriodic();
	void Drive(float left_speed, float right_speed);
	void Zero();
	driveVal InvertDrive(float left, float right);
	void SetMode(ControlMode);
};

//CAN Talon Init

//Teleop Drive

//TankDrive Init

//Drive X Inches

//Turn Left (ARC or PIVOT)

//Turn Right (ARC or PIVOT)

//Differential Drive

//Shift (If Robot Has)

//Set Mode of Motor Control



#endif /* SRC_TANKDRIVE_H_ */
