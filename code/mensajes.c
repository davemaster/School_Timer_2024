
void mensajeModelo()
{
   LCD_SetPosition(FIRST_LINE);
   LCD_PutChar  (" DaveWare Timer ");
   
}

void mensajeBienvenida()
{
   LCD_SetPosition(FIRST_LINE);
   LCD_PutChar  (" DaveWare Timer ");
   LCD_SetPosition(SECOND_LINE);
   LCD_PutChar  ("   Modelo 2024  ");   
}


void mensajeConfig()
{
   LCD_SetPosition(FIRST_LINE);
   LCD_PutChar("CONFIGURAR HORA ");
}

void muestraTiempo()
{
   LCD_SetPosition(SECOND_LINE);
   //printf(LCD_PutChar,"    %02X:%02X:%02X    ",tiempo[2],tiempo[1],tiempo[0]);
   printf(LCD_PutChar,"    %02d:%02d:%02d    ",horas,mins,segs);
   
}

void muestraEvento()
{
   LCD_SetPosition(SECOND_LINE);
   printf(LCD_PutChar,"Evento#%02d %02X:%02X",cEvento+1,alHor,alMin);
   
}

void ConfiguradoOK()
{
   LCD_SetPosition(SECOND_LINE);
   printf(LCD_PutChar," Reloj Cambiado ");
   
}
