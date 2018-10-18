/*
 * AdvancedTrapMove.h
 *
 *  Created on: Mar 4, 2018
 *      Author: monke
 */

#include <iostream>
#include <math.h>
#include <fstream>
#include <ctime>
#ifndef SRC_SERVICES_MOVEMENTS_ADVANCEDTRAPMOVE_H_
#define SRC_SERVICES_MOVEMENTS_ADVANCEDTRAPMOVE_H_
using namespace std;
class AdvancedTrapMove {
	float m_accelTo, m_decelTo, m_accel, m_decel, m_accelTime, m_decelTime, m_distance;
	float dTime,dPos,velo,totTime,moveTime,movePos;
	float m_t1, m_t2, m_t3;
public:
	void SetAll(float accelTo, float accelTime, float decelTo, float decelTime, float distance);
	void CalcParams();
	float GetTotalTime();
	float Position(float time);
	string path = "/home/lvuser/outputs/AdvanceTrapTest.csv";
	std::ofstream stream;

	AdvancedTrapMove();
	virtual ~AdvancedTrapMove();
};

#endif /* SRC_SERVICES_MOVEMENTS_ADVANCEDTRAPMOVE_H_ */
