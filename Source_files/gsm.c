#include "uart_defines.h"
#include<string.h>
//abxdDDE
#include "uart.h"
#include "LCD.h"
#include "Delay.h"
char received_msg[50];
extern char buffer[300],read_flag,ch;
extern unsigned char dummy;
extern int i,flag;
unsigned int check_device;
int gsm_flag=0;
void gsm_init()
{
   str_lcd("GSM");
   delay_ms(1000);
   cmd_lcd(0x01);
  /*if(((U0LSR>>DR)&1)==0)
  {
  cmd_lcd(0x01);
    str_lcd("GSM FAULT!!");
	while(1)
	{
		if(((U0LSR>>DR)&1)==1)
		{
			break;
		}
	}
	}*/
	/*cmd_lcd(0x01);
    strlcd(buffer);
	delay_ms(1000);
	cmd_lcd(0x01);*/
  delay_ms(1000);
  memset(buffer,0,300);
  i=0;
  read_flag=0;

  //display AT command on lcd in first line
  cmd: U0_TX_BYTE_string("AT\r\n");
  cmd_lcd(0x80);
  str_lcd("AT");
  delay_ms(2000);
  while(i<2){
  		check_device++;
		if(check_device<3)
			goto cmd;
		else{
			cmd_lcd(0x01);
			str_lcd("device not availiable");
			while(1);
		}
		
	}
  /*if(((U0LSR>>DR)&1)==0)
  {
  cmd_lcd(0x01);
    str_lcd("GSM FAULT!!");
	while(1)
	{
		if(((U0LSR>>DR)&1)==1)
		{
			break;
		}
	}
	}*/
  if(strstr(buffer,"OK"))
  {
     //flash GSM response on lcd in second line
				                                                                                                                                                                                                                                                                                                                              		
	 str_lcd("OK");
	 delay_ms(1000);
  }
  else
  {
     //flash error on lcd in second line
	 cmd_lcd(0xc0);
	 str_lcd("Error");
	 delay_ms(1000);
  }
  memset(buffer,0,300);
  i=0;
  read_flag=0;
  U0_TX_BYTE_string("ATE0\r\n");
  cmd_lcd(0x01);
  cmd_lcd(0x80);
  str_lcd("ATE0");									
	//while(read_flag==0);
  while(i<2);
  if(strstr(buffer,"OK"))
  {
     //flash GSM response on lcd in second line
	 cmd_lcd(0xc0);
	 str_lcd("OK");
	 delay_ms(1000);
  }
  else
  {
     cmd_lcd(0xc0);
	 str_lcd(buffer);
	 delay_ms(1000);

     //flash error on lcd in second line
  }
  memset(buffer,0,300);
  i=0;
  read_flag=0;
  cmd_lcd(0x01);
  cmd_lcd(0x80);
  U0_TX_BYTE_string("AT+CMGF=1\r\n");	
  str_lcd("AT+CMGF=1");
  while(i<2);
  if(strstr(buffer,"OK"))
  {
     //flash GSM response on lcd in second line
	 cmd_lcd(0xc0);
	 str_lcd("OK");
	 delay_ms(1000);
  }
  else
  {
     cmd_lcd(0xc0);
	 str_lcd(buffer);
	 delay_ms(1000);
  }
  memset(buffer,0,300);
  i=0;
  read_flag=0;
  U0_TX_BYTE_string("AT+CMGD=1\r\n");
  //while(read_flag==0);                                                                                                                          
  delay_ms(2000);
  cmd_lcd(0x01);
  cmd_lcd(0x80);
  str_lcd("AT+CMGD=1");
  delay_ms(1000);
   if(strstr(buffer,"OK"))
	{
		cmd_lcd(0xc0);
	    str_lcd("OK");
	}
	else
	{				   
		cmd_lcd(0xc0);
	    str_lcd("ERROR");
	}
	memset(buffer,0,300);
	i=0;
	//received_flag=0;
    U0_TX_BYTE_string("AT+CNMI=2,1,0,0,0\r\n");
	i=0;
	memset(buffer,0,300);
	delay_ms(3000);
	cmd_lcd(0x01);
    cmd_lcd(0x80);
	str_lcd("CNMI");
	
	while(i<4);
	buffer[i]='\0';
	cmd_lcd(0xc0);
    str_lcd(buffer);
    delay_ms(1000);

	if(strstr(buffer,"OK"))
	{
	  	cmd_lcd(0x01);
		cmd_lcd(0xc0);
		str_lcd("OK");
		delay_ms(100);
		cmd_lcd(0x01);
	}
/*	else
	{
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("GSM NOT CONNECTED");
	delay_ms(1000);
	}*/
	gsm_flag=1;
 }
