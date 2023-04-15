
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
	// vdg_SCI_init();		// PC�ƃV���A���ʐM�������ꍇ�͎g��
	vdg_SPI_init();
	// vdg_IRQ_init();		// �t�H�g�C���^���v�^���ŊO�������݂������ꍇ�͎g��
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

	// /*****jetson�ʐM�m�F�f�o�b�O�p******/
	// while(1)
	// {
	// 	if(u1g_exspri0_xrspirec == 1)
	// 	{
	// 		u1g_exspri0_xrspirec = 0;
	// 		vdg_rspicnt_recget();
	// 		// vdg_rspicnt_sendset();
	// 	}
	// }

	// /*****jetson�ʐM�m�F�f�o�b�O�p******/

	// /*****�������[�^���_�w�K*****/
	// // Tmp -s-
	// while(1)
	// {
	// 	while(u1g_extgia0_interrupt != 1);			//TGIA0�R���y�A�}�b�`����܂őҋ@
	// 	u1g_extgia0_interrupt = 0;					//������^�X�N�J�n�g���K���Z�b�g
	// 	if(u1g_exspri0_xrspirec == 1)
	// 	{
	// 		u1g_exspri0_xrspirec = 0;
	// 		vdg_rspicnt_recget();
	// 	}
	// 	if(u1g_rspicnt_idmoderq >= 1)		//Jetson����SPI�Ō��_�w�K�v��������܂�while�őҋ@
	// 	{
	// 		mtorigin_flag == 1;
	// 		PORT_GENERAL_P1 = 1;
	// 	}
	// 	vdg_rspicnt_sendset();
	// }
	// // Tmp -e-

	MTU.TSTR.BYTE = 0xC7;									//MTU0,1,2,3,4��TCNT�J�E���g�J�n
	vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
	vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);

	// ����͕K��Jetson���猴�_�w�K�v��������܂őҋ@
	recwait(ID_MODE_MTORIGIN);
	vdg_mtcnt_mtorigin();									//���_�w�K����
	//���C�����[�v����Ńt���[�z�C�[����Ԃɂ��Ă������߂ɑS�o�͂ƃJ�E���g�l��OFF�ɐݒ肵�Ă���
	vdg_mtcnt_freewheelm1();
	vdg_mtcnt_freewheelm2();
							// u1g_mtcnt_idstagem1 = ID_STAGE2;

	//Jetson����ʏ탂�[�h�w�߂�����܂őҋ@�ˌ��_�w�K�������_�ŏ����Jteson���Ńm�[�}�����[�h�Ɏ����J�ڂŗǂ�����
	recwait(ID_MODE_NORMAL);
	vdg_rspicnt_sendset(ID_MODE_NORMAL);

	// /*****idstagem12�̎Z�o*****/
	// vdg_mtsensor_encoder();						//���݊p�x�Z�o�ˌ��݉�]���Z�o
	// vdg_mtcnt_nmsmcal();						//���݃��[�^��]����LPF����
	// vdg_mtcnt_orthantjdg();						//LPF���]���Ɓu�ڕWNm-LPF���]���v����4�ی���Ԕ���
	// vdg_mtcnt_stagephasejdg();					//�i�s�����ƌ��ݓd�C�p����X�e�[�W�ݒ�(stagejdg)
	// /*****idstagem12�̎Z�o*****/

	/********** ���C�����[�v���� **********/
	while(1)
	{
										//20221016 �f�o�b�O�p
										// u1g_rspicnt_idmoderq = ID_MODE_NORMAL;

		while(u1g_extgia0_interrupt != 1);			//TGIA0�R���y�A�}�b�`����܂őҋ@(�L�����A����)
		u1g_extgia0_interrupt = 0;					//������^�X�N�J�n�g���K���Z�b�g

										// PORT_GENERAL_P1 = !PORT_GENERAL_P1;

		/********************��������o�͐ݒ�p�̏���********************/
		//-----��{�A��������̏o�͐ݒ�͑O�����ŎZ�o�������ʂ𔽉f����
		vdg_mtcnt_outset(ID_MOTOR1,u1g_mtcnt_idstagem1,u2g_mtcnt_cntm1);
		vdg_mtcnt_outset(ID_MOTOR2,u1g_mtcnt_idstagem2,u2g_mtcnt_cntm2);
		/********************��������o�͐ݒ�p�̏���********************/

		/********************�������o�͗p�̏���********************/
		/*****idstagem12�̎Z�o*****/
		vdg_mtsensor_encoder();						//���݊p�x�Z�o�ˌ��݉�]���Z�o
		vdg_mtcnt_nmsmcal();						//���݃��[�^��]����LPF����
		vdg_mtcnt_orthantjdg();						//LPF���]���Ɓu�ڕWNm-LPF���]���v����4�ی���Ԕ���
		vdg_mtcnt_stagephasejdg();					//�i�s�����ƌ��ݓd�C�p����X�e�[�W�ݒ�(stagejdg)
		/*****idstagem12�̎Z�o*****/

							// u1g_mtcnt_idorthantm1=ID_MTRUN_FWDPWR;

		/*****�d���Z���T����*****/
		while(u1g_exs12adi0_xadcex != 1);			//ADC�I���҂�
		u1g_exs12adi0_xadcex = 0;
		vdg_current_cal();							//�d���Z���TADC���ʂ���d���l�Z�o
		/*****�d���Z���T����*****/

		//SPI��M����Ă�����ڕW��]���X�V�A���䃂�[�h����
		if(u1g_exspri0_xrspirec == 1)
		{
			u1g_exspri0_xrspirec = 0;
			vdg_rspicnt_recget();
		}

		/*****�v�����[�h�ɍ��킹�ď����I��*****/
		switch(u1g_rspicnt_idmoderq)
		{
			case ID_MODE_NORMAL:					//�ʏ�^�]���[�h
				/*****Motor1*****/
				u1g_mtcnt_idmode = ID_MODE_NORMAL;
				if ((u1g_mtcnt_idorthantm1==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm1==ID_MTRUN_REVPWR))
				{
					vdg_mtcnt_tgrpwrcalm1();			//�͍s��Duty�Z�o
				}
				else	//�񐶔��莞�͈�U�t���[�z�C�[����Ԃɂ��Ă����B�����ł������犄�ƌ�������͂��B
				{
					vdg_mtcnt_freewheelm1();
					// vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
					// vdg_mtcnt_tgrregcalm1();			//�񐶎�Duty�Z�o
				}

				/*****Motor2*****/
				if ((u1g_mtcnt_idorthantm2==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm2==ID_MTRUN_REVPWR))
				{
					vdg_mtcnt_tgrpwrcalm2();			//�͍s��Duty�Z�o
				}
				else	//�񐶔��莞�͈�U�t���[�z�C�[����Ԃɂ��Ă����B�����ł������犄�ƌ�������͂��B
				{
					vdg_mtcnt_freewheelm2();
					// vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
					// vdg_mtcnt_tgrregcalm2();			//�񐶎�Duty�Z�o
				}
			break;

			case ID_MODE_MTORIGIN:			//���_�w�K���[�h
				u1g_mtcnt_idmode = ID_MODE_MTORIGIN;
				//���s�����ۂ��Ő�ɒ�ԏ��������邩�ۂ����߂�
				//������ԏ�ԂŌ��_�w�K�ł����ԂȂ���{
				vdg_mtcnt_mtorigin();
			break;

			case ID_MODE_STOP:						//�ԗ���ԃ��[�h
				u1g_mtcnt_idmode = ID_MODE_STOP;
				//�܂����s���̏ꍇ�͉������ꏈ������E�E�E�H
				// �������̕ϐ���OFF��Ԃɂ��Ă���
				vdg_mtcnt_freewheelm1();
				vdg_mtcnt_freewheelm2();
				// vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
				// vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
			break;
		}
		/*****�v�����[�h�ɍ��킹�ď����I��*****/
		/********************�������o�͗p�̏���********************/

		//SPI�o�b�t�@�ɑ��M�f�[�^��������
		vdg_rspicnt_sendset(u1g_mtcnt_idmode);

		// //PC�Ƃ̃V���A���ʐM�K�v�������珈�������B���̂Ƃ���}�X�g�ł͂Ȃ��B
	}//while
}//main

//(�o����΂���Ă݂�B���[�^��Ԍ��o�B�O���b�v���A�쓮�X���b�v���A��쓮�X���b�v��)�@�\�z�u�I��Jetson�ł���ׂ������B