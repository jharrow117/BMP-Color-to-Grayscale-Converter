#pragma once

#include <opencv2/imgcodecs.hpp>

typedef cv::Vec3b Pixel;
/* 
Luminosity method without gamma correction
0.2126 × R + 0.7152 × G + 0.0722 × B
*/
class GrayscaleConverter
{
public:
	GrayscaleConverter();
	~GrayscaleConverter() = default;

	bool ReadFile(std::string fullSourcePath, std::string destPath);
	bool Convert();
	std::string GetFullDestPath() const { return strFullDestPath; }

protected:
	std::string strFullSourcePath;
	std::string strSourceFileName;
	std::string strDestPath;
	std::string strDestFileName;
	std::string strFullDestPath;
	cv::Mat bmp;

	void ProcessPixelWithLuminosityMethod(cv::Vec3b &bgrPixel);
};