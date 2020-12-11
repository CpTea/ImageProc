using System;
using System.Windows.Input;

namespace Mvvm.Core
{
    public class RelayCommand : ICommand
    {
        private readonly Action mAction;

        public RelayCommand(Action action) => mAction = action;

        public event EventHandler CanExecuteChanged = new EventHandler((sender, e) => { });

        public bool CanExecute(object parameter) => true;

        public void Execute(object parameter) => mAction?.Invoke();
    }

    public class RelayCommand<T> : ICommand
    {
        private readonly Action<T> mAction;

        public RelayCommand(Action<T> action) => mAction = action;

        public event EventHandler CanExecuteChanged = new EventHandler((sender, e) => { });

        public bool CanExecute(object parameter) => true;

        public void Execute(object parameter) => mAction?.Invoke((T)parameter);
    }
}
