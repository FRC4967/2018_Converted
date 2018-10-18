/*
 * CI.h
 *
 *  Created on: Jan 10, 2018
 *      Author: monke
 */

#ifndef SRC_CI_H_
#define SRC_CI_H_
#include <WPILib.h>
#include "RobotMap.h"
#include "ctre/Phoenix.h"
class ControllerInterface {

public:
	// Motors
	TalonSRX* mLeftDriveMaster = new TalonSRX(CANBUSMap::MOTOR_LEFT_DRIVE1);
	TalonSRX* mLeftDriveFollower1 = new TalonSRX(CANBUSMap::MOTOR_LEFT_DRIVE2);
	VictorSPX* mLeftDriveFollower2 = new VictorSPX(CANBUSMap::MOTOR_LEFT_DRIVE3);
	TalonSRX* mRightDriveMaster = new TalonSRX(CANBUSMap::MOTOR_RIGHT_DRIVE1);
	TalonSRX* mRightDriveFollower1 = new TalonSRX(CANBUSMap::MOTOR_RIGHT_DRIVE2);
	VictorSPX* mRightDriveFollower2 = new VictorSPX(CANBUSMap::MOTOR_RIGHT_DRIVE3);
	TalonSRX* mLiftLeftMaster = new TalonSRX(CANBUSMap::MOTOR_LIFT_LEFT);
	TalonSRX* mLiftRightFollower = new TalonSRX(CANBUSMap::MOTOR_LIFT_RIGHT);
	VictorSPX* mLiftOutput = new VictorSPX(CANBUSMap::MOTOR_CUBE_OUTPUT1);
//	VictorSPX* mLiftOutputRight = new VictorSPX(CANBUSMap::MOTOR_CUBE_OUTPUT2);
	VictorSPX* mFrontIntakeLeft = new VictorSPX(CANBUSMap::MOTOR_FRONT_INTAKE1);
	VictorSPX* mFrontIntakeRight = new VictorSPX(CANBUSMap::MOTOR_FRONT_INTAKE2);
//	VictorSPX* mBackIntakeMaster = new VictorSPX(CANBUSMap::MOTOR_BACK_INTAKE1);
//	VictorSPX* mBackIntakeFollower = new VictorSPX(CANBUSMap::MOTOR_BACK_INTAKE2);
	TalonSRX* mArmManipulator = new TalonSRX(CANBUSMap::MOTOR_ARM_MANIPULATOR);
	PowerDistributionPanel* pdp = new PowerDistributionPanel(0);
	ControllerInterface() {
	}
	virtual ~ControllerInterface() {
	}

};

#endif /* SRC_CI_H_ */
