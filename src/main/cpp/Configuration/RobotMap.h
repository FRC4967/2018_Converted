	/*
 * RobotMap.h
 *
 *  Created on: Jan 10, 2018
 *      Author: monke
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

enum DriverStationUsbMap {
	STICK_LEFT = 0,
	STICK_RIGHT = 1,
	STICK_MANIPULATOR = 2,
};

enum CANBUSMap {
	MOTOR_RIGHT_DRIVE1 = 1,
	MOTOR_RIGHT_DRIVE2 = 2,
	MOTOR_RIGHT_DRIVE3 = 3,
	MOTOR_LEFT_DRIVE1 = 4,
	MOTOR_LEFT_DRIVE2 = 5,
	MOTOR_LEFT_DRIVE3 = 6,
	MOTOR_LIFT_LEFT = 11,
	MOTOR_LIFT_RIGHT = 12,
	MOTOR_FRONT_INTAKE1 = 7,
	MOTOR_FRONT_INTAKE2 = 8,
	//MOTOR_BACK_INTAKE1 = 9,
	//MOTOR_BACK_INTAKE2 = 10,
	MOTOR_CUBE_OUTPUT1 = 13,
	MOTOR_ARM_MANIPULATOR = 14,
};

enum LeftButtonMap {
	BUTTON_REVERSE = 1,
	BUTTON_VISION = 4,
	BUTTON_VISION_TUNE = 11,
};
enum RightButtonMap {
	BUTTON_LOCK_SPEED = 1,
	BUTTON_FLASH_DEF_PER = 2,
	BUTTON_RAINBOW = 3,
};
enum ManipButtonMap {
//	BUTTON_INTAKE_CUBE = 4, //good
//	BUTTON_REVERSE_INTAKE = 11, //do not think we need this
//	BUTTON_LIFT_TO_SCALE = 5, //good
//	BUTTON_LIFT_TO_GROUND = 6, //good
//	BUTTON_LIFT_TO_SWITCH = 3, //good
	BUTTON_OUTPUT_CUBE = 1, //good
//	BUTTON_ZERO_LIFT = 10,
	BUTTON_ENABLE_ZERO=9, //good
	BUTTON_REVERSE_OUTPUT = 2, //good
//	BUTTON_ARM_UP = 8,
//	BUTTON_ARM_DOWN = 7,
//	BUTTON_MAKE_THE_LIFT_NOT_SUCK = 12,//
	ARM_FORWARD_45 = 5,
	ARM_BACKWARD_45 = 6,
	ARM_FORWARD_DOWN = 3,
	ARM_BACKWARD_DOWN =4,
	ARM_UP=10
};

class RobotMap {

};

#endif /* SRC_ROBOTMAP_H_ */
