/*
 * Observations.h
 *
 *  Created on: Jan 22, 2018
 *      Author: harsh
 */

#include <WPILib.h>
#include <CameraServer.h>
#include "Services/Movements/ExecuteMovements.h"
#include "Services/Vision/WeaponizedGRIP.h"
#include  "Services/Vision/WeaponizedTranspose.h"
#include "Configuration/OperatorInterface.h"
#include "Objects/TankDrive.h"
#include "Objects/Mechanisms/CubeManipulator/CubeManipulator.h"

#ifndef SRC_VISION_OBSERVATIONS_H_
#define SRC_VISION_OBSERVATIONS_H_

class CubeVision {
private:
	typedef struct visionReportedError {
		double angleError;
		double distance;
	};
	double dblCenterX;
	double dblCurrentX = 0;
	double dblCurrentY = 0;
	double dblDeltaX = 0;
	double dblXResolution = 240;
	double dblYResolution = 160;
	double dblViewingAngle = 74;
	double dblAngleError = 0;
	double dblHeightOfCamera = 27.5;
	double dblAngleOfCamera = 16.35;
	double dblVisionGroundAngle;
	bool m_blnUsingForProcessing = false;
	cs::UsbCamera m_CameraFront;
	cs::UsbCamera m_CameraBack;
	cs::CvSource m_OutputStream;
	cs::CvSource m_ProcessedOutputStream;
	cs::CvSource m_BinaryOutputStream;
	cv::Mat m_UnprocessedImage;
	cs::CvSink m_CvSinkFront;
	cs::CvSink m_CvSinkBack;
	grip::WeaponizedGRIP visionRefrence;
	grip::WeaponizedTranspose m_EchangeVision;
	ExecuteMovements &m_execMove;
	TankDrive &m_tank;
	CubeManipulator &m_cubeManip;
	OperatorInterface m_oi;
	TrapezoidalMove m_trapMove;
	Timer m_timer;
//	std::thread thrLiveFeed;
	visionReportedError GetError();
	void EnableLiveFeed();

public:
	enum VisionCorrectionModes {
		TURN,
		FORWARD,
		CALCULATE_ERROR,
		CENTERED,
		INIT
	};
	int m_intState;
	void TuneVision();
	std::string FindOrientation();
	bool AcquireCube();
	void TeleopPeriodic();
	CubeVision(TankDrive&, ExecuteMovements&, CubeManipulator&);
	virtual ~CubeVision();
	void GrabFrameIm();

	bool Exchange();
};

#endif /* SRC_VISION_OBSERVATIONS_H_ */
