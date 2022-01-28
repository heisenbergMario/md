/**
[USB]https://blog.csdn.net/wangguchao/article/details/103886606
[USB]https://blog.csdn.net/duan_xiaosu/article/details/68930643
[numpy]https://mp.weixin.qq.com/s?__biz=MzIzNjc1NzUzMw==&mid=2247515127&idx=4&sn=c7322ef021fe1af216d3e20c6086d46f&scene=0#wechat_redirect
[DMA]https://www.cnblogs.com/Hx1024/p/14437476.html
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
    ui.btnA3->setEnabled(false);
    ui.btnB2->setEnabled(false);
    ui.btnB3->setEnabled(false);
    ui.btnC2->setEnabled(false);
    ui.btnC3->setEnabled(false);
}

void QtWidgetsApplication2::lot() {
    //ui.lw3->scrollToBottom();
    //btnC1();
    int r = 0;
    r = libusb_bulk_transfer(dev_handle, EP1_IN, dataReceive, 1, &actualLenth, 100);
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
            libusb_close(dev_handle);
            r = libusb_open(dev, &dev_handle);
            ui.lw3->addItem("[libusb_open]" + QString::number(r));
            if (r < 0) {
                ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
                return;
            }           
            r = libusb_kernel_driver_active(dev_handle, INTERFACE_NUMBER);
            ui.lw3->addItem("[libusb_kernel_driver_active]" + QString::number(r));
            if (r < 0) {
                ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
                //return;
            }
            if (r == 1) {
                r = libusb_detach_kernel_driver(dev_handle, INTERFACE_NUMBER);
                ui.lw3->addItem("[libusb_detach_kernel_driver]" + QString::number(r));
                if (r < 0) {
                    ui.lw3->addItem("[ERR]" + QString::fromUtf8(libusb_error_name(r)));
                    return;
                }
            }
            r = libusb_claim_interface(dev_handle, INTERFACE_NUMBER);
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
                ui.btnL1->setEnabled(false);
                ui.btnA2->setEnabled(true);
                ui.btnA3->setEnabled(true);
                ui.btnB2->setEnabled(true);
                ui.btnB3->setEnabled(true);
                ui.btnC2->setEnabled(true);
                ui.btnC3->setEnabled(true);
                ui.lw3->addItem("[USB] opened");                
            }
            break;
                    
        }
        else {
            ui.lw3->addItem("[USB] Not target device");
        }
    }    
}

void QtWidgetsApplication2::openImg()
{
    QString fileName, openFilePath;   
    fileName = QFileDialog::getOpenFileName(
        this,
        "选择图片",
        "",
        "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)"
    );
    if (fileName.isEmpty())
    {
        ui.lw3->addItem("[ERR]null image");
        return;
    }    
    /*QImage*/
    QImage img(fileName);
    qtPrintImg(img);
    //qtShowImg(img);
    

    /*OpenCV*/
    Mat m = imread(fileName.toStdString()); 
    cout << strlen((char*)m.data) << endl;
    cout << m.data << endl;
    //cout << m << endl;
    //cvPrintImg(m);
    cvShowImg(m);      

    //writeQtImg(img);
    writeCvImg(m);
}

void QtWidgetsApplication2::qtPrintImg(QImage img)
{    
    ui.tb->append(QString::number(img.byteCount()));
    ui.tb->append(QString::number(img.colorCount()));    
    ui.tb->append(QString::fromUtf8(reinterpret_cast<char*>(img.bits()), 1000));

    //非正常数据
    /*QByteArray byteArray;
    QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
    dataStream << QPixmap::fromImage(img);
    QString str = QString::fromLocal8Bit(byteArray.toBase64());
    ui.tb->append(str);*/
    int r = 0;
    int actualLenth = 0;
    int timeout = 2000;
    /*method1:指针遍历*/
    //uchar* p = img.bits();
    //QString str;
    //for (int i = 0; i < (img.width() * img.height() * 3); i++)
    //{
    //    str.append(QString::number(p[i]));
    //    //writeDataA((unsigned char*)p[i], 4, timeout);    
    //    //ui.tb->append(QString::number(p[i]));
    //}
    //ui.tb->append(str);

    /*method2: QRgb遍历*/
    //QString str;
    //for (int i = 0; i < img.height(); i++)
    //{
    //    QRgb* line =(QRgb*)img.scanLine(i);
    //    //writeDataA((unsigned char*)line, img.bytesPerLine(), timeout);
    //    for (int j = 0; j < img.width(); j++)
    //    {            
    //        //str.append(QString::number(line[j], 16));
    //        //ui.tb->append(QString::number(line[j], 16));
    //    }
    //    ui.tb->append((QString::number)(*line));
    //}
    //ui.tb->append(str);   
}

