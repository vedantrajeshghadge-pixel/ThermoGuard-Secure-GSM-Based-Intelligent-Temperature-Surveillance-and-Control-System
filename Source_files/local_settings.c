#include"Delay.h"
#include"LCD.h"
#include"keypad.h"
#include<string.h>
#include<stdlib.h>
#include"i2c.h"
#include"gsm.h"
void local_setpoint(void);
extern int setpoint;
extern char received_msg[50];
int set_point_flag=0;
extern char mobilenumber[11];
char password[5]="7580";
// Array to store entered password1
char input[5],set[11];

int check_password(void)
{
   int i=0;
   char key;
	 i=0;
		   // Clear LCD and display password prompt
	       cmd_lcd(0x01);
		   cmd_lcd(0x80);
		   str_lcd("Enter the pin:");
		   cmd_lcd(0xc0);
		 	// Read password input
		   while(1)
		  {
				 // Scan keypad key
			   key=keyscan();

			   if(key=='\0')
				    continue;
				 // Backspace operation
				if(key=='#')
			{
				if(i>0)
				{
					i--;
					cmd_lcd(0x10);
					char_lcd(' ');
					cmd_lcd(0x10);
				}
			}
			// Accept only numeric digits
			else if(key>='0' && key<='9')
			{
				// Accept maximum 4 digits
				if(i<4)
				{
			     input[i]=key;
			     char_lcd(key);
			     delay_ms(100);
					 // password hiding
			     cmd_lcd(0x10);
			     char_lcd('*');
			     i++;
				}
			 }
			// '*' key used as ENTER key
			else if(key == '*')   
      {
			  if(i==4)
			  {
           input[i] = '\0';
           break;
			  }				
     }
			// Wait until key release
			//while(colscan());
	  }
		    input[i]='\0';
		    cmd_lcd(0x01);
		    delay_ms(100);
		    // Compare entered password with stored password
		    if(strcmp(input,password)==0)
		    {
            // Open menu continuously
			     local_setpoint();
					      return 1;
		    }
		    else
		    {      
            // Wrong password message					
			      str_lcd("Access Denied");
			      delay_ms(1000);
				  cmd_lcd(0X01);
					  gsm_send(mobilenumber,"unauthorized local access attempted");
				    return 0;
		    }
}
void local_setpoint()
{
	char key;
	//int digit=0,var=0;
	int i=0; 
	char new_setpoint[10];
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("Enter setpoint");
	cmd_lcd(0xC0);
	cmd_lcd(0x01);
	while(1)
	{
		key=keyscan();
		if(key=='\0')
		  continue;
		 if(key=='#')
			{
				if(i>0)
				{
					i--;
					cmd_lcd(0x10);
					char_lcd(' ');
					cmd_lcd(0x10);
				}
			}
		if(key>='0' && key<='9')
		{
			//new_setpoint[i++]=(new_setpoint*10)+(key-'0');
			new_setpoint[i++]=key;
			char_lcd(key);
			set_point_flag=1;
			//i++;
		}
		else if(key=='*')
		{
		    new_setpoint[i]='\0';
			break;
		}
		//while(colscan());
	}

	if(set_point_flag==1)
	{
	//setpoint=new_setpoint;
	set_point_flag=0;
	//clear before update 
	i2c_byte_write(0x50,0x01,atoi(new_setpoint));
	delay_ms(10);
	gsm_send(mobilenumber,"Setpoint updated");
	delay_ms(1000);
	gsm_send(mobilenumber,new_setpoint);
	
		/*var=setpoint;
		i=0;
		while(var)
		{
		 digit=var%10;
		 var=var/10;
		 i++;
		//msg[i++]=digit+48;
		//msg[3]=(temp%10)+48;
		}
		set[i]='\0';
		var=setpoint;
		while(--i>1)
		{
		  digit=var%10;
		  set[i]=digit;
		  var=var/10;
		 }*/
		 /*while(
		 set[10]='\0';
		 gsm_send(mobilenumber,set);  */
	
	//gsm_send(mobilenumber,);
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("SMS SENT");
	delay_ms(1000);
	set_point_flag=0;
	cmd_lcd(0x01);
	}
}
