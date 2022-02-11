#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication2.h"
#include<libusb.h>
#include"myThread.h"
#include<QImage>
#include <QBuffer>
#include <QFileInfo>
#include <QFileDialog>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define CTRL_STD_IN	        (LIBUSB_REQUEST_TYPE_STANDARD | LIBUSB_ENDPOINT_IN)
#define CTRL_STD_OUT		(LIBUSB_REQUEST_TYPE_STANDARD | LIBUSB_ENDPOINT_OUT)
#define STD_REQUEST         0x04
#define CTRL_VENDOR_IN	    (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN)
#define CTRL_VENDOR_OUT		(LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)
#define VENDOR_REQUEST      0x04
#define CTRL_RESERVE_IN	    (LIBUSB_REQUEST_TYPE_RESERVED | LIBUSB_ENDPOINT_IN)
#define CTRL_RESERVE_OUT    (LIBUSB_REQUEST_TYPE_RESERVED | LIBUSB_ENDPOINT_OUT)
#define RESERVE_REQUEST     0x04


#define USB_VID 0x0D7D
#define USB_PID 0x010D
#define EP1_IN 0x91
#define EP1_OUT 0x11
#define EP2_IN 0x82
#define EP2_OUT 0x02
#define EP3_IN 0x83
#define EP3_OUT 0x03

#define INTERFACE_NUMBER 0

class QtWidgetsApplication2 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget *parent = Q_NULLPTR);
    void openUSB();
    void writeData(int epNum);
    void readData(int epNum);
    void writeDataA(unsigned char* data, int lenth, int timeout);
    void writeQtImg(QImage img);
    void writeCvImg(Mat m);
    void openImg();
    void qtPrintImg(QImage imgOrigal);
    void qtShowImg(QImage imgOrigal);
    void cvPrintImg(Mat m);
    void cvShowImg(Mat m);
    

    libusb_device_handle* dev_handle = NULL;
    

private:
    Ui::QtWidgetsApplication2Class ui;
    Mat mSource;
    bool openFlag;
    myThread* th1;
    unsigned char dataReceive[1024];
    int lenth;
    int actualLenth;


private slots:
    void btnA1();
    void btnA2();
    void btnA3();
    void btnB1();
    void btnB2();
    void btnB3();
    void btnC1();
    void btnC2();
    void btnC3();
    void btnD1();
    void btnD2();
    void btnD3();
    void btnE1();
    void btnE2();
    void btnE3();
    void btnF1();
    void btnF2();
    void btnF3();
    void btnG1();
    void btnG2();
    void btnG3();
    void btnH1();
    void btnH2();
    void btnH3();
    void btnI1();
    void btnI2();
    void btnI3();
    void btnJ1();
    void btnJ2();
    void btnJ3();
    void btnK1();
    void btnK2();
    void btnK3();
    void btnL1();
    void btnL2();
    void btnL3();
 

    void lot();
};

