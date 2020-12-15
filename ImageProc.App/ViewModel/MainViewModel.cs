using ImageProc.App.View;
using Mvvm.Core;
using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows.Controls;

namespace ImageProc.App.ViewModel
{
    public class MainViewModel : NotifyPropertyChanged
    {
        public MainViewModel()
        {
            Menubar = new ObservableCollection<Model.IconTextButtonMdl>
            {
                new Model.IconTextButtonMdl {Icon = "\xe832", Text = "打开", ID = 0},
                new Model.IconTextButtonMdl {Icon = "\xe504", Text = "保存", ID = 1},
                new Model.IconTextButtonMdl {Icon = "\xe63c", Text = "图像", ID = 2},
                new Model.IconTextButtonMdl {Icon = "\xe6f5", Text = "直方图", ID = 3},
                new Model.IconTextButtonMdl {Icon = "\xe65b", Text = "均衡化", ID = 4},
                new Model.IconTextButtonMdl {Icon = "\xe65b", Text = "排序滤波", ID = 5},
            };
            
            ImgMdl = new Model.ImageMdl();
            HistMdl = new Model.HistogramMdl { IsShowColumn = true, HistMode = Core.ImreadModes.Color };
        }

        private Model.ImageMdl m_imgmdl;
        public Model.ImageMdl ImgMdl
        {
            get => m_imgmdl;
            set => Set(ref m_imgmdl, value);
        }

        public Model.HistogramMdl m_histmdl;
        public Model.HistogramMdl HistMdl
        {
            get => m_histmdl;
            set => Set(ref m_histmdl, value);
        }

        public ObservableCollection<Model.IconTextButtonMdl> Menubar { get; set; }

        private UserControl m_workspace;
        public UserControl Workspace
        {
            get => m_workspace;
            set => Set(ref m_workspace, value);
        }

        private UserControl m_toolbar;
        public UserControl Toolbar
        {
            get => m_toolbar;
            set => Set(ref m_toolbar, value);
        }

        public RelayCommand<int> Command
        {
            get => new RelayCommand<int>(ExecuteCommand);
        }
        
        public RelayCommand GrayscaleImageCmd
        {
            get => new RelayCommand(ExecuteGrayscaleImageCmd);
        }

        public RelayCommand OriginalImageCmd
        {
            get => new RelayCommand(ExecuteOriginalImageCmd);
        }

        public RelayCommand ColorImageCmd
        {
            get => new RelayCommand(ExecuteColorImageCmd);
        }

        public RelayCommand ShowColorHistCmd
        {
            get => new RelayCommand(ExecuteShowColorHistCmd);
        }

        public RelayCommand ShowGrayHistCmd
        {
            get => new RelayCommand(ExecuteShowGrayHistCmd);
        }

        public RelayCommand ShowLineChartCmd
        {
            get => new RelayCommand(ExecuteShowLineChartCmd);
        }

        public RelayCommand ShowColumnChartCmd
        {
            get => new RelayCommand(ExecuteShowColumnChartCmd);
        }

        private void ExecuteCommand(int id)
        {
            switch (id)
            {
                case 0: ExecuteOpenImageCmd(); break;
                case 1: ExecuteSaveImageCmd(); break;
                case 2: ExecuteShowImageCmd(); break;
                case 3: ExecuteCalcHistogramCmd(); break;
                case 4: ExecuteEqualizeHistogramCmd(); break;
                case 5: ExecuteSortFilterCmd(); break;
                default: break;
            }
        }

        private void ExecuteSortFilterCmd()
        {
            if (ImgMdl.FileName is null) return;
        }

        private void ExecuteOpenImageCmd()
        {
            System.Windows.Forms.OpenFileDialog ofd = new System.Windows.Forms.OpenFileDialog
            {
                Filter = "所有格式|*.*|png格式(*png)|*.png|jpeg格式(*.jpg;*.jpeg)|*.jpg;*.jpeg",
                ValidateNames = true,
                CheckFileExists = true,
                CheckPathExists = true
            };
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                ImgMdl.FileName = ofd.FileName;
                Workspace = new ImageWorkspace();
                Toolbar = new ImageToolbar();
            }
        }

        private void ExecuteEqualizeHistogramCmd()
        {
            if (ImgMdl.FileName is null) return;
            Toolbar = new ImageToolbar();
            Workspace = new ImageWorkspace();
            ImgMdl.Interface.EqualizeHistogram(ImgMdl.ImageMode);
        }

        private void ExecuteCalcHistogramCmd()
        {
            if (ImgMdl.FileName is null) return;
            HistMdl.Hists = ImgMdl.Interface.CalcHistogram(HistMdl.HistMode, true);
            Workspace = new ChartWorkspace();
            Toolbar = new ChartToolbar();

        }

        private void ExecuteShowImageCmd()
        {
            if (ImgMdl.FileName is null) return;
            Workspace = new ImageWorkspace();
            Toolbar = new ImageToolbar();
        }

        private void ExecuteSaveImageCmd()
        {
            if (ImgMdl.FileName is null) return;
            System.Windows.Forms.SaveFileDialog sfd = new System.Windows.Forms.SaveFileDialog
            {
                Filter = "所有格式|*.*|png格式(*png)|*.png|jpeg格式(*.jpg;*.jpeg)|*.jpg;*.jpeg",
                ValidateNames = true,
                FileName = Path.GetFileName(ImgMdl.FileName),
                CheckPathExists = true
            };
            if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                ImgMdl.Interface.SaveCurrentImage(sfd.FileName, ImgMdl.ImageMode);
                MessageBox message = new MessageBox();
                message.ShowDialog();
            }
        }

        private void ExecuteGrayscaleImageCmd()
        {
            ImgMdl.ImageMode = Core.ImreadModes.Grayscale;
            HistMdl.HistMode = ImgMdl.ImageMode;
        }

        private void ExecuteOriginalImageCmd()
        {
            ImgMdl.ImageMode = Core.ImreadModes.Color;
            ImgMdl.FileName = ImgMdl.FileName;
            HistMdl.HistMode = ImgMdl.ImageMode;
        }

        private void ExecuteColorImageCmd()
        {
            ImgMdl.ImageMode = Core.ImreadModes.Color;
            HistMdl.HistMode = ImgMdl.ImageMode;
        }

        private void ExecuteShowColorHistCmd()
        {
            HistMdl.HistMode = Core.ImreadModes.Color;
            ExecuteCalcHistogramCmd();
        }

        private void ExecuteShowGrayHistCmd()
        {
            HistMdl.HistMode = Core.ImreadModes.Grayscale;
            ExecuteCalcHistogramCmd();
        }

        private void ExecuteShowLineChartCmd()
        {
            HistMdl.IsShowColumn = false;
        }

        private void ExecuteShowColumnChartCmd()
        {
            HistMdl.IsShowColumn = true;
        }


    }
}