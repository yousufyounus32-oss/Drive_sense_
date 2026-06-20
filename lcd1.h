#include"delay_header.h"
#define LCD_PORT 0xf<<4
#define RS 1<<15
#define EN 1<<16
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_str(char *);

void lcd_init(void)
{
    IODIR0|= LCD_PORT | RS | EN;
    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void lcd_cmd(unsigned char cmd)
{
   
    IOCLR0 = LCD_PORT;
    IOSET0 = (cmd& 0xF0) << 0;
    IOCLR0 = RS;
    IOSET0 = EN;
    delay_ms(2);
    IOCLR0 = EN;
   
    IOCLR0 = LCD_PORT;
    IOSET0 = (cmd & 0x0F) <<4;
    IOCLR0 = RS;
    IOSET0 = EN;
    delay_ms(2);
    IOCLR0 = EN;
}

void lcd_data(unsigned char data)
{
   
    IOCLR0 = LCD_PORT;
    IOSET0 = (data& 0xF0)<< 0;
    IOSET0 = RS;
    IOSET0 = EN;
    delay_ms(2);
    IOCLR0 = EN;
    
   
    IOCLR0 = LCD_PORT;
    IOSET0 = (data & 0x0F) << 4;
    IOSET0 = RS;
    IOSET0 = EN;
    delay_ms(2);
    IOCLR0 = EN;
}

void lcd_str(char *str)
{
    while(*str)
        lcd_data(*str++);
}

void LCD_INTEGER(int n)

{

unsigned char arr[5];

signed char i=0;

if(n==0)

lcd_data('0');

else

{

if(n<0)

{

lcd_data('-');

n=-n;

}

while(n>0)

{

arr[i++]=n%10;

n=n/10;

}

for(--i;i>=0;i--)

lcd_data(arr[i]+48);

}

}

void LCD_FLOAT(float f)

{

int temp;

temp=f;

LCD_INTEGER(temp);

lcd_data('.');

temp=(f-temp)*1000;

LCD_INTEGER(temp);

}
 