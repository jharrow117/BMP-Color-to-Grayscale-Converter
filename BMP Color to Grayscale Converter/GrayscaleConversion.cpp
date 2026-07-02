#include "GrayscaleConversion.h"
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

GrayscaleConverter::GrayscaleConverter()
{

}

bool GrayscaleConverter::ReadFile(std::string fullSourcePath, std::string destPath)
{
	strFullSourcePath = fullSourcePath;
	strSourceFileName = strFullSourcePath.substr(strFullSourcePath.find_last_of("\\"));
	strDestPath = destPath;
	strDestFileName = strSourceFileName;
	strDestFileName.insert(strDestFileName.find("."), "_Grayscale");
	strFullDestPath = strDestPath;
	strFullDestPath.append(strDestFileName);
	bmp = cv::imread(strFullSourcePath, cv::COLOR_RGB2BGR);
	return false;
}

bool GrayscaleConverter::Convert()
{
	if (bmp.empty())
		return false;

	//for (int r = 0; r < bmp.rows; r++)
	//{
	//	for (int c = 0; c < bmp.cols; c++)
	//	{
	//		ProcessPixelWithLuminosityMethod(bmp.at<Pixel>(r, c));
	//	}
	//}

	//cv::imwrite(strFullDestPath, bmp);

	cv::Mat newBmp;
	cv::cvtColor(bmp, newBmp, cv::COLOR_BGR2GRAY);
	cv::imwrite(strFullDestPath, newBmp);

	return true;
}

/*
Luminosity method without gamma correction
0.2126 × R + 0.7152 × G + 0.0722 × B

0.299 x R + 0.587 x G + 0.114 x B
*/
void GrayscaleConverter::ProcessPixelWithLuminosityMethod(cv::Vec3b &bgrPixel)
{
    //bgrPixel[0] *= 0.0722;
    //bgrPixel[1] *= 0.7152;
    //bgrPixel[2] *= 0.2126;

	bgrPixel[0] *= 0.114;
	bgrPixel[1] *= 0.587;
	bgrPixel[2] *= 0.299;
}


