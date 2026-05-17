#include <reg51.h>

#define lcd P1

sbit rs = P3^2;
sbit en = P3^3;

sbit ALE = P2^5;
sbit SOC = P2^6;
sbit EOC = P2^7;

unsigned char value;
unsigned int temp;

void delay(unsigned int ms)
{
    int i,j;

    for(i=0;i<ms;i++)
        for(j=0;j<1275;j++);
}

void lcd_cmd(unsigned char cmd)
{
    lcd=(lcd&0x0F)|(cmd&0xF0);

    rs=0;
    en=1;
    delay(2);
    en=0;

    lcd=(lcd&0x0F)|((cmd<<4)&0xF0);

    en=1;
    delay(2);
    en=0;
}

void lcd_data(unsigned char dat)
{
    lcd=(lcd&0x0F)|(dat&0xF0);

    rs=1;
    en=1;
    delay(2);
    en=0;

    lcd=(lcd&0x0F)|((dat<<4)&0xF0);

    en=1;
    delay(2);
    en=0;
}

void lcd_string(char *str)
{
    while(*str)
        lcd_data(*str++);
}

void lcd_init()
{
    delay(20);

    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

unsigned char adc_read()
{
    ALE=1;
    SOC=1;

    delay(1);

    ALE=0;
    SOC=0;

    while(EOC==0);

    return (P0 >> 2);
}

void main()
{
    lcd_init();

    while(1)
    {
        value=adc_read();

        temp=value;

        lcd_cmd(0x80);

        lcd_string("Temp = ");

        lcd_data((temp/10)+'0');
        lcd_data((temp%10)+'0');

        lcd_data(223);
        lcd_data('C');

        delay(300);
    }
}