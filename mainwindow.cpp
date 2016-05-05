#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Use UI defined by QDesigner
    ui->setupUi(this);
    //this->setStyleSheet("background-color: gray");
    // Create and configure QGauge class member
    initGaugeRPM();
    initGaugeTemps();
    initPBar();
    // add to layout
    initLayout();
    // create the CAN thread
    initCan();
    // connect signals to slots
    connectSlots();
    // start reading CAN messages
    can_thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGaugeRPM() {
    mGauge = new QcGaugeWidget;

    mGauge->addArc(75);
    mGauge->addDegrees(85)->setValueRange(0,8);
    QcColorBand * clrBand = mGauge->addColorBand(70);
    clrBand->setValueRange(0,8);

    QcValuesItem* values = mGauge->addValues(86);
    values->setValueRange(0,8);
    values->setStep(1);

    mGauge->addLabel(90)->setText("RPM (x1000)");

    QcLabelItem * lab = mGauge->addLabel(60);
    lab->setText("0");

    mNeedle = mGauge->addNeedle(80);

    mNeedle->setLabel(lab);
    mNeedle->setColor(Qt::blue);
    mNeedle->setValueRange(0,8);
    mGauge->addBackground(7);
}

void MainWindow::initGaugeTemps() {
    int arc     = 45;
    int degrees = 55;
    int band    = 40;
    int values  = 56;
    int lbl1    = 50;
    int lbl2    = 30;
    int needle  = 50;

    /* ### HS TEMP GAUGE ### */
    mGaugeHS_TEMP = new QcGaugeWidget;

    mGaugeHS_TEMP->addArc(arc);
    mGaugeHS_TEMP->addDegrees(degrees)->setValueRange(0,8);
    QcColorBand * HSclrBand = mGaugeHS_TEMP->addColorBand(band,30,70,90);
    HSclrBand->setValueRange(0,90);

    QcValuesItem* HSvalues = mGaugeHS_TEMP->addValues(values);
    HSvalues->setValueRange(0,90);
    HSvalues->setStep(10);

    mGaugeHS_TEMP->addLabel(lbl1)->setText("Drive Temp");

    QcLabelItem * HSlab = mGaugeHS_TEMP->addLabel(lbl2);
    HSlab->setText("0");

    mNeedleHS_TEMP = mGaugeHS_TEMP->addNeedle(needle);

    mNeedleHS_TEMP->setLabel(HSlab);
    mNeedleHS_TEMP->setColor(Qt::blue);
    mNeedleHS_TEMP->setValueRange(0,90);
    mGaugeHS_TEMP->addBackground(7);


    /* ### M TEMP GAUGE ### */
    mGaugeM_TEMP = new QcGaugeWidget;

    mGaugeM_TEMP->addArc(arc);
    mGaugeM_TEMP->addDegrees(degrees)->setValueRange(0,8);
    QcColorBand * MclrBand = mGaugeM_TEMP->addColorBand(band,30,90,120);
    MclrBand->setValueRange(0,120);

    QcValuesItem* Mvalues = mGaugeM_TEMP->addValues(values);
    Mvalues->setValueRange(0,120);
    Mvalues->setStep(10);

    mGaugeM_TEMP->addLabel(lbl1)->setText("Motor Temp");

    QcLabelItem * Mlab = mGaugeM_TEMP->addLabel(lbl2);
    Mlab->setText("0");

    mNeedleM_TEMP = mGaugeM_TEMP->addNeedle(needle);

    mNeedleM_TEMP->setLabel(Mlab);
    mNeedleM_TEMP->setColor(Qt::blue);
    mNeedleM_TEMP->setValueRange(0,120);
    mGaugeM_TEMP->addBackground(7);

    /* ### BMS TEMP GAUGE ### */
    mGaugeBMS_TEMP = new QcGaugeWidget;

    mGaugeBMS_TEMP->addArc(arc);
    mGaugeBMS_TEMP->addDegrees(degrees)->setValueRange(0,8);
    QcColorBand * BMSclrBand = mGaugeBMS_TEMP->addColorBand(band,30,60,70);
    BMSclrBand->setValueRange(0,70);

    QcValuesItem* BMSvalues = mGaugeBMS_TEMP->addValues(values);
    BMSvalues->setValueRange(0,70);
    BMSvalues->setStep(10);

    mGaugeBMS_TEMP->addLabel(lbl1)->setText("Battery Temp");

    QcLabelItem * BMSlab = mGaugeBMS_TEMP->addLabel(lbl2);
    BMSlab->setText("0");

    mNeedleBMS_TEMP = mGaugeBMS_TEMP->addNeedle(needle);

    mNeedleBMS_TEMP->setLabel(BMSlab);
    mNeedleBMS_TEMP->setColor(Qt::blue);
    mNeedleBMS_TEMP->setValueRange(0,70);
    mGaugeBMS_TEMP->addBackground(7);
}

