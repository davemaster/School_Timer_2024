#define DS1307_I2C_WRITE_ADDR 0xd0
#define DS1307_I2C_READ_ADDR 0xd1

//#define RTC_SDA PIN_D1
//#define RTC_SCL PIN_D0

#define RTC_SDA  PIN_C4
#define RTC_SCL  PIN_C3 

#USE I2C (Master, SDA=RTC_SDA, SCL=RTC_SCL,SLOW)

//contiene tiempos a escribir y leidos del RTC ds1307
static byte tiempo[]={0x00,0x00,0x0,0x00,28,02,07,0x00};

//////////////////////////Declaracion funciones DS1307///////////////////////////
void Init_ds1307();
void ds1307_start();
byte ds1307_read_byte(byte address);
void ds1307_write_byte(byte address, byte data);
void Setup_ds1307(byte year,byte month,byte day,byte nday,byte hrs,byte min);
void leerTiempo();
////////////////////// Definicion Funciones DS1307//////////////////////////////

void ds1307_start()
{
   output_float(rtc_scl);
   output_float(rtc_sda);
}

byte ds1307_read_byte(byte address)
{
   byte data;
   i2c_start();
   i2c_write(DS1307_I2C_WRITE_ADDR);
   i2c_write(address);
   i2c_start();
   i2c_write(DS1307_I2C_READ_ADDR);
   data = i2c_read(0);
   i2c_stop();
   return(data);

}

void ds1307_write_byte(byte address, byte data)
{
   i2c_start();
   i2c_write(DS1307_I2C_WRITE_ADDR);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();

}

void Setup_ds1307(byte year,byte month,byte day,byte nday,byte hrs,byte min)
{
   ds1307_write_byte(0x01, min);
   delay_us(40);
   ds1307_write_byte(0x02, hrs);
   delay_us(40);
   ds1307_write_byte(0x03, nday);
   delay_us(40);
   ds1307_write_byte(0x04, day);
   delay_us(40);
   ds1307_write_byte(0x05, month);
   delay_us(40);
   ds1307_write_byte(0x06, year);
   delay_us(40);
}

void leerTiempo()
{
   tiempo[0]=ds1307_read_byte(0);   //segundos
   delay_us(40);
   tiempo[1]=ds1307_read_byte(1);   //minutos
   delay_us(40);
   tiempo[2]=ds1307_read_byte(2);   //horas
   delay_us(40);
   tiempo[3]=ds1307_read_byte(3);   //ndia
   delay_us(40);
   tiempo[4]=ds1307_read_byte(4);   //dia
   delay_us(40);
   tiempo[5]=ds1307_read_byte(5);   //mes
   delay_us(40);
   tiempo[6]=ds1307_read_byte(6);   //año

}

BYTE bin2bcd(BYTE binary_value)
{
  BYTE temp;
  BYTE retval;

  temp = binary_value;
  retval = 0;

  while(1)
  {
    // Get the tens digit by doing multiple subtraction
    // of 10 from the binary value.
    if(temp >= 10)
    {
      temp -= 10;
      retval += 0x10;
    }
    else // Get the ones digit by adding the remainder.
    {
      retval += temp;
      break;
    }
  }

  return(retval);
} 

// Input range - 00 to 99.
BYTE bcd2bin(BYTE bcd_value)
{
  BYTE temp;

  temp = bcd_value;
  // Shifting upper digit right by 1 is same as multiplying by 8.
  temp >>= 1;
  // Isolate the bits for the upper digit.
  temp &= 0x78;

  // Now return: (Tens * 8) + (Tens * 2) + Ones

  return(temp + (temp >> 2) + (bcd_value & 0x0f));
}



void ds1307_get_date(BYTE &day, BYTE &mth, BYTE &year, BYTE &dow)
{
  i2c_start();
  i2c_write(DS1307_I2C_WRITE_ADDR);
  delay_us(50);
  i2c_write(0x03);            // Start at REG 3 - Day of week
  delay_us(50);
  i2c_start();
  i2c_write(DS1307_I2C_READ_ADDR);
  dow  = bcd2bin(i2c_read() & 0x7f);   // REG 3
  day  = bcd2bin(i2c_read() & 0x3f);   // REG 4
  mth  = bcd2bin(i2c_read() & 0x1f);   // REG 5
  year = bcd2bin(i2c_read(0));            // REG 6
  i2c_stop();
}

void ds1307_get_time(BYTE &hr, BYTE &min, BYTE &sec)
{
  i2c_start();
  i2c_write(DS1307_I2C_WRITE_ADDR);
  delay_us(50);
  i2c_write(0x00);            // Start at REG 0 - Seconds
  delay_us(50);
  i2c_start();
  i2c_write(DS1307_I2C_READ_ADDR);
  sec = bcd2bin(i2c_read() & 0x7f);
  min = bcd2bin(i2c_read() & 0x7f);
  hr  = bcd2bin(i2c_read(0) & 0x3f);
  i2c_stop();

} 

void ds1307_set_date_time(BYTE day, BYTE mth, BYTE year, BYTE dow, BYTE hr, BYTE min, BYTE sec)
{
  sec &= 0x7F;
  hr &= 0x3F;

  i2c_start();
  i2c_write(DS1307_I2C_WRITE_ADDR);            // I2C write address  
  i2c_write(0x00);            // Start at REG 0 - Seconds
  i2c_write(bin2bcd(sec));      // REG 0
  delay_us(50);
  i2c_write(bin2bcd(min));      // REG 1
  delay_us(50);
  i2c_write(bin2bcd(hr));      // REG 2
  delay_us(50);
  i2c_write(bin2bcd(dow));      // REG 3
  delay_us(50);
  i2c_write(bin2bcd(day));      // REG 4
  delay_us(50);
  i2c_write(bin2bcd(mth));      // REG 5
  delay_us(50);
  i2c_write(bin2bcd(year));      // REG 6
  i2c_write(0x80);            // REG 7 - Disable squarewave output pin
  i2c_stop();
} 


