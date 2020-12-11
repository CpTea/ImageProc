#include "Histogram.h"

// 灰度等级
constexpr auto GRAYSCALE_LEVEL = 256;

std::vector<double> ImageProc::Core::Histogram::Histogram::CalcHistogramForChannel(const cv::Mat& image, bool norm)
{
    double hist[GRAYSCALE_LEVEL] = { 0 };
    if (image.empty()) return std::vector<double>();
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            ++hist[image.at<uchar>(i, j)];
    // 归一化
    if (norm)
    {
        int nPixels = image.rows * image.cols;
        for (int i = 0; i < GRAYSCALE_LEVEL; i++) hist[i] /= nPixels;
    }
    return std::vector<double>(hist, hist + GRAYSCALE_LEVEL);
}

std::vector<std::vector<double>> ImageProc::Core::Histogram::Histogram::CalcHistogram(const cv::Mat& image, bool norm)
{
    std::vector<std::vector<double>> hists;
    if (image.empty()) return hists;
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    for (int i = 0; i < channels.size(); i++) hists.push_back(CalcHistogramForChannel(channels[i], norm));
    return hists;
}

cv::Mat ImageProc::Core::Histogram::Histogram::EqualizeHistogram(const cv::Mat& image)
{
    std::vector<cv::Mat> channels;
    cv::split(image, channels); // 分割多通道图像
    for (int i = 0; i < channels.size(); i++)
        channels[i] = EqualizeHistogramForChannel(channels[i]);
    cv::Mat outputImage;
    cv::merge(channels, outputImage); // 合并多通道图像
    return outputImage;
}

cv::Mat ImageProc::Core::Histogram::Histogram::EqualizeHistogramForChannel(const cv::Mat& image)
{
    // 计算直方图
    int grayscaleValue[GRAYSCALE_LEVEL] = { 0 };
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            ++grayscaleValue[image.at<uchar>(i, j)];

    // 均衡归一化后的直方图
    double nPixels = image.rows * image.cols;
    double distribution[GRAYSCALE_LEVEL] = { 0 };
    uchar  equal[GRAYSCALE_LEVEL] = { 0 };
    distribution[0] = grayscaleValue[0] / nPixels;
    equal[0] = 255 * distribution[0] + 0.5;
    for (int i = 1; i < GRAYSCALE_LEVEL; i++)
    {
        distribution[i] = distribution[i-1] + grayscaleValue[i] / nPixels;
        equal[i] = 255 * distribution[i] + 0.5;
    }

    cv::Mat outputImage = image.clone();
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            outputImage.at<uchar>(i, j) = equal[outputImage.at<uchar>(i, j)];
    return outputImage;
}
