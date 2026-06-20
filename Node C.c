/* main_nodeB_RX.c */
#include "header.h"
#include "can2_driver.h"
#include "uart_driver.h"
#include"lcd1.h"
#include<string.h>

#define	led 1<<17

void int_str(int);

char buf[10];
char temp[10];
char dist[10];
char fuel[10];
int ls=0;

main()
{	
	CAN2_MSG m1;
	IODIR0|=led;
	lcd_init();
	can2_init();
	uart0_init(115200);
	uart0_tx_string("nodeB TESTING CAN\r\n");
	 lcd_cmd(0X80);
lcd_str("V25CE5M1");
lcd_cmd(0XC0);
lcd_str("YOUSUF");
 lcd_cmd(0X94);
lcd_str("(MAJOR PROJECT)");
lcd_cmd(0XD4);
lcd_str("DRIVE SENSE");
delay_ms(2500);
	 lcd_cmd(0X01);
	while(1){		
		can2_rx(&m1);
				 
		if((m1.rtr==0)&&(m1.id==0x150))
		{  
			uart0_tx_string("nodeB: DATA-FRAME is received\r\n");
			uart0_tx_hex(m1.id);
			uart0_tx(' ');
			uart0_tx_hex(m1.dlc);
			uart0_tx(' ');
			uart0_tx_integer(m1.byteA);
			uart0_tx(' ');
			uart0_tx_integer(m1.byteB);
			uart0_tx_string("\r\n");
						  
		

		uart0_tx_string("Fuel: ");
		uart0_tx_integer(m1.byteA);
			uart0_tx_string("%");
		 uart0_tx_string("\r\n");
		 int_str(m1.byteA);
		  strcpy(fuel,buf);
//lcd_cmd(0XC0);
//lcd_str("FUEL: "); 
//lcd_str((unsigned char *)buf); 
//lcd_data('%');

  if(m1.byteB == 1)
       { ls=1; 
// lcd_cmd(0xD4);
//	lcd_str("LED  ON");
            IOCLR0 = led;
            uart0_tx_string("LED ON\r\n");
        }
        else
        {   ls=0;
//		lcd_cmd(0xD4);
//	lcd_str("LED OFF");
            IOSET0 = led;
            uart0_tx_string("LED OFF\r\n");
        } }

	   	else if((m1.rtr==0)&&(m1.id==0x120))
		{ 
		uart0_tx_string("Temp: ");
		uart0_tx_integer(m1.byteA);
			uart0_tx_string("°c");
		 uart0_tx_string("\r\n");
		 int_str(m1.byteA);
		 strcpy(temp,buf);
//		lcd_cmd(0X80);
//		lcd_str("TEMP: "); 
//lcd_str((unsigned char *)buf); 
//lcd_data(0xDF);
//lcd_data('c'); 
 
uart0_tx_string("Dist: ");
		uart0_tx_integer(m1.byteB);
			uart0_tx_string("cm");
		 uart0_tx_string("\r\n");
		 int_str(m1.byteB);
		 strcpy(dist,buf);
//lcd_cmd(0X94);
//lcd_str("DIST:"); 
//lcd_str((unsigned char *)buf); 
//lcd_str(" CM");

		}	
		else{
			uart0_tx_string("nodeB: REMOTE-FRAME is received\r\n");
			uart0_tx_hex(m1.id);
			uart0_tx(' ');
			uart0_tx_hex(m1.dlc);
			uart0_tx(' ');
			uart0_tx_string("\r\n");
		}
lcd_cmd(0x01);
lcd_cmd(0X80);
lcd_str("TEMP: "); 
lcd_str((char *)temp); 
lcd_data(0xDF);
lcd_data('c');

lcd_cmd(0XC0);
lcd_str("FUEL: "); 
lcd_str((char *)fuel); 
lcd_data('%');

lcd_cmd(0X94);
lcd_str("DIST:"); 
lcd_str((char *)dist); 
lcd_str(" CM");
if(ls==1)
{
lcd_cmd(0xD4);
	lcd_str("LED  ON");}
else{	
lcd_cmd(0xD4);
	lcd_str("LED OFF");}
delay_ms(500);

	}
}
void int_str(int num){
	sprintf(buf,"%d",num);
}
