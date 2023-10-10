#include "ds18b20.h"

//��ʱ����
void delay(unsigned int t)
{
  while(t--);
}

//DS18B20��ʼ������
bit init_ds18b20(void)
{
  bit initflag = 0;
  DQ = 1;
  delay(12);
  DQ = 0;
  delay(80); // ��ʱ����480us
  DQ = 1;
  delay(10);  // 14
  initflag = DQ;     // initflag����1��ʼ��ʧ��
  delay(5);
  
  return initflag;
}

//ͨ���������������дһ���ֽ�
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

//ͨ�������ߴӴ�������һ���ֽ�
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

//�¶�ת������ȡ�����ݴ���������
unsigned char rd_temperature(void)
{
  unsigned char low,high;
  char temp;
  
  init_ds18b20();
  wr_ds18b20(0xCC);
  wr_ds18b20(0x44); //�����¶�ת��
  delay(200);

  init_ds18b20();
  wr_ds18b20(0xCC);
  wr_ds18b20(0xBE); //��ȡ�Ĵ���
  
  low = rd_ds18b20(); //���ֽ�
  high = rd_ds18b20(); //���ֽ�
  
  temp = high<<4;
  temp |= (low>>4);
  
  return temp;
}


