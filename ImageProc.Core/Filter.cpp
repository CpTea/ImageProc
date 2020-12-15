#include "Filter.h"


cv::Mat ImageProc::Core::Filter::MedianBlur(const cv::Mat& image, cv::Size kernels)
{
    int cacheSize = kernels.width * kernels.height;
    uchar* cache = new uchar[cacheSize];
    
    cv::Mat output = image.clone();
    for (int h = 0; h < image.rows; h++)
    {
        const int HeightLowerBound = Max(0, h - kernels.height);
        const int HeightUpperBound = Min(image.rows - 1, h + kernels.height);
        const int HeightInterval = HeightUpperBound - HeightLowerBound + 1;
            
        for (int w = 0; w < image.cols; w++)
        {
            const int WidthLowerBound = Max(0, w - kernels.width);
            const int WidthUpperBound = Min(image.cols - 1, w + kernels.width);
            const int WidthInterval = WidthUpperBound - WidthLowerBound + 1;

            int idx = 0;
            for (int i = HeightLowerBound; i <= HeightUpperBound; ++i) 
            {
                for (int j = WidthLowerBound; j <= WidthUpperBound; ++j) 
                {
                    cache[idx++] = image.at<uchar>(i, j);
                }
            }

            SelectSort(cache, cacheSize);
            output.at<uchar>(h, w) = cache[cacheSize / 2];
        }
    }
    delete cache;
    return output;
}

void ImageProc::Core::Filter::SelectSort(uchar* arr, int length)
{
    const int middle = length / 2;
    for (int i = 0; i <= middle; i++) {
        uchar min = arr[i];
        int minInd = i;
        for (int j = i + 1; j < length; j++) {
            if (min > arr[j]) {
                minInd = j;
                min = arr[j];
            }
        }
        uchar tmp = arr[minInd];
        arr[minInd] = arr[i];
        arr[i] = tmp;
    }
}
