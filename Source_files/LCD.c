#include<lpc21xx.h>
#include "LCD.h"
#include "Delay.h"
#define RS 5
#define RW 4
#define EN 6
#define DATA 8

int data;
char str[100];
void write_lcd(int data)
{
	//RW->0 for write operation
	IOCLR0=1<<RW;
	//clear display and update command
	IOPIN0=(IOPIN0&~(0XFF<<DATA))|(data<<DATA);
  //enable should be high to low for write operation
	IOSET0=1<<EN;
	delay_us(10);
	IOCLR0=1<<EN;
	delay_ms(2);
}

void cmd_lcd(int data)
{
	//rs-> 0 for setting command register
	IOCLR0=1<<RS;
	write_lcd(data);
}

void char_lcd(int data)
{
	//RS->1 for data register
	IOSET0=1<<RS;
	write_lcd(data);
}

void LCD_INIT()
{
	IODIR0|=1<<RS;
	IODIR0|=1<<RW;
	IODIR0|=1<<EN;
	IODIR0|=0XFF<<DATA;
	delay_ms(15);
	//setting LCD to 8bit mode 1 line
	cmd_lcd(0X30);
	delay_ms(5);
	cmd_lcd(0X30);
	delay_ms(10);
	cmd_lcd(0x30);
	//display settings
	cmd_lcd(0x38);//8 bit mode 2 lines
	cmd_lcd(0x0F);//display on,cursor on,blink on
	cmd_lcd(0x01);//clear lcd
	cmd_lcd(0x06);//shift cursor,shift display
	cmd_lcd(0x0c);
}

void str_lcd(char *str)
{
	while(*str)
	{
		char_lcd(*str++);
	}
}

void int_LCD(int n)
{
  int a[10],i=0;
  if(n==0)
  {
    char_lcd('0');
  }
  else
  {
  while(n)
  {
    a[i++]=(n%10)+48;
    n=n/10;
   }
   for(--i;i>=0;i--)
   {
     char_lcd(a[i]);
   }
   }
 }

void float_lcd(float f,int nDP)
{
  int i,n;
  if(f<0)
  {
    char_lcd('-');
  }
  else
  {
    n=f;
    int_LCD(n);
    char_lcd('.');
   }
   for(i=0;i<=nDP;i++)
   {
     f=(f-n)*10;
	n=f;
	char_lcd(n+48);
   }
}

void neg_lcd(int n)
{
  if(n<0)
  {
    char_lcd('-');
    n=-n;
    int_LCD(n);
   }
}
/*int main()
{
  LCD_INIT();
	//STRING
  cmd_lcd(0x80);//1st row,1st pos
	str_lcd("SHALINI");
	//CHAR
	cmd_lcd(0xC0);//line2,1st pos
  str_lcd("CHAR: ");
	char_lcd('A');
	delay_ms(2000);
	cmd_lcd(0x01);//clear lcd
	delay_ms(2);
	//INT
	cmd_lcd(0x80);
	str_lcd("INT: ");
	int_LCD(123);
	//FLOAT
	cmd_lcd(0xC0);
	str_lcd("FLOAT: ");
	float_lcd(25.6,1);
	delay_ms(2000);
	cmd_lcd(0x01);
	delay_ms(2);
	//NEG NUM
	cmd_lcd(0x80);
	str_lcd("NEG: ");
	neg_lcd(-50);
	while(1);
}*/
