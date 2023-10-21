
#include <SPI.h>

/*****SPIライブラリの関数補足*****/
/***SPIsettings***/
// SPIsettings(clock, bitOrder, dataMode)
// All 3 parameters
// Given to SPI.beginTransaction()
/*clock*/
// Maximum frequency is fapb/2
// fspi = fapb/( (SPI_CLKCNT_N + 1) * (SPI_CLKDIV_PRE + 1) )
/*bitOrder*/
// SPI_MSBFIRST or SPI_LSBFIRST
/*dataMode*/
// SPI_MODE0~3
/*****SPIライブラリの関数補足*****/

/*****Macro*****/
#define SPI_SS_M1 16
#define SPI_SS_M2 17
#define SPI_CLK_SET 200000


/*****Global Variable*****/
uint32_t senddata = 123456789;
uint32_t recdata;
SPIClass * vspi = NULL;                 //uninitalised pointers to SPI objects

/*****Instance*****/

void setup()
{
  vspi = new SPIClass(VSPI);            //インスタンス生成
  vspi->begin();                      //生成したVSPIインスタンスのポインタからbegin()関数コール
  pinMode(SPI_SS_M1, OUTPUT);   //vspiインスタンスのSSを任意ポート指定し出力設定
  vspi->beginTransaction(SPISettings(SPI_CLK_SET, SPI_MSBFIRST, SPI_MODE0));

  // Serial Monitor用Serial API設定
  Serial.begin(115200);             // シリアル通信の準備をする
  while (!Serial);                  // 準備が終わるのを待つ
  Serial.println("プログラム開始");    // シリアル通信でメッセージをPCに送信
}

void spicommand(SPIClass *spi, uint32_t data)
{
  digitalWrite(SPI_SS_M1, LOW);    //Pull SS low
  recdata = spi->transfer32(data);
  digitalWrite(SPI_SS_M1, HIGH);   //Pull SS high
}

void loop()
{
  spicommand(vspi, senddata);
  Serial.println(recdata);
  delay(100);
}
