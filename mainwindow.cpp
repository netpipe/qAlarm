#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QSound>
#include <QFileDialog>
#include <QBuffer>
#include <QProcess>
#include <QHeaderView>
#include <QFileInfo>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QDebug>

QString mediadir = "./media/"; //change mediadir2 in main.cpp aswell if needed


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    if(!ConnectDatabase())
    {
        QMessageBox::warning(0,"Warning", "You failed database connect.");
        this->close();
        qApp->exit(0);
    }
    player = new QMediaPlayer(this);
    counter = 0;
    CreateTable();
    InitTable();
    setCurrentState();

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->timeCountProgress->setValue(0);

    QIcon oIcon("./resource/Icon.png");
    trayIcon = new QSystemTrayIcon(oIcon);

    QAction *quit_action = new QAction( "Exit", trayIcon );
    QAction *setting_action = new QAction( "Setting", trayIcon );
    connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()));
    connect(setting_action, SIGNAL(triggered()), this, SLOT(enableSettingWin));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( quit_action );
    trayIconMenu->addAction(setting_action);
    trayIcon->setContextMenu( trayIconMenu);
    trayIcon->setVisible(true);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    QTimer *timerClock = new QTimer(this);
    connect(timerClock,SIGNAL(timeout()), this, SLOT(showTime()));
    timerClock->setInterval(1000);
    timerClock->start(1000);
    this->hide();
}

bool MainWindow::ConnectDatabase()
{
    bool flag;
    myDB = QSqlDatabase::addDatabase("QSQLITE");

    QString pathToDB = QString(QApplication::applicationDirPath() + "/database.sql");
    myDB.setDatabaseName(pathToDB);

    QFileInfo checkFile(pathToDB);
    if (checkFile.isFile())
    {
        if (myDB.open())
            flag = true;
        else
            flag = false;
    }
    return flag;
}

void MainWindow::CreateTable()
{
    QSqlQuery queryCreate(myDB);
    queryCreate.exec("CREATE TABLE IF NOT EXISTS tbl_setting (alarm_time text NOT NULL, setting_time text NOT NULL, sound_path text, description text)");
}

void MainWindow::enableSettingWin()
{
    this->show();
}

void MainWindow::playsound(QString test){
      QFile file(test.toLatin1());
      file.open(QIODevice::ReadOnly);
      QByteArray* arr = new QByteArray(file.readAll());
      file.close();
      QBuffer* buffer = new QBuffer(arr);
      buffer->open(QIODevice::ReadOnly);
      buffer->seek(0);
      player->setVolume(10);
      player->setMedia(QMediaContent(), buffer);

      QMediaPlaylist *playlist = new QMediaPlaylist();
      playlist->addMedia(QUrl(test));
      playlist->setPlaybackMode(QMediaPlaylist::Loop);

      player->setPlaylist(playlist);
      player->play();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
    QApplication::quit();
}

void MainWindow::on_exit()
{
    this->close();
    QApplication::quit();
}

void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    ui->timeLcd->display(text);
    counter++;
    if(counter >= rate)
    {
        ui->timeCountProgress->setValue(ui->timeCountProgress->value() + rate);
        counter = 0;
    }


    if(_alarmDT.time().toString("hh:mm:ss") == text)
    {
        QDate date = QDate::currentDate();
        switch(date.dayOfWeek())
        {
         case 1 :
            if(_dates.contains("monday"))
                playsound(_soundFile);
            break;
        case 2 :
           if(_dates.contains("tuesday"))
               playsound(_soundFile);
           break;
        case 3:
           if(_dates.contains("wensday"))
               playsound(_soundFile);
           break;
        case 4:
           if(_dates.contains("thursday"))
               playsound(_soundFile);
           break;
        case 5:
           if(_dates.contains("friday"))
               playsound(_soundFile);
           break;
        case 6:
           if(_dates.contains("saturday"))
               playsound(_soundFile);
           break;
        case 7:
           if(_dates.contains("sunday"))
               playsound(_soundFile);
           break;
        }
    }
}

void MainWindow::on_calendarWidget_selectionChanged()
{

}

