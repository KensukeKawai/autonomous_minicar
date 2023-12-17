#include <Ps3Controller.h>
#include <SPI.h>

/***************Macro***************/
// Controller
#define ID_MODE_STP 1
#define ID_MODE_RUN 2
#define ID_MODE_ORG 3
#define ID_MODE_RUNTOSTP 4
#define ID_MODE_RECONLY 15    // 0xF0

// Communication with Motor Controller
#define SPI_SS_FRONT 16
#define SPI_SS_REAR 17
#define SPI_CLK_SET 200000
#define BITMASK_MODE_STP (ID_MODE_STP<<28)
#define BITMASK_MODE_RUN (ID_MODE_RUN<<28)
#define BITMASK_MODE_ORG (ID_MODE_ORG<<28)
#define BITMASK_MODE_RUNTOSTP (ID_MODE_RUNTOSTP<<28)
#define BITMASK_ROTDIR (1<<13)
#define BITMASK_NMTGT 0x1FFF
#define BITMASK_MODE 0xF0000000
#define BITMASK_BYTE 0xFF
#define BITSHIFT_NM2 14
#define BITSHIFT_MODE 28

// Jetson Receive
#define INCOMING_BYTE 8     // Jetsonからのシリアル通信データバイト数(1モータ4Byte×2=8Byte)

// Timer
#define TIMER_MS 50


/***************Global Variable***************/
// Controller
volatile uint8_t u1g_cntr_xcircle;
volatile uint8_t u1g_cntr_xcross;
volatile uint8_t u1g_cntr_xtriangle;
volatile uint8_t u1g_cntr_idmoderq = ID_MODE_STP;
volatile uint8_t u1g_cntr_idmoderqz = ID_MODE_STP;
volatile uint8_t u1g_cntr_idmodetgt = ID_MODE_STP;

// Motor Receive
uint32_t u4g_mtrec_fmtdata;
uint32_t u4g_mtrec_rmtdata;
uint8_t u1g_mtrec_fmtid;
uint8_t u1g_mtrec_rmtid;

// Motor Send
uint32_t u4g_mtsend_fmtdata;
uint32_t u4g_mtsend_rmtdata;
volatile uint8_t u1g_mtsend_idfmtset;
volatile uint8_t u1g_mtsend_idrmtset;

// Jetson Receive
uint32_t u4g_jetson_fmttgt;
uint32_t u4g_jetson_rmttgt;

// Jetson Send



// Communication with Motor Controller
uint32_t u4g_spi_fmtsend;
uint32_t u4g_spi_rmtsend;
SPIClass * vspi = NULL;                 //uninitalised pointers to SPI objects

// Timer
hw_timer_t * timer = NULL;

// Debug LED
#define LED_PIN 15

/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ Controller ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/
void vdg_controller_get()
{
  if ( Ps3.event.button_down.start )  { Serial.write(255); }
  if ( Ps3.event.button_down.circle ) { u1g_cntr_idmoderq = ID_MODE_RUN; }
  if ( Ps3.event.button_down.cross ) { u1g_cntr_idmoderq = ID_MODE_STP; }
  if ( Ps3.event.button_down.triangle ) { u1g_cntr_idmoderq = ID_MODE_ORG; }
}

void vdg_controller_battery()
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

void vdg_jetson_sendstr()
{
  Serial.write(Ps3.data.analog.stick.ly+128);
  Serial.write(Ps3.data.analog.stick.rx+128);
}
/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ Controller ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/

/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ Motor ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/

