
void config_tmr1()
{
   //setup_timer_1(T1_EXTERNAL|T1_CLK_OUT);      
   setup_timer_1( T1_INTERNAL | T1_DIV_BY_8 ); // initialize 16-bit Timer1 to interrupt
                                              // exactly every 65536 clock cycles
                                              // (about 76 times per second)
     
  enable_interrupts( INT_TIMER1 );            // Start RTC 
  enable_interrupts(GLOBAL); 
}

void evento(int i)
{
   int dir;
   int timeout=BTN_TIMEOUT;
   int actual=0;
   
   dir=DIR_EVENT+i*2;   
   
   alHor=read_eeprom(dir);
   alMin=read_eeprom(dir+1);   
   
   LCD_SetPosition(FIRST_LINE);
   printf(LCD_PutChar,"   EVENTO #%u    ",i+1);
   LCD_SetPosition(SECOND_LINE);
   printf(LCD_PutChar,"     %02u:%02u     ",alHor,alMin);
   delay_ms(1000);
   
   modificado=false;
   
   do
   {            
      if(!input(pCONFIG))
      {
         while(!input(pCONFIG));
         timeout=BTN_TIMEOUT;
         delay_ms(BTN_DEBOUNCE);
         actual++;
         if(actual==2)
            actual=0;
      }
      
      else if(!input(pUP))
      {
         while(!input(pUP));
         
         timeout=BTN_TIMEOUT;
         delay_ms(BTN_DEBOUNCE);
         modificado=true;
         if(actual==0)
         {
            alHor++;
            if(alHor>23)
               alHor=0;         
         }
         
         else if(actual==1)
         {
            alMin++;
            if(alMin>59)
               alMin=0;
         }         
      }
      
      else if(!input(pDOWN))
      {
         while(!input(pDOWN));
         timeout=BTN_TIMEOUT;
         delay_ms(BTN_DEBOUNCE);
         modificado=true;
         
         if(actual==0)
         {
            
            if(alHor==0)
               alHor=23;                           
            else 
               alHor--;                   
         }
         
         else if(actual==1)
         {
            
            if(alMin==0)
               alMin=59;                           
            else 
               alMin--;
         }                
      }
      
      else
      {
         LCD_SetPosition(SECOND_LINE);
         
         if(actual==0)
         {
            if(!blinked)
            {
               blinked=true;
               printf(LCD_PutChar,"     %02u:%02u     ",alHor,alMin);
            }
            else
            {
               blinked=false;
               printf(LCD_PutChar,"       :%02u     ",alMin);
            }         
         }         
         
         else if(actual==1)
         {
            if(!blinked)
            {
               blinked=true;
               printf(LCD_PutChar,"     %02u:%02u     ",alHor,alMin);
            }
            else
            {
               blinked=false;
               printf(LCD_PutChar,"     %02u:       ",alHor);
            }         
         }
      }
   }  
   while(input(pENTER)&&timeout-->1);
   
   if(!input(pENTER))
   {
      if(modificado)
      {
         modificado=false;
         write_eeprom(dir,alHor);
         write_eeprom(dir+1,alMin);
      }               
   }
}

