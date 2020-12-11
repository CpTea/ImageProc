using Mvvm.Core;
using ImageProc.Core;
using System.Windows.Media;

namespace ImageProc.App.Model
{
    public class ImageMdl : NotifyPropertyChanged
    {
        private string m_filename;
        public string FileName
        {
            get => m_filename;
            set
            {
                Set(ref m_filename, value);
                m_interface.SetImage(m_filename, ImreadModes.Color);
                RaisePropertyChanged(nameof(Source));
            }
        }

        private ImreadModes m_imageMode = ImreadModes.Color;
        public ImreadModes ImageMode
        {
            get => m_imageMode;
            set
            {
                Set(ref m_imageMode, value);
                RaisePropertyChanged(nameof(ImageChannelsString));
                RaisePropertyChanged(nameof(Source));
            }
        }

        public ImageSource Source
        {
            get => m_interface.GetImageSource(ImageMode);
        }

        public int ImageWidth { get => m_interface.GetImageWidth(); }

        public int ImageHeight { get => m_interface.GetImageHeight(); }

        public int ImageChannels { get => m_interface.GetImageChannels(ImageMode); }

        public int ImagePixels { get => ImageWidth * ImageHeight * ImageChannels; }

        public string ImageWidthString
        {
            get => string.Format("宽度：{0}px", ImageWidth.ToString());
        }

        public string ImageHeighttring
        {
            get => string.Format("高度：{0}px", ImageHeight.ToString());
        }

        public string ImageChannelsString
        {
            get => string.Format("通道数：{0}", ImageChannels.ToString());
        }

        public string ImagePixelsString
        {
            get => string.Format("像素总和：{0}", ImagePixels.ToString());
        }

        private readonly Core.Interface m_interface = new Core.Interface();
        public Core.Interface Interface { get => m_interface; }
    }
}
