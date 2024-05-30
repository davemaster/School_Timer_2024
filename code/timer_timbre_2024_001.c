//************************************************************************************************
//Programa        : timer006_2022_1.c 
//Programador     : David E. Flores Escalante
//Fecha           : sabado 03/08/2020
//Funcionalidad   : 
//Observación     : 
//
//************************************************************************************************

#include <16F877A.h>

#device *=16 //ADC=16

#use delay ( clock = 4000000 )

#fuses XT, NOPROTECT, PUT, NOWDT, NOLVP,BROWNOUT//NOBROWNOUT

////////////////////////////// Funcion Principal /////a/////////////////////////////////
#use standard_io(A)
#use fast_io(B)
#use standard_io(C)
#use standard_io(D)

#rom 0x2100={
             0,0,    // evento 1 hora y minuto
             0,30,    // evento 2 hora y minuto
             1,0,    // evento 3 hora y minuto
             1,30,    // evento 4 hora y minuto
             2,0,    // evento 5 hora y minuto
             2,30,    // evento 6 hora y minuto
             17,0,    // evento 7 hora y minuto
             17,30,    // evento 8 hora y minuto
             18,0,    // evento 9 hora y minuto
             18,30,    // evento 10 hora y minuto
             19,0,    // evento 11 hora y minuto
             19,30,  // evento 12 hora y minuto                 
             20,0,    // evento 13 hora y minuto
             20,30,    // evento 14 hora y minuto
             21,0,    // evento 15 hora y minuto
             21,30,    // evento 16 hora y minuto
             22,0,    // evento 17 hora y minuto
             22,30,    // evento 18 hora y minuto
             23,0,    // evento 19 hora y minuto
             23,30}   // evento 20 hora y minuto

/////////////////////////// Variables Globales //////////////////////////////////
#define VAL_T1 65536-32768//62500 + 39 // para llegar a 25 milisegundos

#define MAX_EVENT 12//antes, 20      nº eventos  
#define DIR_EVENT 0

#define LCD_LIGHT PIN_B1               //pin para encender backlight del LCD
#define PWR_LED   PIN_C5               //pin para enceder power led

#define relay_timbre    PIN_A0

///-///////////////////////// botonera /////////////////////////////////////////
#define pENTER  PIN_B4
#define pCONFIG PIN_B5
#define pUP     PIN_B6
#define pDOWN   PIN_B7


#define BTN_TIMEOUT     65 //75
#define BTN_DEBOUNCE    30 //40

#define TIMBRE_DURACION 30             // 39 son 15 segundos; 60 son 30 segs

static int1 f_seg=true;
static int1 blinked=false;
//static int1 configurado=false;
static int1 modificado=false;

static unsigned int alHor;
static unsigned int alMin;
static int timbre=0;
static int lastHor=60;
static int lastMin=60;
//static int1 ListaEventos[MAX_EVENT]={false};

static byte horas=12,mins=0,segs=0;
//static byte ndia=1,dias=1,mes=1,anno=11;
   
static int cEvento=0;

// Test whether a given year is a leap year.
// This optimized version only works for the period 2001 - 2099
////////////////////////////////////////////////////////////////////////////////
#define IS_LEAP(year) (year%4 == 0) 

#include "lcd-dav.c"                    //libreria dav lcd
#include "ds1307-02-dav.c"              //libreria dav rtc ds1307
#include "mensajes.c"                   //mensajes
#include "time_functions.c"             //tonos
//#include "tones-dav.c"                  //tonos

/////////////////////////// Declaracion de Funciones //////////////////////////////////
void iniciar();
void deteccion();
void configurar();
void configuraTiempo();
void encender();
void apagar();
void tonos(int n);

////////////////////////////////////////////////////////////////////////////////

#INT_TIMER1      //
void tmr1_isr()  // cada segundo
{   
   f_seg=1;
   
   if(timbre==1||timbre==0)
   {
      timbre=0;
      output_low(relay_timbre);
   }
   else
   {
      timbre--;
      //output_high(out1);
   }
   
}


//////////////////////////////////////////////////////////////////////////////
////// Funcion Principal
//////////////////////////////////////////////////////////////////////////////
void main()
{
   iniciar();
   mensajeBienvenida();
   
   delay_ms(3000); 
            
   while(TRUE)                         //Hacer indefinidamente
   {     
      if(!input(pENTER))   // si TECLA ENTER presionada, hacer
      {
         cEvento=0;
         while(!input(pENTER))   // mientras TECLA ENTER es presionada
         {
            evento(cEvento);
            delay_ms(500);
            cEvento++;
            if(cEvento==MAX_EVENT)
               cEvento=0;
         }
         delay_ms(1000);         
         
      }
      
      else if(!input(pCONFIG))   // si TECLA CONFIG presionada, hacer
      {         
         while(!input(pCONFIG)); // mientras TECLA CONFIG es presionada
         configuraTiempo();
         delay_ms(1000);  
      }
      
      else if(f_seg)
      {
         f_seg=0;
         
         //leerTiempo();
         ds1307_get_time(horas,mins,segs);   
         //ds1307_get_date(dias,mes,anno,ndia); 
         mensajeModelo();
         muestraTiempo();
         chequeaAlarma();
         if(timbre)
         {
           /*generate_tone(500, 100);
           generate_tone(600, 100);
           generate_tone(500, 100);
           generate_tone(600, 100);
           generate_tone(500, 100);
           generate_tone(600, 100);*/
         }
      }
      
   }
}

//////////////////////////////////////////////////////////////////////////////
////// Iniciar Timer
//////////////////////////////////////////////////////////////////////////////
void iniciar()
{
   delay_ms(500);
   
   set_tris_a(0);
   set_tris_b(0b11110000);
   set_tris_d(0b00000000);    
   
   //output_a(0);
   output_b(0);
   //output_d(0);
   output_high(PWR_LED);
   
   bit_set(tiempo[0],7);
   

   //inicia i2s con el rtc ds1307
   ds1307_start();
   
   //lee hora del rtc ds1307
   leerTiempo();

   //si el RTC esta OFF, ponerlo en ON
   if (bit_test(tiempo[0],7))
   {
         ds1307_write_byte(0,tiempo[0] & 0x7F); // enable oscillator(bit 7 =0)   0111 1111         
   }
   
   /*if(tiempo[0]==0&&tiempo[1]==0&&tiempo[2]==0&&tiempo[3]==0&&
      tiempo[4]==0&&tiempo[5]==0&&tiempo[6]==0)
   {
      tiempo[0]=tiempo[1]=0;
      tiempo[2]=12;
      tiempo[3]=tiempo[4]=tiempo[5]=1;
      tiempo[6]=11;
      ds1307_set_date_time(tiempo[4], tiempo[5], tiempo[6], tiempo[3], tiempo[2], tiempo[1], tiempo[0]);
   
   }*/

   //iniciar la pantalla LCD del MARKADOR
   LCD_init();
   config_tmr1();
      
   //enable_interrupts(GLOBAL);  
     
}



