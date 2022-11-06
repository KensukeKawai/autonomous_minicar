
#include "common.h"

void vdg_wait_nop(unsigned long N)
{
	unsigned long i;
	for(i=0;i<N;i++){ nop();}
}

void initialize()
{
	vdg_HW_init();
	vdg_IO_init();
	vdg_MPC_init();
	vdg_MTU2a_init();
	vdg_S12AD_init();
	// vdg_SCI_init();
	vdg_SPI_init();
	// vdg_IRQ_init();
	vdg_IPR_init();
}

void main()
{
	volatile unsigned char mtorigin_flag;
	initialize();
	MTU.TSTR.BYTE = 0xC7;			//Tmp

	/*****初期モータ原点学習*****/
	// Tmp -s-
	while(1)
	{
		while(u1g_extgia0_interrupt != 1);			//TGIA0コンペアマッチするまで待機
		u1g_extgia0_interrupt = 0;					//定周期タスク開始トリガリセット
		if(u1g_exspri0_xrspirec == 1)
		{
			u1g_exspri0_xrspirec = 0;
			vdg_rspicnt_recget();
		}
		if(u1g_rspicnt_idmoderq >= 1)		//JetsonからSPIで原点学習要求が来るまでwhileで待機
		{
			mtorigin_flag == 1;
			PORT_GENERAL_P1 = 1;
		}
		vdg_rspicnt_sendset();
	}
	// Tmp -e-

	MTU.TSTR.BYTE = 0xC7;									//MTU0,1,2,3,4のTCNTカウント開始
	vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
	vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
	vdg_mtcnt_mtorigin();									//原点学習処理

							u1g_mtcnt_idstagem1 = ID_STAGE2;

	vdg_rspicnt_sendset();							//Jetsonに原点学習完了を知らせる

	while(u1g_rspicnt_idmoderq != ID_MODE_NORMAL);	//Jetsonから通常モード指令が来るまで待機

	// 原点学習処理⇒通常モード遷移後のループ処理に移行
	while(1)
	{
										//20221016 デバッグ用
										u1g_rspicnt_idmoderq = ID_MODE_NORMAL;

		while(u1g_extgia0_interrupt != 1);			//TGIA0コンペアマッチするまで待機
		u1g_extgia0_interrupt = 0;					//定周期タスク開始トリガリセット

										// PORT_GENERAL_P1 = !PORT_GENERAL_P1;


		/********************今回周期出力設定用の処理********************/
		//-----基本、今回周期の出力設定は前周期で算出した結果を反映する

										//20221016 デバッグ用
										s4g_rspicnt_nm1tgt = 500;

		//ここに前周期tgrcalで算出したdutycntを入れる
		vdg_mtcnt_outset(ID_MOTOR1,u1g_mtcnt_idstagem1,u2g_mtcnt_cntm1);
		vdg_mtcnt_outset(ID_MOTOR2,u1g_mtcnt_idstagem2,u2g_mtcnt_cntm1);
		/********************今回周期出力設定用の処理********************/

		/********************次周期出力用の処理********************/
		vdg_mtsensor_encoder();						//現在角度算出⇒現在回転数算出
		vdg_mtcnt_nmsmcal();						//モータ回転数LPF処理
		vdg_mtcnt_orthantjdg();						//4象限状態判定
		vdg_mtcnt_stagephasejdg();					//象限と進行方向からステージ設定

							u1g_mtcnt_idorthantm1=ID_MTRUN_FWDPWR;

		/*****電流センサ処理*****/
		while(u1g_exs12adi0_xadcex != 1);			//ADC終了待ち
		u1g_exs12adi0_xadcex = 0;
		vdg_current_cal();							//電流センサADC結果から電流値算出
		/*****電流センサ処理*****/

		//SPI受信されていたら目標回転数更新、制御モード決定
		if(u1g_exspri0_xrspirec == 1) { vdg_rspicnt_recget(); }

		/*****要求モードに合わせて処理選択*****/
		switch(u1g_rspicnt_idmoderq)
		{
			case ID_MODE_NORMAL:					//通常運転モード
			//Motor1
			if ((u1g_mtcnt_idorthantm1==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm1==ID_MTRUN_REVPWR))
			{
				vdg_mtcnt_tgrpwrcalm1();			//力行時Duty算出
			}
			else
			{
				//ここはフリーホイール状態にしておく。回生させない。
				vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
				// vdg_mtcnt_tgrregcalm1();			//回生時Duty算出
			}
			//Motor2
			if ((u1g_mtcnt_idorthantm2==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm2==ID_MTRUN_REVPWR))
			{
				vdg_mtcnt_tgrpwrcalm2();			//力行時Duty算出
			}
			else
			{
				//ここはフリーホイール状態にしておく。回生させない。
				vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
				// vdg_mtcnt_tgrregcalm2();			//回生時Duty算出
			}
			break;

			case ID_MODE_MTORIGIN:			//原点学習モード
			//走行中か否かで先に停車処理させるか否か決める
			//もし停車状態で原点学習できる状態なら実施
			vdg_mtcnt_mtorigin();
			break;

			case ID_MODE_STOP:						//車両停車モード
			//ここで停車処理たたく
			vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
			vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
			break;
		}
		/*****要求モードに合わせて処理選択*****/
		/********************次周期出力用の処理********************/

		//SPIバッファに送信データ書き込み
		vdg_rspicnt_sendset();

		// //PCとのシリアル通信必要だったら処理入れる。今のところマストではない。
	}//while
}//main

//(出来ればやってみる。モータ状態検出。グリップか、駆動スリップか、非駆動スリップか)機能配置的にJetsonであるべきかも。