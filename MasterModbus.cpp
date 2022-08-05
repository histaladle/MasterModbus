#include "MasterModbus.h"
#include "cstring"
//01 03 00 00 00 01 84 0a
MasterModbus::MasterModbus() :
    txlen(0),
    rxlen(0),
    transmitted(false),
    received(false),
    retransmission(0),
    waitclock(0),
    rxfinclock(0),
    rxmem(MASTERMODBUS_RINGMEMORY_BUFFERSIZE)
{

}

void MasterModbus::call()
{
    bool ok=false;
    uint16_t crc=0;
    unsigned char crch=0,crcl=0;
    int startindex=0,endindex=0;
    //int rqindex=0,rpindex=0;
    uint8_t bytes[MASTERMODBUS_RINGMEMORY_BUFFERSIZE];
    rxmem.copy(bytes,rxmem.readableCount(),0);
    for(startindex=0; startindex<rxmem.readableCount()-2; startindex++) {
        for(endindex=startindex+2; endindex<rxmem.readableCount(); endindex++) {
            crc=getcrc(bytes+startindex,endindex-startindex+1-2);
            crch=(crc>>8)&0xff;
            crcl=crc&0xff;
            if(crch==bytes[endindex-1] && crcl==bytes[endindex]) {
                if(endindex-startindex+1<4) continue;
                if(bytes[startindex]!=txbytes[0]) continue;
                if(bytes[1]&0x80)
                {
                    ok=((bytes[1]&(~0x80)&0xff)==txbytes[1]);
                    if(!ok) continue;
                    memcpy(rxbytes,bytes+startindex,endindex-startindex+1);
                    rxlen=endindex-startindex+1;
                    received=1;
                }
                else if(bytes[1]==0x03)
                {
                    ok=(bytes[1]==txbytes[1]);
                    if(!ok) continue;
                    memcpy(rxbytes,bytes+startindex,endindex-startindex+1);
                    rxlen=endindex-startindex+1;
                    received=1;
                }
                else if(bytes[1]==0x06)
                {
                    ok=(bytes[1]==txbytes[1]);
                    if(!ok) continue;
                    memcpy(rxbytes,bytes+startindex,endindex-startindex+1);
                    rxlen=endindex-startindex+1;
                    received=1;
                }
            }
        }
    }
}

uint16_t MasterModbus::getcrc(uint8_t *bytes, int len)
{
    uint16_t res = 0xffff;
    while(len --)
    {
        res ^= *bytes ++;
        for(int i = 0; i < 8; i ++)
        {
            if(res & 0x01)
            {
                res = (res >> 1) ^ 0xa001;
            }
            else
            {
                res = res >> 1;
            }
        }
    }
    res = ((res & 0xff) << 8) | (res >> 8);
    return res;
}
