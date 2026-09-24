#include<lpc21xx.h>
#include "LCD.h"
#include "i2c.h"
//#include"uart.h"
//SCL,SDA defines
#define SCL_PIN 0x00000010 //0.2
#define SDA_PIN 0x00000040 //0.3

//Defines for I2C speed
#define CCLK 60000000
#define PCLK (CCLK/4)
#define I2C_SPEED 100000
#define LOADVAL ((PCLK/I2C_SPEED)/2)

//Defines for CONSET
#define AA_BIT 2
#define SI_BIT 3
#define STO_BIT 4
#define STA_BIT 5
#define I2EN_BIT 6

//Defines for CONCLR
#define AAC_BIT 2
#define SIC_BIT 3
#define STAC_BIT 5
#define I2ENC_BIT 6

#define LED 0
/*void delay_ms(unsigned int DLYMS)
{
   for(DLYMS*=12000;DLYMS>0;DLYMS--);
}*/
//INITIALIZATION OF SPI
void init_i2c(void)
{
	//configure SCL and SDA Bit
	PINSEL0|=SCL_PIN|SDA_PIN;
	//configure speed for I2C
	I2SCLL=LOADVAL;
	I2SCLH=LOADVAL;
	//Enable the I2C
	I2CONSET=1<<I2EN_BIT;
	/*cmd_lcd(0x80);
	str_lcd("i2c INIT");
	delay_ms(500);*/
}

void I2C_START()
{
	//start condition
	I2CONSET=1<<STA_BIT;
	//CLEAR SI BIT
	I2CONCLR=1<<SIC_BIT;
	//Check SI bit
	while(((I2CONSET>>SI_BIT)&1)==0);
	//clear start condition
	I2CONCLR = 1<<STAC_BIT;
}

void I2C_STOP()
{
	//stop condition
	I2CONSET=1<<STO_BIT;
	//CLEAR SI BIT
	I2CONCLR=1<<SIC_BIT;
	//check SIC 
	//while(((I2CONSET>>SI_BIT)&1)==0);
	//clear stop bit
	//I2CONCLR
}

void I2C_RESTART()
{
	//start condition
	I2CONSET=1<<STA_BIT;
	//CLEAR SI BIT
	I2CONCLR=1<<SIC_BIT;
	//Check SI bit
	while(((I2CONSET>>SI_BIT)&1)==0);
	//clear start condition
	I2CONCLR = 1<<STAC_BIT;
}

char I2C_nack()
{
	I2CONCLR = 1<<AAC_BIT;
  //clear SIC Bit
  I2CONCLR=1<<SIC_BIT;
  while(((I2CONSET>>SI_BIT)&1)==0);
  return I2DAT;
}

char I2C_mack()
{
	///I2CONSET = 1<<AA_BIT;
  //I2CONCLR=1<<AAC_BIT;
  //clear SIC Bit
  I2CONCLR=1<<SIC_BIT;
  I2CONSET = 1<<AA_BIT;
  while(((I2CONSET>>SI_BIT)&1)==0);
  //I2CONCLR=1<<AAC_BIT;
  return I2DAT;
}

void I2C_write(char data1)
{
	//save the data into buffer
	I2DAT=data1;
	//clear the SI BIT
	I2CONCLR=1<<SIC_BIT;
  
	//Check SI bit
	while(((I2CONSET>>SI_BIT)&1)==0);
}

void i2c_byte_write(char slaveaddr,short int wbuffAddr,char dat)
{
   	 I2C_START();
	 //slave address with write operation
	 I2C_write(slaveaddr<<1);
	 //write buffer address
	 I2C_write(wbuffAddr>>8);
	 I2C_write(wbuffAddr&0XFF);
	 I2C_write(dat);
	 I2C_STOP();
	 delay_ms(10);
}

char i2c_random_read(char slaveaddr,short int RBuffAddr)
{ 
     char dat;
     I2C_START();
	 //slave address with write operation
	 I2C_write(slaveaddr<<1);
	 //write buffer address
	 I2C_write(RBuffAddr>>8);
	 I2C_write(RBuffAddr&0XFF);
	 I2C_RESTART();
	 //slave address with read operation
	 I2C_write((slaveaddr<<1)|1);
	  dat= I2C_nack();
	 I2C_STOP();
	 return dat;
}

void i2c_page_write(char slaveaddr,short int wBuffAddr,char *p,char nBytes)
{
     int j;
   	 I2C_START();
	 //slave address with write operation
	 I2C_write(slaveaddr<<1);
	 //write buffer address
	 I2C_write(wBuffAddr>>8);
	 I2C_write(wBuffAddr&0XFF);
	 for(j=0;j<nBytes;j++)
	 {
	   I2C_write(p[j]);
	 }
	 I2C_STOP();
	 delay_ms(10);
}

void i2c_seq_read(char slaveaddr,short int RBuffAddr,char *p,char nBytes)
{ 
     char j;
     I2C_START();
	 //slave address with write operation
	 I2C_write(slaveaddr<<1);
	 //write buffer address
	 I2C_write(RBuffAddr>>8);
	 I2C_write(RBuffAddr&0XFF);
	 I2C_RESTART();
	 //slave address with read operation
	 I2C_write((slaveaddr<<1)|1);
	 for(j=0;j<nBytes-1;j++)
	 {
	     p[j]=I2C_mack();
	 }
       p[j]=I2C_nack();
	   I2C_STOP();
}					                                                                                                                      
 //char t,S[9]="Lahare",data1[9],read[6];
 int j;
/*int main()
{
	char str[]="SHALINI";
	char data;
	int i;
	LCD_INIT();
	init_i2c();
	//write shalini into eeprom
	for(i=0;str[i]!='\0';i++)
	{
		i2c_byte_write(0x50,i,str[i]);
	}
	//read frm eeprom and disp
	cmd_lcd(0x80);
	for(i=0;str[i]!='\0';i++)
	{
		data=i2c_random_read(0x50,i);
		char_lcd(data);
	}
	while(1);
}*/
/*int main()
{
	char data;
	LCD_INIT();
	init_i2c();
	cmd_lcd(0x80);
	str_lcd("WRITE");
	i2c_byte_write(0x50,0,'S');
	//delay_ms(20);
	cmd_lcd(0xC0);
	str_lcd("READ");
	data=i2c_random_read(0x50,0);
	cmd_lcd(0xC0);
	char_lcd(data);
  while(1);
}*/
