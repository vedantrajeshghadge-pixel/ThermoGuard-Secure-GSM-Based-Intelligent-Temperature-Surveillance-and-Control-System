#include<lpc21xx.h>
#include"uart_defines.h"
#include<string.h>
#include"uart.h"
#include"LCD.h"
char buffer[300],read_flag;
volatile int i=0,received_flag=0;
unsigned char ch,dummy;
void INIT_UART()
{
	//cfg p0.0,0.1 as TX,RX
	//clear previous pin settings
	PINSEL0&=~15;
	//update the TX,Rx to pins
	PINSEL0|=TXD0_PIN_EN|RXD0_PIN_EN;
	
	//Cfg UART frame for 8N1(8bits,0parity,1 Stopbit)
	U0LCR=(1<<DLAB_BIT)|WORD_LEN_SEL_BITS;
	
	//Baud rate cfg
	U0DLL= DIVISOR;
	U0DLM= DIVISOR>>8;
	
	//clear DLAB
	U0LCR&=~(1<<DLAB_BIT);
	
	//Enable UART0  interrupt for both receving and transmiting 
	U0IER= (0X03);
	/*cmd_lcd(0x80);
	str_lcd("UART INIT");
	delay_ms(500); */
}

void U0_TXBYTE(char SBYTE)
{
	//write to TX via TX_BUFFER
	U0THR=SBYTE;
	//wait until TX complete status
	while(((U0LSR>>TEMT_BIT)&1)==0);
}
void U0_TX_BYTE_string(char *SBYTE)
{
	while(*SBYTE)
	{
		U0_TXBYTE(*SBYTE++);
	}
	//write to TX via TX_BUFFER

	//U0THR =SBYTE;

	//wait until TX complete status

	while(((U0LSR>>TEMT_BIT)&1)==0);

}
void U0_TX_BYTE_hex(int data)

{

	int digit,rev=0;

	while(data)

	{

		digit=data%16;

		rev=rev*16+digit;

		data=data/16;

	}

	while(rev)

	{

		digit=rev%16;

		if(digit<10)

			U0_TXBYTE(digit+48);

		else

			U0_TXBYTE(digit+55);

		rev=rev/16;

  }

}

char U0_RX_BYTE()
{
	//wait until byte received and buffered
	while(((U0LSR>>DR)&1)==0);
	//return data stored in buffer
	return U0RBR;
}

void uart0_isr(void) __irq			 
{
	if((U0IIR & 0X0E)== 0X04)
	{
	    ch=U0RBR;
		if(ch!='\n'|| ch!='\r')
		{
	    buffer[i]=ch;
	    i++;
		//buffer[i]='\0';
		}
		if(strstr(buffer,"+CMTI")!=NULL)
		{						                                                         
		 received_flag=1;
		 //cmd_lcd(0x01);
		 //cmd_lcd(0x80);
		 //str_lcd("cmti");
		}	
		if(ch=='\n'|| ch=='\r')
		{
		   buffer[i]='\0';
		   read_flag=1;
		   //i=0;
		}
		 
    }
	else
	{
	  dummy=U0IIR;
	}
	VICVectAddr=0;

}
/*void uart0_isr(void) __irq			 

{

	if((U0IIR & 0X0E)== 0X04)
	{
	    ch=U0RBR;
	   	if(ch!='\r' && ch!='\n')
		{
	    buffer[i]=ch;
	    i++;
		//buffer[i]='\0';
	    }
		if(strstr(buffer,"+CMTI"))
		{
		 received_flag=1;
		 cmd_lcd(0x01);
		 cmd_lcd(0x80);
		 str_lcd("SMS");
		 delay_ms(1000);
		}
		else
		{
	       buffer[i]='\0';
		   //i=0;
		   read_flag=1;		  
		}  

    }

	else
	{
	  dummy=U0IIR;
	}

	VICVectAddr=0;

} */


void enable_uart0()
{
	//select the type of interrupt
	VICIntSelect=0x00000000;
	//Assign the isr address
	VICVectAddr0=(unsigned int)uart0_isr;
	//select the slot0 for UART0 and channel 6
	VICVectCntl0=(1<<5)| 6;
	//Enable UART0 interrupt
	VICIntEnable=(1<<6);
	cmd_lcd(0x80);
	str_lcd("uart intt INIT");
	delay_ms(500);
}
