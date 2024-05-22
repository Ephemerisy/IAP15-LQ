#include <STC15F2K60S2.H>
#include <intrins.h>

//============================宏定义================================

#define Y4  0x80
#define Y5  0xa0
#define Y6  0xc0
#define Y7  0xe0

//===========================全局变量===============================



//===========================比赛文件===============================



//===========================函数定义===============================

void delay_ms(unsigned int ms)  // 12MHz
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

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x20;				//设置定时初始值
	TH0 = 0xD1;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
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

//===========================中断服务===============================

void Timer0_Isr(void) interrupt 1
{
}

void Timer1_Isr(void) interrupt 3
{
}

//============================ MAIN ================================
void main()
{
  Timer0_Init();
  Timer1_Init();
  while(1)
  {
    
  }
}