void MainWindow::on_pushButton_clicked()
{
    if(ui->dateTimeEdit->dateTime() < QDateTime::currentDateTime())
    {
        QMessageBox::warning(0, "Warning", "Datetime for alarm setting can't small than current datatime.");
        return;
    }

    if(ui->label->text().isEmpty())
    {
        QMessageBox::warning(0,"Warning","Please setting sound file.");
        return;
    }

    QString daystr;
    if(ui->checkMon->isChecked())
        daystr += "monday";
    if(ui->checkTuesday->isChecked())
        daystr += "tuesday";
    if(ui->checkWensday->isChecked())
        daystr += "wensday";
    if(ui->checkThursday->isChecked())
        daystr += "thursday";
    if(ui->checkFriday->isChecked())
        daystr += "friday";
    if(ui->checkSaturday->isChecked())
        daystr += "saturday";
    if(ui->checkSunday->isChecked())
        daystr += "sunday";

    _alarmDT = ui->dateTimeEdit->dateTime();
    _soundFile = ui->label->text();
    _dates = daystr;


    if(daystr == "")
    {
        QMessageBox::warning(0,"Warning","You didn't choose date.");
        return;
    }

    QSqlQuery query(myDB);
    query.prepare("INSERT INTO tbl_setting (alarm_time, setting_time, sound_path, description) VALUES (:alarm_t, :current_t, :sound, :dates)");
    query.bindValue(":alarm_t", ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":current_t", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":sound", ui->label->text());
    query.bindValue(":dates", daystr);
    query.exec();

    counter = 0;
    InitTable();
    setCurrentState();
}


void MainWindow::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QApplication::applicationDirPath(),
                                                    tr("Sounds (*.wav *.mp3)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                            tr("File error"),
                            tr("Failed to open\n%1").arg(fileName));
        return;
    }
    ui->label->setText(fileName);
}

void MainWindow::InitTable()
{
    QSqlQuery query(myDB);
    if(query.exec("SELECT * FROM tbl_setting"))
    {
        int row = 0;
        while(query.next())
        {
            ui->tableWidget->setRowCount(row + 1);
            ui->tableWidget->setItem(row,0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget->setItem(row,1,new QTableWidgetItem(query.value(1).toString()));
            ui->tableWidget->setItem(row,2,new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget->setItem(row,3,new QTableWidgetItem(query.value(3).toString()));
            row++;
        }
    }
}

void MainWindow::setCurrentState()
{
    QSqlQuery query(myDB);
    if(query.exec("SELECT * FROM tbl_setting"))
    {
        query.last();
        ui->dateTimeEdit->setDateTime(query.value(0).toDateTime());
        _alarmDT = query.value(0).toDateTime();
        ui->label->setText(query.value(2).toString());
        _soundFile = query.value(2).toString();

        ui->checkMon->setChecked(false);
        ui->checkTuesday->setChecked(false);
        ui->checkWensday->setChecked(false);
        ui->checkThursday->setChecked(false);
        ui->checkFriday->setChecked(false);
        ui->checkSaturday->setChecked(false);
        ui->checkSunday->setChecked(false);

        QString dates = query.value(3).toString();
        _dates = dates;

        QDateTime now = query.value(1).toDateTime();
        lefttime = now.secsTo(_alarmDT);
        rate = double(lefttime/100);
        ui->timeCountProgress->setMinimum(0);
        ui->timeCountProgress->setMaximum(lefttime);

        if(dates.contains("monday"))
            ui->checkMon->setChecked(true);
        if(dates.contains("tuesday"))
            ui->checkTuesday->setChecked(true);
        if(dates.contains("wensday"))
            ui->checkWensday->setChecked(true);
        if(dates.contains("thursday"))
            ui->checkThursday->setChecked(true);
        if(dates.contains("friday"))
            ui->checkFriday->setChecked(true);
        if(dates.contains("saturday"))
            ui->checkSaturday->setChecked(true);
        if(dates.contains("sunday"))
            ui->checkSunday->setChecked(true);

    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query(myDB);
    query.exec("DELETE FROM tbl_setting WHERE alarm_time = '" + ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss") + "'");
    InitTable();
    setCurrentState();
}

void MainWindow::on_Volumeslider_valueChanged(int value)
{
    player->setVolume(value);
}

void MainWindow::on_pushButton_3_clicked()
{
    player->stop();
}

void MainWindow::on_checkAll_clicked(bool checked)
{
    if(checked){
        ui->checkMon->setChecked(true);
        ui->checkTuesday->setChecked(true);
        ui->checkWensday->setChecked(true);
        ui->checkThursday->setChecked(true);
        ui->checkFriday->setChecked(true);
        ui->checkSaturday->setChecked(true);
        ui->checkSunday->setChecked(true);
    }else{
        ui->checkMon->setChecked(false);
        ui->checkTuesday->setChecked(false);
        ui->checkWensday->setChecked(false);
        ui->checkThursday->setChecked(false);
        ui->checkFriday->setChecked(false);
        ui->checkSaturday->setChecked(false);
        ui->checkSunday->setChecked(false);
    }
}
