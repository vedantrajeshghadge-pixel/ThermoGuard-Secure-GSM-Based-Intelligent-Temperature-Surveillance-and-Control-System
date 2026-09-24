#include<string.h>
#include"i2c.h"
#include"LCD.h"
#include"Delay.h"
#include"gsm.h"
char passkey[]="7580";
extern char mobilenumber[11];
extern int temp,setpoint;
int var,digit;
int check_syntax(char *message)
{
	int i;
	//passkey length
	if(strlen(message)<6)
		return 0;
	for(i=0;i<4;i++)
	{
		if(message[i]!=passkey[i])
			return 0;
	}
	if(message[4]!='T'&&message[4]!='M'&&message[4]!='I'&&message[4]!='S')
		return 0;
	if(message[4]=='T')
	{
		i=5;
		while(message[i]!='$')
		{
			if(message[i]<'0'||message[i]>'9')
				return 0;
			i++;
		}
	}
	else if(message[4]=='M')
	{
		int digits=0;
		i=5;
		while(message[i]!='$')
		{
			if(message[i]<'0'||message[i]>'9')
				return 0;
			digits++;
			i++;
		}
		if(digits!=10)
			return 0;
	}
	else if(message[4]=='I')
	{
		if(message[5]!='$')
			return 0;
	}
	/*
	  else if(message[4]=='D')
	{
		if(message[5]!='$')

			return 0;
	}

	*/
	
   else if(message[4]=='S')
	{
		if(message[5]!='$')
			return 0;
	}
	else
		return 0;
	return 1;
}
void read(char *message)
{
	int i,j;
	//char number[11];
	if(message[4]=='T')
	{
		i=5,setpoint=0;
		while(message[i]!='$')
		{
			setpoint=(setpoint*10)+(message[i]-48);
			i++;
		}
		i2c_byte_write(0x50,0x01,setpoint);
    delay_ms(10);
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		str_lcd("Setpoint");
		delay_ms(1000);
		cmd_lcd(0xc0);
		int_LCD(setpoint);
		delay_ms(1000);
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		str_lcd("updated");
		delay_ms(1000);
	}
	else if(message[4]=='M')
	{
		i=5,j=0;
		while(message[i]!='$')
		{
			mobilenumber[j]=message[i];
      i++;
      j++;
		}
     mobilenumber[j]='\0';
     i2c_page_write(0x50,0x02,mobilenumber,10);
		delay_ms(10);
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		str_lcd("Number");
		cmd_lcd(0xc0);
		str_lcd(mobilenumber);
		delay_ms(1000);
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		str_lcd("updated");
		delay_ms(2000);
	}
	else if(message[4]=='I')
	{
		char msg[20];
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		int_LCD(temp);
		delay_ms(5000);
		msg[0]='T';
		msg[1]=':';
		var=temp;
		i=2;
		while(var)
		{
		 digit=var%10;
		 var=var/10;
		 i++;
		//msg[i++]=digit+48;
		//msg[3]=(temp%10)+48;
		}
		msg[i]='\0';
		var=temp;
		while(--i>1)
		{
		  digit=var%10;
		  msg[i]=digit+48;
		  var=var/10;
		 }
		 gsm_send(mobilenumber,msg);
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		str_lcd("SENT");
		delay_ms(1000);	
   }
    else if(message[4]=='S')
    {
    char msg[40],rev[10];
    int temp_setpoint;
    int i = 0,k=0,j=0;
    msg[i++]='S';
    msg[i++]='P';
    msg[i++]=':';

    temp_setpoint = setpoint;
        while(temp_setpoint)
        {
            rev[j++] = (temp_setpoint % 10) + '0';
            temp_setpoint /= 10;
        }

        while(j>0)
        {
            msg[i++] = rev[--j];
        }
    msg[i++]=' ';
    msg[i++]='M';
    msg[i++]='N';
    msg[i++]=':';

    for(k=0;k<10;k++)
    {
        msg[i++] = mobilenumber[k];
    }

    msg[i]='\0';

    gsm_send(mobilenumber,msg);

    cmd_lcd(0x01);
    cmd_lcd(0x80);
    str_lcd("STATUS SENT");
    delay_ms(1000);
}
}
