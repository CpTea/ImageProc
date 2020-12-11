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
#include <vector>
#include <opencv2/opencv.hpp>

namespace ImageProc
{
    /**
     * @brief ����ͼ�����������
     */
    namespace Core
    {
        /**
         *@brief ֱ��ͼ��������
         */
        namespace Histogram
        {
            class Histogram
            {
            public:
                /**
                 * @brief �����ͨ��ͼ��ֱ��ͼ
                 * @param image  Mat���͵�ͼ��
                 *        norm   ���ݽ���Ƿ���⻯
                 * @return ��ͨ��ֱ��ͼ
                 */
                static std::vector<std::vector<double>> CalcHistogram(const cv::Mat& image, bool norm = true);

                /**
                 * @brief ���㵥ͨ��ͼ��ֱ��ͼ
                 * @param image  Mat���͵�ͼ��
                 *        norm   ���ݽ���Ƿ���⻯
                 * @return ��ͨ��ֱ��ͼ
                 */
                static std::vector<double> CalcHistogramForChannel(const cv::Mat& image, bool norm = true);

                /**
                 * @brief ��ͨ��ֱ��ͼ���⻯
                 * @param image  Mat���͵�ͼ��
                 * @return ���⻯���ͼ��
                 */
                static cv::Mat EqualizeHistogram(const cv::Mat& image);

                /**
                 * @brief ��ͨ��ֱ��ͼ���⻯
                 * @param image  Mat���͵�ͼ��
                 * @return ���⻯���ͼ��
                 */
                static cv::Mat EqualizeHistogramForChannel(const cv::Mat& image);
            };
        }
    }
}