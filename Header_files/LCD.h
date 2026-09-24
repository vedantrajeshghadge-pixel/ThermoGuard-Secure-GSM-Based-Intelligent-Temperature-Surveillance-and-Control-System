//LCD.h
void write_lcd(int data);
void cmd_lcd(int data);
void char_lcd(int data);
void LCD_INIT(void);
void str_lcd(char *str);
void int_LCD(int n);
void neg_lcd(int n);
void float_lcd(float f,int nDP);
void delay_ms(unsigned int dly);
