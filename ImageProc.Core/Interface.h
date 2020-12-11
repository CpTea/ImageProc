#pragma once
#include <opencv2/opencv.hpp>
#include <msclr/marshal_cppstd.h>
#include "Histogram.h"

#using <System.Drawing.dll>
#using <PresentationCore.dll>
#using <WindowsBase.dll>

using namespace System::Collections::Generic;

namespace ImageProc
{
    namespace Core
    {
        public enum class ImreadModes
        {
            Color = cv::IMREAD_COLOR,
            Grayscale = cv::IMREAD_GRAYSCALE
        };

        public ref class Interface
        {
        public:
            Interface();
            Interface(System::String^ filename, ImreadModes mode);
            ~Interface();
            
            void SetImage(System::String^ filename, ImreadModes mode);
            void SaveCurrentImage(System::String^ filename, ImreadModes mode);

            System::Windows::Media::ImageSource^ GetImageSource(ImreadModes mode);
            cv::Mat GetCvImage(const cv::Mat& image, ImreadModes mode);

            List<array<double>^>^ CalcHistogram(ImreadModes mode, bool norm);

            int GetImageWidth();
            int GetImageHeight();
            int GetImageChannels(ImreadModes mode);

        protected:
            cv::Mat AutoResize(const cv::Mat& image);
            cv::Mat CvtToGrayscale(const cv::Mat& image);
            System::Windows::Media::ImageSource^ CvtMatToImageSource(const cv::Mat& image);

        private:
            cv::Mat* m_pImage;
        };
    }
}