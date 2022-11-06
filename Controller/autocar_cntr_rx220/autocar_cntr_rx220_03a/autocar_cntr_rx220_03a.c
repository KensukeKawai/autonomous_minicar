
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

	/*****�������[�^���_�w�K*****/
	// Tmp -s-
	while(1)
	{
		while(u1g_extgia0_interrupt != 1);			//TGIA0�R���y�A�}�b�`����܂őҋ@
		u1g_extgia0_interrupt = 0;					//������^�X�N�J�n�g���K���Z�b�g
		if(u1g_exspri0_xrspirec == 1)
		{
			u1g_exspri0_xrspirec = 0;
			vdg_rspicnt_recget();
		}
		if(u1g_rspicnt_idmoderq >= 1)		//Jetson����SPI�Ō��_�w�K�v��������܂�while�őҋ@
		{
			mtorigin_flag == 1;
			PORT_GENERAL_P1 = 1;
		}
		vdg_rspicnt_sendset();
	}
	// Tmp -e-

	MTU.TSTR.BYTE = 0xC7;									//MTU0,1,2,3,4��TCNT�J�E���g�J�n
	vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
	vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
	vdg_mtcnt_mtorigin();									//���_�w�K����

							u1g_mtcnt_idstagem1 = ID_STAGE2;

	vdg_rspicnt_sendset();							//Jetson�Ɍ��_�w�K������m�点��

	while(u1g_rspicnt_idmoderq != ID_MODE_NORMAL);	//Jetson����ʏ탂�[�h�w�߂�����܂őҋ@

	// ���_�w�K�����˒ʏ탂�[�h�J�ڌ�̃��[�v�����Ɉڍs
	while(1)
	{
										//20221016 �f�o�b�O�p
										u1g_rspicnt_idmoderq = ID_MODE_NORMAL;

		while(u1g_extgia0_interrupt != 1);			//TGIA0�R���y�A�}�b�`����܂őҋ@
		u1g_extgia0_interrupt = 0;					//������^�X�N�J�n�g���K���Z�b�g

										// PORT_GENERAL_P1 = !PORT_GENERAL_P1;


		/********************��������o�͐ݒ�p�̏���********************/
		//-----��{�A��������̏o�͐ݒ�͑O�����ŎZ�o�������ʂ𔽉f����

										//20221016 �f�o�b�O�p
										s4g_rspicnt_nm1tgt = 500;

		//�����ɑO����tgrcal�ŎZ�o����dutycnt������
		vdg_mtcnt_outset(ID_MOTOR1,u1g_mtcnt_idstagem1,u2g_mtcnt_cntm1);
		vdg_mtcnt_outset(ID_MOTOR2,u1g_mtcnt_idstagem2,u2g_mtcnt_cntm1);
		/********************��������o�͐ݒ�p�̏���********************/

		/********************�������o�͗p�̏���********************/
		vdg_mtsensor_encoder();						//���݊p�x�Z�o�ˌ��݉�]���Z�o
		vdg_mtcnt_nmsmcal();						//���[�^��]��LPF����
		vdg_mtcnt_orthantjdg();						//4�ی���Ԕ���
		vdg_mtcnt_stagephasejdg();					//�ی��Ɛi�s��������X�e�[�W�ݒ�

							u1g_mtcnt_idorthantm1=ID_MTRUN_FWDPWR;

		/*****�d���Z���T����*****/
		while(u1g_exs12adi0_xadcex != 1);			//ADC�I���҂�
		u1g_exs12adi0_xadcex = 0;
		vdg_current_cal();							//�d���Z���TADC���ʂ���d���l�Z�o
		/*****�d���Z���T����*****/

		//SPI��M����Ă�����ڕW��]���X�V�A���䃂�[�h����
		if(u1g_exspri0_xrspirec == 1) { vdg_rspicnt_recget(); }

		/*****�v�����[�h�ɍ��킹�ď����I��*****/
		switch(u1g_rspicnt_idmoderq)
		{
			case ID_MODE_NORMAL:					//�ʏ�^�]���[�h
			//Motor1
			if ((u1g_mtcnt_idorthantm1==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm1==ID_MTRUN_REVPWR))
			{
				vdg_mtcnt_tgrpwrcalm1();			//�͍s��Duty�Z�o
			}
			else
			{
				//�����̓t���[�z�C�[����Ԃɂ��Ă����B�񐶂����Ȃ��B
				vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
				// vdg_mtcnt_tgrregcalm1();			//�񐶎�Duty�Z�o
			}
			//Motor2
			if ((u1g_mtcnt_idorthantm2==ID_MTRUN_FWDPWR) || (u1g_mtcnt_idorthantm2==ID_MTRUN_REVPWR))
			{
				vdg_mtcnt_tgrpwrcalm2();			//�͍s��Duty�Z�o
			}
			else
			{
				//�����̓t���[�z�C�[����Ԃɂ��Ă����B�񐶂����Ȃ��B
				vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
				// vdg_mtcnt_tgrregcalm2();			//�񐶎�Duty�Z�o
			}
			break;

			case ID_MODE_MTORIGIN:			//���_�w�K���[�h
			//���s�����ۂ��Ő�ɒ�ԏ��������邩�ۂ����߂�
			//������ԏ�ԂŌ��_�w�K�ł����ԂȂ���{
			vdg_mtcnt_mtorigin();
			break;

			case ID_MODE_STOP:						//�ԗ���ԃ��[�h
			//�����Œ�ԏ���������
			vdg_mtcnt_outset(ID_MOTOR1, ID_ALLOFF, CNT_OUTOFF);
			vdg_mtcnt_outset(ID_MOTOR2, ID_ALLOFF, CNT_OUTOFF);
			break;
		}
		/*****�v�����[�h�ɍ��킹�ď����I��*****/
		/********************�������o�͗p�̏���********************/

		//SPI�o�b�t�@�ɑ��M�f�[�^��������
		vdg_rspicnt_sendset();

		// //PC�Ƃ̃V���A���ʐM�K�v�������珈�������B���̂Ƃ���}�X�g�ł͂Ȃ��B
	}//while
}//main

//(�o����΂���Ă݂�B���[�^��Ԍ��o�B�O���b�v���A�쓮�X���b�v���A��쓮�X���b�v��)�@�\�z�u�I��Jetson�ł���ׂ������B