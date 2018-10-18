/*
 * Observations.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: harsh
 */

#include <Services/Vision/CubeVision.h>

CubeVision::CubeVision(TankDrive &m_tankDrive, ExecuteMovements &m_executeMovements, CubeManipulator &m_cubeManipulator)
						: m_tank(m_tankDrive), m_execMove(m_executeMovements), m_cubeManip(m_cubeManipulator) {

	// Field limits us to 7mbs for entire robot


	//Start the Camera Server and bind it to a USB camera
	m_CameraFront = CameraServer::GetInstance()->StartAutomaticCapture(0);
	//Setting the Resolution of the Camera
	m_CameraFront.SetResolution(dblXResolution, dblYResolution);

//	m_CameraBack = CameraServer::GetInstance()->StartAutomaticCapture(1);
//
//	m_CameraBack.SetResolution(dblXResolution, dblYResolution);

	//Set up the Frame Grabber Thing
	m_CvSinkFront = CameraServer::GetInstance()->GetVideo();

//	m_CvSinkBack = CameraServer::GetInstance()->GetVideo();

	//Publishing the Image to the Driver Station
	m_ProcessedOutputStream = CameraServer::GetInstance()->PutVideo("ProcessedImage", dblXResolution, dblYResolution);

	m_BinaryOutputStream = CameraServer::GetInstance()->PutVideo("Binary Image", dblXResolution, dblYResolution);

	printf("Vision Init Complete");

	m_intState = INIT;
//	while (true) {
//			m_CvSink.GrabFrame(m_UnprocessedImage);
//			m_OutputStream.PutFrame(m_UnprocessedImage);
//		}

//	thrLiveFeed = std::thread(&Observations::EnableLiveFeed, this);
//	thrLiveFeed.detach();
}

CubeVision::~CubeVision() {
	// TODO Auto-generated destructor stub
//	thrLiveFeed.join();
}

CubeVision::visionReportedError CubeVision::GetError() {

	visionReportedError error;

	GrabFrameIm();

	cv::Point2f currentLocation = visionRefrence.Process(m_UnprocessedImage, false);

	m_BinaryOutputStream.PutFrame(*visionRefrence.GetBlurOutput());

	double dblOffset = 40;
	cv::rectangle(m_UnprocessedImage, cv::Point(currentLocation.x-dblOffset, currentLocation.y-dblOffset),
									  cv::Point(currentLocation.x+dblOffset, currentLocation.y+dblOffset), cv::Scalar(34,98,201.2), 10);

	m_ProcessedOutputStream.PutFrame(m_UnprocessedImage);

	dblCenterX = Preferences::GetInstance()->GetDouble("CenterX", 0);
	dblCurrentX = currentLocation.x;
	dblCurrentY = currentLocation.y;

	printf("Current X: %f, Current Y: %f \n", dblCurrentX, dblCurrentY);

	dblDeltaX = (dblCurrentX - dblCenterX);

	dblAngleError = (dblDeltaX/dblXResolution) * (dblViewingAngle);

	double dblDistance; //= ((48.0/262.0)*(dblYResolution - dblCurrentY));
//	printf("Distance: %f\n", dblDistance);


	double dblAngleDelta = (dblCurrentY - (dblYResolution/2))*(dblViewingAngle/dblXResolution);
	dblVisionGroundAngle = dblAngleOfCamera + dblAngleDelta;
	dblVisionGroundAngle = dblVisionGroundAngle * (3.14159/180);
	dblDistance = (dblHeightOfCamera/tan(dblVisionGroundAngle));

	printf("Distance To Cube: %f\n", dblDistance);
	printf("Angle Error: %f\n", dblAngleError);

	error.angleError = dblAngleError;
	error.distance = dblDistance;

	return error;
}

//void Observations::EnableLiveFeed() {
//	printf("Thread Initiated");
//}

bool CubeVision::AcquireCube() {
	//TODO put back in
	static visionReportedError error;
	static double dblAcceptedAngleError = 7;
	static double dblDistanceToMove;
	printf("Vision Enabled\n");

	if (m_cubeManip.IntakeCube(0)) {
		m_intState = CENTERED;
	}

	switch (m_intState) {
		case INIT:
			error.angleError = 0;
			error.distance = 0;
			dblDistanceToMove = 0;
			dblAcceptedAngleError = 5; //get from dash board
			printf("State is INIT/n");

			m_intState = CALCULATE_ERROR;
			break;
		case CALCULATE_ERROR:
			error = GetError();
			printf("State is Calculate_Error/n");
			if (fabs(error.angleError) < dblAcceptedAngleError) {
				m_intState = FORWARD;
				m_timer.Reset();
				m_timer.Start();
			} else {
				m_intState = TURN;
			}
			break;
		case TURN:
			if (m_execMove.TurnXDegrees(error.angleError)) {
				m_intState = CALCULATE_ERROR;
				break;
			}
			printf("State is Turn/n");
			break;
		case CENTERED:
			printf("State is CENTERED/n");
			//TODO Turn on LED
			return true;
			break;
		case FORWARD:
			dblDistanceToMove = (error.distance *.95);

			if (m_execMove.MoveForwardXInch(dblDistanceToMove)) {
				m_intState = CALCULATE_ERROR;
				break;
			}
			printf("State is Forward/n");
			break;

		default:
			break;
	}
	return false;
}


void CubeVision::TeleopPeriodic() {
	bool blnVisionButton = m_oi.joystick_driverLeft->GetRawButton(LeftButtonMap::BUTTON_VISION);
	bool blnVisionTuneButton = m_oi.joystick_driverLeft->GetRawButton(LeftButtonMap::BUTTON_VISION_TUNE);
	if (blnVisionButton) {
		AcquireCube();
	}
	else{
		m_tank.SetMode(ControlMode::PercentOutput);
		m_intState = INIT;
	}
	if (blnVisionTuneButton) {
		TuneVision();
		//FindOrientation();
	}
}

void CubeVision::TuneVision() {
//	m_CvSinkFront.GrabFrame(m_UnprocessedImage);
//
//	visionRefrence.Process(m_UnprocessedImage);
	//GetError();
	Exchange();
}

void CubeVision::GrabFrameIm() {
	m_CvSinkFront.GrabFrame(m_UnprocessedImage);
}

std::string CubeVision::FindOrientation() {
	GrabFrameIm();
	cv::Point2f lenghts = visionRefrence.Process(m_UnprocessedImage, true);

	double ratio = (lenghts.x/lenghts.y);

	if ((1 - ratio) < .04) {
		//square
		printf("Square\n");
	} else if (ratio > 1) {
		//normal
		printf("Normal\n");
	} else if (ratio < 1) {
		//upright
		printf("Upright\n");
	}


}

bool CubeVision::Exchange() {
//	double dblOffset = 40;
//
//	cv::Mat unprocessed;
//	unprocessed = GrabFrameIm();
//
//	cv::Point2f currentLocation = m_EchangeVision.Process(unprocessed);
//
//	m_ProcessedOutputStream.PutFrame(unprocessed);
//
//	m_BinaryOutputStream.PutFrame(*m_EchangeVision.GetHsvThresholdOutput());
//
//
//	cv::rectangle(unprocessed, cv::Point(currentLocation.x-dblOffset, currentLocation.y-dblOffset),
//									  cv::Point(currentLocation.x+dblOffset, currentLocation.y+dblOffset), cv::Scalar(34,98,201.2), 10);

	return false;
}