void vdg_mtcnt_sendrec()
{
  uint32_t u4t_mtrec_fmtdata;
  uint32_t u4t_mtrec_rmtdata;

  //Jetsonからのtgt情報をラッチ(更新されていなければ前回値をそのままラッチするだけ)
  u4g_mtsend_fmtdata = u4g_jetson_fmttgt;
  u4g_mtsend_rmtdata = u4g_jetson_rmttgt;

  /*****SPI Communication to Motor Controller*****/
  //Front
  digitalWrite(SPI_SS_FRONT, LOW);    //Pull SS low
  u4g_mtrec_fmtdata = vspi->transfer32(u4g_mtsend_fmtdata);
  digitalWrite(SPI_SS_FRONT, HIGH);   //Pull SS high

  //Rear
  digitalWrite(SPI_SS_REAR, LOW);    //Pull SS low
  u4g_mtrec_rmtdata = vspi->transfer32(u4g_mtsend_rmtdata);
  digitalWrite(SPI_SS_REAR, HIGH);   //Pull SS high

  //Get to Motor Status ID
  u4t_mtrec_fmtdata = u4g_mtrec_fmtdata;
  u4t_mtrec_rmtdata = u4g_mtrec_rmtdata;
  u1g_mtrec_fmtid = (uint8_t)(u4t_mtrec_fmtdata >> BITSHIFT_MODE);
  u1g_mtrec_rmtid = (uint8_t)(u4t_mtrec_rmtdata >> BITSHIFT_MODE);
}

void vdg_mtcnt_sendrec_dbg()
{
  static uint32_t u4s_mtsend_fmtdata;
  static uint32_t u4s_mtrec_fmtdata;

  u4s_mtsend_fmtdata = u4s_mtrec_fmtdata + 1;

  /*****SPI Communication to Motor Controller*****/
  //Front
  digitalWrite(SPI_SS_FRONT, LOW);    //Pull SS low
  u4s_mtrec_fmtdata = vspi->transfer32(u4s_mtsend_fmtdata);
  digitalWrite(SPI_SS_FRONT, HIGH);   //Pull SS high
}

void vdg_mtcnt_reconly()
{
  uint32_t u4t_mtrec_fmtdata;
  uint32_t u4t_mtrec_rmtdata;

  //Front
  digitalWrite(SPI_SS_FRONT, LOW);    //Pull SS low
  u4g_mtrec_fmtdata = vspi->transfer32(ID_MODE_RECONLY << BITSHIFT_MODE);
  digitalWrite(SPI_SS_FRONT, HIGH);   //Pull SS high

  //Rear
  digitalWrite(SPI_SS_REAR, LOW);    //Pull SS low
  u4g_mtrec_rmtdata = vspi->transfer32(ID_MODE_RECONLY << BITSHIFT_MODE);
  digitalWrite(SPI_SS_REAR, HIGH);   //Pull SS high

  u4t_mtrec_fmtdata = u4g_mtrec_fmtdata;
  u4t_mtrec_rmtdata = u4g_mtrec_rmtdata;
  u1g_mtrec_fmtid = (uint8_t)(u4t_mtrec_fmtdata >> BITSHIFT_MODE);
  u1g_mtrec_rmtid = (uint8_t)(u4t_mtrec_rmtdata >> BITSHIFT_MODE);
}

