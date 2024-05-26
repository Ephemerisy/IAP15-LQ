#include <STC15F2K60S2.H>
#include <intrins.h>

//=========================== 宏 定 义 ===============================

#define UP_RATE 10

#define Y4  0x80
#define Y5  0xa0
#define Y6  0xc0
#define Y7  0xe0

#define DQ  P14
#define scl P20
#define sda P21
#define RST P13
#define SCK P17
#define SDA P23

//========================== 全 局 变 量 =============================

unsigned int temperature;
unsigned char code Seg_Table[] = 
{
  0xc0, //0
  0xf9, //1
  0xa4, //2
  0xb0, //3
  0x99, //4
  0x92, //5
  0x82, //6
  0xf8, //7
  0x80, //8
  0x90, //9
  0x88, //A
  0x83, //b
  0xc6, //C
  0xa1, //d
  0x86, //E
  0x8e, //F
  0xbf//-
};

//========================== 比 赛 文 件 =============================

/* # 单总线代码片段说明
1. 本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
2. 参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速
度和试题
中对单片机时钟频率的要求，进行代码调试和修改。
*/
//
void delay_OneWire(unsigned int t) 
{
  unsigned char i;
  while(t--){
    for(i=0;i<12;i++);
  }
}
//
void Write_DS18B20(unsigned char dat)
{
  unsigned char i;
  EA = 0;
  for(i=0;i<8;i++)
  {
    DQ = 0;
    DQ = dat&0x01;
    delay_OneWire(5);
    DQ = 1;
    dat >>= 1;
  }
  delay_OneWire(5);
  EA = 1;
}
//
unsigned char Read_DS18B20(void)
{
  unsigned char i;
  unsigned char dat;
   
  EA = 0;
  for(i=0;i<8;i++)
  {
    DQ = 0;
    dat >>= 1;
    DQ = 1;
    if(DQ)
    {
      dat |= 0x80;
    } 
    delay_OneWire(5);
  }
  EA = 1;
  return dat;
}
//
bit init_ds18b20(void)
{
 bit initflag = 0;
 
 DQ = 1;
 delay_OneWire(12);
 DQ = 0;
 delay_OneWire(80);
 DQ = 1;
 delay_OneWire(10); 
 initflag = DQ; 
 delay_OneWire(5);
 
 return initflag;
}
/* # I2C 代码片段说明
1. 本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
2. 参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速
度和试题
中对单片机时钟频率的要求，进行代码调试和修改。
*/
#define DELAY_TIME 10
//
static void I2C_Delay(unsigned char n)
{
 do
 {
   _nop_();_nop_();_nop_();_nop_();_nop_();
   _nop_();_nop_();_nop_();_nop_();_nop_();
   _nop_();_nop_();_nop_();_nop_();_nop_();
 }
 while(n--); 
}
//
void I2CStart(void)
{
  sda = 1;
  scl = 1;
  I2C_Delay(DELAY_TIME);
  sda = 0;
  I2C_Delay(DELAY_TIME);
  scl = 0; 
}
//
void I2CStop(void)
{
  sda = 0;
  scl = 1;
  I2C_Delay(DELAY_TIME);
  sda = 1;
  I2C_Delay(DELAY_TIME);
}
//
void I2CSendByte(unsigned char byt)
{
  unsigned char i;
  for(i=0; i<8; i++){
    scl = 0;
    I2C_Delay(DELAY_TIME);
    if(byt & 0x80){
      sda = 1;
    }
    else{
      sda = 0;
    }
    I2C_Delay(DELAY_TIME);
    scl = 1;
    byt <<= 1;
    I2C_Delay(DELAY_TIME);
  }
  scl = 0; 
}
//
unsigned char I2CReceiveByte(void)
{
  unsigned char da;
  unsigned char i;
  for(i=0;i<8;i++){ 
    scl = 1;
    I2C_Delay(DELAY_TIME);
    da <<= 1;
    if(sda) 
    da |= 0x01;
    scl = 0;
    I2C_Delay(DELAY_TIME);
  }
  return da; 
}
//
unsigned char I2CWaitAck(void)
{
  unsigned char ackbit;
  scl = 1;
  I2C_Delay(DELAY_TIME);
  ackbit = sda; 
  scl = 0;
  I2C_Delay(DELAY_TIME);
  return ackbit;
}
//
void I2CSendAck(unsigned char ackbit)
{
  scl = 0;
  sda = ackbit; 
  I2C_Delay(DELAY_TIME);
  scl = 1;
  I2C_Delay(DELAY_TIME);
  scl = 0; 
  sda = 1;
  I2C_Delay(DELAY_TIME);
}
/* # DS1302 代码片段说明
1. 本文件夹中提供的驱动代码供参赛选手完成程序设计参考。
2. 参赛选手可以自行编写相关代码或以该代码为基础，根据所选单片机类型、运行速
度和试题
中对单片机时钟频率的要求，进行代码调试和修改。
*/
//
void Write_Ds1302(unsigned char temp) 
{
  unsigned char i;
  for (i=0;i<8;i++) 
  { 
    SCK = 0;
    SDA = temp&0x01;
    temp>>=1; 
    SCK=1;
  }
} 
//
void Write_Ds1302_Byte( unsigned char address,unsigned char dat ) 
{
  RST=0; _nop_();
  SCK=0; _nop_();
  RST=1; _nop_(); 
  Write_Ds1302(address);
  Write_Ds1302(dat);
  RST=0; 
}
//
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
  unsigned char i,temp=0x00;
  RST=0; _nop_();
  SCK=0; _nop_();
  RST=1; _nop_();
  Write_Ds1302(address);
  for (i=0;i<8;i++) 
  {
    SCK=0;
    temp>>=1;
    if(SDA)
    temp|=0x80;
    SCK=1;
  } 
  RST=0; _nop_();
  SCK=0; _nop_();
  SCK=1; _nop_();
  SDA=0; _nop_();
  SDA=1; _nop_();
  return (temp);
}

