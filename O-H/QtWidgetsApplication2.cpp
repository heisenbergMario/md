#include "QtWidgetsApplication2.h"


QtWidgetsApplication2::QtWidgetsApplication2(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    th1 = new myThread(this);
    connect(th1, SIGNAL(sig()), this, SLOT(lot()));
    openFlag = false;
    th1->stop = true;
    ui.btn2->setEnabled(false);
    ui.btn3->setEnabled(false);
    ui.btnB->setEnabled(false);
    ui.btnC->setEnabled(false);
}

void QtWidgetsApplication2::lot() {
    //ui.lw3->scrollToBottom();
    //btn3();
    int r = 0;
    r = libusb_bulk_transfer(dev_handle, EP1_IN, dataReceive, 1, &actualLenth, 0);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        return;
    }
    else
    {
        ui.lw3->addItem("[read]" + QString::number(actualLenth) + "byte");
        ui.lw3->addItem("[read]" + QString(QLatin1String((char*)dataReceive)));
    }
    return;
}

void QtWidgetsApplication2::openUSB() {   
    ui.lw3->addItem("\r\n openUSB");
    int r = 255;
    int i = 0;
    libusb_device** devs;    
    struct libusb_device_descriptor desc;
    libusb_device* dev;
    r = libusb_init(NULL);
    ui.lw3->addItem("[libusb_init]" + QString::number(r));
    if (r < 0) {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        return;
    }
    libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL);
    r = libusb_get_device_list(NULL, &devs);
    ui.lw3->addItem("[libusb_get_device_list]" + QString::number(r));
    if (r < 0) {
        ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
        return;
    }
    while ((dev = devs[i++])!=NULL){
        r = libusb_get_device_descriptor(dev, &desc);
        ui.lw3->addItem("[libusb_get_device_descriptor]" + QString::number(r));
        if (r < 0) {
            ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
            return;
        }
        ui.lw3->addItem("[USB]VID-" + QString("%1").arg(int(desc.idVendor), 4, 16, QLatin1Char('0')).toUpper() + "  PID-" + QString("%1").arg(int(desc.idProduct), 4, 16, QLatin1Char('0')).toUpper());
        if (desc.idVendor == USB_VID && desc.idProduct == USB_PID) {
            r = libusb_open(dev, &dev_handle);
            ui.lw3->addItem("[libusb_open]" + QString::number(r));
            if (r < 0) {
                ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
                return;
            }           
            r = libusb_kernel_driver_active(dev_handle, 0);
            ui.lw3->addItem("[libusb_kernel_driver_active]" + QString::number(r));
            if (r < 0) {
                ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
                //return;
            }
            if (r == 1) {
                r = libusb_detach_kernel_driver(dev_handle, 0);
                ui.lw3->addItem("[libusb_detach_kernel_driver]" + QString::number(r));
                if (r < 0) {
                    ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
                    return;
                }
            }
            r = libusb_claim_interface(dev_handle, 0);
            ui.lw3->addItem("[libusb_claim_interface]" + QString::number(r));
            if (r < 0) {
                ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
                return;
            }
            else {
                openFlag = true;
                ui.btn2->setEnabled(true);
                ui.btn3->setEnabled(true);
                ui.btnB->setEnabled(true);
                ui.btnC->setEnabled(false);
                ui.lw3->addItem("[USB] opened");                
            }
            break;
                    
        }
        else {
            ui.lw3->addItem("[USB] Not target device");
        }
    }    
}

void QtWidgetsApplication2::btn1() {
    ui.lw3->addItem("\r\n btn1");
    openUSB();  
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::writeData() {
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    int actualLenth = 0;
    int timeout = 2000;
    //unsigned char* data = (unsigned char*)ui.pte1->toPlainText().toLatin1().data();
    r = libusb_bulk_transfer(dev_handle, EP1_OUT, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), &actualLenth, timeout);
    if (r < 0)
    {        
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        return;
    }
    else
    {
        ui.lw3->addItem("[write]" + QString::number(actualLenth) + "byte");
        //ui.lw3->addItem("[read]" + QString(QLatin1String((char*)data)));
    }
}

void QtWidgetsApplication2::btn2() {
    ui.lw3->addItem("\r\n btn2");    
    writeData();
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::readData() {
    int r = 0;
    
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    int timeout = 100;
    r = libusb_bulk_transfer(dev_handle, EP1_IN, dataReceive, lenth, &actualLenth, 100);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        return;
    }
    else
    {
        ui.lw3->addItem("[read]" + QString::number(actualLenth) + "byte");
        ui.lw3->addItem("[read]" + QString(QLatin1String((char*)dataReceive)));
    }
}

void QtWidgetsApplication2::btn3() {
    ui.lw3->addItem("\r\n btn3");    
    readData();
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btn4() {
    ui.lw3->addItem("\r\n btn4");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btn5() {
    ui.lw3->addItem("\r\n btn5");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btn6() {
    ui.lw3->addItem("\r\n btn6");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btn7() {
    ui.lw3->addItem("\r\n btn7: ep2 write");
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    r = libusb_bulk_transfer(dev_handle, EP2_OUT, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), &actualLenth, 1000);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        ui.lw3->addItem("[write]" + QString::number(actualLenth) + "byte");
        //ui.lw3->addItem("[read]" + QString(QLatin1String((char*)data)));
        ui.lw3->scrollToBottom();
    }    
}

void QtWidgetsApplication2::btn8() {
    ui.lw3->addItem("\r\n btn8: ep2 read");
    int r = 0;
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    r = libusb_bulk_transfer(dev_handle, EP2_IN, dataReceive, lenth, &actualLenth, 100);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        ui.lw3->addItem("[read]" + QString::number(actualLenth) + "byte");
        ui.lw3->addItem("[read]" + QString(QLatin1String((char*)dataReceive)));
        ui.lw3->scrollToBottom();
    }    
}

void QtWidgetsApplication2::btn9() {
    ui.lw3->addItem("\r\n btn9: ep3 write");
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    r = libusb_bulk_transfer(dev_handle, EP3_OUT, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), &actualLenth, 1000);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        ui.lw3->addItem("[write]" + QString::number(actualLenth) + "byte");
        //ui.lw3->addItem("[read]" + QString(QLatin1String((char*)data)));
        ui.lw3->scrollToBottom();
    }    
}

void QtWidgetsApplication2::btnA() {
    ui.lw3->addItem("\r\n btnA: ep3 read");
    int r = 0;
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    r = libusb_bulk_transfer(dev_handle, EP3_IN, dataReceive, lenth, &actualLenth, 100);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        ui.lw3->addItem("[read]" + QString::number(actualLenth) + "byte");
        ui.lw3->addItem("[read]" + QString(QLatin1String((char*)dataReceive)));
        ui.lw3->scrollToBottom();
    }    
}

void QtWidgetsApplication2::btnB() {
    ui.lw3->addItem("\r\n btnB: th start");
    th1->stop = false;
    th1->start();
    ui.btnB->setEnabled(false);
    ui.btnC->setEnabled(true);
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnC() {
    ui.lw3->addItem("\r\n btnC: th stop");
    if (th1->isRunning())
    {
        ui.btnB->setEnabled(true);
        ui.btnC->setEnabled(false);
        th1->stop = 1;
    }    
    ui.lw3->scrollToBottom();
}

