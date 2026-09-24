#include "gsm.h"

#include "uart.h"

#include "uart_defines.h"

#include "LCD.h"

#include "keypad.h"

#include "interrupt.h"

#include "ADC.h"

#include "main_defines.h"

#include "Delay.h"

#include "local_settings.h"

#include "i2c.h"

#include <string.h>

#define BUZZER_PIN 17
int verify_number(char *buffer);

//set point of the temperature

int setpoint=200;

int temp,temp_alert_sent;

extern volatile int eint_flag;

extern int received_flag,read_flag,i;

char mobilenumber[11]="9573641321",new_buffer[20];

extern char received_msg[50],buffer[300];

extern char passkey[];

char *message,eflag[7]="7580@a",tem[7];

extern int gsm_flag;

void INIT()

{

  LCD_INIT();

  INIT_UART();

  init_i2c();

  enable_uart0();

  INIT_ADC();

  KPM_INIT();

  enable_eint0();

  /*i2c_byte_write(0x50,0x01,setpoint);

  i2c_page_write(0x50,0x02,mobilenumber,10);

  i2c_page_write(0x50,0x20,passkey,4);*/

  //i2c_page_write(0x50,0x40,1107);

  delay_ms(10);

  //i2c_byte_write(0x50,0x40,

  

  cmd_lcd(0x01);

  delay_ms(1000);

  str_lcd(buffer);

  delay_ms(1000);

  if(strstr(buffer,"MODEM:STARTUP"))
  //if((strcmp(buffer,"+PBREADY")==0))
  {
  cmd_lcd(0xc0);	
  str_lcd("GSM");
  delay_ms(1000);
  cmd_lcd(0x01);
  gsm_init();
  }
  else
  {
     str_lcd("GSM fault");
	 delay_ms(1000);
  }
	  
   //gsm_init();
   IODIR0|=(1<<BUZZER_PIN);
   //IOCLR0=(1<<BUZZER_PIN);
  memset(tem,0,sizeof(tem));

  i2c_seq_read(0x50,0x40,tem,6);

  tem[6]='\0';

  //i2c_page_write(0x50,0x02,mobilenumber,10);

  if(strcmp(tem,eflag)!=0)

  {

  i2c_byte_write(0x50,0x01,setpoint);

  i2c_page_write(0x50,0x02,mobilenumber,10);

  i2c_page_write(0x50,0x20,passkey,4);

  //i2c_page_write(0x50,0x40,1107);

  i2c_page_write(0x50,0x40,eflag,7);

  }

  setpoint = i2c_random_read(0x50,0x01);

  i2c_seq_read(0x50,0x02,mobilenumber,10);

  i2c_seq_read(0x50,0x20,passkey,4);

  mobilenumber[10] = '\0'; 




/*  i2c_byte_write(0x50,0x01,setpoint);

  i2c_page_write(0x50,0x02,mobilenumber,10);

  i2c_page_write(0x50,0x20,passkey,4);

  delay_ms(10);

  setpoint = i2c_random_read(0x50,0x01);

  i2c_seq_read(0x50,0x02,mobilenumber,10);

  i2c_seq_read(0x50,0x20,passkey,10);	                     */

}			  

int main()

{

  int dval;

  float ear;

  INIT();


	/*i2c_byte_write(0x50,0x00,setpoint);

	i2c_page_write(0x50,0x01,mobilenumber,10);

	delay_ms(10);*/

    while(1)

   {

    /*if(((U0LSR>>DR)&1)==0)

	{

	   str_lcd("GSM FAULT!!!");

	   while(1);

	} */ 

     if(eint_flag==1)

		 {

			 eint_flag=0;

			check_password();

		 }

			  

		Read_ADC(CH1,&dval,&ear);

		temp=ear*100;
		//cmd_lcd(0x01);
		//display_temp();

		cmd_lcd(0x80);
		str_lcd("TEMP:   ");
		cmd_lcd(0x85);
	//	str_lcd(" ");
		int_LCD(temp);
		char_lcd(0xDF);
		char_lcd('C');

	    /*char_lcd(30);
	    str_lcd("TEMP:");
	    int_LCD(temp);
	    char_lcd(0xDF);
     	char_lcd('C'); */

		if(temp>setpoint)
		{
		   IOSET0=(1<<BUZZER_PIN);
		   if(gsm_flag==1){
			if(temp_alert_sent==0)
			{
         gsm_send(mobilenumber,"Temperature exceeded");
         	temp_alert_sent=1;
			}
			}				
        }
		else
		{
		   IOCLR0=(1<<BUZZER_PIN);
		   //temp_alert_sent=0;
		}
		//cmd_lcd(0x01);
		//cmd_lcd(0x80);
//		str_lcd("waiting function");
		delay_ms(500);
		while(received_flag==0 && gsm_flag==0);

		if(received_flag==1)
        {
		  cmd_lcd(0x01);

		  cmd_lcd(0x80);

		  str_lcd("FLAG");

		  delay_ms(1000);

		  received_flag=0;

		  message=gsm_receive();
		  if(message!=0)
		  {

		     cmd_lcd(0x01);

			   cmd_lcd(0x80);

			   str_lcd(message);

		     delay_ms(3000);

			 //mobile number verification

			 /*cmd_lcd(0x01);

			 cmd_lcd(0x80);

			 delay_ms(2000);

			 cmd_lcd(0xc0);

			 str_lcd(buffer);

			 delay_ms(2000);*/

			  if(verify_number(buffer)==0)

			  {

			    if(check_syntax(message))

			    {

						//if(check_number())

						//{

						cmd_lcd(0x80);
						str_lcd("valid");
						cmd_lcd(0xc0);
						 read(message);
						//}
				  }
				  else
				  {
						 cmd_lcd(0x01);
						 cmd_lcd(0x80);
						 str_lcd("Invalid Message");
						 delay_ms(1000);
						 cmd_lcd(0x01);
				   }
			   }
			   //memset(buffer,0,300);
			   else
			   {
			     	cmd_lcd(0x01);
				    cmd_lcd(0x80);

				    str_lcd("unauthorised");
					delay_ms(1000);
					cmd_lcd(0x01);
				   gsm_send(mobilenumber,"unauthorized access");
			    }
           }	 
         }
     }
}
int verify_number(char *buffer)
{
  int i=0,cnt=0;
  while(buffer[i]!='9')i++;
  i=i+2;
  while(cnt<10)
  {
    new_buffer[cnt++]=buffer[i++];
  }
  new_buffer[cnt]='\0';
  if(strcmp(new_buffer,mobilenumber)==0)
     return 0;
  return 1;

}
