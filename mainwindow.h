#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_calendarWidget_selectionChanged();
    void on_pushButton_clicked();
    void showTime();
    void on_exit();
    void enableSettingWin();
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();
    void on_Volumeslider_valueChanged(int value);
    void on_pushButton_3_clicked();
    void on_checkAll_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    void on_actionExit_triggered();
    void playsound(QString test);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void updateCheck();
    bool ConnectDatabase();
    void CreateTable();
    void InitTable();
    void setCurrentState();

    QMediaPlayer *player;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QSqlDatabase myDB;
    QDateTime _alarmDT;
    QString _soundFile;
    QString _dates;
    int lefttime;
    double rate;
    double counter;
};
#endif // MAINWINDOW_H
