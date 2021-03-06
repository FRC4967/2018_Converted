#pragma once
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>

namespace grip {

/**
* WeaponizedTranspose class.
* 
* An OpenCV pipeline generated by GRIP.
*/
class WeaponizedTranspose {
	private:
		double dblCurrentX = -1;
		double dblCurrentY = -1;
		cv::Mat hsvThresholdOutput;
		cv::Mat cvErodeOutput;
		cv::Mat cvDilateOutput;
		std::vector<std::vector<cv::Point>> findContoursOutput;
		void hsvThreshold(cv::Mat &, double [], double [], double [], cv::Mat &);
		void cvErode(cv::Mat &, cv::Mat &, cv::Point &, double , int , cv::Scalar &, cv::Mat &);
		void cvDilate(cv::Mat &, cv::Mat &, cv::Point &, double , int , cv::Scalar &, cv::Mat &);
		void findContours(cv::Mat &, bool , std::vector<std::vector<cv::Point> > &);

	public:
		WeaponizedTranspose();
		cv::Point2f Process(cv::Mat& source0);
		cv::Mat* GetHsvThresholdOutput();
		cv::Mat* GetCvErodeOutput();
		cv::Mat* GetCvDilateOutput();
		std::vector<std::vector<cv::Point> >* GetFindContoursOutput();
};


} // end namespace grip


