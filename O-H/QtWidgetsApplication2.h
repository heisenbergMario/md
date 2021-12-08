#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication2.h"
#include<libusb.h>
#include"myThread.h"

#define USB_VID 0x0D7D
#define USB_PID 0x010D
#define EP1_IN 0x91
#define EP1_OUT 0x11
#define EP2_IN 0x82
#define EP2_OUT 0x02
#define EP3_IN 0x83
#define EP3_OUT 0x03

class QtWidgetsApplication2 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget *parent = Q_NULLPTR);
    void openUSB();
    void writeData();
    void readData();

    libusb_device_handle* dev_handle = NULL;
    

private:
    Ui::QtWidgetsApplication2Class ui;
    bool openFlag;
    myThread* th1;
    unsigned char dataReceive[100];
    int lenth;
    int actualLenth;


private slots:
    void btn1();
    void btn2();
    void btn3();
    void btn4();
    void btn5();
    void btn6();
    void btn7();
    void btn8();
    void btn9();
    void btnA();
    void btnB();
    void btnC();
    void lot();
};

