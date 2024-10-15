#ifndef QTUPGRADETOOLS_IAP_H
#define QTUPGRADETOOLS_IAP_H
#include <QtGlobal>
#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include "../packager/packager.h"

class iap {
public:
    iap() ;


    QByteArray send_data_zheng(int index);

    QByteArray send_data_yu();

    QByteArray send_ask_cmd();

    uint8_t channel_=0x5A;
    uint8_t flg_=0x00;
    //刷固件状态
    //0.无效，1.正在请求刷固，2.正在刷固,3.完成
    int iap_state_ =0;


    QString fileName = "";

    bool iap_resault;
    int package_size = 2048;
    int package_num = 0;//总包数
    int packet_zheng = 0;//整包数
    int packet_yu = 0;//不足1包的剩余字节数
    int now_index = 0;//正在发送包索引

    QByteArray ResponseData;//请求响应缓冲区
    QByteArray log_msg;
    QByteArray databuf;
    int start_repeat = 300;//握手信号重发次数
    int timeout = 5000 / 50;//刷固件超时时间 单位： 50ms【对应定时器周期】
    int iap_cmd_count = 0;//计时变量
    int ask_cmd_count=10;
};
#endif //QTUPGRADETOOLS_IAP_H
