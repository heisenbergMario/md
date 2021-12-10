/**
https://blog.csdn.net/wangguchao/article/details/103886606
https://blog.csdn.net/duan_xiaosu/article/details/68930643
**/
#include "QtWidgetsApplication2.h"


QtWidgetsApplication2::QtWidgetsApplication2(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    th1 = new myThread(this);
    connect(th1, SIGNAL(sig()), this, SLOT(lot()));
    openFlag = false;
    th1->stop = true;
    ui.btnB1->setEnabled(false);
    ui.btnC1->setEnabled(false);
    ui.btnD1->setEnabled(false);
    ui.btnE1->setEnabled(false);
    ui.btnF1->setEnabled(false);
    ui.btnG1->setEnabled(false);
    ui.btnK1->setEnabled(false);
    ui.btnL1->setEnabled(false);
    ui.btnA2->setEnabled(false);
    ui.btnB2->setEnabled(false);
    ui.btnC2->setEnabled(false);
    ui.btnD2->setEnabled(false);
    ui.btnE2->setEnabled(false);
    ui.btnF2->setEnabled(false);
}

void QtWidgetsApplication2::lot() {
    //ui.lw3->scrollToBottom();
    //btnC1();
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
                ui.btnB1->setEnabled(true);
                ui.btnC1->setEnabled(true);
                ui.btnD1->setEnabled(true);
                ui.btnE1->setEnabled(true);
                ui.btnF1->setEnabled(true);
                ui.btnG1->setEnabled(true);
                ui.btnK1->setEnabled(true);
                ui.btnL1->setEnabled(true);
                ui.btnA2->setEnabled(true);
                ui.btnB2->setEnabled(true);
                ui.btnC2->setEnabled(true);
                ui.btnD2->setEnabled(true);
                ui.btnE2->setEnabled(true);
                ui.btnF2->setEnabled(true);
                ui.btnK1->setEnabled(true);
                ui.btnL1->setEnabled(false);
                ui.lw3->addItem("[USB] opened");                
            }
            break;
                    
        }
        else {
            ui.lw3->addItem("[USB] Not target device");
        }
    }    
}

