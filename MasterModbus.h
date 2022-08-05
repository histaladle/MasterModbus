#ifndef MASTERMODBUS_H
#define MASTERMODBUS_H

#include "RingMemory.h"
#define MASTERMODBUS_RINGMEMORY_BUFFERSIZE 32
#define MASTERMODBUS_MAX_TXLEN MASTERMODBUS_RINGMEMORY_BUFFERSIZE
#define MASTERMODBUS_MAX_RXLEN MASTERMODBUS_RINGMEMORY_BUFFERSIZE
class MasterModbus
{
public:
    MasterModbus();
    uint8_t txbytes[MASTERMODBUS_MAX_TXLEN];//已发送的帧
    int txlen;//已发送的帧长度
    uint8_t rxbytes[MASTERMODBUS_MAX_RXLEN];//已接收的帧
    int rxlen;//已接收的帧长度
    bool transmitted;//是否发送
    bool received;//是否接收
    int retransmission;//重发次数
    int waitclock;//超时计时
    int rxfinclock;//接受结束累计时间(3.5个字符)
    RingMemory rxmem;
    void call();
    static uint16_t getcrc(uint8_t *bytes,int len);
};

#endif // MASTERMODBUS_H
