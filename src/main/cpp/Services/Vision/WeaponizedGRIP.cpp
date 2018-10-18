#include "WeaponizedGRIP.h"

namespace grip {

WeaponizedGRIP::WeaponizedGRIP() {

}
/**
* Runs an iteration of the pipeline and updates outputs.
*/
cv::Point2f WeaponizedGRIP::Process(cv::Mat& source0, bool lenghts){

	double dblHueMin = Preferences::GetInstance()->GetDouble("HueMin", 0);
	double dblHueMax = Preferences::GetInstance()->GetDouble("HueMax", 180);
	double dblSatMin = Preferences::GetInstance()->GetDouble("SatMin", 0);
	double dblSatMax = Preferences::GetInstance()->GetDouble("SatMax", 255);
	double dblValMin = Preferences::GetInstance()->GetDouble("ValMin", 0);
	double dblValMax = Preferences::GetInstance()->GetDouble("ValMax", 255);
	double dblCurrentX = -1;
	double dblCurrentY = -1;
	double dblLargestContourArea = 0;
	int intLargestContour = 0;

	//Step HSV_Threshold0:
	//input
	//initializes HSV parameters for filtering
	cv::Mat hsvThresholdInput = source0;
	double hsvThresholdHue[] = {dblHueMin, dblHueMax};
	double hsvThresholdSaturation[] = {dblSatMin, dblSatMax};
	double hsvThresholdValue[] = {dblValMin, dblValMax};
	//filters the frame using HSV
	hsvThreshold(hsvThresholdInput, hsvThresholdHue, hsvThresholdSaturation, hsvThresholdValue, this->hsvThresholdOutput);
	//Step Blur0:
	//input
	//Blurs the image to smooth out the thingamablob
	cv::Mat blurInput = hsvThresholdOutput;
	BlurType blurType = BlurType::BOX;
	double blurRadius = Preferences::GetInstance()->GetDouble("BlurRadius", 20);  // default Double
	blur(blurInput, blurType, blurRadius, this->blurOutput);
	//Step Find_Contours0:
	//input
	//finds contours in the frame
	cv::Mat findContoursInput = blurOutput;
	bool findContoursExternalOnly = false;  // default Boolean
	findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);

	printf("Contours: %u", findContoursOutput.size());
	//finds the contour with the largest area, as this will be the closest or the best targeted
	intLargestContour = 0;
	dblLargestContourArea = 0;
	for (int i = 0; i < findContoursOutput.size(); i++) {
		if (cv::contourArea(findContoursOutput[i], false) > dblLargestContourArea) {
			dblLargestContourArea = cv::contourArea(findContoursOutput[i], false);
			intLargestContour = i;
		}
	}

	//figures out current X and Y using the moments function
	dblCurrentX = -1;
	dblCurrentY = -1;

	double xLength;
	double yLength;

	if (lenghts) {
		std::vector<cv::Point> listPoints;

		cv::Rect rectangle = cv::boundingRect(findContoursOutput[intLargestContour]);

		xLength = rectangle.width;
		yLength = rectangle.height;

		return cv::Point2f(xLength, yLength);
	} else {
		CvMoments mtsContourMoments [findContoursOutput.size()];

		mtsContourMoments[intLargestContour] = cv::moments(findContoursOutput[intLargestContour], false);

		dblCurrentX = (mtsContourMoments[intLargestContour].m10/mtsContourMoments[intLargestContour].m00);
		dblCurrentY = (mtsContourMoments[intLargestContour].m01/mtsContourMoments[intLargestContour].m00);

		Preferences::GetInstance()->PutDouble("CurrentX", dblCurrentX);
		Preferences::GetInstance()->PutDouble("CurrentY", dblCurrentY);

		return cv::Point2f(dblCurrentX, dblCurrentY);
	}
}

/**
 * This method is a generated getter for the output of a HSV_Threshold.
 * @return Mat output from HSV_Threshold.
 */
cv::Mat* WeaponizedGRIP::GetHsvThresholdOutput(){
	return &(this->hsvThresholdOutput);
}
/**
 * This method is a generated getter for the output of a Blur.
 * @return Mat output from Blur.
 */
cv::Mat* WeaponizedGRIP::GetBlurOutput(){
	return &(this->blurOutput);
}
/**
 * This method is a generated getter for the output of a Find_Contours.
 * @return ContoursReport output from Find_Contours.
 */
std::vector<std::vector<cv::Point> >* WeaponizedGRIP::GetFindContoursOutput(){
	return &(this->findContoursOutput);
}
	/**
	 * Segment an image based on hue, saturation, and value ranges.
	 *
	 * @param input The image on which to perform the HSL threshold.
	 * @param hue The min and max hue.
	 * @param sat The min and max saturation.
	 * @param val The min and max value.
	 * @param output The image in which to store the output.
	 */
	void WeaponizedGRIP::hsvThreshold(cv::Mat &input, double hue[], double sat[], double val[], cv::Mat &out) {
		cv::cvtColor(input, out, cv::COLOR_BGR2HSV);
		cv::inRange(out,cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), out);
	}

	/**
	 * Softens an image using one of several filters.
	 *
	 * @param input The image on which to perform the blur.
	 * @param type The blurType to perform.
	 * @param doubleRadius The radius for the blur.
	 * @param output The image in which to store the output.
	 */
	void WeaponizedGRIP::blur(cv::Mat &input, BlurType &type, double doubleRadius, cv::Mat &output) {
		int radius = (int)(doubleRadius + 0.5);
		int kernelSize;
		switch(type) {
			case BOX:
				kernelSize = 2 * radius + 1;
				cv::blur(input,output,cv::Size(kernelSize, kernelSize));
				break;
			case GAUSSIAN:
				kernelSize = 6 * radius + 1;
				cv::GaussianBlur(input, output, cv::Size(kernelSize, kernelSize), radius);
				break;
			case MEDIAN:
				kernelSize = 2 * radius + 1;
				cv::medianBlur(input, output, kernelSize);
				break;
			case BILATERAL:
				cv::bilateralFilter(input, output, -1, radius, radius);
				break;																																															//This comment was snuck in by a mentor, now it can't be used to compete
        }
	}
	/**
	 * Finds contours in an image.
	 *
	 * @param input The image to find contours in.
	 * @param externalOnly if only external contours are to be found.
	 * @param contours vector of contours to put contours in.
	 */
	void WeaponizedGRIP::findContours(cv::Mat &input, bool externalOnly, std::vector<std::vector<cv::Point> > &contours) {
		std::vector<cv::Vec4i> hierarchy;
		contours.clear();
		int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
		int method = cv::CHAIN_APPROX_SIMPLE;
		cv::findContours(input, contours, hierarchy, mode, method);
	}



} // end grip namespace

