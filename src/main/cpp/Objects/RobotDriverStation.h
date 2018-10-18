/*
 *
 * DriverStation.h
 *
 *  Created on: Jan 22, 2018
 *      Author: developer
 */

#ifndef SRC_ROBOTDRIVERSTATION_H_
#define SRC_ROBOTDRIVERSTATION_H_

class RobotDriverStation {
public:
	RobotDriverStation();
	virtual ~RobotDriverStation();
	void GetStartingPosition();
	void GetInstance();
	void GetGameSpecificMessage();
	void GetString();
	void GetBoolean();
	void GetTotalTime();
	void GetHsvThresholdOutput();
	void GetBlurOutput();
	void GetFindContoursOutput();
	void getCurrentXAndY();
};

#endif /* SRC_ROBOTDRIVERSTATION_H_ */
