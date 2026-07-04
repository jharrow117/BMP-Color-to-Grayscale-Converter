#include "GrayscaleConversion.h"
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <Windows.h>

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

bool GrayscaleConverter::ConvertToGrayscale()
{
	if (bmp.empty())
		return false;

	SYSTEMTIME st;
	SYSTEMTIME st2;

	GetSystemTime(&st);

	Pixel* pPixel = bmp.ptr<Pixel>(0,0);

	const Pixel* pEndPixel = bmp.ptr<Pixel>(bmp.rows-1, bmp.cols-1);

	while (pPixel != pEndPixel)
	{
		ProcessPixelWithLuminosityMethod(*pPixel);
		pPixel++;
	}

	GetSystemTime(&st2);

	wstrConversionTime_ms = std::to_wstring(st2.wMilliseconds - st.wMilliseconds);

	cv::imwrite(strFullDestPath, bmp);

	return true;
}

/*
0.299 x R + 0.587 x G + 0.114 x B
*/
void GrayscaleConverter::ProcessPixelWithLuminosityMethod(Pixel &bgrPixel)
{
	uint grayscale = 0.299 * bgrPixel[2] + 0.587 * bgrPixel[1] + 0.114 * bgrPixel[0];
	bgrPixel[0] = grayscale;
	bgrPixel[1] = grayscale;
	bgrPixel[2] = grayscale;
}


