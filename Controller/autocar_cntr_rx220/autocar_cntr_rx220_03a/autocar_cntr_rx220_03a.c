
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
	// vdg_SCI_init();		// PCとシリアル通信したい場合は使う
	vdg_SPI_init();
	// vdg_IRQ_init();		// フォトインタラプタ等で外部割込みしたい場合は使う
	vdg_IPR_init();
}

void recwait(unsigned char idmoderq)
{
	while(u1g_rspicnt_idmoderq != idmoderq)
	{
		if(u1g_exspri0_xrspirec == 1)
		{
			u1g_exspri0_xrspirec = 0;
			vdg_rspicnt_recget();
		}
	}
}

void main()
{
	// volatile unsigned char mtorigin_flag;
	initialize();
	// MTU.TSTR.BYTE = 0xC7;			//Tmp

	// /*****jetson通信確認デバッグ用******/
	// while(1)
	// {
	// 	if(u1g_exspri0_xrspirec == 1)
	// 	{
	// 		u1g_exspri0_xrspirec = 0;
	// 		vdg_rspicnt_recget();
	// 		// vdg_rspicnt_sendset();
	// 	}
	// }

	// /*****jetson通信確認デバッグ用******/

	// /*****初期モータ原点学習*****/
	// // Tmp -s-
	// while(1)
	// {
	// 	while(u1g_extgia0_interrupt != 1);			//TGIA0コンペアマッチするまで待機
	// 	u1g_extgia0_interrupt = 0;					//定周期タスク開始トリガリセット
	// 	if(u1g_exspri0_xrspirec == 1)
	// 	{
	// 		u1g_exspri0_xrspirec = 0;
	// 		vdg_rspicnt_recget();
	// 	}
	// 	if(u1g_rspicnt_idmoderq >= 1)		//JetsonからSPIで原点学習要求が来るまでwhileで待機
	// 	{
	// 		mtorigin_flag == 1;
	// 		PORT_GENERAL_P1 = 1;
	// 	}
	// 	vdg_rspicnt_sendset();
	// }
	// // Tmp -e-

	MTU.TSTR.BYTE = 0xC7;									//MTU0,1,2,3,4のTCNTカウント開始
	vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
	vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);

	// 初回は必ずJetsonから原点学習要求が来るまで待機
	recwait(ID_MODE_MTORIGIN);
	vdg_mtcnt_mtorigin();									//原点学習処理
	//メインループ初回でフリーホイール状態にしておくために全出力とカウント値をOFFに設定しておく
	vdg_mtcnt_freewheelm1();
	vdg_mtcnt_freewheelm2();
							// u1g_mtcnt_idstagem1 = ID_STAGE2;

	//Jetsonから通常モード指令が来るまで待機⇒原点学習完了時点で初回はJteson側でノーマルモードに自動遷移で良いかも
	recwait(ID_MODE_NORMAL);
	vdg_rspicnt_sendset(ID_MODE_NORMAL);

	// /*****idstagem12の算出*****/
	// vdg_mtsensor_encoder();						//現在角度算出⇒現在回転数算出
	// vdg_mtcnt_nmsmcal();						//現在モータ回転数にLPF処理
	// vdg_mtcnt_orthantjdg();						//LPF後回転数と「目標Nm-LPF後回転数」から4象限状態判定
	// vdg_mtcnt_stagephasejdg();					//進行方向と現在電気角からステージ設定(stagejdg)
	// /*****idstagem12の算出*****/

	/********** メインループ処理 **********/
	while(1)
	{
										//20221016 デバッグ用
										// u1g_rspicnt_idmoderq = ID_MODE_NORMAL;

		while(u1g_extgia0_interrupt != 1);			//TGIA0コンペアマッチするまで待機(キャリア周期)
		u1g_extgia0_interrupt = 0;					//定周期タスク開始トリガリセット

										// PORT_GENERAL_P1 = !PORT_GENERAL_P1;

		/********************今回周期出力設定用の処理********************/
		//-----基本、今回周期の出力設定は前周期で算出した結果を反映する
		vdg_mtcnt_outset(ID_MOTOR1,u1g_mtcnt_idstagem1,u2g_mtcnt_cntm1);
		vdg_mtcnt_outset(ID_MOTOR2,u1g_mtcnt_idstagem2,u2g_mtcnt_cntm2);
		/********************今回周期出力設定用の処理********************/

		/********************次周期出力用の処理********************/
		/*****idstagem12の算出*****/
		vdg_mtsensor_encoder();						//現在角度算出⇒現在回転数算出
		vdg_mtcnt_nmsmcal();						//現在モータ回転数にLPF処理
		vdg_mtcnt_orthantjdg();						//LPF後回転数と「目標Nm-LPF後回転数」から4象限状態判定
		vdg_mtcnt_stagephasejdg();					//進行方向と現在電気角からステージ設定(stagejdg)
		/*****idstagem12の算出*****/

							// u1g_mtcnt_idorthantm1=ID_MTRUN_FWDPWR;

		/*****電流センサ処理*****/
		while(u1g_exs12adi0_xadcex != 1);			//ADC終了待ち
		u1g_exs12adi0_xadcex = 0;
		vdg_current_cal();							//電流センサADC結果から電流値算出
		/*****電流センサ処理*****/

		//SPI受信されていたら目標回転数更新、制御モード決定
		if(u1g_exspri0_xrspirec == 1)
		{
			u1g_exspri0_xrspirec = 0;
			vdg_rspicnt_recget();
		}

		/*****要求モードに合わせて処理選択*****/
		switch(u1g_rspicnt_idmoderq)
		{
			case ID_MODE_NORMAL:					//通常運転モード
				/*****Motor1*****/
				u1g_mtcnt_idmode = ID_MODE_NORMAL;
				if ((u1g_mtcnt_idorthantm1==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm1==ID_MTRUN_REVPWR))
				{
					vdg_mtcnt_tgrpwrcalm1();			//力行時Duty算出
				}
				else	//回生判定時は一旦フリーホイール状態にしておく。損失でかいから割と減速するはず。
				{
					vdg_mtcnt_freewheelm1();
					// vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
					// vdg_mtcnt_tgrregcalm1();			//回生時Duty算出
				}

				/*****Motor2*****/
				if ((u1g_mtcnt_idorthantm2==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm2==ID_MTRUN_REVPWR))
				{
					vdg_mtcnt_tgrpwrcalm2();			//力行時Duty算出
				}
				else	//回生判定時は一旦フリーホイール状態にしておく。損失でかいから割と減速するはず。
				{
					vdg_mtcnt_freewheelm2();
					// vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
					// vdg_mtcnt_tgrregcalm2();			//回生時Duty算出
				}
			break;

			case ID_MODE_MTORIGIN:			//原点学習モード
				u1g_mtcnt_idmode = ID_MODE_MTORIGIN;
				//走行中か否かで先に停車処理させるか否か決める
				//もし停車状態で原点学習できる状態なら実施
				vdg_mtcnt_mtorigin();
			break;

			case ID_MODE_STOP:						//車両停車モード
				u1g_mtcnt_idmode = ID_MODE_STOP;
				//まだ走行中の場合は何か特殊処理する・・・？
				// 次周期の変数をOFF状態にしておく
				vdg_mtcnt_freewheelm1();
				vdg_mtcnt_freewheelm2();
				// vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
				// vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
			break;
		}
		/*****要求モードに合わせて処理選択*****/
		/********************次周期出力用の処理********************/

		//SPIバッファに送信データ書き込み
		vdg_rspicnt_sendset(u1g_mtcnt_idmode);

		// //PCとのシリアル通信必要だったら処理入れる。今のところマストではない。
	}//while
}//main

//(出来ればやってみる。モータ状態検出。グリップか、駆動スリップか、非駆動スリップか)機能配置的にJetsonであるべきかも。