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

QString mediadir = "./media/"; //change mediadir2 in main.cpp aswell if needed


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap oPixmap(32,32);
    oPixmap.load ( mediadir + "qtimer2.png");

    QIcon oIcon( oPixmap );

    trayIcon = new QSystemTrayIcon(oIcon);

    QAction *quit_action = new QAction( "Exit", trayIcon );
    connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( quit_action );

    trayIcon->setContextMenu( trayIconMenu);
    trayIcon->setVisible(true);
    //trayIcon->showMessage("Test Message", "Text", QSystemTrayIcon::Information, 1000);
    //trayIcon->show();


    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    //ui->timeCountProgress->setRange(0, int(timerPeriod.toFloat() * 60));
    //ui->timeCountProgress->setValue(0);

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(updateCheck()));
    //timer->start(int(timerPeriod.toFloat() * 60) * 1000); // int(timerPeriod) * 60 * 1000

    QTimer *timerClock = new QTimer(this);
    connect(timerClock,SIGNAL(timeout()), this, SLOT(showTime()));
    timerClock->start(1000);

    showTime();

   // QSound::play( mediadir + "phone.wav");


    QMessageBox msgBox;
    msgBox.setText("Skipped.");
   // msgBox.exec();

 //   QFile file(dataFile);
  //  file.open(QFile::Append | QFile::Text);
   // QTextStream out(&file);   // we will serialize the data into the file
   // out << QString("skipped")  + "," + currentDate + "," + QTime::currentTime().toString()+","+type+","+count+","+cost << endl;
   // file.close();

 //   if( QFileInfo("settings.txt").exists())    {        on_actionOpenCoin_triggered();    }
    player=new QMediaPlayer();
    playsound("Resource/sounds/ec1_mono.ogg");

}

void MainWindow::playsound(QString test){
    // player->setMedia(QUrl("qrc:/sounds/ec1_mono.ogg"));
    // player->setMedia(QUrl::fromLocalFile("./paddle_hit.wav"));
     //or play from memory
      QFile file(test.toLatin1());
      file.open(QIODevice::ReadOnly);
      QByteArray* arr = new QByteArray(file.readAll());
      file.close();
      QBuffer* buffer = new QBuffer(arr);
      buffer->open(QIODevice::ReadOnly);
      buffer->seek(0);
//qDebug() << "Media supported state -> " << QMediaPlayer::hasSupport("video/mp4"); // this gives a "1"
      player->setVolume(10);
 //    media->setMedia("sound.mp3");
     player->setMedia(QMediaContent(), buffer);
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
   QString text = time.toString("hh:mm");
   if ((time.second() % 2) == 0)
       text[2] = ' ';
   ui->timeLcd->display(text);

   //qDebug()<<  ui->timeCountProgress->value();
   ui->timeCountProgress->setValue( ui->timeCountProgress->value() + 1);

   if(ui->timeCountProgress->value() == ui->timeCountProgress->maximum())
   {
       updateCheck();
       ui->timeCountProgress->setValue(0);
   }

}


void MainWindow::updateCheck()
{

    // this->setFocus(); this does not work :-(

    //ui->timeCountProgress->setValue(0);

    this->show();
    //this->setWindowFlags((windowFlags() & Qt::WindowStaysOnTopHint));
    this->activateWindow();
    this->raise();
   // this->setWindowState(Qt::WindowActive);
    //if( soundEnable.compare("1") == 0)
    //{

        QSound::play( mediadir + "phone.wav");
    //}
 //  if(messageEnable.compare("1") == 0)
  //  {
    //    showMessage();
    //}

}

void MainWindow::on_calendarWidget_selectionChanged()
{
   // ui->calendarWidget->
}

void MainWindow::on_Volumeslider_sliderReleased()
{
    player->setVolume(1);
    //save volume
}

void MainWindow::on_pushButton_clicked()
{
   /*
            ui->checkMon
            ui->checkTuesday
            ui->checkWensday
            ui->checkThursday
            ui->checkFriday
            ui->checkSaturday
            ui->checkSunday
            ui->checkAll
     */

    QString daystr;

    if       (    ui->checkMon->isChecked() )
        daystr += "monday";
    if       (    ui->checkTuesday->isChecked() )
        daystr += "tuesday";
    if       (    ui->checkWensday->isChecked() )
        daystr += "wensday";
    if       (    ui->checkThursday->isChecked() )
        daystr += "thursday";
    if       (    ui->checkFriday->isChecked() )
        daystr += "friday";
    if       (    ui->checkSaturday->isChecked() )
        daystr += "saturday";
    if       (    ui->checkSunday->isChecked() )
        daystr += "sunday";
    if       (    ui->checkAll->isChecked() )
        daystr += "alldays";




}
