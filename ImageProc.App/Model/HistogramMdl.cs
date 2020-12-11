using System.Collections.Generic;
using Mvvm.Core;
using System.Windows.Media;
using LiveCharts.Wpf;
using LiveCharts;
using ImageProc.Core;

namespace ImageProc.App.Model
{
    public class HistogramMdl : NotifyPropertyChanged
    {
        private List<double[]> m_hists;
        public List<double[]> Hists
        {
            get => m_hists;
            set
            {
                Set(ref m_hists, value);
                RaisePropertyChanged(nameof(Series));
            }
        }

        private bool m_isShowColumn;
        public bool IsShowColumn
        {
            get => m_isShowColumn;
            set
            {
                Set(ref m_isShowColumn, value);
                RaisePropertyChanged(nameof(Series));
            }
        }

        public ImreadModes HistMode
        {
            get; set;
        }

        private SeriesCollection m_series;
        public SeriesCollection Series
        {
            get
            {
                m_series = new SeriesCollection { };
                if (IsShowColumn)
                {
                    for (int i = 0; i < Hists.Count; i++)
                    {
                        m_series.Add(new ColumnSeries
                        {
                            Values = new ChartValues<double>(Hists[i]),
                            Title = i.ToString(),
                            Fill = brushes[i],
                            Stroke = brushes[i],
                            ColumnPadding = 0
                        });
                    }
                }
                else
                {
                    for (int i = 0; i < Hists.Count; i++)
                    {
                        m_series.Add(new LineSeries
                        {
                            Values = new ChartValues<double>(Hists[i]),
                            Title = i.ToString(),
                            Fill = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0)),
                            PointGeometry = null,
                            Stroke = brushes[i]
                        });
                    }
                }
                return m_series;
            }
        }

        private readonly SolidColorBrush[] brushes = new SolidColorBrush[]
        {
            Style.ColorBrushes.Blue,
            Style.ColorBrushes.Green,
            Style.ColorBrushes.Red,
            Style.ColorBrushes.Gray
        };

        public double[] StdGrayscaleValue
        {
            get
            {
                double[] means = m_means;
                double[] stds = new double[m_hists.Count];

                for (int i = 0; i < m_hists.Count; i++)
                {
                    double std = 0;
                    for (int j = 0; j < m_hists[i].Length; j++)
                    {
                        std += (m_hists[i][j] - means[i]) * (m_hists[i][j] - means[i]);
                    }
                    stds[i] = std / m_hists[i].Length;
                }
                return stds;
            }
        }

        private double[] m_means;
        public double[] MeanGrayscaleValue
        {
            get
            {
                m_means = new double[m_hists.Count];
                for (int i = 0; i < m_hists.Count; i++)
                {
                    double mean = 0;
                    for (int j = 0; j < m_hists[i].Length; j++)
                    {
                        mean += m_hists[i][j] * j;
                    }
                    m_means[i] = mean;
                }
                return m_means;
            }
        }

        public string MeanGrayscaleValueString
        {
            get => MultiValuesToString(MeanGrayscaleValue, "平均灰度值：{0}");
        }

        public string StdGrayscaleValueString
        {
            get => MultiValuesToString(StdGrayscaleValue, "标准差：{0}");
        }

        private string MultiValuesToString(double[] arr, string outputtemplate, string valuestemplate = "CH{0}: {1}")
        {
            List<string> meansStrList = new List<string> { };
            for (int i = 0; i < arr.Length; i++)
            {
                meansStrList.Add(string.Format(valuestemplate, i, arr[i].ToString("F2")));
            }
            string meansStr = string.Join("，", meansStrList);
            return string.Format(outputtemplate, meansStr);
        }

    }
}
