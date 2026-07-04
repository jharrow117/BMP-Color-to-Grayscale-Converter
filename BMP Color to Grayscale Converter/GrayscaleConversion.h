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
	bool ConvertToGrayscale();
	std::string GetFullDestPath() const { return strFullDestPath; }
	std::wstring GetTimeElapsedWString_ms() const { return wstrConversionTime_ms; }

protected:
	std::string strFullSourcePath;
	std::string strSourceFileName;
	std::string strDestPath;
	std::string strDestFileName;
	std::string strFullDestPath;
	cv::Mat bmp;

	std::wstring wstrConversionTime_ms;

	void ProcessPixelWithLuminosityMethod(Pixel &bgrPixel);
};