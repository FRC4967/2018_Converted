///*
// * ExhangeVision.cpp
// *
// *  Created on: Feb 20, 2018
// *      Author: harsh
// */
//
//#include <Services/Vision/ExchangeVision.h>
//
//ExchangeVision::ExchangeVision(ExecuteMovements &m_executeMovements, CubeVision &m_cubeVision)
//								: m_execMove(m_executeMovements), m_cubeVis(m_cubeVision) {
//	// TODO Auto-generated constructor stub
//
//}
//
//ExchangeVision::~ExchangeVision() {
//	// TODO Auto-generated destructor stub
//}
//
//bool ExchangeVision::CenterToExchange(){
//	m_UnprocessedImage = m_cubeVis.GrabFrame();
//	cv::Point2f currentLocation = visionRefrence.Process(m_UnprocessedImage);
//
//	return true;
//}
//
//CubeVision::visionReportedError ExchangeVision::InterpretData() {
//	m_UnprocessedImage = m_cubeVis.GrabFrame();
//	cv::Point2f currentLocation = visionRefrence.Process(m_UnprocessedImage);
//
//	return m_reportedError;
//}
