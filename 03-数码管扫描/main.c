#include <STC15F2K60S2.H>
#include <intrins.h>

//=========================== 宏 定 义 ===============================

#define Y4  0x80
#define Y5  0xa0
#define Y6  0xc0
#define Y7  0xe0

//========================== 全 局 变 量 =============================

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

void clean_SEGs(void)
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
//========================== 函 数 中 断 =============================


//============================== MAIN ================================

void main()
{
  unsigned char i;
  
  clean_SEGs();
  for(i=0;i<8;i++)
  {
    display_SEG(i,i,1);
    delay_ms(300);
  }
  
  clean_SEGs();
  for(i=0;i<10;i++)
  {
    display_SEG(0,i,i%2);
    delay_ms(300);
  }
  
  clean_SEGs();
  while(1)
  {
    display_SEG(0,3,0); delay_ms(2);
    display_SEG(1,1,1); delay_ms(2);
    display_SEG(2,4,1); delay_ms(2);
    display_SEG(3,1,1); delay_ms(2);
    display_SEG(4,5,1); delay_ms(2);
  }
}