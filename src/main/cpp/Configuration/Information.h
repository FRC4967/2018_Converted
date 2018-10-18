/*
 * Information.h
 *
 *  Created on: Jan 15, 2018
 *      Author: Collin M
 */

#ifndef SRC_INFORMATION_H_
#define SRC_INFORMATION_H_
struct fieldDimensions {
		static constexpr float wallToSwitch = 140;
        static constexpr float wallToBackOfSwitch = 190;
		static constexpr float wallToFrontOfPlatform = 261.47;
		static constexpr float sideToSwitch = 85.25;
		static constexpr float fieldWidth = 360;
		static constexpr float fieldLength = 648;
		static constexpr float wallToNullTerritory = 288;
		static constexpr float wallToScalePlate = 299.65;
		static constexpr float sideToScalePlate = 71.57;
		static constexpr float wallToCubeZone = 98;
		static constexpr float centerToExchange=12;
		static constexpr float wallToBaseline=120;
		static constexpr float sideToPlatform=95.25;
		static constexpr float nullTerritoryWidth = 72;
		static constexpr float switchMaxHeight = 19;
		static constexpr float scaleFieldHeight = 60;
	};
	struct robotDimensions {
		static constexpr float length = 32.25;
		static constexpr float width = 27;
		static constexpr float wheelDiameter = 6;
		static constexpr float wheelCircumference = wheelDiameter*3.14159;
		static constexpr float encoderTicksPerRevolution = 4096;
		static constexpr float encoderTicksPerInch = encoderTicksPerRevolution/wheelCircumference;
		static constexpr float encoderTicksPerDegree = (encoderTicksPerRevolution * 6)/360; // 6 to 1 ratio on external gears
		//TODO Get actually values
		static constexpr float distanceBetweenWheels = 23.5;
		static constexpr float centerToWheels=distanceBetweenWheels/2;
		static constexpr float voltPerDegree = .016016;
		static constexpr float zeroAngleVolt = .4;
		//The amount of encoder ticks required to move the lift one inch
		//The 10919 came from some testing where we pulled the chain one foot, so we would divide by 12, and then there is a multiplier of 3 from the inner
		//section to the chain, hence the 36.
		static constexpr float encoderTicksPerInchLift = 10919/36;
		static constexpr float liftMaxHeight = 70;
		static constexpr float switchHeightToAdd = 22;
		static constexpr float scaleHeightToAdd = 10;
		static constexpr float scaleLiftHeight = fieldDimensions::scaleFieldHeight + scaleHeightToAdd;
		static constexpr float switchLiftHeight = fieldDimensions::switchMaxHeight+switchHeightToAdd;
		static constexpr float scaleTrimDistance = liftMaxHeight-scaleLiftHeight;

		static constexpr float leftOutputMotorToRightRatio=1.5; // How many rotations the Left Motor does for the Right
	};

class Information {

	// inches.
};

#endif /* SRC_INFORMATION_H_ */
