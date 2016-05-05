#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include "qgauge.h"
#include "interfacecan.h"

// Page numbers
#define DASH    0
#define DEBUG   1
#define PREF    2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow * ui;

    QcGaugeWidget * mGauge;     // The Gauge Widget (background)
    QcNeedleItem * mNeedle;     // The needle for the gauge
    QProgressBar * mPBar;       // Battery Indicator (not implemented yet)

    // temperature gauges
   QcGaugeWidget * mGaugeHS_TEMP;
   QcNeedleItem * mNeedleHS_TEMP;
   QcGaugeWidget * mGaugeM_TEMP;
   QcNeedleItem * mNeedleM_TEMP;
   QcGaugeWidget * mGaugeBMS_TEMP;
   QcNeedleItem * mNeedleBMS_TEMP;

    InterfaceCan * can_thread;  // Reads the CAN bus
    QVBoxLayout * tempBox;

    void initGaugeRPM();
    void initGaugeTemps();
    void initPBar();
    void initLayout();
    void initCan();
    void connectSlots();

private slots:
    void setValue(double val);  // set needle value
    void setTempHS(int val);
    void setTempM(int val);
    void setTempBMS(int val);
    void setBattery(double val);
    // Navigation between screens
    void toDebugPage();
    void toDashPage();
    void toPreferences();
    void cancelPref();
    void acceptPref();
};

#endif // MAINWINDOW_H
