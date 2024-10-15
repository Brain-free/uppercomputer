#include "iap.h"
#include "../serial/serial_port.h"
iap::iap()
{

}
uint16_t crc_16(const uint8_t *data, size_t length) {
    uint16_t crc = 0;
    while (length--) {
        crc ^= *data++;

        for (int i = 0; i < 8; ++i) {
            crc = (crc & 0x0001) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
        }
    }
    return crc;
}



QByteArray iap::send_data_zheng(int index){

    std::vector<uint8_t> buf(2 + package_size);
    buf[0] = index;
    buf[1] = index >> 8;
    //负载
    for (int i = 0; i < package_size; i++)
    {
        buf[2 + i] = databuf[(index-1) * package_size+i];
    }

    whisper::payload pl(buf.data(), package_size+2);

    whisper::packager pak;
    std::vector<uint8_t> data_vector = pak.pack(channel_, flg_, pl);

    if (!data_vector.empty()) {
        QByteArray data(reinterpret_cast<const char*>(data_vector.data()), data_vector.size());
        return data;
    } else {
        qWarning() << "data_vector is empty!";
    }

}

//余包
QByteArray iap::send_data_yu(){
    
    std::vector<uint8_t> buf(2 + packet_yu);
    buf[0] = package_num;
    buf[1] = package_num >> 8;

    for (int i = 0; i < packet_yu; i++)
    {
        buf[2 + i] = databuf[packet_zheng * package_size+i];
    }
    whisper::payload pl(buf.data(), packet_yu+2);

    whisper::packager pak;
    std::vector<uint8_t> data_vector = pak.pack(channel_, flg_, pl);

    if (!data_vector.empty()) {
        QByteArray data(reinterpret_cast<const char*>(data_vector.data()), data_vector.size());
        return data;
    } else {
        qWarning() << "data_vector is empty!";
    }
}

//请求帧
QByteArray iap::send_ask_cmd()
{
    std::vector<uint8_t> buf(2);
    buf[0] = package_num;
    buf[1] = package_num >> 8;

    whisper::payload pl(buf.data(),2);

    whisper::packager pak;
    std::vector<uint8_t> data_vector = pak.pack(channel_, flg_, pl);

    if (!data_vector.empty()) {
        QByteArray data(reinterpret_cast<const char*>(data_vector.data()), data_vector.size());
        return data;
    } else {
        qWarning() << "data_vector is empty!";
    }

}

