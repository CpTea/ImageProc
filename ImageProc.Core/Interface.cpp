#include "Interface.h"

constexpr auto IMAGE_MAX_SIZE = 1024;

ImageProc::Core::Interface::Interface()
{
    m_pImage = nullptr;
}

ImageProc::Core::Interface::Interface(System::String^ filename, ImreadModes mode)
{
    SetImage(filename, mode);
}

ImageProc::Core::Interface::~Interface()
{
    delete m_pImage;
}

void ImageProc::Core::Interface::SetImage(System::String^ filename, ImreadModes mode)
{
    if (System::String::IsNullOrEmpty(filename)) return;
    delete m_pImage;
    m_pImage = new cv::Mat(cv::imread(msclr::interop::marshal_as<std::string>(filename),
        System::Convert::ToInt32(mode)));
}

void ImageProc::Core::Interface::SaveCurrentImage(System::String^ filename, ImreadModes mode)
{
    if (!m_pImage) return;
    cv::imwrite(msclr::interop::marshal_as<std::string>(filename), GetCvImage(*m_pImage, mode));
}

cv::Mat ImageProc::Core::Interface::AutoResize(const cv::Mat& image)
{
    cv::Mat output;
    if (image.empty()) return output;
    int MaxLength = image.cols > image.rows ? image.cols : image.rows;
    double rate = MaxLength < IMAGE_MAX_SIZE ? 1 : (double)IMAGE_MAX_SIZE / MaxLength;
    cv::resize(image, output, { 0,0 }, rate, rate);
    return output;
}

cv::Mat ImageProc::Core::Interface::CvtToGrayscale(const cv::Mat& image)
{
    cv::Mat output;

    switch (image.channels())
    {   
    case 1:
        output = image; break;
    case 3:
        cv::cvtColor(image, output, cv::COLOR_BGR2GRAY); break;
    case 4:
        cv::cvtColor(image, output, cv::COLOR_BGRA2GRAY); break;
    default:
        break;
    }
        
    return output;
}

cv::Mat ImageProc::Core::Interface::GetCvImage(const cv::Mat& image, ImreadModes mode)
{
    return mode == ImreadModes::Grayscale ? CvtToGrayscale(image) : image;
}

System::Windows::Media::ImageSource^ ImageProc::Core::Interface::CvtMatToImageSource(const cv::Mat& image)
{
    cv::Mat img = AutoResize(image);

    int totSize = img.total() * img.elemSize();
    auto managedArray = gcnew array<System::Byte>(totSize);
    System::Runtime::InteropServices::Marshal::Copy(System::IntPtr((void*)img.data),
        managedArray, 0, totSize);

    auto graphics = System::Drawing::Graphics::FromHwnd(System::IntPtr::Zero);
    auto pixelFormat = img.channels() == 1 ? System::Windows::Media::PixelFormats::Gray8 :
        (img.channels() == 3 ? System::Windows::Media::PixelFormats::Bgr24 :
            System::Windows::Media::PixelFormats::Bgra32);

    return System::Windows::Media::Imaging::BitmapImage::Create(
        img.cols, img.rows, graphics->DpiX, graphics->DpiY, pixelFormat,
        System::Windows::Media::Imaging::BitmapPalettes::WebPalette, managedArray, img.step);
}

System::Windows::Media::ImageSource^ ImageProc::Core::Interface::GetImageSource(ImreadModes mode)
{
    return CvtMatToImageSource(GetCvImage(*m_pImage, mode));
}

List<array<double>^>^ ImageProc::Core::Interface::CalcHistogram(ImreadModes mode,bool norm)
{
    cv::Mat image = mode == ImreadModes::Grayscale ? CvtToGrayscale(*m_pImage) : *m_pImage;
    auto histVec = Histogram::Histogram::CalcHistogram(image, norm);
    List<array<double>^>^ histList = gcnew List<array<double>^>;
    for (int i = 0; i < histVec.size(); i++)
    {
        array<double>^ hist = gcnew array<double>(histVec[i].size());
        for (int j = 0; j < hist->Length; j++) hist[j] = histVec[i][j];
        histList->Add(hist);
    }
    return histList;
}

int ImageProc::Core::Interface::GetImageWidth()
{
    return m_pImage->cols;
}

int ImageProc::Core::Interface::GetImageHeight()
{
    return m_pImage->rows;
}

int ImageProc::Core::Interface::GetImageChannels(ImreadModes mode)
{
    return GetCvImage(*m_pImage, mode).channels();
}
