#include <Ps3Controller.h>
#include <SPI.h>

// Debug LED
#define LED_PIN 15

/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓Controller Method↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/

/*****Macro*****/
#define ID_MODE_STOP 1
#define ID_MODE_NORMAL 2
#define ID_MODE_ORG 3
#define ID_MODE_RUNTOSTP 4

/*****Global Variable*****/
volatile unsigned char u1g_event_xcircle;
volatile unsigned char u1g_event_xcross;
volatile unsigned char u1g_event_xtriangle;
volatile unsigned char u1g_event_idmode = ID_MODE_STOP;

void controller_get()
{
  if ( Ps3.event.button_down.start )  { Serial.write(255); }
  if ( Ps3.event.button_down.circle ) { u1g_event_idmode = ID_MODE_NORMAL; }
  if ( Ps3.event.button_down.cross ) { u1g_event_idmode = ID_MODE_STOP; }
  if ( Ps3.event.button_down.triangle ) { u1g_event_idmode = ID_MODE_ORG; }
}

void controller_battery()
{
  switch ( Ps3.data.status.battery )
  {
    case ps3_status_battery_full:
    Ps3.setPlayer(4);
    break;
    case ps3_status_battery_high:
    Ps3.setPlayer(3);
    break;
    case ps3_status_battery_low:
    Ps3.setPlayer(2);
    break;
    case ps3_status_battery_dying:
    Ps3.setPlayer(1);
    break;
  }
}

/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑Controller Method↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/

/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓SPI Method↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/

/*****Macro*****/
#define SPI_SS_M1 16
#define SPI_SS_M2 17
#define SPI_CLK_SET 200000
#define BITMASK_MODE_STP (ID_MODE_STOP<<28)
#define BITMASK_MODE_RUN (ID_MODE_NORMAL<<28)
#define BITMASK_MODE_ORG (ID_MODE_ORG<<28)
#define BITMASK_MODE_RUNTOSTP (ID_MODE_RUNTOSTP<<28)
#define BITMASK_ROTDIR (1<<13)
#define BITMASK_NMTGT 0x1FFF
#define BITMASK_MODE 0xF0000000
#define BITMASK_BYTE 0xFF
#define BITSHIFT_NM2 14
#define BITSHIFT_MODE 28

/*****Global Variable*****/
uint32_t senddata = 123456789;
uint32_t recdata;
SPIClass * vspi = NULL;                 //uninitalised pointers to SPI objects

void spicommand(SPIClass *spi, uint32_t data)
{
  digitalWrite(SPI_SS_M1, LOW);    //Pull SS low
  recdata = spi->transfer32(data);
  digitalWrite(SPI_SS_M1, HIGH);   //Pull SS high
}

/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑SPI Method↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/

/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓Timer Method↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/

/*****Macro*****/
#define TIMER_MS 50

/*****Global Variable*****/
hw_timer_t * timer = NULL;

void IRAM_ATTR onTimer() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑Timer Method↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/

void setup()
{
  /*****Controller*****/
  Serial.begin(115200);
  while (!Serial);                  // 準備が終わるのを待つ
  Ps3.attach(autocar_cntget);
  Ps3.begin("FC:F5:C4:45:75:8E");

  /*****SPI*****/
  vspi = new SPIClass(VSPI);            //インスタンス生成
  vspi->begin();                      //生成したvspiインスタンスのポインタからbegin()関数コール(ポインタからメソッドコール時は「->」)
  pinMode(SPI_SS_M1, OUTPUT);   //vspiインスタンスのSSを任意ポート指定し出力設定
  vspi->beginTransaction(SPISettings(SPI_CLK_SET, SPI_MSBFIRST, SPI_MODE0));

  // Serial Monitor用Serial API設定
  // Serial.begin(115200);             // シリアル通信の準備をする
  // while (!Serial);                  // 準備が終わるのを待つ
  Serial.println("プログラム開始");    // シリアル通信でメッセージをPCに送信

  /*****Timer*****/
  pinMode(LED_PIN, OUTPUT);
  timer = timerBegin(0, getApbFrequency()/1000000, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, TIMER_MS*1000, true);
  timerAlarmEnable(timer);
}

void loop()
{
  /*****SPI Communication*****/
  spicommand(vspi, senddata);
  Serial.println(recdata);

  // while(Ps3.isConnected())
  // {
  //   while(1)
  //   {
  //     while(Serial.available())
  //     {
  //       /*****SPI Communication*****/
  //       spicommand(vspi, senddata);
  //       Serial.println(recdata);
      
  //       /*****Controller*****/
  //       Serial.write(Ps3.data.analog.stick.ly+128);
  //       Serial.write(Ps3.data.analog.stick.rx+128);
  //       Serial.write(u1g_event_idmode);
          //  controller_battery
  //     }
  //   }
  // }
}
