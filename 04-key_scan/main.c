#include <STC15F2K60S2.H>
#include <intrins.h>

//==========   宏定义   ==========

#define Y4  0x80
#define Y5  0xa0
#define Y6  0xc0
#define Y7  0xe0

#define DQ P14 
#define scl P20 
#define sda P21 
#define RST P13 
#define SCK P17 
#define SDA P23
#define TX  P10
#define RX  P11

//==========  全局变量  ==========

unsigned char count;
unsigned char key_down,key_val,key_old;
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
  0xbf //- 
}; 

//==========  比赛文件  ==========


//==========  函数定义  ==========

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

unsigned char key_scan()
{
  unsigned char key_num = 0;
  P44 = 0; P42 = 1; P35 = 1; P34 = 1;
  if(P33==0) key_num = 4;
  if(P32==0) key_num = 5;
  if(P31==0) key_num = 6;
  if(P30==0) key_num = 7;
  P44 = 1; P42 = 0; P35 = 1; P34 = 1;
  if(P33==0) key_num = 8;
  if(P32==0) key_num = 9;
  if(P31==0) key_num = 10;
  if(P30==0) key_num = 11;
  P44 = 1; P42 = 1; P35 = 0; P34 = 1;
  if(P33==0) key_num = 12;
  if(P32==0) key_num = 13;
  if(P31==0) key_num = 14;
  if(P30==0) key_num = 15;
  P44 = 1; P42 = 1; P35 = 1; P34 = 0;
  if(P33==0) key_num = 16;
  if(P32==0) key_num = 17;
  if(P31==0) key_num = 18;
  if(P30==0) key_num = 19;
  return key_num;
}

void display_SEG(unsigned char wei,unsigned char duan,bit dp)
{
  P0 = 0x01<<wei;
  set_hc573(6);
  P0 = Seg_Table[duan];
  P07 &= dp;
  set_hc573(7);
}

void clean_SEG()
{
  P0 = 0xff;
  set_hc573(6);
  set_hc573(7);
}

//==========  中断函数  ==========


//==========    MAIN    ==========

void main() 
{
  while(1)  
  {
    key_val = key_scan();
    key_down = key_val&(key_val^key_old);
    key_old = key_val;
    if(key_down==4) count++;
    if(key_down==8) count--;
    clean_SEG();
    display_SEG(7,count%10,1); delay_ms(1);
    if(count>9) display_SEG(6,count/10%10,1), delay_ms(1);
    if(count>99)display_SEG(5,count/100%10,1), delay_ms(1);
  }
}