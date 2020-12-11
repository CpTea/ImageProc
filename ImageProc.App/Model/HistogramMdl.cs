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
                //RaisePropertyChanged(nameof(IsShowColumn));
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


    }
}
