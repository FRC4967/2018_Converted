/*
 * AdvancedTrapMove.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: monke
 */

#include <Services/Movements/AdvancedTrapMove.h>
using namespace std;
AdvancedTrapMove::AdvancedTrapMove() {
	m_accelTo = 0;
	m_accelTime= 0;
	m_accel = 0;
	dTime=0;
	dPos=0;
	velo=0;
	totTime=0;
	moveTime=0;
	movePos=0;
	m_decelTo = 0;
	m_decelTime = 0;
	m_decel = 0;

	m_distance = 0;
	stream.open(path);
	m_t1 = 0;
	m_t2 = 0;
	m_t3 = 0;
}

AdvancedTrapMove::~AdvancedTrapMove() {
	// TODO Auto-generated destructor stub
}

void AdvancedTrapMove::SetAll(float accelTo, float accelTime, float decelTo, float decelTime, float distance){
	m_accelTo = accelTo;
	m_accelTime = accelTime;
	m_decelTo = decelTo;
	m_decelTime = decelTime;
	m_distance = distance;
	CalcParams();
}


void AdvancedTrapMove::CalcParams(){
	float accel, decel, min_dist, accel_dist, decel_dist;
	accel = m_accelTo / m_accelTime;
	decel = (m_accelTo-m_decelTo) / m_decelTime;
	accel_dist = 0.5*accel * m_accelTime*m_accelTime;
	decel_dist = (0.5*decel * m_decelTime*m_decelTime)+(m_decelTo*m_decelTime);
	min_dist = accel_dist + decel_dist;
	cout << "Min Distance" << min_dist << "\n";
	if (min_dist > m_distance) // never gets up to speed.
		{
		m_t1 = pow(2.0* m_distance/(accel + (accel*accel)/decel),0.5);
		cout << "T1 is " << m_t1 << "\n";
		m_t2 = m_t1; // no constant velocity time
		m_t3 = accel/decel*m_t1 + m_t1;
		}
	else
		{
		m_t1 = m_accelTime;
		m_t2 = (m_distance - min_dist) / m_accelTo + m_t1; // time at end of constanct velocity
		m_t3 = m_decelTime + m_t2;
		cout << "Accel Distance: " << accel_dist << "\n";
		cout << "Decel Distance: " << decel_dist << "\n";
		cout << "T1 Time: " << m_t1 << "\n";
		cout << "T2 Time: " << m_t2 << "\n";
		cout << "T3 Time: " << m_t3 << "\n";
		};
	m_accel=accel;
	m_decel=decel;
//	SmartDashboard::PutNumber("T1", m_t1);
//	SmartDashboard::PutNumber("T2", m_t2);
//	SmartDashboard::PutNumber("T3", m_t3);
//	SmartDashboard::PutNumber("accel dist", accel_dist);
//	SmartDashboard::PutNumber("accel time", accel_time);
//	SmartDashboard::PutNumber("decel_dist", decel_dist);
}

float AdvancedTrapMove::Position(float time){

	float position = 0.0;
	if (time < m_t1) position = 0.5*m_accel*time*time;
	else if (time < m_t2) position = 0.5*m_accel*m_t1*m_t1 + (time - m_t1)*m_accelTo;
	else if (time <= m_t3) position = 0.5*m_accel*m_t1*m_t1 + (m_t2 - m_t1)*m_accelTo +(((2*m_accelTo)-(m_decel*(time-m_t2)))/2)*(time-m_t2);
	else position = m_distance;
	dTime = time-moveTime;
	dPos = position-movePos;
	velo=dPos/dTime;
	totTime+=dTime;
	stream << totTime << ", " << dTime << ", " <<position<< ", " <<dPos<< ", " << velo<<std::endl;
	moveTime = time;
	movePos = position;
	return position;
}
float AdvancedTrapMove::GetTotalTime() {
	return m_t3;
}
