#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QSound>

QString mediadir = "./media/"; //change mediadir2 in main.cpp aswell if needed


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap oPixmap(32,32);
    oPixmap.load ( mediadir + "qtimer.png");

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

    QSound::play( mediadir + "phone.wav");


    QMessageBox msgBox;
    msgBox.setText("Skipped.");
   // msgBox.exec();

 //   QFile file(dataFile);
  //  file.open(QFile::Append | QFile::Text);
   // QTextStream out(&file);   // we will serialize the data into the file
   // out << QString("skipped")  + "," + currentDate + "," + QTime::currentTime().toString()+","+type+","+count+","+cost << endl;
   // file.close();
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




void MainWindow::on_listAlmBtn_accepted()
{
    QString name;
  //  name=this->GetTime().toString()+"  ";

    /*
    connect(ui->chkMon,SIGNAL(clicked(bool)),this,SLOT(ToggleMon(bool)));
    connect(ui->chkTues,SIGNAL(clicked(bool)),this,SLOT(ToggleTue(bool)));
    connect(ui->chkWed,SIGNAL(clicked(bool)),this,SLOT(ToggleWed(bool)));
    connect(ui->chkThurs,SIGNAL(clicked(bool)),this,SLOT(ToggleThur(bool)));
    connect(ui->chkFri,SIGNAL(clicked(bool)),this,SLOT(ToggleFri(bool)));
    connect(ui->chkSat,SIGNAL(clicked(bool)),this,SLOT(ToggleSat(bool)));
    connect(ui->chkSun,SIGNAL(clicked(bool)),this,SLOT(ToggleSun(bool)));

    if(this->isMonEnabled())
        name.append(" M");
    if(this->isTueEnabled())
        name.append(" T");
    if(this->isWedEnabled())
        name.append(" W");
    if(this->isThurEnabled())
        name.append(" Th");
    if(this->isFriEnabled())
        name.append(" F");
    if(this->isSatEnabled())
        name.append(" Sat");
    if(this->isSunEnabled())
        name.append(" Sun");
    if(this->isCustomEnabled())
        name.append("  "+this->_CustomAlarm.toString());

    */


}

void MainWindow::on_VolumeSlider_sliderReleased()
{
 //   media->setVolume(Volume);
 //   FileIO::SaveVolume(Volume);
}
