
#include <lpc21xx.h>
#include "can2_driver.h"
#include"delay_header.h"
#include "uart_driver.h"
#define LDR         17
#define LED         1<<16

unsigned int adc_read(void)
{  unsigned int adc;
PINSEL1|= 1<<22;
ADCR=(1<<0)|(4<<8)|(1<<21);
ADCR |= (1<<24);
while(!(ADDR&0X80000000));
adc=((ADDR>>6)&0X3FF);
return adc;
 }                       

unsigned char Read_Fuel_percentage(void)
{
    unsigned int value =adc_read();
     return (unsigned char)((value * 100UL) / 1023);
}


/* --- Main -------------------------------------------------- */
int main(void)
{
    unsigned char fuel_percentage;
    CAN2_MSG m1;
    can2_init();
	  m1.id=0x150;
	  m1.rtr=0;//data frame
	  m1.dlc=8;

   
    uart0_init(115200);

	IODIR0|=LED;
    while (1)
    {
        fuel_percentage = Read_Fuel_percentage();

        /* Print Fuel */
        uart0_tx_string("Fuel  : ");
        uart0_tx_integer(fuel_percentage);
				m1.byteA=(unsigned int)fuel_percentage;
        uart0_tx_string("%\r\n");
			
			if(((IOPIN0 >> LDR )&1)==1)
      {
        m1.byteB=1;
        uart0_tx_string("LED  ON");
        IOCLR0= LED;
      }
      else
      {
	      m1.byteB=0;
        uart0_tx_string("LED OFF");
        IOSET0=LED;
      }

      can2_tx(m1);
			delay_ms(1000);
    }
}
