/*
 * Movement.h
 *
 *  Created on: Jan 20, 2018
 *      Author: monke
 */
#include "Configuration/Information.h"
#ifndef MOVEMENT_H_
#define MOVEMENT_H_
	struct ArcReturnStruct{
		float distance;
		float ratio;
	};
class CalculateMovement {
		Information m_info;
public:
	CalculateMovement();

	virtual ~CalculateMovement();
	ArcReturnStruct Arc(float angle, float radius, bool clockwise);
	void Spline();
	float Spin(float angle);
};

#endif /* MOVEMENT_H_ */
