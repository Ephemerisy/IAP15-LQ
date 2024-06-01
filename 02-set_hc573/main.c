#include <STC15F2K60S2.H>
#include <intrins.h>

//==========   宏定义   ==========

#define Y4  0x80
#define Y5  0xa0
#define Y6  0xc0
#define Y7  0xe0

//==========  全局变量  ==========

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

//==========  中断函数  ==========

//==========    MAIN    ==========

void main() 
{
  while(1)  
  {
    P0 = 0xff;
    delay_ms(500);
    set_hc573(4);
    P0 = 0x00;
    delay_ms(500);
    set_hc573(4);
  }
}