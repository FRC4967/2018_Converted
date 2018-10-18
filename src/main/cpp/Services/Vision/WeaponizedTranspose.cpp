#include "WeaponizedTranspose.h"

namespace grip {

WeaponizedTranspose::WeaponizedTranspose() {
}
/**
* Runs an iteration of the pipeline and updates outputs.
*/
cv::Point2f WeaponizedTranspose::Process(cv::Mat& source0){
	//Step HSV_Threshold0:
	//input
	cv::Mat hsvThresholdInput = source0;
	double hsvThresholdHue[] = {0.0, 22.424242424242422};
	double hsvThresholdSaturation[] = {59.62230215827338, 255.0};
	double hsvThresholdValue[] = {0.0, 199.1919191919192};
	hsvThreshold(hsvThresholdInput, hsvThresholdHue, hsvThresholdSaturation, hsvThresholdValue, this->hsvThresholdOutput);
	//Step CV_erode0:
	//input
//	cv::Mat cvErodeSrc = hsvThresholdOutput;
//	cv::Mat cvErodeKernel;
//	cv::Point cvErodeAnchor(-1, -1);
//	double cvErodeIterations = 3.0;  // default Double
//    int cvErodeBordertype = cv::BORDER_CONSTANT;
//	cv::Scalar cvErodeBordervalue(-1);
//	cvErode(cvErodeSrc, cvErodeKernel, cvErodeAnchor, cvErodeIterations, cvErodeBordertype, cvErodeBordervalue, this->cvErodeOutput);
//	//Step CV_dilate0:
//	//input
//	cv::Mat cvDilateSrc = cvErodeOutput;
//	cv::Mat cvDilateKernel;
//	cv::Point cvDilateAnchor(-1, -1);
//	double cvDilateIterations = 9.0;  // default Double
//    int cvDilateBordertype = cv::BORDER_CONSTANT;
//	cv::Scalar cvDilateBordervalue(-1);
//	cvDilate(cvDilateSrc, cvDilateKernel, cvDilateAnchor, cvDilateIterations, cvDilateBordertype, cvDilateBordervalue, this->cvDilateOutput);
	//Step Find_Contours0:
	//input
	cv::Mat findContoursInput = hsvThresholdOutput;
	bool findContoursExternalOnly = false;  // default Boolean
	findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);

	std::vector<std::vector<cv::Point>> approx;
	int vertexAmounts[findContoursOutput.size()];
	for (int i = 0; i < findContoursOutput.size(); i++) {
		cv::approxPolyDP(cv::Mat(findContoursOutput[i]), approx,
						 cv::arcLength(cv::Mat(findContoursOutput[i]), true) * .03, true);

		vertexAmounts[i] = approx.size();
	}

	int largestVertexCountContour = 0;
	for (int i = 0; i < findContoursOutput.size(); i++) {
		if (vertexAmounts[i] > vertexAmounts[largestVertexCountContour]) {
			largestVertexCountContour = i;
		}
	}

	dblCurrentX = -1;
	dblCurrentY = -1;
	CvMoments mtsContourMoments [findContoursOutput.size()];

	mtsContourMoments[largestVertexCountContour] = cv::moments(findContoursOutput[largestVertexCountContour], false);

	dblCurrentX = (mtsContourMoments[largestVertexCountContour].m10/mtsContourMoments[largestVertexCountContour].m00);
	dblCurrentY = (mtsContourMoments[largestVertexCountContour].m01/mtsContourMoments[largestVertexCountContour].m00);

//	Preferences::GetInstance()->PutDouble("CurrentX", dblCurrentX);
//	Preferences::GetInstance()->PutDouble("CurrentY", dblCurrentY);

	return cv::Point2f(dblCurrentX, dblCurrentY);
}

/**
 * This method is a generated getter for the output of a HSV_Threshold.
 * @return Mat output from HSV_Threshold.
 */
cv::Mat* WeaponizedTranspose::GetHsvThresholdOutput(){
	return &(this->hsvThresholdOutput);
}
/**
 * This method is a generated getter for the output of a CV_erode.
 * @return Mat output from CV_erode.
 */
cv::Mat* WeaponizedTranspose::GetCvErodeOutput(){
	return &(this->cvErodeOutput);
}
/**
 * This method is a generated getter for the output of a CV_dilate.
 * @return Mat output from CV_dilate.
 */
cv::Mat* WeaponizedTranspose::GetCvDilateOutput(){
	return &(this->cvDilateOutput);
}
/**
 * This method is a generated getter for the output of a Find_Contours.
 * @return ContoursReport output from Find_Contours.
 */
std::vector<std::vector<cv::Point> >* WeaponizedTranspose::GetFindContoursOutput(){
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
	void WeaponizedTranspose::hsvThreshold(cv::Mat &input, double hue[], double sat[], double val[], cv::Mat &out) {
		cv::cvtColor(input, out, cv::COLOR_BGR2HSV);
		cv::inRange(out,cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), out);
	}

	/**
	 * Expands area of lower value in an image.
	 * @param src the Image to erode.
	 * @param kernel the kernel for erosion.
	 * @param anchor the center of the kernel.
	 * @param iterations the number of times to perform the erosion.
	 * @param borderType pixel extrapolation method.
	 * @param borderValue value to be used for a constant border.
	 * @param dst Output Image.
	 */
	void WeaponizedTranspose::cvErode(cv::Mat &src, cv::Mat &kernel, cv::Point &anchor, double iterations, int borderType, cv::Scalar &borderValue, cv::Mat &dst) {
		cv::erode(src, dst, kernel, anchor, (int)iterations, borderType, borderValue);
	}

	/**
	 * Expands area of higher value in an image.
	 * @param src the Image to dilate.
	 * @param kernel the kernel for dilation.
	 * @param anchor the center of the kernel.
	 * @param iterations the number of times to perform the dilation.
	 * @param borderType pixel extrapolation method.
	 * @param borderValue value to be used for a constant border.
	 * @param dst Output Image.
	 */
	void WeaponizedTranspose::cvDilate(cv::Mat &src, cv::Mat &kernel, cv::Point &anchor, double iterations, int borderType, cv::Scalar &borderValue, cv::Mat &dst) {
		cv::dilate(src, dst, kernel, anchor, (int)iterations, borderType, borderValue);
	}

	/**
	 * Finds contours in an image.
	 *
	 * @param input The image to find contours in.
	 * @param externalOnly if only external contours are to be found.
	 * @param contours vector of contours to put contours in.
	 */
	void WeaponizedTranspose::findContours(cv::Mat &input, bool externalOnly, std::vector<std::vector<cv::Point> > &contours) {
		std::vector<cv::Vec4i> hierarchy;
		contours.clear();
		int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
		int method = cv::CHAIN_APPROX_SIMPLE;
		cv::findContours(input, contours, hierarchy, mode, method);
	}



} // end grip namespace

