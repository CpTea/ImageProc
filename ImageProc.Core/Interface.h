/**
 * Copyright 2020 CpTea
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
            void EqualizeHistogram(ImreadModes mode);

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