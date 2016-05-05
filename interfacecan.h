#ifndef INTERFACECAN
#define INTERFACECAN

#include <QThread>
#include "can/cancomm.h"
#include <signal.h>
#include <iomanip>

class InterfaceCan : public QThread
{
    Q_OBJECT
public:
    void run();
    void runCan();

signals:
    void updateBatteryTemp(int temp);
    void updateRPM(double rpm);
    void updateRMScurr(int curr);
    void updateDCvolt(double volt);
    void updateHStemp(int temp);
    void updateMotorTemp(int temp);
    void updateVoltAngle(int angle);
    void updateIQcurr(int angle);
    void sendEMCY6(int emc);
    void sendEMCY7(int emc);
    void sendD6stat(int stat);
    void sendD7stat(int stat);
};

void my_handler(int s);

#endif // INTERFACECAN