void vdg_mtcnt_idjdg()
{
  // 原点学習要求のトリガ処理
  if (u1g_cntr_idmoderqz == ID_MODE_ORG && u1g_cntr_idmoderq == ID_MODE_ORG)
  {
    u1g_cntr_idmodetgt = ID_MODE_STP;
  }
  else
  {
    u1g_cntr_idmodetgt = u1g_cntr_idmoderq;
  }
  u1g_cntr_idmoderqz = u1g_cntr_idmoderq;  // コントローラ要求の前回値ラッチ処理

  // モータ制御状態とコントローラ要求を調停
  switch (u1g_mtrec_fmtid)
  {
    case ID_MODE_STP:         // モータが停止中ならコントローラ要求をセット
      u1g_mtsend_idfmtset = u1g_cntr_idmodetgt;
    break;

    case ID_MODE_RUN:
      if (u1g_cntr_idmodetgt == ID_MODE_ORG) { u1g_mtsend_idfmtset = ID_MODE_RUN; }   // 原点学習要求をリジェクト
      else { u1g_mtsend_idfmtset = u1g_cntr_idmodetgt; }
    break;

    case ID_MODE_ORG:         // モータが原点学習中の場合はそのまま
      u1g_mtsend_idfmtset = ID_MODE_ORG;
    break;

    case ID_MODE_RUNTOSTP:
      u1g_mtsend_idfmtset = ID_MODE_STP;
    break;

    default:
      u1g_mtsend_idfmtset = ID_MODE_STP;
  }

  switch (u1g_mtrec_rmtid)
  {
    case ID_MODE_STP:
      u1g_mtsend_idrmtset = u1g_cntr_idmodetgt;
    break;

    case ID_MODE_RUN:
      if (u1g_cntr_idmodetgt == ID_MODE_ORG) { u1g_mtsend_idrmtset = ID_MODE_RUN; }
      else { u1g_mtsend_idrmtset = u1g_cntr_idmodetgt; }
    break;

    case ID_MODE_ORG:
      u1g_mtsend_idrmtset = ID_MODE_ORG;
    break;

    case ID_MODE_RUNTOSTP:
      u1g_mtsend_idrmtset = ID_MODE_STP;
    break;

    default:
      u1g_mtsend_idrmtset = ID_MODE_STP;
  }
}

/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ Motor ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/

/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ Communication with Jetson ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/

void vdg_jetson_rectgt()
{
  uint32_t u4t_jetson_fmtrecb0;
  uint32_t u4t_jetson_fmtrecb1;
  uint32_t u4t_jetson_fmtrecb2;
  uint32_t u4t_jetson_fmtrecb3;
  uint32_t u4t_jetson_rmtrecb0;
  uint32_t u4t_jetson_rmtrecb1;
  uint32_t u4t_jetson_rmtrecb2;
  uint32_t u4t_jetson_rmtrecb3;

  //Get to Front Motor Target Value
  u4t_jetson_fmtrecb0 = Serial.read();
  u4t_jetson_fmtrecb1 = Serial.read();
  u4t_jetson_fmtrecb2 = Serial.read();
  u4t_jetson_fmtrecb3 = Serial.read();
  u4g_jetson_fmttgt = u4t_jetson_fmtrecb0 | (u4t_jetson_fmtrecb1 << 8) | (u4t_jetson_fmtrecb2 << 16) | (u4t_jetson_fmtrecb3 << 24);

  //Get to Rear Motor Target Value
  u4t_jetson_rmtrecb0 = Serial.read();
  u4t_jetson_rmtrecb1 = Serial.read();
  u4t_jetson_rmtrecb2 = Serial.read();
  u4t_jetson_rmtrecb3 = Serial.read();
  u4g_jetson_rmttgt = u4t_jetson_rmtrecb0 | (u4t_jetson_rmtrecb1 << 8) | (u4t_jetson_rmtrecb2 << 16) | (u4t_jetson_rmtrecb3 << 24);
}