void configuraTiempo()
{
   static int timeout=BTN_TIMEOUT;
   int actual=0;
      
   byte auxhor=12,auxmin=0,auxseg=0;
   byte auxndia=1,auxdia=1,auxmes=1,auxanno=11;
      
   ds1307_get_time(auxhor,auxmin,auxseg);   
   ds1307_get_date(auxdia,auxmes,auxanno,auxndia); 
      
   modificado=false;
   mensajeconfig();
   
   do
   {       
      if(!input(pCONFIG))
      {
         while(!input(pCONFIG));
         delay_ms(BTN_DEBOUNCE);
         delay_ms(BTN_DEBOUNCE);
         delay_ms(BTN_DEBOUNCE);
         timeout=BTN_TIMEOUT;         
         
         actual++;    
         if(actual==6)
            actual=0;                     
      }
      
      else if(!input(pUP))
      {
         modificado=true;
         
         while(!input(pUP));
         
         delay_ms(BTN_DEBOUNCE);
         delay_ms(BTN_DEBOUNCE);
         delay_ms(BTN_DEBOUNCE);
         timeout=BTN_TIMEOUT; 
         
         if(actual==0)
         {
            auxhor++;
            if(auxhor>23)
            {
               auxhor=0;
            }
         }
         
         else if(actual==1)
         {
            auxmin++;
            if(auxmin>59)
            {               
               auxmin=0;    
            }
         }         
         
         else if(actual==2)
         {
            auxseg++;
            if(auxseg>59)
            {
               auxseg=0;
            }
         }        
         
         else if(actual==3)
         {
            auxdia++;
            
            if (  (auxdia == 29 && auxmes==2 && !IS_LEAP(auxanno))||  
                  (auxdia == 30 && auxmes==2)||  
                  (auxdia == 31 && (auxmes==4 || auxmes==6 || auxmes==9 || auxmes==11 ))||  
                  (auxdia == 32) ) 
            {              
               auxdia=1;
            }            
         }
         
         else if(actual==4)
         {
            auxmes++;
            
            if(auxmes == 13) 
            {               
               auxmes=1;
            }
         }   
         
         else if(actual==5)
         {
            auxanno++;
            
            if(auxanno == 30) 
            {
               auxanno=22; 
            }
         }            
         
      }
      
      else if(!input(pDOWN))
      {
         modificado=true;
         
         while(!input(pDOWN));
         
         delay_ms(BTN_DEBOUNCE);
         delay_ms(BTN_DEBOUNCE);
         delay_ms(BTN_DEBOUNCE);
         timeout=BTN_TIMEOUT; 
         
         if(actual==0)
         {            
            if(auxhor==0)
            {
               auxhor=23;
            }
            else
               auxhor--;
         }
         
         else if(actual==1)
         {
            
            if(auxmin==0)
            {
               auxmin=59;
            }
            else
               auxmin--;
         }
         
         else if(actual==2)
         {            
            if(auxseg==0)
            {
               auxseg=59;
            }
            else
               auxseg--;
         }
         
         else if(actual==3)
         {
            auxdia--;
                             
            if(auxmes==2)
            {
               if(IS_LEAP(auxanno))
                  auxdia=29;
               else
                  auxdia=28;
            }
            else if (auxmes==4 || auxmes==6 || auxmes==9 || auxmes==11 )
            {
               auxdia=30;
            }
            else                  
               auxdia=31;               
         }
         
         else if(actual==4)
         {
            auxmes--;
            
            if ( auxmes==0)
            {
               auxmes=12;
            }       
         }
         
      }
      
      else
      {
         LCD_SetPosition(SECOND_LINE);
         
         if(actual<=2)
         {
            if(!blinked)
            {       
               blinked=true;
               printf(LCD_PutChar,"Hora %02d:%02d:%02d  ",auxhor,auxmin,auxseg);        
            }
            else
            {
               blinked=false;
               if(actual==0)
               {
                  printf(LCD_PutChar,"Hora   :%02d:%02d  ",auxmin,auxseg);        
               }
               
               else if(actual==1)
               {
                  printf(LCD_PutChar,"Hora %02d:  :%02d  ",auxhor,auxseg);        
               }
               
               else if(actual==2)
               {
                  printf(LCD_PutChar,"Hora %02d:%02d:    ",auxhor,auxmin);        
               }               
            }
            
         }
         else
         {
            if(!blinked)
            {       
               blinked=true;               
               printf(LCD_PutChar,"Fecha %02d/%02d/%02d ",auxdia,auxmes,auxanno);        
            }
            else
            {
               blinked=false;
               if(actual==3)
               {
                  printf(LCD_PutChar,"Fecha   /%02d/%02d ",auxmes,auxanno);        
               }
               
               else if(actual==4)
               {
                  printf(LCD_PutChar,"Fecha %02d/  /%02d ",auxdia,auxanno);        
               }
               
               else if(actual==5)
               {
                  printf(LCD_PutChar,"Fecha %02d/%02d/   ",auxdia,auxmes);        
               }               
            }
         }
         delay_ms(100);
      }
   }
   while(input(pENTER)&&timeout-->1);
   
   if(!input(pENTER))
   {         
      while(!input(pENTER));
      if(modificado)
      {
         modificado=false;      
         ds1307_set_date_time(auxdia, auxmes, auxanno, auxndia, auxhor, auxmin, auxseg);
         LCD_SetPosition(FIRST_LINE);
         printf(LCD_PutChar," Hora %02d:%02d:%02d  ",auxhor,auxmin,auxseg);
         LCD_SetPosition(SECOND_LINE);
         printf(LCD_PutChar," Fecha %02d/%02d/%02d ",auxdia,auxmes,auxanno);        
      }               
   }
   delay_ms(1000);

}

void chequeaAlarma()
{
   int dir;
   int i;
   
   for(i=0;i<MAX_EVENT;i++)
   {      
      dir=DIR_EVENT+i*2;
      alHor=read_eeprom(dir);
      alMin=read_eeprom(dir+1);
      
      //if(!ListaEventos[i])
      {
         //if(alhor==tiempo[2]&&almin==tiempo[1])
         if(alhor==horas&&almin==mins)
         {
            //ListaEventos[i]=true;
            if(lastHor==alhor&&lastMin==almin)
               return;
            
            if(timbre==0)
            {
               lastHor=alhor;
               lastMin=almin;
               timbre=TIMBRE_DURACION;            
               output_high(relay_timbre);               
               return;
            }      
         }           
      }
   }
   if(!timbre)
   {
      output_low(relay_timbre);      //si no coincide con ninguno apaga salida
      
   }
}