void QtWidgetsApplication2::btnA1() {
    ui.lw3->addItem("\r\n btnA1");
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

void QtWidgetsApplication2::btnB1() {
    ui.lw3->addItem("\r\n btnB1");    
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

void QtWidgetsApplication2::btnC1() {
    ui.lw3->addItem("\r\n btnC1");    
    readData();
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnD1() {
    ui.lw3->addItem("\r\n btnD1: ep2 write");
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

void QtWidgetsApplication2::btnE1() {
    ui.lw3->addItem("\r\n btnE1: ep2 read");    
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

void QtWidgetsApplication2::btnF1() {
    ui.lw3->addItem("\r\n btnF1: ep3 write");   
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

void QtWidgetsApplication2::btnG1() {
    ui.lw3->addItem("\r\n btnG1: ep3 read");
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

void QtWidgetsApplication2::btnH1() {
    ui.lw3->addItem("\r\n btnH1");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnI1() {
    ui.lw3->addItem("\r\n btnI1");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnJ1() {
    ui.lw3->addItem("\r\n btnJ1");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnK1() {
    ui.lw3->addItem("\r\n btnK1: th start");
    th1->stop = false;
    th1->start();
    ui.btnK1->setEnabled(false);
    ui.btnL1->setEnabled(true);
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnL1() {
    ui.lw3->addItem("\r\n btnL1: th stop");
    if (th1->isRunning())
    {
        ui.btnK1->setEnabled(true);
        ui.btnL1->setEnabled(false);
        th1->stop = 1;
    }    
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnA2() {
    ui.lw3->addItem("\r\n btnA2 contorl std");
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    int request = ui.le1->text().toInt();
    ui.lw3->addItem(ui.le1->text());
    /*
    dev_handle: 这就是之前libopen函数获得的句柄
    bmRequestType: 请求字段的类型
                   D7:0=HOST2DEVICE,1=DEVICE2HOST
                   D6:0=标准,1=类,2=厂商,3=保留
                   D4~D0:0=设备,1=接口,2=端点,3=其他,4...31=保留
    bRequest: 命令
    wValue: RequestType+Request
    wIndex: 简单点可以理解为字段内容的位置，假设你选择准备发送string类型的话，可以会有多个string，分开发送，那肯定就会出现index从0开始到N结束。
    data 字段内容
    wLength 字段内容长度，记得+1
    timeout 超时设置，以毫秒为单位
    */
    r = libusb_control_transfer(dev_handle, CTRL_STD_OUT, STD_REQUEST, (CTRL_STD_OUT<<8)| STD_REQUEST, 0, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), 100);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        //ui.lw3->addItem("[write]" + QString::number(actualLenth) + "byte");
        //ui.lw3->addItem("[read]" + QString(QLatin1String((char*)data)));
        ui.lw3->scrollToBottom();
    }
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnB2() {
    ui.lw3->addItem("\r\n btnB2 contorl std");
    int r = 0;
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    r = libusb_control_transfer(dev_handle, CTRL_STD_IN, STD_REQUEST, (CTRL_STD_IN << 8) | STD_REQUEST, 0, dataReceive, lenth, 0);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        //ui.lw3->addItem("[read]" + QString::number(actualLenth) + "byte");
        ui.lw3->addItem("[read]" + QString(QLatin1String((char*)dataReceive)));
        ui.lw3->scrollToBottom();
    }
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnC2() {
    ui.lw3->addItem("\r\n btnC2 control vendor");
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    /*
    dev_handle: 这就是之前libopen函数获得的句柄
    bmRequestType: 请求字段的类型
                   D7:0=HOST2DEVICE,1=DEVICE2HOST
                   D6:0=标准,1=类,2=厂商,3=保留
                   D4~D0:0=设备,1=接口,2=端点,3=其他,4...31=保留
    bRequest: 命令
    wValue: RequestType+Request
    wIndex: 简单点可以理解为字段内容的位置，假设你选择准备发送string类型的话，可以会有多个string，分开发送，那肯定就会出现index从0开始到N结束。
    data 字段内容
    wLength 字段内容长度，记得+1
    timeout 超时设置，以毫秒为单位
    */
    r = libusb_control_transfer(dev_handle, CTRL_VENDOR_OUT, VENDOR_REQUEST, 0, 0, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), 100);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        //ui.lw3->addItem("[write]" + QString::number(actualLenth) + "byte");
        //ui.lw3->addItem("[read]" + QString(QLatin1String((char*)data)));
        ui.lw3->scrollToBottom();
    }
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnD2() {
    ui.lw3->addItem("\r\n btnD2 control vendor");
    int r = 0;
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    r = libusb_control_transfer(dev_handle, CTRL_VENDOR_IN, VENDOR_REQUEST, (CTRL_VENDOR_IN << 8) | VENDOR_REQUEST, 0, dataReceive, lenth, 0);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        //ui.lw3->addItem("[read]" + QString::number(actualLenth) + "byte");
        ui.lw3->addItem("[read]" + QString(QLatin1String((char*)dataReceive)));
        ui.lw3->scrollToBottom();
    }
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnE2() {
    ui.lw3->addItem("\r\n btnE2 control reserve");
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    /*
    dev_handle: 这就是之前libopen函数获得的句柄
    bmRequestType: 请求字段的类型
                   D7:0=HOST2DEVICE,1=DEVICE2HOST
                   D6:0=标准,1=类,2=厂商,3=保留
                   D4~D0:0=设备,1=接口,2=端点,3=其他,4...31=保留
    bRequest: 命令
    wValue: RequestType+Request
    wIndex: 简单点可以理解为字段内容的位置，假设你选择准备发送string类型的话，可以会有多个string，分开发送，那肯定就会出现index从0开始到N结束。
    data 字段内容
    wLength 字段内容长度，记得+1
    timeout 超时设置，以毫秒为单位
    */
    r = libusb_control_transfer(dev_handle, CTRL_RESERVE_OUT, RESERVE_REQUEST, (CTRL_RESERVE_OUT << 8) | RESERVE_REQUEST, 0, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), 100);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        //ui.lw3->addItem("[write]" + QString::number(actualLenth) + "byte");
        //ui.lw3->addItem("[read]" + QString(QLatin1String((char*)data)));
        ui.lw3->scrollToBottom();
    }
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnF2() {
    ui.lw3->addItem("\r\n btnF2 control reserve");
    int r = 0;
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    r = libusb_control_transfer(dev_handle, CTRL_RESERVE_IN, RESERVE_REQUEST, (CTRL_RESERVE_IN << 8) | RESERVE_REQUEST, 0, dataReceive, lenth, 0);
    if (r < 0)
    {
        ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
        ui.lw3->scrollToBottom();
        return;
    }
    else
    {
        //ui.lw3->addItem("[read]" + QString::number(actualLenth) + "byte");
        ui.lw3->addItem("[read]" + QString(QLatin1String((char*)dataReceive)));
        ui.lw3->scrollToBottom();
    }
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnG2() {
    ui.lw3->addItem("\r\n btnG2");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnH2() {
    ui.lw3->addItem("\r\n btnH2");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnI2() {
    ui.lw3->addItem("\r\n btnI2");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnJ2() {
    ui.lw3->addItem("\r\n btnJ2");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnK2() {
    ui.lw3->addItem("\r\n btnK2");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnL2() {
    ui.lw3->addItem("\r\n btnL2");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnA3() {
    ui.lw3->addItem("\r\n btnA3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnB3() {
    ui.lw3->addItem("\r\n btnB3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnC3() {
    ui.lw3->addItem("\r\n btnC3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnD3() {
    ui.lw3->addItem("\r\n btnD3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnE3() {
    ui.lw3->addItem("\r\n btnE3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnF3() {
    ui.lw3->addItem("\r\n btnF3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnG3() {
    ui.lw3->addItem("\r\n btnG3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnH3() {
    ui.lw3->addItem("\r\n btnH3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnI3() {
    ui.lw3->addItem("\r\n btnI3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnJ3() {
    ui.lw3->addItem("\r\n btnJ3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnK3() {
    ui.lw3->addItem("\r\n btnK3");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnL3() {
    ui.lw3->addItem("\r\n btnL3");
    ui.lw3->scrollToBottom();
}
