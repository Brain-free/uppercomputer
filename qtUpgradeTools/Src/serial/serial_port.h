#ifndef QTUPGRADETOOLS_serial_port_H
#define QTUPGRADETOOLS_serial_port_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <functional> // 引入函数对象支持
#include <memory>     // 引入智能指针支持
#include <string>     // 引入字符串支持
#include <thread>     // 引入线程支持
#include <vector>     // 引入动态数组支持
#include <cstdint>

enum class DataBits : uint8_t {
    Five = 5,
    Six,
    Seven,
    Eight
};

enum class StopBits : uint8_t {
    One = 1,
    OnePointFive,
    Two
};

enum class Parity : uint8_t {
    None,
    Odd,
    Even,
    Mark,
    Space
};

struct Options {
    std::string port;
    uint32_t baudrate;
    DataBits databits;
    StopBits stopbits;
    Parity parity;

    Options(std::string p = "/dev/ttyUSB0",
            uint32_t br = 115200,
            DataBits db = DataBits::Eight,
            StopBits sb = StopBits::One,
            Parity pa = Parity::None)
            : port(std::move(p)), baudrate(br), databits(db), stopbits(sb), parity(pa) {}
};

class serial_port : public QObject {
Q_OBJECT
public:
    serial_port()
            : serial_(std::make_unique<QSerialPort>()) {
        ready_ = false;
        running_ = false;
    }


    serial_port(const serial_port &) = delete;

    serial_port &operator=(const serial_port &) = delete;


    ~serial_port() {
        if (running_) {
            stop();
        }
    }

    uint8_t open(const Options &opts);

    void stop();

    void readLoop();
    bool send(const QByteArray &data);
    QStringList scan_serial();
signals:
    void serial_rx(const QByteArray &data);
    void update_log(QString message);

private:
    QByteArray dataBuffer;
    std::atomic<bool> ready_{};
    std::atomic<bool> running_{};
    std::thread rx_thread_{};
    std::unique_ptr<QSerialPort> serial_;
};


#endif //QTUPGRADETOOLS_serial_port_H
