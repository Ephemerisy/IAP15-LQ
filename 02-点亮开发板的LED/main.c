#include <STC15F2K60S2.H>
#include <intrins.h>

//=========================== 宏 定 义 ===============================

#define Y4  0x80
#define Y5  0xa0
#define Y6  0xc0
#define Y7  0xe0

//========================== 全 局 变 量 =============================


//========================== 比 赛 文 件 =============================


//========================== 函 数 定 义 =============================

void delay_ms(unsigned int ms)  // @12.000MHz
{
  unsigned char data i, j;
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

//========================== 函 数 中 断 =============================


//============================== MAIN ================================

void main()
{
  unsigned char i;
  while(1)
  {
    // LED闪烁
    P0 = 0x00;
    set_hc573(4);
    delay_ms(500);
    P0 = 0xff;
    set_hc573(4);
    delay_ms(500);
    
    // 流水灯
    for(i=0;i<8;i++)
    {
      P0 = 0x01<<i;
      P0 = ~P0;
      set_hc573(4);
      delay_ms(50);
    }
    
    for(i=0;i<8;i++)
    {
      P0 = 0x80>>i;
      P0 = ~P0;
      set_hc573(4);
      delay_ms(50);
    }
  }
}