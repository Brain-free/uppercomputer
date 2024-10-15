#ifndef QTUPGRADETOOLS_MAINWINDOW_H
#define QTUPGRADETOOLS_MAINWINDOW_H

#include <QWidget>
#include "serial/serial_port.h"
#include "packager/packager.h"
#include "iap/iap.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

public slots:

    void connect_signals();

    void init_serial_port();

    void serial_rx_unpack(const QByteArray &data);

    void open_serial_pushbutton();


    void load_file();

    void iap_update();
private:
    iap *iap_;
    serial_port *serial_;
    whisper::packager *packager_;
    QTimer *time_iap_;

private:
    uint8_t  channel_{};
    uint8_t  flags_{};
    uint16_t length_ = 0;
    std::vector<uint8_t> payload;
private:
    Ui::MainWindow *ui;
};


#endif //QTUPGRADETOOLS_MAINWINDOW_H
