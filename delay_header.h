#include<LPC21xx.H>
void delay_second(unsigned int);
void delay_ms(unsigned int);
void delay_us(unsigned int);
void delay_second(unsigned int second)
{
T0PR=60000000-1;
T0TCR=0X01;
while(T0TC<second);
T0TCR=0X03;
T0TCR=0X00;
}

void delay_ms(unsigned int milli)
{
T0PR=60000-1;
T0TCR=0X01;
while(T0TC<milli);
T0TCR=0X03;
T0TCR=0X00;
}

void delay_us(unsigned int micro)
{
T0PR=60-1;
T0TCR=0X01;
while(T0TC<micro);
T0TCR=0X03;
T0TCR=0X00;
}




