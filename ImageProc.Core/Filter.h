#pragma once
#include <opencv2/opencv.hpp>

#define Max(x,y) ((x)>(y)?(x):(y))
#define Min(x,y) ((x)<(y)?(x):(y))

namespace ImageProc
{
    namespace Core
    {
        namespace  Filter
        {
            cv::Mat MedianBlur(const cv::Mat& image, cv::Size kernels);

            void SelectSort(uchar* arr, int length);
        };
    }
}