void MainWindow::initPBar() {
    mPBar = new QProgressBar;
    mPBar->setOrientation(Qt::Vertical);
    mPBar->setTextVisible(0);
    mPBar->setProperty("border","20px solid gray");

    mPBar->setFixedWidth(100);
    mPBar->setStyleSheet("::chunk {background-color: green}");
}

void MainWindow::initLayout() {
    tempBox = new QVBoxLayout;
    ui->gaugeBox->addLayout(tempBox);

    tempBox->addWidget(mGaugeHS_TEMP);
    tempBox->addWidget(mGaugeM_TEMP);
    tempBox->addWidget(mGaugeBMS_TEMP);


    ui->gaugeBox->addWidget(mGauge,400);

    // connected to the voltage
    ui->gaugeBox->addWidget(mPBar);}

void MainWindow::initCan() {
    can_thread = new InterfaceCan;
}

void MainWindow::connectSlots() {
    // move needle
    connect(can_thread,SIGNAL(updateRPM(double)),this,SLOT(setValue(double)));

    connect(can_thread,SIGNAL(updateHStemp(int)),this,SLOT(setTempHS(int)));
    connect(can_thread,SIGNAL(updateMotorTemp(int)),this,SLOT(setTempM(int)));
    connect(can_thread,SIGNAL(updateBatteryTemp(int)),this,SLOT(setTempBMS(int)));

    // battery indicator, currently shows voltage VALUE, not percentage
    connect(can_thread,SIGNAL(updateDCvolt(double)),this,SLOT(setBattery(double)));

    // debug console, change values to double when needed
    connect(can_thread,SIGNAL(updateRPM(double)),ui->lcdNumber,SLOT(display(double)));
    connect(can_thread,SIGNAL(updateBatteryTemp(int)),ui->lcdNumber_2,SLOT(display(int)));
    connect(can_thread,SIGNAL(updateRMScurr(int)),ui->lcdNumber_3,SLOT(display(int)));
    connect(can_thread,SIGNAL(updateDCvolt(double)),ui->lcdNumber_4,SLOT(display(double)));
    connect(can_thread,SIGNAL(updateHStemp(int)),ui->lcdNumber_5,SLOT(display(int)));
    connect(can_thread,SIGNAL(updateMotorTemp(int)),ui->lcdNumber_6,SLOT(display(int)));
    connect(can_thread,SIGNAL(updateVoltAngle(int)),ui->lcdNumber_7,SLOT(display(int)));
    connect(can_thread,SIGNAL(updateIQcurr(int)),ui->lcdNumber_8,SLOT(display(int)));
    connect(can_thread,SIGNAL(sendEMCY6(int)),ui->lcdNumber_9,SLOT(display(int)));
    connect(can_thread,SIGNAL(sendEMCY7(int)),ui->lcdNumber_10,SLOT(display(int)));
    connect(can_thread,SIGNAL(sendD6stat(int)),ui->lcdNumber_11,SLOT(display(int)));
    connect(can_thread,SIGNAL(sendD7stat(int)),ui->lcdNumber_12,SLOT(display(int)));

    // button navigation
    connect(ui->toDebugButton,SIGNAL(clicked(bool)),this,SLOT(toDebugPage()));
    connect(ui->toDashButton,SIGNAL(clicked(bool)),this,SLOT(toDashPage()));
    connect(ui->prefButton,SIGNAL(clicked(bool)),this,SLOT(toPreferences()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(cancelPref()));
    connect(ui->acceptButton,SIGNAL(clicked(bool)),this,SLOT(cancelPref()));
}

void MainWindow::setValue(double val) {
    // change where the needle points
    mNeedle->setCurrentValue(val/1000.0);
}

void MainWindow::toDebugPage() {
    ui->stackedWidget->setCurrentIndex(DEBUG);
}

void MainWindow::setTempHS(int val) {
    mNeedleHS_TEMP->setCurrentValue(val);
}

void MainWindow::setTempM(int val) {
    mNeedleM_TEMP->setCurrentValue(val);
}

void MainWindow::setTempBMS(int val) {
    mNeedleBMS_TEMP->setCurrentValue(val);
}

void MainWindow::setBattery(double val) {
    int iVal = int(val);
    mPBar->setValue(iVal);
}

void MainWindow::toDashPage() {
    ui->stackedWidget->setCurrentIndex(DASH);
}

void MainWindow::toPreferences() {
    ui->stackedWidget->setCurrentIndex(PREF);
}

void MainWindow::cancelPref() {
    // reject preferences changes, exit, reset page
    ui->stackedWidget->setCurrentIndex(DASH);
}

void MainWindow::acceptPref() {
    // accept prefences, send can
    // reset page?
    ui->stackedWidget->setCurrentIndex(DASH);
}
