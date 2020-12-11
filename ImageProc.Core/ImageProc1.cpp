#include <opencv2/opencv.hpp>
#include <msclr/marshal_cppstd.h>
#include "ImageProcImpl.h"

#using <System.Drawing.dll>
#using <PresentationCore.dll>
#using <WindowsBase.dll>

using namespace System;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Windows::Media::Imaging;
using namespace System::Runtime::InteropServices;

namespace ImageProc
{
    namespace Core
    {
        public enum class ImreadModes
        {
            Grayscale = cv::IMREAD_GRAYSCALE,
            Color = cv::IMREAD_COLOR,
        };

        public ref  class ImageProc
        {
            ImageProcImpl* imageproc;

        public:
            ImageProc() : imageproc(new ImageProcImpl()) {}
            ImageProc(String^ filename, ImreadModes mode) : imageproc(
                new ImageProcImpl(msclr::interop::marshal_as<std::string>(filename), Convert::ToInt32(mode))) {}
            ~ImageProc() { delete imageproc; }

            void SetImage(String^ filename, ImreadModes mode)
            {
                return imageproc->SetImage(msclr::interop::marshal_as<std::string>(filename), Convert::ToInt32(mode));
            }

            BitmapImage^ GetBitmapImage()
            {
                return CvtBitmapToBitmapImage(CvtMatToBitmap(&imageproc->GetImage()));
            }

            /*Bitmap^ GetBitmap()
            {
                return CvtMatToBitmap(&imageproc->GetImage());
            }*/

            /*array<byte>^ GetBytes()
            {
                try 
                {
                    cv::Mat image = imageproc->GetImage();
                    int len = image.cols * image.rows * image.channels();
                    if (len == 0) return nullptr;
                    array<byte>^ bytes = gcnew array<byte>(len);
                    byte* ptr = image.data;
                    Marshal::Copy((IntPtr)ptr, bytes, 0, len);
                    return bytes;
                }
                catch(std::exception e)
                {
                    return gcnew array<byte>(0);
                }
            }*/

        protected:
            Bitmap^ CvtMatToBitmap(cv::Mat* cvImage)
            {
                System::Drawing::Bitmap^ bitmap = gcnew Bitmap(
                    cvImage->cols, cvImage->rows, PixelFormat::Format24bppRgb);
                BitmapData^ data = bitmap->LockBits(
                    *(gcnew System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height)),
                    ImageLockMode::ReadWrite,
                    bitmap->PixelFormat);

                IntPtr ptr = data->Scan0;
                int bytes = Math::Abs(data->Stride) * bitmap->Height;
                array<byte>^ rgbValues = gcnew array<byte>(bytes);
                if (cvImage->channels() == 3)
                {
                    for (int j = 0; j < bitmap->Height; j++)
                    {
                        for (int i = 0; i < bitmap->Width; i++)
                        {
                            rgbValues[j * data->Stride + i * 3] = cvImage->data[j * cvImage->cols * 3 + i * 3];
                            rgbValues[j * data->Stride + i * 3 + 1] = cvImage->data[j * cvImage->cols * 3 + i * 3 + 1];
                            rgbValues[j * data->Stride + i * 3 + 2] = cvImage->data[j * cvImage->cols * 3 + i * 3 + 2];
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < bitmap->Height; j++)
                    {
                        for (int i = 0; i < bitmap->Width; i++)
                        {
                            rgbValues[j * data->Stride + i * 3 + 2]
                                = rgbValues[j * data->Stride + i * 3 + 1]
                                = rgbValues[j * data->Stride + i * 3]
                                = cvImage->data[j * cvImage->cols * 1 + i * 1];
                        }
                    }
                }

                Marshal::Copy(rgbValues, 0, ptr, bytes);
                cvImage->release();
                bitmap->UnlockBits(data);
                return bitmap;
            }

            System::Windows::Media::ImageSource^ GetLatestImage()
            {
                cv::Mat frame;
                /*if (!videoSrc->read(frame))
                    return nullptr;*/

                int totSize = frame.total() * frame.elemSize();
                auto managedArray = gcnew array<System::Byte>(totSize);
                System::Runtime::InteropServices::Marshal::Copy(System::IntPtr((void*)frame.data),
                    managedArray, 0, totSize); //assume isContinuous is always true at this point (?)

                return System::Windows::Media::Imaging::BitmapImage::Create(
                    frame.cols, frame.rows, 96, 96, System::Windows::Media::PixelFormats::Bgr24, //Rgb24,
                    System::Windows::Media::Imaging::BitmapPalettes::WebPalette, managedArray, frame.step);
            }

            //Bitmap^ CvtMatToBitmap(cv::Mat srcImg)
            //{
            //    int stride = srcImg.size().width * srcImg.channels();//calc the srtide
            //    int hDataCount = srcImg.size().height;

            //    System::Drawing::Bitmap^ retImg;

            //    System::IntPtr ptr(srcImg.data);

            //    //create a pointer with Stride
            //    if (stride % 4 != 0) {//is not stride a multiple of 4?
            //                          //make it a multiple of 4 by fiiling an offset to the end of each row

            //        uchar* dataPro = new uchar[((srcImg.size().width * srcImg.channels() + 3) & -4) * hDataCount];//to hold processed data

            //        uchar* data = srcImg.ptr();

            //        //current position on the data array
            //        int curPosition = 0;
            //        //current offset
            //        int curOffset = 0;

            //        int offsetCounter = 0;

            //        //itterate through all the bytes on the structure
            //        for (int r = 0; r < hDataCount; r++) {
            //            //fill the data
            //            for (int c = 0; c < stride; c++) {
            //                curPosition = (r * stride) + c;

            //                dataPro[curPosition + curOffset] = data[curPosition];
            //            }

            //            //reset offset counter
            //            offsetCounter = stride;

            //            //fill the offset
            //            do {
            //                curOffset += 1;
            //                dataPro[curPosition + curOffset] = 0;

            //                offsetCounter += 1;
            //            } while (offsetCounter % 4 != 0);
            //        }

            //        ptr = (System::IntPtr)dataPro;//set the data pointer to new/modified data array

            //                                      //calc the stride to nearest number which is a multiply of 4
            //        stride = (srcImg.size().width * srcImg.channels() + 3) & -4;

            //        //retImg = gcnew System::Drawing::Bitmap(srcImg.size().width, srcImg.size().height,
            //        //	stride,
            //        //	System::Drawing::Imaging::PixelFormat::Format24bppRgb,
            //        //	ptr);
            //    }
            //    if (srcImg.channels() == 4)
            //    {
            //        retImg = gcnew System::Drawing::Bitmap(srcImg.size().width, srcImg.size().height,
            //            stride, System::Drawing::Imaging::PixelFormat::Format32bppArgb, ptr);

            //    }
            //    else if (srcImg.channels() == 3)
            //    {
            //        retImg = gcnew System::Drawing::Bitmap(srcImg.size().width, srcImg.size().height,
            //            stride, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);

            //    }
            //    else
            //    {
            //        retImg = gcnew System::Drawing::Bitmap(srcImg.size().width, srcImg.size().height,
            //            stride, System::Drawing::Imaging::PixelFormat::Format8bppIndexed, ptr);
            //    }

            //    array<System::Byte>^ imageData;
            //    System::Drawing::Bitmap^ output;

            //    // Create the byte array.
            //    {
            //        System::IO::MemoryStream^ ms = gcnew System::IO::MemoryStream();
            //        //		System::Drawing::Imaging::ImageFormat^ png = System::Drawing::Imaging::ImageFormat::Png;
            //        retImg->Save(ms, System::Drawing::Imaging::ImageFormat::Bmp);

            //        imageData = ms->ToArray();
            //        delete ms;
            //    }

            //    // Convert back to bitmap
            //    {
            //        System::IO::MemoryStream^ ms = gcnew System::IO::MemoryStream(imageData);
            //        output = (System::Drawing::Bitmap^)System::Drawing::Bitmap::FromStream(ms);
            //        delete ms;
            //    }

            //    return output;
            //}

            BitmapImage^ CvtBitmapToBitmapImage(Bitmap^ bitmap)
            {
                MemoryStream^ stream = gcnew MemoryStream();
                bitmap->Save(stream, ImageFormat::Png);
                BitmapImage^ bitmapImage = gcnew BitmapImage();
                bitmapImage->BeginInit();
                bitmapImage->CacheOption = BitmapCacheOption::OnLoad;
                bitmapImage->StreamSource = stream;
                bitmapImage->EndInit();
                bitmapImage->Freeze();
                return bitmapImage;
            }
        };
    }
}