#ifndef LCD_DAV_H
#define LCD_DAV_H

#define LCD_D4      PIN_D4
#define LCD_D5      PIN_D5
#define LCD_D6      PIN_D6
#define LCD_D7      PIN_D7
#define LCD_EN      PIN_D3
#define LCD_RS      PIN_D2
#define LCD_RW      PIN_D1


#define FIRST_LINE  0X00
#define SECOND_LINE 0x40
#define CLEAR_DISP  0x01

//#define LCD_LIGHT PIN_B1               //pin para encender backlight del LCD

////////////////////////////////LCD ROUTINES////////////////////////////////////
void LCD_Init ( void );
void LCD_SetPosition ( unsigned int cX );
void LCD_PutChar ( unsigned int cX );
void LCD_PutCmd ( unsigned int cX );
void LCD_PulseEnable ( void );
void LCD_SetData ( unsigned int cX );
void LCD_Clear();

//LCD line address LCD 16x2
//Line 1     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
//Line 2     40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
//////////////////////////Declaracion funciones Recepcion/////////////////////////


#endif
