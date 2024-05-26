#include <STC15F2K60S2.H>
#include <intrins.h>

//=========================== 宏 定 义 ===============================

#define press_shot  2
#define press_long  50

#define Y4  0x80
#define Y5  0xa0
#define Y6  0xc0
#define Y7  0xe0

//========================== 全 局 变 量 =============================

unsigned int tick_timer1;
unsigned char S0_count,S4_count,S5_count,S8_count,S9_count;
unsigned char key_down;
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

bit long_press_flag = 1;

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

unsigned key_scan(void)
{
  unsigned char data temp1,temp2,temp;
  temp1 = 0; temp2 = 0; temp = 0;
  P32 = 1; P33 = 1;
  P42 = 0; P44 = 0;
  if(P32 == 0) temp1 = 1;
  if(P33 == 0) temp1 = 0;
  _nop_();
  P32 = 0; P33 = 0;
  P42 = 1; P44 = 1;
  if(P42 == 0) temp2 = 8;
  if(P44 == 0) temp2 = 4;
  temp = temp1+temp2;
  return temp;
}

void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x20;				//设置定时初始值
	TH1 = 0xD1;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
}

//========================== 函 数 中 断 =============================

void Timer1_Isr(void) interrupt 3
{
  if(++tick_timer1==1001) tick_timer1=1;
  
  if(tick_timer1%20==0)   // 按键检测任务
  {
    switch(key_scan())
    {
      case 0:
        if(++S0_count>200) S0_count = 200; break;
      case 4:
        if(++S4_count>200) S4_count = 200; break;
      case 5:
        if(++S5_count>200) S5_count = 200; break;
      case 8:
        if(++S8_count>200) S8_count = 200; break;
      case 9:
        if(++S9_count>200) S9_count = 200; break;
    }
  }
}

//============================== MAIN ================================

void main()
{
  EA = 1;
  Timer1_Init();
  while(1)
  {
    switch(key_scan())
    {
      case 0:
        if(S0_count>1)
        {
          key_down = 0;
          long_press_flag = 1;
          S0_count = 0;
          S4_count = 0; S5_count = 0;
          S8_count = 0; S9_count = 0;
        }
      case 4:
        if(S4_count>press_shot&&S4_count<press_long) key_down = 4;
        if(S4_count>press_long) long_press_flag = 0;
        break;
      case 5:
        if(S5_count>press_shot&&S5_count<press_long) key_down = 5;
        if(S5_count>press_long) long_press_flag = 0;
        break;
      case 8:
        if(S8_count>press_shot&&S8_count<press_long) key_down = 8;
        if(S8_count>press_long) long_press_flag = 0;
        break;
      case 9:
        if(S9_count>press_shot&&S9_count<press_long) key_down = 9;
        if(S9_count>press_long) long_press_flag = 0;
        break;
    }

    
    display_SEG(0,key_down,long_press_flag);
  }
}