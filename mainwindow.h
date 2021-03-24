#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

        void on_actionExit_triggered();

private slots:
        void on_calendarWidget_selectionChanged();

        void on_listAlmBtn_accepted();

        void on_VolumeSlider_sliderReleased();

private:
    Ui::MainWindow *ui;


    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showTime();
        void updateCheck();

    void on_exit();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;


};
#endif // MAINWINDOW_H
