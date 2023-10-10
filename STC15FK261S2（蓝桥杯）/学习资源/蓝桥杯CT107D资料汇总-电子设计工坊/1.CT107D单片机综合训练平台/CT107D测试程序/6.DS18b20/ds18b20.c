#include "ds18b20.h"

//延时函数
void delay(unsigned int t)
{
  while(t--);
}

//DS18B20初始化函数
bit init_ds18b20(void)
{
  bit initflag = 0;
  DQ = 1;
  delay(12);
  DQ = 0;
  delay(80); // 延时大于480us
  DQ = 1;
  delay(10);  // 14
  initflag = DQ;     // initflag等于1初始化失败
  delay(5);
  
  return initflag;
}

//通过单总线向从器件写一个字节
void wr_ds18b20(unsigned char byt)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
    DQ = 0;
    DQ = byt&0x01;
    delay(5);
    DQ = 1;
    byt >>= 1;
  }
  delay(5);
}

//通过单总线从从器件读一个字节
unsigned char rd_ds18b20(void)
{
  unsigned char i;
  unsigned char byt;
  
  for(i=0;i<8;i++)
  {
    DQ = 0;
    byt >>= 1;
    DQ = 1;
    if(DQ)
    {
      byt |= 0x80;
    }    
    delay(5);
  }
  return byt;
}

//温度转换、读取及数据处理器函数
unsigned char rd_temperature(void)
{
  unsigned char low,high;
  char temp;
  
  init_ds18b20();
  wr_ds18b20(0xCC);
  wr_ds18b20(0x44); //启动温度转换
  delay(200);

  init_ds18b20();
  wr_ds18b20(0xCC);
  wr_ds18b20(0xBE); //读取寄存器
  
  low = rd_ds18b20(); //低字节
  high = rd_ds18b20(); //高字节
  
  temp = high<<4;
  temp |= (low>>4);
  
  return temp;
}


