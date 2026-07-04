#include "GrayscaleConversion.h"
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <Windows.h>

bool GrayscaleConverter::ReadFile(std::string fullSourcePath, std::string destPath)
{
	strFullSourcePath = fullSourcePath;
	strSourceFileName = strFullSourcePath.substr(strFullSourcePath.find_last_of("\\"));
	strDestPath = destPath;
	strDestFileName = strSourceFileName;
	strDestFileName.insert(strDestFileName.find("."), "_Grayscale");
	strFullDestPath = strDestPath;
	strFullDestPath.append(strDestFileName);
	bmp = cv::imread(strFullSourcePath);

	if (bmp.empty())
		return false;
	else
		return true;
}

bool GrayscaleConverter::ConvertToGrayscale(bool bUseOpenCV)
{
	// Sanity check
	if (bmp.empty())
		return false;

	SYSTEMTIME st;
	SYSTEMTIME st2;

	GetSystemTime(&st);

	if (bUseOpenCV)
	{
		cv::Mat newBmp;
		cv::cvtColor(bmp, newBmp, cv::COLOR_BGR2GRAY);
		cv::imwrite(strFullDestPath, newBmp);
	}

	else
	{
		Pixel* pPixel = bmp.ptr<Pixel>(0, 0);

		const Pixel* pEndPixel = bmp.ptr<Pixel>(bmp.rows - 1, bmp.cols - 1);

		while (pPixel != pEndPixel)
		{
			ProcessPixelWithLuminosityMethod(*pPixel);
			pPixel++;
		}

		cv::imwrite(strFullDestPath, bmp);
	}	

	GetSystemTime(&st2);

	wstrConversionTime_ms = std::to_wstring(st2.wMilliseconds - st.wMilliseconds);

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


