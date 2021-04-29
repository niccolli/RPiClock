/*
 * SPIなどで時刻情報を受け取ったら、Timer2でインクリメントとLED制御をする。
 * 常時通信しっぱなしにすることはない。
 */
#include <MsTimer2.h>
#include <SPI.h>
volatile int spi_count = 0;
unsigned char spi_receive[3];

// DP_G_F_E_D_C_B_A
#define LED_SEG_0   0b00111111
#define LED_SEG_1   0b00000110
#define LED_SEG_2   0b01011011
#define LED_SEG_3   0b01001111
#define LED_SEG_4   0b01100110
#define LED_SEG_5   0b01101101
#define LED_SEG_6   0b01111101
#define LED_SEG_7   0b00000111
#define LED_SEG_8   0b01111111
#define LED_SEG_9   0b01101111
#define LED_SEG_COL 0b00000011
#define LED_SEG_OFF 0b00000000

const unsigned char LED_COLS[] = {A1, A2, A3, A4, A5};
const unsigned char LED_SEGS[] = {2, 3, 4, 5, 6, 7, 8, 9};

const unsigned char LED_SEG_NUM[] = {LED_SEG_0, LED_SEG_1, LED_SEG_2, LED_SEG_3, LED_SEG_4, LED_SEG_5, LED_SEG_6, LED_SEG_7, LED_SEG_8, LED_SEG_9};

unsigned char col = 0;
unsigned char time_second = 20;
unsigned char time_minute = 34;
unsigned char time_hour   = 21;

void time(){
  time_second++;
  if(time_second >= 60){
    time_second = 0;
    time_minute++;
  }
  if(time_minute >= 60){
    time_minute = 0;
    time_hour++;
  }
  if(time_hour >= 24){
    time_hour = 0;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(SS, INPUT_PULLUP);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, INPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();  //allows SPI interrupt

  for (size_t i = 0; i < sizeof(LED_COLS); i++)
  {
    pinMode(LED_COLS[i], OUTPUT);
    digitalWrite(LED_COLS[i], LOW);
  }
  for (size_t i = 0; i < sizeof(LED_SEGS); i++)
  {
    pinMode(LED_SEGS[i], OUTPUT);
    digitalWrite(LED_SEGS[i], HIGH);
  }
  
  MsTimer2::set(1000, time);
  MsTimer2::start();
}

void DriveLED(){
  digitalWrite(LED_COLS[col], LOW);
  unsigned char num = ~LED_SEG_OFF;
  col++;
  if(col >= sizeof(LED_COLS)){
    col = 0;
  }

  switch (col){
    case 0: // Hour 10の桁
      num = ~LED_SEG_NUM[time_hour/10];    
      break;
    case 1: // Hour 1の桁
      num = ~LED_SEG_NUM[time_hour%10];    
      break;
    case 2: // Minute 10の桁
      num = ~LED_SEG_NUM[time_minute/10];    
      break;
    case 3: // Minute 1の桁
      num = ~LED_SEG_NUM[time_minute%10];    
      break;
    case 4: // コロン
      // 偶数秒はコロンを点灯
      if(time_second%2){
        num = ~LED_SEG_OFF;
      } else {
        num =~LED_SEG_COL;
      }
      break;
    default:
      break;
  }
  for (size_t i = 0; i < sizeof(LED_SEGS); i++)
  {
    unsigned char out = num & 0b00000001;
    digitalWrite(LED_SEGS[i], out);
    num >>= 1;
  }
  // 準備ができてから点灯
  digitalWrite(LED_COLS[col], HIGH);
}

void loop(void)
{
  // if (i == 2)
  // {
  //   int x = (int)myArray[0]<<8|(int)myArray[1];
  //   Serial.print("Received 16-bit data item from Master: ");
  //   Serial.println(x, HEX);
  //   i=0;
  //   Serial.println("=============================================");
  // }
  DriveLED();
  delay(1);
}

ISR (SPI_STC_vect)   //Inerrrput routine function
{
  spi_receive[spi_count] = SPDR;
  spi_count++;
  if(spi_count >= 3){
    time_hour   = spi_receive[0];
    time_minute = spi_receive[1];
    time_second = spi_receive[2];
    spi_count = 0;
  }
}
