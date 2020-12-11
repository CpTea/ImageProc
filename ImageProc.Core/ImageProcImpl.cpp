#include "ImageProcImpl.h"


ImageProc::Core::ImageProcImpl::ImageProcImpl()
{

}

ImageProc::Core::ImageProcImpl::ImageProcImpl(const std::string& filename, int flag)
{
    SetImage(filename, flag);
}

ImageProc::Core::ImageProcImpl::~ImageProcImpl()
{

}

cv::Mat ImageProc::Core::ImageProcImpl::GetImage(double rate) const
{
    if (rate == -1)
    {
        int MaxLength = m_image.cols > m_image.rows ? m_image.cols : m_image.rows;
        rate = MaxLength < IMAGE_MAX_SIZE ? 1 : (double)IMAGE_MAX_SIZE / MaxLength;
    }
    return Resize(m_image, rate);
    //return m_image;
}

void ImageProc::Core::ImageProcImpl::SetImage(const std::string& filename, int flag)
{
    m_image = cv::imread(filename, flag);
    //cv::imshow("test", m_image);
    //cv::waitKey(0);
}

int ImageProc::Core::ImageProcImpl::GetImageWidth() const
{
    return m_image.cols;
}

int ImageProc::Core::ImageProcImpl::GetImageHeight() const
{
    return m_image.rows;
}

int ImageProc::Core::ImageProcImpl::GetImageChannels() const
{
    return m_image.channels();
}

cv::Mat ImageProc::Core::ImageProcImpl::Resize(const cv::Mat& cvImage, double rate) const
{
    cv::Mat image;
    cv::resize(m_image, image, { 0,0 }, rate, rate);
    return image;
}