//========================== 函 数 定 义 =============================

void delay_ms(unsigned int ms)
{
  unsigned char data i,j;
  while(ms--)
  {
    i = 12;
    j = 169;
    do
    {
      while (--j);
    } while (--i);
  }
}

void set_hc573(unsigned char pipe)
{
  P2 &= 0x1f;
  switch(pipe)
  {
    case 4: P2 |= Y4; break;
    case 5: P2 |= Y5; break;
    case 6: P2 |= Y6; break;
    case 7: P2 |= Y7; break;
  }
  P2 &= 0x1f;
}

void cleanSMGs(void)
{
  P0 = 0xff;
  set_hc573(6);
  set_hc573(7);
}

void display_SEG(unsigned char wei,unsigned char duan,bit dp)
{
  P0 = 0x01<<wei;
  set_hc573(6);
  P0 = Seg_Table[duan];
  P07 &= dp;    // 若小数点符号位为0则在原数字上加上小数点
  set_hc573(7);
}

void read_temperature(void)
{
  unsigned char lsb,msb;
  
  init_ds18b20();
  Write_DS18B20(0xcc);
  Write_DS18B20(0x44);
  init_ds18b20();
  Write_DS18B20(0xcc);
  Write_DS18B20(0xbe);
  lsb = Read_DS18B20();
  msb = Read_DS18B20();
  temperature = msb;
  temperature = (temperature<<8)|lsb;
  
  temperature >>= 4;
  temperature *= UP_RATE;
  temperature = temperature+(lsb&0x0f)*0.0625*UP_RATE;
}

//========================== 函 数 中 断 =============================


//============================== MAIN ================================

void main()
{
  init_ds18b20();
  Write_DS18B20(0xcc);
  Write_DS18B20(0x44);
  delay_ms(700);
  while(1)
  {
    read_temperature();
    display_SEG(5,temperature/100%10,1);
    delay_ms(2);
    display_SEG(6,temperature/10%10,0);
    delay_ms(2);
    display_SEG(7,temperature%10,1);
  }
}