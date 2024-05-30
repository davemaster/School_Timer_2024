
#include "lcd-dav.h"

/////////////////////////Definicion Funciones LCD //////////////////////////////

void LCD_Init ( void )
{   
    LCD_SetData ( 0x00 );
    delay_ms ( 100 );       /* wait enough time after Vdd rise */
    output_low ( LCD_RS );
    output_low ( LCD_RW );
    LCD_SetData ( 0x03 );   /* init with specific nibbles to start 4-bit mode */
    LCD_PulseEnable();
    delay_ms ( 5 );
    LCD_PulseEnable();
    delay_us ( 200 );    
    LCD_PulseEnable();
    delay_us ( 200 );        
    LCD_SetData ( 0x02 );   /* set 4-bit interface */    
    //LCD_SetData ( 0x28 );   /* set 4-bit interface */    
    LCD_PulseEnable();      /* send dual nibbles hereafter, MSN first */
    //LCD_PutCmd ( 0x38 );    /* function set (2 lines, 5x7 characters) */
    LCD_PutCmd (0x20 | (2<< 2));
    //LCD_PutCmd ( 0x0E );    /* display ON, cursor on, no blink */
    LCD_PutCmd ( 0x0C );    // display ON, cursor off, no blink 0000 1100
    LCD_PutCmd ( 0x06 );    /* entry mode set, increment & scroll left */
    LCD_PutCmd ( 0x01 );    /* clear display */
}

void LCD_Clear()
{
   LCD_PutCmd ( 0x01 );
}

void LCD_off ( void )
    {
    LCD_SetData ( 0x00 );
    delay_ms ( 200 );       /* wait enough time after Vdd rise */
    output_low ( LCD_RS );
    LCD_SetData ( 0x03 );   /* init with specific nibbles to start 4-bit mode */
    LCD_PulseEnable();
    LCD_PulseEnable();
    LCD_PulseEnable();
    LCD_SetData ( 0x38 );   /* set 4-bit interface */
    LCD_PulseEnable();      /* send dual nibbles hereafter, MSN first */
    LCD_PutCmd ( 0x1C );    /* function set (2 lines, 5x7 characters) */
    LCD_PutCmd ( 0x08 );    /* display OFF, cursor off, no blink */
    LCD_PutCmd ( 0x06 );    /* entry mode set, increment & scroll left */
    LCD_PutCmd ( 0x01 );    /* clear display */
    }

void LCD_SetPosition ( unsigned int cX )
    {
    /* this subroutine works specifically for 4-bit Port A */
    LCD_SetData ( swap ( cX ) | 0x08 ); //0x81
    LCD_PulseEnable();
    LCD_SetData ( swap ( cX ) );
    LCD_PulseEnable();
    }

void LCD_PutChar ( unsigned int cX )
    {
    /* this subroutine works specifically for 4-bit Port A */
    output_high ( LCD_RS );
    LCD_SetData ( swap ( cX ) );     /* envia high nibble */
    LCD_PulseEnable();
    LCD_SetData ( swap ( cX ) );     /* envia low nibble */
    LCD_PulseEnable();
    output_low ( LCD_RS );
    }

void LCD_PutCmd ( unsigned int cX )
    {
    /* this subroutine works specifically for 4-bit Port A */
    LCD_SetData ( swap ( cX ) );     /* envia high nibble */
    LCD_PulseEnable();
    LCD_SetData ( swap ( cX ) );     /* envia low nibble */
    LCD_PulseEnable();
    }

void LCD_PulseEnable ( void )
{
   output_high ( LCD_EN );
   delay_us ( 10 );  //15
   output_low ( LCD_EN );
   delay_ms ( 5 );   //10
    
   /*delay_cycles(5);
   output_high ( LCD_EN );
   delay_us(10);
   output_low ( LCD_EN );*/
}

void LCD_SetData ( unsigned int cX )
    {
    output_bit ( LCD_D4, cX & 0x01 ); //00000001
    output_bit ( LCD_D5, cX & 0x02 ); //00000010
    output_bit ( LCD_D6, cX & 0x04 ); //00000100
    output_bit ( LCD_D7, cX & 0x08 ); //00001000
    }

