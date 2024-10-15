#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QIcon"
#include <QPushButton>
#include <iostream>
#include <thread>
#include <QDir>
#include <QTimer>
#include <QFile>
#include <qfiledialog.h>
#include <QIODevice>
#include <QDataStream>
#include <QTextStream>

Options customOptions("COM1", 115200, DataBits::Eight, StopBits::One, Parity::Even);
uint8_t buf[10] = {0};

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/scanner"));

    serial_ = new serial_port;
    packager_ = new whisper::packager;
    iap_ = new iap;
    time_iap_ = new QTimer();

    init_serial_port();
    connect_signals();
//    ui->send_pushButton->setEnabled(false);
}

void MainWindow::init_serial_port() {
    QStringList serialStrList;
    serialStrList = serial_->scan_serial();
    for (int i = 0; i < serialStrList.size(); i++) {
        ui->serial_port_comboBox->addItem(serialStrList[i]);
    }
}

void MainWindow::connect_signals() {
    connect(time_iap_, &QTimer::timeout, this, &MainWindow::iap_update);

    connect(serial_, &serial_port::serial_rx, this, &MainWindow::serial_rx_unpack);
    connect(ui->open_pushButton, &QPushButton::clicked, this, &MainWindow::open_serial_pushbutton);
    connect(ui->scan_port_pushButton, &QPushButton::clicked, this, [this]() {
        ui->serial_port_comboBox->clear();
        QStringList serialStrList;
        serialStrList = serial_->scan_serial();
        for (int i = 0; i < serialStrList.size(); i++) {
            ui->serial_port_comboBox->addItem(serialStrList[i]);
        }
    });

    connect(ui->load_file_pushButton, &QPushButton::clicked, this, [this]() {
        ui->tx_msg_textBrowser->append("load firmware");
        if (iap_ == nullptr) {
            return;
        }

        QString curPath = QDir::currentPath();
        iap_->fileName = QFileDialog::getOpenFileName(this, tr("加载一个固件"), curPath, tr("固件(*.bin)"));

        if (iap_->fileName.isEmpty()) {
            ui->tx_msg_textBrowser->append("firmware empty");
            return;
        }

        ui->firm_path_lineEdit->setText(iap_->fileName);
        ui->send_pushButton->setEnabled(true);

        if (!QFile::exists(iap_->fileName)) {
            ui->tx_msg_textBrowser->append("firmware inexistence");
            return;
        }

        load_file();
    });

    connect(serial_, &serial_port::update_log, this, [this](const QString &str) {
        ui->tx_msg_textBrowser->append(str);
    });
    connect(ui->send_pushButton, &QPushButton::clicked, [this]() {
        if (ui->send_pushButton->text() == "下载") {
            ui->send_pushButton->setText(tr("取消"));
            iap_->iap_state_ = 1;
            iap_->ResponseData.clear();
            time_iap_->setInterval(500);
            time_iap_->start();
        } else {
            ui->send_pushButton->setText(tr("下载"));
            time_iap_->stop();
            iap_->iap_state_ = 0;
        }
    });
}

MainWindow::~MainWindow() {
    serial_->stop();
    delete packager_;
    delete ui;
}

void MainWindow::serial_rx_unpack(const QByteArray &data) {
    size_t size = data.size();
    packager_->unpack(data.data(), size);
    channel_ = packager_->get_channel();
    length_ = packager_->get_length();
    payload = packager_->get_payload_();
}

void MainWindow::open_serial_pushbutton() {
    if (ui->open_pushButton->text() == tr("打开")) {
        customOptions.port = ui->serial_port_comboBox->currentText().toStdString();
        customOptions.baudrate = ui->baud_rate_comboBox->currentText().toInt();
        if (!serial_->open(customOptions)) {
            ui->serial_port_comboBox->setEnabled(false);
            ui->baud_rate_comboBox->setEnabled(false);
            ui->open_pushButton->setText(tr("关闭"));
            ui->scan_port_pushButton->setEnabled(false);
        }
    } else {
        serial_->stop();
//        time_iap_->stop();
        ui->serial_port_comboBox->setEnabled(true);
        ui->baud_rate_comboBox->setEnabled(true);
        ui->open_pushButton->setText(tr("打开"));
        ui->scan_port_pushButton->setEnabled(true);
    }
}


void MainWindow::load_file() {
    QFile file;
    file.setFileName(iap_->fileName);
    file.open(QIODevice::ReadOnly);
    iap_->databuf.resize(file.size());
    iap_->databuf = file.readAll();

    iap_->packet_zheng = iap_->databuf.length() / iap_->package_size;
    iap_->packet_yu = iap_->databuf.length() % iap_->package_size;
    iap_->package_num = iap_->packet_zheng + (iap_->packet_yu == 0 ? 0 : 1);
    QString str = QString("固件长度：%1\r\n包大小：%2\r\n包总数：%3").arg(file.size()).arg(iap_->package_size).arg(
            iap_->package_num);
    ui->tx_msg_textBrowser->append(str);
}

