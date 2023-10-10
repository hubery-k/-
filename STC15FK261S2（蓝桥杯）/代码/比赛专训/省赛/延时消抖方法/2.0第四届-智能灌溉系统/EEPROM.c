#include "EEPROM.h"
#include "iic.h"

/*AT24C02设备地址有一个字节组成。前四位是固定值为1010。后三位是可编程部分，由于在设计图中全部接地了所以全部为0。最后一位是读写位，0写1读*/
#define AT24C02_ADDRESS 0XA0


void AT24C02_WriteByte(unsigned char Word_Addr,unsigned char Data)
{
	IIC_Start(); 
	IIC_SendByte(AT24C02_ADDRESS);//发送从机设备地址+写入数据命令
	IIC_WaitAck();
	IIC_SendByte(Word_Addr);//发送数据要存入的地址
	IIC_WaitAck();
	IIC_SendByte(Data);
	IIC_WaitAck();
	IIC_Stop();	
}

unsigned char AT24C02_Read(unsigned char Word_Addr)
{
	unsigned char Data;
	IIC_Start(); 
	IIC_SendByte(AT24C02_ADDRESS);
	IIC_WaitAck();
	IIC_SendByte(Word_Addr);//（定位到）存取数据的地址。（写入的地址会存入地址指针中，下个时序的数据读取就会从该地址开始）
	IIC_WaitAck();
	
	IIC_Start(); //另起一个时序重新指定读写方向
	IIC_SendByte(AT24C02_ADDRESS|0X01);//发送从机设备地址+读出数据命令
	IIC_WaitAck();
	Data=IIC_RecByte(); //读取数据
	IIC_SendAck(1);//发送非应答表示结束
	IIC_Stop();
	return Data;
}