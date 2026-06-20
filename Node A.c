
#include <lpc21xx.h>
#include "lcd1.h"
#include "can2_driver.h"
  // Uncomment this only if you remove the duplicate definitions

#define TRIG_PIN         (1u << 10)
#define ECHO_PIN         (1u << 11)
#define BUZZER_PIN       (1u << 12)
#define OBSTACLE_DIST_CM 20
 void Buzzer_Init(void)
{
    IODIR0 |= BUZZER_PIN;
    IOSET0 = BUZZER_PIN; 
}

void Buzzer_On(void)
{
    IOCLR0 = BUZZER_PIN;
}

void Buzzer_Off(void)
{
    IOSET0 = BUZZER_PIN;
}

void ADC0_init(void)
{
    PINSEL1|= 1<<22;
    ADCR=(1<<0)|(4<<8)|(1<<21);
}

unsigned int adc_read(void)
{
    unsigned int adc;
    ADCR |= (1<<24);
    while(!(ADDR&0X80000000));
    adc=((ADDR>>6)&0X3FF);
    return adc;
}

unsigned int read_temperature(void)
{
    unsigned long total = 0;
	unsigned int avg;
    int i;

    /* Turn buzzer OFF during ADC reads to reduce power rail noise */
    Buzzer_Off();

    /* 8 samples with 5ms gap — stable and fast enough */
    for (i = 0; i < 8; i++)
    {
        total += adc_read();
        delay_ms(5);
    }

     avg = (unsigned int)(total / 8);

    /* LM35: 10mV/°C, Vref = 3.3V
       Temp = (avg * 3300) / (1023 * 10)
            = (avg * 330) / 1023             */
    return (avg * 330u) / 1023u;
}

void ultrasonic_init(void)
{
    IODIR0 |= TRIG_PIN;    // Trigger is Output
    IODIR0 &= ~ECHO_PIN;   // Echo is Input
    IOCLR0 = TRIG_PIN;     // Initialize Trigger Low
}

unsigned int ultrasonic_Read(void)
{
    unsigned int value;
    
    IOCLR0 = TRIG_PIN;
    delay_us(2);
    IOSET0 = TRIG_PIN;
    delay_us(10);
    IOCLR0 = TRIG_PIN;
    
    while (!(IOPIN0 & ECHO_PIN));
    
    T1PR = 59;          
    T1TCR = 0x02;       
    T1TCR = 0x01;       
    
    while (IOPIN0 & ECHO_PIN);
    
    T1TCR = 0x00;       
    value = T1TC;       
    
    return (value / 58u); 
}


int main() // FIXED: Removed stray 'c'
{
    unsigned int temperature, distance;
    CAN2_MSG m1;
	
    ADC0_init();
    ultrasonic_init();
    Buzzer_Init();
    can2_init();
    lcd_init();
	   m1.id=0x120;
	   m1.rtr=0;
	   m1.dlc=8;

    
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_str("Yousuf");
    delay_ms(2000); 
    
    while (1)
    {
        temperature = read_temperature();
        distance = ultrasonic_Read();
        
        lcd_cmd(0x01); 
        
        lcd_cmd(0x80); 
        lcd_str("Temp: ");
        LCD_INTEGER(temperature);
			  m1.byteA=((int)temperature);
        lcd_str(" C");
        
        lcd_cmd(0xC0); 
        lcd_str("Dist: ");
        LCD_INTEGER(distance);
			  m1.byteB=((int)distance);
        lcd_str(" cm");
        
        if (distance < OBSTACLE_DIST_CM)
        {
            Buzzer_On();
        }
        else
        {
            Buzzer_Off();
        }
        can2_tx(m1);
        delay_ms(1000);				
    }
}