void QtWidgetsApplication2::qtShowImg(QImage img)
{

}

void QtWidgetsApplication2::cvPrintImg(Mat m)
{       
    ui.tb->append(QString::fromUtf8(reinterpret_cast<char*>(m.data), 1000));

    int r = 0;
    int actualLenth = 0;
    int timeout = 2000;
    /*method1: mat直接转qstring*/
    //ostringstream oss;
    //oss << m;
    //QString str(oss.str().c_str());  
    //ui.tb->append(str);
    //writeDataA((unsigned char*)str.toLatin1().data(), str.length(), timeout);

    /*method2: 指针遍历*/
    /*QString str;
    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {           
            str.append(QString::number(m.ptr<Vec3b>(i)[j][0],16)
                + QString::number(m.ptr<Vec3b>(i)[j][1],16)
                + QString::number(m.ptr<Vec3b>(i)[j][2],16)
            );           
        }
    }
    ui.tb->append(str);
    writeDataA((unsigned char*)str.toLatin1().data(), str.length(), timeout);*/

}

void QtWidgetsApplication2::cvShowImg(Mat m)
{    
    //cvtColor(m, m, CV_BGR2RGB);
    //LUT
    Mat matSingleOut, matThreeOut;
    //Single
    uchar lutReverse[256];
    for (int i = 0; i < 256; i++)
    {
        lutReverse[i] = 255 - i;
    }
    Mat matReverse(1, 256, CV_8U, lutReverse);
    ::LUT(m, matReverse, matSingleOut);
    //Three
    uchar lutFirst[256];
    for (int i = 0; i < 256; i++)
    {
        lutFirst[i] = i / 2;
    }
    Mat matFirst(1, 256, CV_8U, lutFirst);
    uchar lutSecond[256];
    for (int i = 0; i < 256; i++)
    {
        lutSecond[i] = i / 3;
    }
    Mat matsecond(1, 256, CV_8U, lutSecond);
    uchar lutThird[256];
    for (int i = 0; i < 256; i++)
    {
        lutThird[i] = i / 4;
    }
    Mat matThird(1, 256, CV_8U, lutThird);
    vector<Mat> mergeMats;
    mergeMats.push_back(matFirst);
    mergeMats.push_back(matsecond);
    mergeMats.push_back(matThird);
    Mat matThree;
    ::merge(mergeMats, matThree);
    ::LUT(m, matThree, matThreeOut);
    ::imshow("m", m);
    ::imshow("n", matSingleOut);
    ::imshow("p", matThreeOut);
    ::moveWindow("n", 500, 1);
    ::moveWindow("p", 1000, 1);
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
    int timeout = 5000;  

    QString str = ui.pte1->toPlainText();  
    std::string stdStr = str.toStdString();
    std::string hexStr ="";
    const char* cch = stdStr.c_str();

    unsigned char* uch = new unsigned char[str.length()/2];
    unsigned char low, high, highLow;
    for (int i = 0; i < str.length(); i++)
    {
        if ((cch[i] >= '0') && (cch[i] <= '9'))
            high = cch[i] - '0';//16进制中的，字符0-9转化成10进制，还是0-9
        else if ((cch[i] >= 'A') && (cch[i] <= 'F'))
            high = cch[i] - 'A' + 10;//16进制中的A-F，分别对应着11-16
        else if ((cch[i] >= 'a') && (cch[i] <= 'f'))
            high = cch[i] - 'a' + 10;//16进制中的a-f，分别对应也是11-16，不区分大小写
        else
            ;   // 其他返回0x10
        if (++i < str.length())
        {
            if ((cch[i] >= '0') && (cch[i] <= '9'))
                low = cch[i] - '0';//16进制中的，字符0-9转化成10进制，还是0-9
            else if ((cch[i] >= 'A') && (cch[i] <= 'F'))
                low = cch[i] - 'A' + 10;//16进制中的A-F，分别对应着11-16
            else if ((cch[i] >= 'a') && (cch[i] <= 'f'))
                low = cch[i] - 'a' + 10;//16进制中的a-f，分别对应也是11-16，不区分大小写
            else
                ;   // 其他返回0x10 
            highLow = (high << 4) | low;
            uch[i / 2] = highLow;
        }      
        //r = libusb_bulk_transfer(dev_handle, EP1_OUT, &highLow,1, &actualLenth, timeout);
    }
    //r = libusb_bulk_transfer(dev_handle, EP1_OUT, (unsigned char*)str.toLatin1().data(), str.length(), &actualLenth, timeout);
    r = libusb_bulk_transfer(dev_handle, EP1_OUT, (unsigned char*)uch, str.length()/2, &actualLenth, timeout);
    delete[] uch;
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

void QtWidgetsApplication2::writeDataA(unsigned char* data,int lenth,int timeout) {
    ui.lw3->addItem("[writeDataA]" + ui.pte1->toPlainText());
    int r = 0;
    int actualLenth = 0;
    r = libusb_bulk_transfer(dev_handle, EP1_OUT, data, lenth, &actualLenth, timeout);
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

void QtWidgetsApplication2::writeQtImg(QImage img) {
    ui.lw3->addItem("[writeQtImg]" + QString::number(img.width()) + "x" + QString::number(img.height()));
    int r = 0;
    int actualLenth = 0;
    int timeout = 10000;
    r = libusb_bulk_transfer(dev_handle, EP1_OUT, img.bits(), img.width() * img.height() * 3, &actualLenth, 0);
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

void QtWidgetsApplication2::writeCvImg(Mat m) {
    ui.lw3->addItem("[writeCvImg]" + QString::number(m.rows) + "x" + QString::number(m.cols));
    int r = 0;
    int actualLenth = 0;
    int timeout = 10000;
    r = libusb_bulk_transfer(dev_handle, EP1_OUT, m.data, m.rows*m.cols*3, &actualLenth, 0);
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
    ui.lw3->addItem("\r\n btnB1: ep1 write");    
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
    ui.lw3->addItem("\r\n btnC1: ep1 read");    
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
    r = libusb_bulk_transfer(dev_handle, EP3_OUT, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), &actualLenth, 2000);
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
    openImg();    
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnI1() {
    ui.lw3->addItem("\r\n btnI1");
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnJ1() {
    ui.lw3->addItem("\r\n btnJ1 reset");
    int r = 0;
    if (openFlag)
    {
        r = libusb_reset_device(dev_handle);
        if (r < 0)
        {
            ui.lw3->addItem("[ERR]" + (QString)libusb_error_name(r));
            ui.lw3->scrollToBottom();
            return;
        }
        else
        {
            ui.lw3->addItem("[reset]");
            //ui.lw3->addItem("[read]" + QString(QLatin1String((char*)data)));
            ui.lw3->scrollToBottom();
        }
    }
    
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
    ui.lw3->addItem("\r\n btnA2 contorl STD");
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    bool ok;
    int request = 0x00;
    if (!ui.leA1->text().isEmpty())
    {
        request = ui.leA1->text().toInt(&ok, 16);
    }
    int value = 0x0000;
    if (!ui.leA2->text().isEmpty())
    {
        value = ui.leA2->text().toInt(&ok, 16);
    }
    int index = 0x00;
    if (!ui.leA3->text().isEmpty())
    {
        index = ui.leA3->text().toInt(&ok, 16);
    }       
    /*
    dev_handle: 这就是之前libopen函数获得的句柄
    bmRequestType: 请求字段的类型
                   D7:0=HOST2DEVICE,1=DEVICE2HOST
                   D6:0=标准,1=类,2=厂商,3=保留
                   D4~D0:0=设备,1=接口,2=端点,3=其他,4...31=保留
    bRequest: 命令
    wValue: reportType+reportID
    wIndex: 简单点可以理解为字段内容的位置，假设你选择准备发送string类型的话，可以会有多个string，分开发送，那肯定就会出现index从0开始到N结束。
    data 字段内容
    wLength 字段内容长度，记得+1
    timeout 超时设置，以毫秒为单位
    */
    r = libusb_control_transfer(dev_handle, CTRL_STD_OUT, request, value, index, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), 100);
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
    ui.lw3->addItem("\r\n btnB2 control VENDOR");
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    bool ok;
    int request = 0x00;
    if (!ui.leB1->text().isEmpty())
    {
        request = ui.leB1->text().toInt(&ok, 16);
    }
    int value = 0x0000;
    if (!ui.leB2->text().isEmpty())
    {
        value = ui.leB2->text().toInt(&ok, 16);
    }
    int index = 0x00;
    if (!ui.leB3->text().isEmpty())
    {
        index = ui.leB3->text().toInt(&ok, 16);
    }

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
    r = libusb_control_transfer(dev_handle, CTRL_VENDOR_OUT, request, value, index, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), 100);
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

void QtWidgetsApplication2::btnC2() {
    ui.lw3->addItem("\r\n btnC2 control RESERVE");
    ui.lw3->addItem("[write]" + ui.pte1->toPlainText());
    int r = 0;
    bool ok;
    int request = 0x00;
    if (!ui.leC1->text().isEmpty())
    {
        request = ui.leC1->text().toInt(&ok, 16);
    }
    int value = 0x0000;
    if (!ui.leC2->text().isEmpty())
    {
        value = ui.leC2->text().toInt(&ok, 16);
    }
    int index = 0x00;
    if (!ui.leC3->text().isEmpty())
    {
        index = ui.leC3->text().toInt(&ok, 16);
    }
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
    r = libusb_control_transfer(dev_handle, CTRL_RESERVE_OUT, request, (CTRL_RESERVE_OUT << 8) | request, 0, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), ui.pte1->toPlainText().length(), 100);
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
    ui.lw3->addItem("\r\n btnD2");
    int r = 0;
    bool ok;
    int requestType = 0x00;
    if (!ui.leD1->text().isEmpty())
    {
        requestType = ui.leD1->text().toInt(&ok, 16);
    }
    int request = 0x00;
    if (!ui.leD2->text().isEmpty())
    {
        request = ui.leD2->text().toInt(&ok, 16);
    }
    int value = 0x0000;
    if (!ui.leD3->text().isEmpty())
    {
        value = ui.leD3->text().toInt(&ok, 16);
    }
    int index = 0x0000;
    if (!ui.leD4->text().isEmpty())
    {
        index = ui.leD4->text().toInt(&ok, 16);
    }
    int lenth = 0x0000;
    if (!ui.leD5->text().isEmpty())
    {
        lenth = ui.leD5->text().toInt(&ok, 16);
    }
    /*
    dev_handle: 这就是之前libopen函数获得的句柄
    bmRequestType: 请求字段的类型
                   D7:0=HOST2DEVICE,1=DEVICE2HOST
                   D6:0=标准,1=类,2=厂商,3=保留
                   D4~D0:0=设备,1=接口,2=端点,3=其他,4...31=保留
    bRequest: 命令
    wValue: reportType+reportID
    wIndex: 简单点可以理解为字段内容的位置，假设你选择准备发送string类型的话，可以会有多个string，分开发送，那肯定就会出现index从0开始到N结束。
    data 字段内容
    wLength 字段内容长度，记得+1
    timeout 超时设置，以毫秒为单位
    */
    r = libusb_control_transfer(dev_handle, requestType, request, value, index, (unsigned char*)ui.pte1->toPlainText().toLatin1().data(), lenth, 100);
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

void QtWidgetsApplication2::btnE2() {
    ui.lw3->addItem("\r\n btnE2");
    
    ui.lw3->scrollToBottom();
}

void QtWidgetsApplication2::btnF2() {
    ui.lw3->addItem("\r\n btnF2");
    
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
    ui.lw3->addItem("\r\n btnA3 contorl STD");
    int r = 0;
    bool ok;
    int request = 0x00;
    if (!ui.leA1->text().isEmpty())
    {
        request = ui.leA1->text().toInt(&ok, 16);
    }
    int value = 0x0000;
    if (!ui.leA2->text().isEmpty())
    {
        value = ui.leA2->text().toInt(&ok, 16);
    }
    int index = 0x00;
    if (!ui.leA3->text().isEmpty())
    {
        index = ui.leA3->text().toInt(&ok, 16);
    }
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    r = libusb_control_transfer(dev_handle, CTRL_STD_IN, request, value, index, dataReceive, lenth, 100);
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

void QtWidgetsApplication2::btnB3() {
    ui.lw3->addItem("\r\n btnB3 control VENDOR");
    int r = 0;
    bool ok;
    int request = 0x00;
    if (!ui.leB1->text().isEmpty())
    {
        request = ui.leB1->text().toInt(&ok, 16);
    }
    int value = 0x0000;
    if (!ui.leB2->text().isEmpty())
    {
        value = ui.leB2->text().toInt(&ok, 16);
    }
    int index = 0x00;
    if (!ui.leB3->text().isEmpty())
    {
        index = ui.leB3->text().toInt(&ok, 16);
    }
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    r = libusb_control_transfer(dev_handle, CTRL_VENDOR_IN, request, value, index, dataReceive, lenth, 0);
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

void QtWidgetsApplication2::btnC3() {
    ui.lw3->addItem("\r\n btnC3 control RESERVE");
    int r = 0;
    bool ok;
    int request = 0x00;
    if (!ui.leC1->text().isEmpty())
    {
        request = ui.leC1->text().toInt(&ok, 16);
    }
    int value = 0x0000;
    if (!ui.leC2->text().isEmpty())
    {
        value = ui.leC2->text().toInt(&ok, 16);
    }
    int index = 0x00;
    if (!ui.leC3->text().isEmpty())
    {
        index = ui.leC3->text().toInt(&ok, 16);
    }
    int lenth = 0, actualLenth;
    if (!ui.pte2->toPlainText().isEmpty())
    {
        lenth = ui.pte2->toPlainText().toInt();
    }
    r = libusb_control_transfer(dev_handle, CTRL_RESERVE_IN, request, value, index, dataReceive, lenth, 0);
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

void QtWidgetsApplication2::btnD3() {
    ui.lw3->addItem("\r\n btnD3");
    int r = 0;
    bool ok;
    int requestType = 0x00;
    if (!ui.leD1->text().isEmpty())
    {
        requestType = ui.leD1->text().toInt(&ok, 16);
    }
    int request = 0x00;
    if (!ui.leD2->text().isEmpty())
    {
        request = ui.leD2->text().toInt(&ok, 16);
    }
    int value = 0x0000;
    if (!ui.leD3->text().isEmpty())
    {
        value = ui.leD3->text().toInt(&ok, 16);
    }
    int index = 0x00;
    if (!ui.leD4->text().isEmpty())
    {
        index = ui.leD4->text().toInt(&ok, 16);
    }
    int lenth = 0x0000;
    if (!ui.leD5->text().isEmpty())
    {
        lenth = ui.leD5->text().toInt(&ok, 16);
    }
    /*
    dev_handle: 这就是之前libopen函数获得的句柄
    bmRequestType: 请求字段的类型
                   D7:0=HOST2DEVICE,1=DEVICE2HOST
                   D6:0=标准,1=类,2=厂商,3=保留
                   D4~D0:0=设备,1=接口,2=端点,3=其他,4...31=保留
    bRequest: 命令
    wValue: reportType+reportID
    wIndex: 简单点可以理解为字段内容的位置，假设你选择准备发送string类型的话，可以会有多个string，分开发送，那肯定就会出现index从0开始到N结束。
    data: 字段内容
    wLength 字段内容长度，记得+1
    timeout 超时设置，以毫秒为单位
    */
    r = libusb_control_transfer(dev_handle, requestType, request, value, index, dataReceive, lenth, 100);
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
