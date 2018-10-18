/*
 * Movement.cpp
 *
 *  Created on: Jan 20, 2018
 *      Author: monke
 *
 *      This class is for calculating specific movement types and specific routes.
 */

#include <stdio.h>
#include <Services/Movements/CalculateMovement.h>
CalculateMovement::CalculateMovement() {

}

CalculateMovement::~CalculateMovement() {
	// TODO Auto-generated destructor stub
}

ArcReturnStruct CalculateMovement::Arc(float angle, float radius, bool clockwise) {
	//Needed multiple values returned, so we're returning a structure.
	ArcReturnStruct Returns;
	//radius of the two curves formed by the wheels
	float innerRadius = radius-robotDimensions::centerToWheels;
	float outerRadius = radius+robotDimensions::centerToWheels;

	//length of each wheel's curve
	float innerArcLength = (angle/360)*6.28318*innerRadius;
	float outerArcLength = (angle/360)*6.28318*outerRadius;

	//ratio of right wheel to left wheel ex. right*ratio=left
	float ratio = outerArcLength/innerArcLength;

	//if the robot is going clockwise, the right wheel is on the inside, if clockwise, it's on the outside
	if (clockwise) {
		Returns.distance = innerArcLength;
	}else{
		Returns.distance = outerArcLength;
		//since the right wheel is now the outer one, we need the reciprocal for the ratio.
		ratio = 1/ratio;
	}
	printf("IArcLength: %f, OArcLength: %f, Ratio: %f",innerArcLength,outerArcLength,ratio);
	Returns.ratio = ratio;
	return Returns;


}
float CalculateMovement::Spin(float angle){
	float radius = robotDimensions::distanceBetweenWheels/2;
	//make it negative because when we are rotating clockwise the right wheel moves backwards.
	float arcLength = -(angle/360)*6.28318*radius;
	// this is added b/c it was going twice as far as it should, we aren't sure why
	float driveDistance = arcLength;
	return driveDistance;
}
