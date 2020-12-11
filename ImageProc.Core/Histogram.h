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
     * @brief 数字图像处理核心运算
     */
    namespace Core
    {
        /**
         *@brief 直方图运算运算
         */
        namespace Histogram
        {
            class Histogram
            {
            public:
                /**
                 * @brief 计算多通道图像直方图
                 * @param image  Mat类型的图像
                 *        norm   数据结果是否均衡化
                 * @return 多通道直方图
                 */
                static std::vector<std::vector<double>> CalcHistogram(const cv::Mat& image, bool norm = true);

                /**
                 * @brief 计算单通道图像直方图
                 * @param image  Mat类型的图像
                 *        norm   数据结果是否均衡化
                 * @return 单通道直方图
                 */
                static std::vector<double> CalcHistogramForChannel(const cv::Mat& image, bool norm = true);

                /**
                 * @brief 多通道直方图均衡化
                 * @param image  Mat类型的图像
                 * @return 均衡化后的图像
                 */
                static cv::Mat EqualizeHistogram(const cv::Mat& image);

                /**
                 * @brief 单通道直方图均衡化
                 * @param image  Mat类型的图像
                 * @return 均衡化后的图像
                 */
                static cv::Mat EqualizeHistogramForChannel(const cv::Mat& image);
            };
        }
    }
}