void gsm_send(char *mobilenumber,char *msg)
{
   memset(buffer,0,300);
   i=0;
   read_flag=0;
   //cmd_lcd(0x01);
   cmd_lcd(0x80);
   str_lcd("AT+CMGS=");
   delay_ms(1000);
   U0_TX_BYTE_string("AT+CMGS=");
   U0_TXBYTE('"');
   U0_TX_BYTE_string(mobilenumber);
   U0_TXBYTE('"');
   U0_TX_BYTE_string("\r\n");
   delay_ms(2000);
    i=0;

      U0_TX_BYTE_string(msg);
	  U0_TXBYTE(0X1A);
	  delay_ms(3000);
	  cmd_lcd(0x01);
	  cmd_lcd(0x80);
	  delay_ms(1000);
	  if(strstr(buffer,"OK"))
	  {
		  cmd_lcd(0x01);
		  cmd_lcd(0x80);
		  str_lcd("SMS SENT");
		  delay_ms(1000);

	  }
	  else
	  {
		  cmd_lcd(0x01);
		  cmd_lcd(0x80);
		  str_lcd(buffer);
		  str_lcd("SMS NOT");
		  delay_ms(1000);
	  }
	  cmd_lcd(0X01);
}

char* gsm_receive()
{
   char *ptr;
   memset(received_msg,0,100);
   memset(buffer,0,300);
   i=0;
   read_flag=0;			              
   U0_TX_BYTE_string("AT+CMGR=1\r\n");
   delay_ms(3000);
	 cmd_lcd(0x01);
	 cmd_lcd(0x80);
   str_lcd("AT+CMGR=1");
	delay_ms(3000);
	//while(i<2);
	buffer[i]='\0';
	 //U0_TX_BYTE_string(buffer);
	 if(strstr(buffer,"+CMGR")!=NULL)
	 {
	    ptr=strtok(buffer,"\r\n");
		cmd_lcd(0x01);
		str_lcd("READ");
		delay_ms(2000);
	    while(ptr!=NULL)
	    {
		  cmd_lcd(0x01);
		  cmd_lcd(0x80); 
		  str_lcd("Enter");
	      if((strstr(ptr,"+CMGR")==NULL))
				{
					strcpy(received_msg,ptr);
					break;
				}
				ptr=strtok(NULL,"\r\n");
	     }
		 cmd_lcd(0x01);
	   cmd_lcd(0x80);
		 str_lcd("MSG RECEIVED");
		 delay_ms(1000);
	   cmd_lcd(0xC0);
	   str_lcd(received_msg);
	   delay_ms(3000);
	 }
	 else
	 {
	   cmd_lcd(0x01);
	   cmd_lcd(0x80);
	   str_lcd(buffer);
	   delay_ms(1000);
		 //return 0;
	 }
	   //memset(buffer,0,300);
	   i=0;
     read_flag=0;
     U0_TX_BYTE_string("AT+CMGD=1\r\n");
	   //while(read_flag==0);                                                                                                                          
	   delay_ms(2000);
	   cmd_lcd(0x01);
	   cmd_lcd(0x80);
	   str_lcd("AT+CMGD=1");
	   delay_ms(1000);
     if(strstr(buffer,"OK"))
		{
			cmd_lcd(0xc0);
	    str_lcd("OK");
		}
		else		  
		{				   
		  cmd_lcd(0xc0);
	    str_lcd("ERROR");
		}
		return received_msg;
}

/*int main()
{
	char *msg;
  gsm_init();
  gsm_send(mobilenumber,"hello");
	msg=gsm_receive();
	str_lcd(msg);
  while(1);
}*/