void vdg_jetson_sendmtsts()
{
  uint8_t u1t_jetson_fmtdatab0;
  uint8_t u1t_jetson_fmtdatab1;
  uint8_t u1t_jetson_fmtdatab2;
  uint8_t u1t_jetson_fmtdatab3;
  uint8_t u1t_jetson_rmtdatab0;
  uint8_t u1t_jetson_rmtdatab1;
  uint8_t u1t_jetson_rmtdatab2;
  uint8_t u1t_jetson_rmtdatab3;
  uint32_t u4t_jetson_fmtdata;
  uint32_t u4t_jetson_rmtdata;

  u4t_jetson_fmtdata = u4g_mtrec_fmtdata;
  u4t_jetson_rmtdata = u4g_mtrec_rmtdata;

  // b0:LSB、b3:MSB
  // Front Motor data
  u1t_jetson_fmtdatab0 = (uint8_t)((u4t_jetson_fmtdata & 0x000000FF) >> 0);
  u1t_jetson_fmtdatab1 = (uint8_t)((u4t_jetson_fmtdata & 0x0000FF00) >> 8);
  u1t_jetson_fmtdatab2 = (uint8_t)((u4t_jetson_fmtdata & 0x00FF0000) >> 16);
  u1t_jetson_fmtdatab3 = (uint8_t)((u4t_jetson_fmtdata & 0x0F000000) >> 24);
  u1t_jetson_fmtdatab3 = u1t_jetson_fmtdatab3 | (u1g_mtsend_idfmtset << 4);
  // Rear Motor data
  u1t_jetson_rmtdatab0 = (uint8_t)((u4t_jetson_rmtdata & 0x000000FF) >> 0);
  u1t_jetson_rmtdatab1 = (uint8_t)((u4t_jetson_rmtdata & 0x0000FF00) >> 8);
  u1t_jetson_rmtdatab2 = (uint8_t)((u4t_jetson_rmtdata & 0x00FF0000) >> 16);
  u1t_jetson_rmtdatab3 = (uint8_t)((u4t_jetson_rmtdata & 0x0F000000) >> 24);
  u1t_jetson_rmtdatab3 = u1t_jetson_rmtdatab3 | (u1g_mtsend_idrmtset << 4);

  Serial.write(u1t_jetson_fmtdatab0);
  Serial.write(u1t_jetson_fmtdatab1);
  Serial.write(u1t_jetson_fmtdatab2);
  Serial.write(u1t_jetson_fmtdatab3);
  Serial.write(u1t_jetson_rmtdatab0);
  Serial.write(u1t_jetson_rmtdatab1);
  Serial.write(u1t_jetson_rmtdatab2);
  Serial.write(u1t_jetson_rmtdatab3);
}

/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ Communication with Jetson ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/


/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ Steering Servo Control ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/


/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ Steering Servo Control ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/


/*↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ Timer ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓*/
void IRAM_ATTR onTimer()               // Timer Interrupt
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));

  while(Ps3.isConnected())
  {
     // Jetsonからシリアル受信したデータバイト数が所定値(8Byte)に達した場合、Jetsonからの目標を更新
    // if(Serial.available() == INCOMING_BYTE) { vdg_jetson_rectgt(); }
    // vdg_mtcnt_sendrec();           //モータコントローラとSPI通信
    vdg_mtcnt_sendrec_dbg();
    // vdg_mtcnt_idjdg();              // コントローラ要求とモータ状態から要求ID判定
    // vdg_jetson_sendstr();    // 前後左右操舵情報をJetsonに送信
    // vdg_jetson_sendmtsts();        // フロント/リアモータ回転数、状態情報をJetsonに送信
    // vdg_controller_battery();       // コントローラバッテリ残量表示
    // Serial.println(recdata);
  }
}
/*↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ Timer ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*/

void setup()
{
  Serial.begin(115200);                  // Prepare to Serial Communication
  while (!Serial);                       // waiting for serial Comm preparation
  Serial.println("プログラム開始");       // シリアル通信でメッセージをPCに送信

  /*****Controller*****/
  Ps3.attach(vdg_controller_get);       // ボタン入力イベント発生時の割り込み処理設定
  Ps3.begin("FC:F5:C4:45:75:8E");

  /*****SPI*****/
  vspi = new SPIClass(VSPI);            //インスタンス生成
  vspi->begin();                        //生成したvspiインスタンスのポインタからbegin()関数コール(ポインタからメソッドコール時は「->」)
  pinMode(SPI_SS_FRONT, OUTPUT);        //vspiインスタンスのSSを任意ポート指定し出力設定
  pinMode(SPI_SS_REAR, OUTPUT);         //vspiインスタンスのSSを任意ポート指定し出力設定
  vspi->beginTransaction(SPISettings(SPI_CLK_SET, SPI_MSBFIRST, SPI_MODE0));

  /*****Servo PWM*****/

  /*****Timer*****/
  pinMode(LED_PIN, OUTPUT);
  timer = timerBegin(0, getApbFrequency()/1000000, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, TIMER_MS*1000, true);
  timerAlarmEnable(timer);
}

void loop(){}
