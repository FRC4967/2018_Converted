/*
 * CubeManipulator.h
 *
 *  Created on: Jan 27, 2018
 *      Author: UwU it's Braeden
 */


#ifndef SRC_MECHANISMS_CubeManipulator_H_
#define SRC_MECHANISMS_CubeManipulator_H_
#include "Services/Movements/TrapezoidalMove.h"
#include "Configuration/ControllerInterface.h"
#include "Configuration/OperatorInterface.h"
#include "Configuration/Information.h"
#include "ctre/Phoenix.h"
#include "WPILib.h"


class CubeManipulator {
	enum teleopStates {
		GET_COMMAND,
		OUTPUT,
		LIFT_TO_GROUND,
		LIFT_TO_SWITCH,
		LIFT_TO_SCALE,
		INTAKE_CUBE,
		REVERSE_INTAKE,
		CHANGE_ARM_UP,
		CHANGE_ARM_DOWN,
		RESET,
		ARM_FORWARD_45 ,
		ARM_BACKWARD_45 ,
		ARM_FORWARD_DOWN ,
		ARM_BACKWARD_DOWN,
		ARM_UP
	};
	int m_state_trim;
	teleopStates m_state_teleop;
	float m_currentArmAngle;
	float m_MaxArmAngle;
	TrapezoidalMove m_trap;
	ControllerInterface &controllerInterface;
	OperatorInterface &operatorInterface;
	Timer m_timer;
	Timer m_timer_arms;
	frc::DigitalInput m_ZeroLimit;
	frc::DigitalInput m_MaxLimit;
	frc::AnalogInput m_Arm420Device;
	bool m_tunePID = false;

public:
	int m_state_lift;
	int m_state_output;
	int m_state_intake;
	bool m_cubeInLift;
	double m_setPoint;
	CubeManipulator(ControllerInterface &controllerInterface,
			OperatorInterface &operatorInterface);;
	virtual ~CubeManipulator();
	void TeleopPeriodic();
	void TeleopInit();
	void SetPID(TalonSRX*,double,double,double,double);
	bool IntakeCube(float timeout);
	bool IntakeJiggle(float timeout);
	void StartIntake();
	void StopIntake();
	void RunIntake(float right, float left);
	void RunOutput(float);
	void ReverseIntake();
	bool MoveArmToAngle(float);
	void SetArmAngleTo(float);
	void TrimScaleLift(float);
	bool OutputCube(bool);
//	void ZeroLift();
//	void MaxLift();
	void ResetStates();
	float GetArmAngle();
	void PositionArms(int);
	bool MoveArmsDown();
	void ArmTeleop();
	void ArmInit();
	void TunePID();
};

#endif /* SRC_MECHANISMS_CubeManipulator_H_ */
