#include "serial_port.h"
#include "QDebug"

uint8_t serial_port::open(const Options &opts) {

    if (!serial_) {
        emit update_log("Serial port not initialized.");
        return -1;
    }
    serial_->setPortName(QString::fromStdString(opts.port));
    serial_->setBaudRate(opts.baudrate);
    serial_->setDataBits(static_cast<QSerialPort::DataBits>(opts.databits));
    serial_->setStopBits(static_cast<QSerialPort::StopBits>(opts.stopbits));
    serial_->setParity(static_cast<QSerialPort::Parity>(opts.parity));

    if (!serial_->open(QIODevice::ReadWrite)) {
        QString str = QString("Failed to open serial port:%1").arg(serial_->errorString());
        emit update_log(str);
        serial_ = nullptr;
        return -1;
    } else {
        ready_ = true;
        running_ = true;
        rx_thread_ = std::thread(&serial_port::readLoop, this);
        return 0;
    }
}

void serial_port::stop() {
    running_ = false;
    if (rx_thread_.joinable()) {
        rx_thread_.join();
    }
    if (serial_->isOpen()) {
        serial_->close();
    }
}

void serial_port::readLoop() {
    while (running_) {
        QByteArray data = serial_->readAll();
        if (!data.isEmpty()) {
            emit serial_rx(data);
        } else {
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool serial_port::send(const QByteArray &data) {
    if (!serial_ || !serial_->isOpen()) {
        qWarning() << "Serial port is not open.";
        return false;
    }
    qint64 bytesWritten = serial_->write(data);
    if (bytesWritten == -1) {
        QString str = QString("Failed to write to serial port:%1").arg(serial_->errorString());
        emit update_log(str);
        return false;
    } else if (bytesWritten != data.size()) {
        emit update_log("Not all data was written to the serial port.");
        return false;
    }
    return true;
}

QStringList serial_port::scan_serial() {
    QStringList serialStrList;
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
            foreach (const QSerialPortInfo &info, availablePorts) {
            serial_->setPort(info);
            if (serial_->isOpen()) {
                serialStrList.append(serial_->portName());
            } else {
                if (serial_->open(QIODevice::ReadWrite)) {
                    serialStrList.append(serial_->portName());
                    serial_->close();
                } else {
                    QString str = QString("Failed to open port：%1  Error:：%2").arg(info.portName()).arg(
                            serial_->errorString());
                    emit update_log(str);
                }
            }
        }
    return serialStrList;
}