enum {
    write_success = 0x01,
    check_error = 0x02,
    write_failed = 0x03
};

void MainWindow::iap_update() {
    time_iap_->stop();
    int flag = 0;
    int idx = 0;
    qDebug()<<"payload.size() "<<payload.size();
    if (payload.size() >= 3) {
        flag = payload.at(0);
        idx = (payload.at(2) << 8 | payload.at(1));
        qDebug() << iap_->channel_ << flag << idx;
        payload.clear();
    } else {
        if (iap_->now_index == 0)serial_->send(iap_->send_ask_cmd());
        else {
            if ((idx) == iap_->packet_zheng && iap_->packet_zheng != iap_->package_num) {
                serial_->send(iap_->send_data_yu());
                ui->tx_msg_textBrowser->append(
                        tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
                iap_->ResponseData.clear();
            } else {
                serial_->send(iap_->send_data_zheng(iap_->now_index));
                ui->tx_msg_textBrowser->append(
                        tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
                iap_->ResponseData.clear();
            }
        }
        time_iap_->start();
        return;
    }

    if (iap_->iap_state_ == 1) {
        if (channel_ == iap_->channel_ && flag == write_success && (idx == iap_->package_num)) {
            iap_->iap_state_ = 2;
            iap_->now_index = 1;
            iap_->iap_cmd_count = 0;
            ui->tx_msg_textBrowser->clear();
            ui->tx_msg_textBrowser->append(tr("收到响应，进入刷固件流程！"));
            if (iap_->package_num > 1) {
                serial_->send(iap_->send_data_zheng(iap_->now_index));
                ui->tx_msg_textBrowser->append(tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
            } else {
                serial_->send(iap_->send_data_yu());
                ui->tx_msg_textBrowser->append(tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
            }
        } else {
            serial_->send(iap_->send_ask_cmd());
            if (iap_->ask_cmd_count) {
                iap_->ask_cmd_count--;
            } else {
                iap_->ask_cmd_count = 10;
                serial_->stop();
                serial_->open(customOptions);
            }
            ui->tx_msg_textBrowser->append(tr("请求更新固件！"));
        }
    } else if (iap_->iap_state_ == 2) {
        iap_->iap_cmd_count++;
        switch (flag) {
            case write_success: {
                if (idx == iap_->now_index) {
                    if (idx == iap_->package_num) {
                        ui->tx_msg_textBrowser->append(tr("Flash写入成功！"));
                        ui->tx_msg_textBrowser->append(tr("更新完成！"));
                        iap_->iap_state_ = 3;
                    } else {
                        //整包已经发完
                        if ((idx) == iap_->packet_zheng) {
                            iap_->now_index++;
                            serial_->send(iap_->send_data_yu());
                            ui->tx_msg_textBrowser->append(
                                    tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
                            iap_->ResponseData.clear();
                        } else {
                            iap_->now_index++;
                            serial_->send(iap_->send_data_zheng(iap_->now_index));
                            ui->tx_msg_textBrowser->append(
                                    tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
                            iap_->ResponseData.clear();
                        }
                    }
                } else {
                    iap_->iap_state_ == 1;
                    //包索引对不上，重新发送
                }
                break;
            }
            case check_error: {
                if ((idx) == iap_->packet_zheng && iap_->packet_zheng != iap_->package_num) {
                    serial_->send(iap_->send_data_yu());
                    ui->tx_msg_textBrowser->append(
                            tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
                    iap_->ResponseData.clear();
                } else {
                    serial_->send(iap_->send_data_zheng(iap_->now_index));
                    ui->tx_msg_textBrowser->append(
                            tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
                    iap_->ResponseData.clear();
                }
                break;
            }
            case write_failed: {
                if ((idx) == iap_->packet_zheng && iap_->packet_zheng != iap_->package_num) {
                    serial_->send(iap_->send_data_yu());
                    ui->tx_msg_textBrowser->append(
                            tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
                    iap_->ResponseData.clear();
                } else {
                    serial_->send(iap_->send_data_zheng(iap_->now_index));
                    ui->tx_msg_textBrowser->append(
                            tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
                    iap_->ResponseData.clear();
                }
                break;
            }
            default:
                break;
        }
    }
    if(iap_->iap_state_ == 1||iap_->iap_state_ == 2)
    {
        time_iap_->start();
    }
    else
    if(iap_->iap_state_ == 0 ||iap_->iap_state_ == 3)
    {
        ui->send_pushButton->setText(tr("下载"));
    }
}
//    if(iap_->iapState == 1)
//    {
//
//        if (static_cast<int>(iap_->ResponseData[2]) == 0x09)
//        {
//            //清除接收缓存
//            iap_->ResponseData.clear();
//            //进入刷固件流程
//            iap_->iapState = 2;
//            iap_->now_index = 1;
//            iap_->timeCount = 0;//清空超时计数
//            ui->tx_msg_textBrowser->append(tr("收到响应，进入刷固件流程！"));
//            if(iap_->package_num>1)
//            {
//                QByteArray data = iap_->send_data_zheng(iap_->now_index);
//                serial_->sendData(data);
//                ui->tx_msg_textBrowser->append(tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
//            }
//            else
//            {
//                QByteArray data = iap_->send_data_yu();
//                serial_->sendData(data);
//                ui->tx_msg_textBrowser->append(tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
//            }
//        }
//        else
//        {
//            //发送请求刷固帧
//            QByteArray data = iap_->send_Ask();
//            serial_->sendData(data);
//            if(recnt)
//            {
//                recnt--;
//
//                serial_->open(ui->portComboBox->currentText(), 921600);
//            }
//            else {
//                recnt=10;
//
//                serial_->open(ui->portComboBox->currentText(), 115200);
//            }
//            ui->tx_msg_textBrowser->append(tr("请求更新固件！"));
//        }
//    }
//    else if(iap_->iapState == 2)        //正在刷固件
//    {
//        iap_->timeCount++;
//        //收到响应帧
//        if (static_cast<int>(iap_->ResponseData[6]) == 0x01)
//        {
//            int idx = (iap_->ResponseData[8] << 8) | iap_->ResponseData[7];
//
//            //包索引是否一致
//            if (iap_->now_index == idx)
//            {
//                //判断结果
//                switch (iap_->ResponseData[6])
//                {
//                    case 0x01:
//                        //正常
//                        if ((idx) == iap_->package_num)
//                        {
//                            //升级完成
//                            ui->tx_msg_textBrowser->append(tr("Flash写入成功！"));
//                            ui->tx_msg_textBrowser->append(tr("更新完成！"));
//                            ui->lcdNumber->display(0);
//                            iap_->iapState = 3;
//                        }
//                        else
//                            //整包已经发完
//                        if ((idx) == iap_->packet_zheng)
//                        {
//                            //开始发最后一包
//                            qDebug()<<"======开始发最后一包======";
//                            iap_->now_index ++;
//                            QByteArray data = iap_->send_data_yu();
//                            serial_->sendData(data);
//                            ui->tx_msg_textBrowser->append(tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
//                            iap_->ResponseData.clear();
//                        }
//                        else
//                        {
//                            //继续下一包发送
//                            iap_->now_index ++;
//                            qDebug()<<"======继续下一包发送======";
//                            QByteArray data = iap_->send_data_zheng(iap_->now_index);
//                            serial_->sendData(data);
//                            ui->tx_msg_textBrowser->append(tr("已发送第%1包，共%2包").arg(iap_->now_index).arg(iap_->package_num));
//                            iap_->ResponseData.clear();
//                        }
//                        break;
//                    case 0x02:
//                    case 0x03:
//                        //数据错误，重新发送
//                        ui->tx_msg_textBrowser->append(tr("数据错误，重新发送!"));
//                        QByteArray data = iap_->send_data_zheng(iap_->now_index);
//                        serial_->sendData(data);
//                        iap_->timeCount = 0;
//                        break;
//                }
//            }
//        }
//        else
//            //没有收到响应帧
//        {
//            //判断是否超时
//            if (iap_->timeCount > iap_->timeout)
//            {
//                //等待超时，重发
//                ui->tx_msg_textBrowser->append(tr("等待超时，重发!"));
//                QByteArray data ;
//                //如果是余包
//                if((iap_->now_index)==iap_->package_num && iap_->packet_zheng!=iap_->package_num)
//                {
//                    data = iap_->send_data_yu();
//                }
//                else
//                    data = iap_->send_data_zheng(iap_->now_index);
//                serial_->sendData(data);
//                iap_->timeCount = 0;
//            }
//        }
//    }
//
//    //通信计数
//    int val= ui->lcdNumber->value();
//    ui->lcdNumber->display(val+1);
//    if(iap_->iapState == 1||iap_->iapState == 2)
//    {
//        //开始计时
//        time_iap_->start();
//    }
//    else
//    if(iap_->iapState == 0 ||iap_->iapState == 3)
//    {
//        ui->iapDownLoadButton->setText(tr("下载"));
//    }
//}