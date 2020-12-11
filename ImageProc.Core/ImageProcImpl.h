#pragma once
#include <opencv2/opencv.hpp>
#include <string>

namespace ImageProc
{
    namespace Core
    {
        class ImageProcImpl
        {
        private:
            cv::Mat m_image;

        public:
            ImageProcImpl();
            ImageProcImpl(const std::string& filename, int flag = cv::IMREAD_COLOR);
            ~ImageProcImpl();

            int GetImageWidth() const;
            int GetImageHeight() const;
            int GetImageChannels() const;
            cv::Mat GetImage(double rate = -1) const;

            void SetImage(const std::string& filename, int flag = cv::IMREAD_COLOR);

        protected:
            int IMAGE_MAX_SIZE = 1024;
            cv::Mat Resize(const cv::Mat& cvImage, double rate) const;

        };
    }
}