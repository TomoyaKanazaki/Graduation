//==============================================================
//
//�v���C���[����[playerModel.cpp]
//Author:����������
//
//==============================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "modelHierarchy.h"
#include "input.h"
#include "motion.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "UI_death.h"
#include "UI_item.h"
#include "particle.h"
#include "object3D.h"

//�}�N����`
#define PRIORITY			(3)			//�D�揇��
#define POS					(10.0f)		//pos�����l
#define CURVE_RL			(0.5f)		//���E�̊p�x
#define CURVE_UP			(0.0f)		//��̊p�x
#define CURVE_DOWN			(1.0f)		//���̊p�x
#define MOVE_Y				(0.7f)		//�ړ���Y
#define ADD_MOVE_Y			(1.5f)		//�ړ���Y���Z���鐔
#define JUMP_HEIGHT			(10.0f)		//�W�����v�̍���
#define MAX_STR				(128)		//�����̍ő吔
#define FRONT_MOVE			(0.6f)		//��O�̎��̈ړ���
#define FRONT_DASH_MOVE		(18.0f)		//��O�̃_�b�V�����̈ړ���
#define MAX_DASH			(2)			//�_�b�V���̍ő吔
#define STOP_MOVE			(0.8f)		//�~�܂锻��̈ړ���
#define FILE_HUMAN			"data\\TEXT\\motion_player.txt"		//�v���C���[���f���̃e�L�X�g

#define SAVE_POS_DEFAULT		(D3DXVECTOR3(-5400.0f, 1000.0f, 0.0f))		//�`���[�g���A���̏����l�̃Z�[�u�|�C���g
#define SAVE_POS_ARTBRIDGE		(D3DXVECTOR3(-2750.0f, 1300.0f, 0.0f))		//�`���[�g���A����1�Ԗڂ̃Z�[�u�|�C���g
#define SAVE_POS_ENEMYBRIDGE	(D3DXVECTOR3(-350.0f, 1000.0f, 0.0f))		//�`���[�g���A����2�Ԗڂ̃Z�[�u�|�C���g

#define SAVE_POS_START		(D3DXVECTOR3(1000.0f, -150.0f, 0.0f))		//�Q�[���̏����l�̃Z�[�u�|�C���g
#define SAVE_POS_DOWN_FLOOR	(D3DXVECTOR3(1200.0f, -950.0f, 0.0f))		//�Q�[����1�Ԗڂ̃Z�[�u�|�C���g
#define SAVE_POS_PIER		(D3DXVECTOR3(50.0f, -1800.0f, 0.0f))		//�Q�[����2�Ԗڂ̃Z�[�u�|�C���g
#define SAVE_POS_POINT_MAZE	(D3DXVECTOR3(1300.0f, -2800.0f, 0.0f))		//�Q�[����3�Ԗڂ̃Z�[�u�|�C���g
#define SAVE_POS_BIGINEND	(D3DXVECTOR3(100.0f, -3600.0f, 0.0f))		//�Q�[����4�Ԗڂ̃Z�[�u�|�C���g
#define SAVE_POS_FALLING	(D3DXVECTOR3(650.0f, -4300.0f, 0.0f))		//�Q�[����5�Ԗڂ̃Z�[�u�|�C���g

#define HIT_CNT				(60 * 2)	//�U��������܂ł̃J�E���g��
#define DAMAGE_CNT			(9)			//�_���[�W�J�E���g��
#define APP_CNT				(100)		//�_�Ŏ���
#define STEP_CNT			(10)		//�������̃J�E���^�[

//�ÓI�����o�ϐ��錾
char *CPlayer::m_apFileName[PARTS_MAX] =
{
	"data\\MODEL\\sphere.x",

};

//==============================================================
//�R���X�g���N�^
//==============================================================
CPlayer::CPlayer()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
	m_moveSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʕۑ��p

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_apObject3D[nCnt] = nullptr;
	}

	m_posSavePoint[POINT_DEFAULT] = SAVE_POS_DEFAULT;			//�`���[�g���A���̏����l�̃Z�[�u�|�C���g
	m_posSavePoint[POINT_ARTBRIDGE] = SAVE_POS_ARTBRIDGE;		//�`���[�g���A���̏����l�̃Z�[�u�|�C���g
	m_posSavePoint[POINT_ENEMYBRIDGE] = SAVE_POS_ENEMYBRIDGE;	//�`���[�g���A���̏����l�̃Z�[�u�|�C���g

	m_posSavePoint[POINT_START] = SAVE_POS_START;			//�Q�[���̏����l�̃Z�[�u�|�C���g
	m_posSavePoint[POINT_DOWN_FLOOR] = SAVE_POS_DOWN_FLOOR;	//�Q�[����1�Ԗڂ̃Z�[�u�|�C���g
	m_posSavePoint[POINT_PIER] = SAVE_POS_PIER;				//�Q�[����2�Ԗڂ̃Z�[�u�|�C���g
	m_posSavePoint[POINT_MAZE] = SAVE_POS_POINT_MAZE;		//�Q�[����3�Ԗڂ̃Z�[�u�|�C���g
	m_posSavePoint[POINT_BIGINEND] = SAVE_POS_BIGINEND;		//�Q�[����4�Ԗڂ̃Z�[�u�|�C���g
	m_posSavePoint[POINT_FALLING] = SAVE_POS_FALLING;		//�Q�[����5�Ԗڂ̃Z�[�u�|�C���g

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̍ő�l
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̍ŏ��l

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = NULL;		//�v���C���[(�p�[�c)�ւ̃|�C���^
	}

	m_nNumModel = 0;		//�v���C���[(�p�[�c)�̑���
	//m_pMotion = NULL;

	m_nPressCounter = 0;	//�L�[���������t���[����

	m_bJump = false;		//�W�����v������
	m_bMove = false;		//�����Ă邩
	m_bLand = true;			//���n����
	m_bDash = false;		//�_�b�V��������
	m_bDashAuto = false;	//�����_�b�V�����邩
	m_bCollisionAlpha = false;		//�����u���b�N�ɓ���������

	m_fRotDest = 0.0f;		//�ڕW
	m_fRotDiff = 0.0f;		//����

	m_nDashCounter = 0;		//�_�b�V��������

	m_particleType = PARTICLETYPE_NONE;	//�p�[�e�B�N���̎��
	m_nType = 1;		//���Ԗڂ̃p�[�e�B�N����
	m_nParticleLife = 50;			//�p�[�e�B�N���̎���
	m_state = STATE_NONE;			//���
	m_nCntDamage = 0;				//�_���[�W�J�E���^�[
	m_nCntHit = HIT_CNT;			//�U��������܂ł̃J�E���^�[
	m_bPad = false;					//�p�b�h�̃X�e�B�b�N��|���Ă邩
	m_nCntSand = STEP_CNT;			//���̃p�[�e�B�N���J�E���^�[
	m_nNumPosSave = 0;		//���Ԗڂ̃Z�[�u�|�C���g��
}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CPlayer::CPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = SAVE_POS_DEFAULT;						//�ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
	m_moveSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʕۑ��p

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_apObject3D[nCnt] = nullptr;
	}

	m_posSavePoint[POINT_DEFAULT] = SAVE_POS_DEFAULT;			//�`���[�g���A���̏����l�̃Z�[�u�|�C���g
	m_posSavePoint[POINT_ARTBRIDGE] = SAVE_POS_ARTBRIDGE;		//�`���[�g���A���̏����l�̃Z�[�u�|�C���g
	m_posSavePoint[POINT_ENEMYBRIDGE] = SAVE_POS_ENEMYBRIDGE;	//�`���[�g���A���̏����l�̃Z�[�u�|�C���g

	m_posSavePoint[POINT_START] = SAVE_POS_START;			//�Q�[���̏����l�̃Z�[�u�|�C���g
	m_posSavePoint[POINT_DOWN_FLOOR] = SAVE_POS_DOWN_FLOOR;	//�Q�[����1�Ԗڂ̃Z�[�u�|�C���g
	m_posSavePoint[POINT_PIER] = SAVE_POS_PIER;				//�Q�[����2�Ԗڂ̃Z�[�u�|�C���g
	m_posSavePoint[POINT_MAZE] = SAVE_POS_POINT_MAZE;		//�Q�[����3�Ԗڂ̃Z�[�u�|�C���g
	m_posSavePoint[POINT_BIGINEND] = SAVE_POS_BIGINEND;		//�Q�[����4�Ԗڂ̃Z�[�u�|�C���g
	m_posSavePoint[POINT_FALLING] = SAVE_POS_FALLING;		//�Q�[����5�Ԗڂ̃Z�[�u�|�C���g

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ���
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̍ő�l
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̍ŏ��l
	m_rot = rot;		//����

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = NULL;		//�v���C���[(�p�[�c)�ւ̃|�C���^
	}

	//m_pMotion = NULL;		//���[�V�����̏��
	m_nNumModel = 0;		//�v���C���[(�p�[�c)�̑���

	m_nPressCounter = 0;	//�L�[���������t���[����

	m_bJump = false;		//�W�����v������
	m_bMove = false;		//�����Ă邩
	m_bLand = true;			//���n����
	m_bDash = false;		//�_�b�V��������
	m_bDashAuto = false;	//�����_�b�V�����邩
	m_bCollisionAlpha = false;		//�����u���b�N�ɓ���������

	m_fRotDest = 0.0f;	//�ڕW
	m_fRotDiff = 0.0f;	//����

	m_nDashCounter = 0;		//�_�b�V��������

	m_particleType = PARTICLETYPE_NONE;	//�p�[�e�B�N���̎��
	m_nType = 1;				//���Ԗڂ̃p�[�e�B�N����
	m_nParticleLife = 50;		//�p�[�e�B�N���̎���
	m_state = STATE_NONE;		//���
	m_nCntDamage = 0;			//�_���[�W�J�E���^�[
	m_nCntHit = HIT_CNT;		//�U��������܂ł̃J�E���^�[
	m_nCntMove = STEP_CNT;		//�����̃J�E���^�[
	m_bPad = false;				//�p�b�h�̃X�e�B�b�N��|���Ă邩
	m_nCntSand = STEP_CNT;		//���̃p�[�e�B�N���J�E���^�[
	m_nNumPosSave = 0;		//���Ԗڂ̃Z�[�u�|�C���g��

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CPlayer::~CPlayer()
{

}

//==============================================================
//�v���C���[�̐�������
//==============================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pPlayerModel = NULL;

	if (pPlayerModel == NULL)
	{//���������g�p����ĂȂ�������

		//�v���C���[�̐���
		pPlayerModel = new CPlayer(pos, rot);
	}

	//����������
	pPlayerModel->Init();

	return pPlayerModel;
}

//==============================================================
//�v���C���[�̏���������
//==============================================================
HRESULT CPlayer::Init(void)
{
	m_fRotDest = m_rot.y;

	//�v���C���[�̐����i�S�p�[�c���j
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_apObject3D[nCnt] = CObject3D::Create();
	}

	CObject::SetType(CObject::TYPE_PLAYER);

	return S_OK;
}

//==============================================================
//�v���C���[�̏I������
//==============================================================
void CPlayer::Uninit(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		if (m_apModel[nCntPlayer] != NULL)
		{//�g�p����Ă�Ƃ�

			//�I������
			m_apModel[nCntPlayer]->Uninit();
			m_apModel[nCntPlayer] = NULL;
		}
	}

	//if (m_pMotion != NULL)
	//{//�g�p����Ă�Ƃ�

	//	//���[�V�����̔j��
	//	delete m_pMotion;
	//	m_pMotion = NULL;
	//}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�v���C���[�̍X�V����
//==============================================================
void CPlayer::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾

	//�O��̈ʒu�X�V
	m_posOld = m_pos;

	//��O���̍X�V����
	CPlayer::UpdateFront();

	//���[�V�����̍X�V����
	//m_pMotion->Update();

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//�v���C���[�̍X�V����
		m_apModel[nCntPlayer]->Update();
	}

	//�f�o�b�O�\��
	pDebugProc->Print("\n�v���C���[�̈ʒu    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("�v���C���[�̈ړ���    (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("�v���C���[�̌���      (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);
	pDebugProc->Print("���݂̃`�F�b�N�|�C���g  (%f, %f, %f)\n", m_posSavePoint[m_nNumPosSave].x, m_posSavePoint[m_nNumPosSave].y, m_posSavePoint[m_nNumPosSave].z);

}

//==============================================================
//��O�̍X�V����
//==============================================================
void CPlayer::UpdateFront(void)
{
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (m_bDashAuto == false && pFade->GetFade() != CFade::FADE_OUT)
	{//�����_�b�V�����ĂȂ��Ƃ�

		//�v���C���[�̑���
		CPlayer::ControlFrontKeyboard();
		CPlayer::ControlFrontJoyPad();

		//if (m_move.y >= JUMP_HEIGHT ||
		//	(m_bJump == true && (m_move.x <= 7.0f && m_move.x >= -7.0f)) ||
		//	(m_bDash == true && (m_move.x <= 7.0f && m_move.x >= -7.0f)))
		//{
		//	//�ړ��ʉ��Z
		//	m_move.y -= MOVE_Y;

		//	if (m_move.y <= 0.0f)
		//	{//���n������

		//		m_bDash = false;
		//	}
		//}
	}

	//�����炷����
	CPlayer::SEStep();

	//�ʒu�X�V
	if (m_bDashAuto == true)
	{//�����_�b�V�����Ă��

		m_pos += m_moveSave;		//�����_�b�V���̈ړ��ʉ��Z
	}
	else if (m_bDashAuto == false)
	{//�����_�b�V�����ĂȂ���

		m_pos += m_move;			//���ʂ̈ړ��ʉ��Z
	}

	//�����������̍s������
	CPlayer::CollisionAction();

	//��ԍX�V
	CPlayer::UpdateState();

	if (m_bDashAuto == false)
	{
		//�ړ��ʂ��X�V
		m_move.x += (0.0f - m_move.x) * 0.1f;
		m_move.y += (0.0f - m_move.y) * 0.1f;
	}

	//�����̕␳
	CPlayer::RotCorrection();

	//�`�F�b�N�|�C���g�X�V
	CPlayer::SavePoint();

	//��ʊO����
	CPlayer::Screen();

	//���[�V�����Ǘ�
	//CPlayer::MotionManager();
}

//==============================================================
//��ԍX�V����
//==============================================================
void CPlayer::UpdateState(void)
{
	CFade *pFade = CManager::GetInstance()->GetFade();

	switch (m_state)
	{
	case STATE_NONE:		//�ʏ���

		if (m_nCntHit > 0 && pFade->GetFade() == CFade::FADE_NONE)
		{
			m_nCntHit--;		//�U��������܂ł̃J�E���^�[���Z
		}

		break;

	case STATE_ATTACK:		//�U�����

		break;

	case STATE_DAMAGE:		//�_���[�W���

		m_nCntDamage--;		//�_���[�W���Ԍ��Z
		m_nCntHit--;		//�U��������܂ł̃J�E���^�[���Z

		if (m_nCntDamage <= 0)
		{//�_���[�W���Ԃ��I�������

			m_state = CObject::STATE_NONE;

			//��Ԑݒ�
			for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
			{
				m_apModel[nCntPlayer]->SetState(m_state);		//�ʏ��Ԃɂ���

			}
		}

		break;

	case STATE_APPEAR:		//�_�ŏ��

		if (pFade->GetFade() == CFade::FADE_IN)
		{
			m_state = CObject::STATE_NONE;		//�ʏ��Ԃɂ���

			m_pos = m_posSavePoint[m_nNumPosSave];		//�Z�[�u�����ꏊ�ɖ߂�
			m_nCntDamage = 0;

			//��Ԑݒ�
			for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
			{
				m_apModel[nCntPlayer]->SetState(m_state);		//��Ԑݒ�

			}
		}

		m_nCntDamage--;		//�_���[�W���Ԍ��Z

		if ((m_nCntDamage % 3) == 0)
		{//��莞�Ԍo������

			//�\���ؑ�
			for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
			{
				m_apModel[nCntPlayer]->SetDisp();

			}
		}

		break;

	case STATE_DEATH:		//���S���

		break;
	}
}

//==============================================================
//�����������̍s������
//==============================================================
void CPlayer::CollisionAction(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//�p�b�h�̏��擾

	bool bLand = CObjectX::Collision(&m_pos, &m_posOld, &m_move, m_min, m_max);

	//�����蔻��
	if (bLand == true)
	{//���n������

		m_nDashCounter = 0;		//�_�b�V�������Z�b�g

		if (m_bLand == false)
		{
			//�p�[�e�B�N���̐���
			//CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXCOLOR(0.1f, 0.2f, 0.4f, 0.8f), PARTICLETYPE_MOVE, 20, 10.0f);
		}

		m_bJump = false;	//�W�����v���ĂȂ���Ԃɂ���
		m_bLand = true;		//���n������Ԃɂ���

	}
	else if (bLand == false &&
		pInputKeyboard->GetPress(DIK_SPACE) == false && pInputJoyPad->GetPress(pInputJoyPad->BUTTON_A, 0) == false)
	{//�n�ʂɂ��ĂȂ� && �W�����v�{�^�������ĂȂ�

		m_bJump = true;		//�W�����v���Ă��Ԃɂ���
		m_bLand = false;	//���n���ĂȂ���Ԃɂ���
	}
}

//==============================================================
//�Z�[�u�|�C���g����
//==============================================================
void CPlayer::SavePoint(void)
{
	if (m_nNumPosSave < POINT_FALLING)
	{//�ő�`�F�b�N�|�C���g����菭�Ȃ�������

		int nNumSave = m_nNumPosSave + 1;		//���̃`�F�b�N�|�C���g

		if ((m_nNumPosSave + 1) == POINT_ARTBRIDGE || (m_nNumPosSave + 1) == POINT_ENEMYBRIDGE)
		{//���̃`�F�b�N�|�C���g���`���[�g���A���̈ʒu��������

			if (m_posSavePoint[nNumSave].x <= m_pos.x)
			{//���̃`�F�b�N�|�C���g��ʂ�߂�����

				m_nNumPosSave += 1;		//�Z�[�u�|�C���g��ύX
			}
		}
		else if ((m_nNumPosSave + 1) == POINT_START)
		{//���̃`�F�b�N�|�C���g���Q�[���J�n�ʒu��������

			if (m_posSavePoint[nNumSave].y >= m_pos.y)
			{//���̃`�F�b�N�|�C���g��ʂ�߂�����

				m_nNumPosSave += 1;		//�Z�[�u�|�C���g��ύX
			}
		}
		else
		{
			if (m_posSavePoint[nNumSave].y >= m_pos.y &&
				m_posSavePoint[nNumSave].x - 100.0f <= m_pos.x &&
				m_posSavePoint[nNumSave].x + 100.0f >= m_pos.x)
			{//���̃`�F�b�N�|�C���g��ʂ�߂�����

				m_nNumPosSave += 1;		//�Z�[�u�|�C���g��ύX
			}
		}
	}
}

//==============================================================
//���[�V�����Ǘ�����
//==============================================================
void CPlayer::MotionManager(void)
{
	//if (m_bMove == true && m_pMotion->GetType() == m_pMotion->MOTIONTYPE_NEUTRAL)
	//{//�����Ă� && �ҋ@���

	//	//��������
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_MOVE);
	//}
	//else if (m_pMotion->IsFinish() == true && m_bAction == true)
	//{//�U�����I�������

	//	m_bAction = false;		//�U�����ĂȂ���Ԃɂ���
	//}
	//else if ((m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_pMotion->IsFinish() == true) ||
	//	(m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_bMove == false && m_bJump == false && m_bLand == true))
	//{//���[�V�������I��������

	//	//�ҋ@��Ԃɖ߂�
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_NEUTRAL);
	//}
}

//==============================================================
//�����̕␳����
//==============================================================
void CPlayer::RotCorrection(void)
{
	//�����̍��������߂�
	m_fRotDiff = m_fRotDest - m_rot.y;

	//�ڕW�̕����܂ł̍������C��
	if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * 2;
	}
	else if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff -= D3DX_PI * 2;
	}

	//��������
	m_rot.y += m_fRotDiff * 0.1f;

	//���݂̕����C��
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==============================================================
//�����炷����
//==============================================================
void CPlayer::SEStep(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//�����炷
	if (m_bMove == true && m_bJump == false)
	{//�����Ă�Ƃ� && �W�����v���ĂȂ�

		if ((m_nCntMove >= STEP_CNT))
		{//��莞�Ԃ�������

			//BGM�Đ�
			pSound->Play(pSound->SOUND_LABEL_SE_STEP);

			m_nCntMove = 0;
		}

		m_nCntMove++;		//�����炷�J�E���^�[���Z
	}
	else if(m_bMove == false)
	{//�����ĂȂ��Ƃ�

		m_nCntMove = STEP_CNT;
	}

}

//==============================================================
//�v���C���[�̃L�[�{�[�h���쏈��(��O��)
//==============================================================
void CPlayer::ControlFrontKeyboard(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾

	CSound *pSound = CManager::GetInstance()->GetSound();

	//�ړ�����
	CPlayer::ControlFrontKeyboardMove();
	
	//�W�����v����
	//CPlayer::ControlFrontKeyboardJump();

	////�_�b�V������
	//if (m_nDashCounter < MAX_DASH)
	//{//�ő�W�����v������ && �I�[�g�_�b�V�����Ă��

	//	CPlayer::ControlFrontKeyboardDash();
	//}
}

//==============================================================
//�v���C���[�̃L�[�{�[�h�̈ړ����쏈��(��O��)
//==============================================================
void CPlayer::ControlFrontKeyboardMove(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾

	//�ړ�
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{//�E
		m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

		m_bMove = true;		//��������
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{//��
		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

		m_bMove = true;		//��������
	}

	if ((m_move.x <= STOP_MOVE && m_move.x >= -STOP_MOVE) && (m_move.z <= STOP_MOVE && m_move.z >= -STOP_MOVE))
	{//�����ĂȂ��Ƃ�

		m_bMove = false;		//�����ĂȂ���Ԃɂ���
	}
}

//==============================================================
//�v���C���[�̃L�[�{�[�h�̃W�����v���쏈��(��O��)
//==============================================================
void CPlayer::ControlFrontKeyboardJump(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputKeyboard->GetPress(DIK_SPACE) == true && m_bJump == false && m_move.y <= JUMP_HEIGHT)
	{
		//�W�����v����
		m_move.y += ADD_MOVE_Y;

		m_nPressCounter++;		//�t���[�������Z

		if (m_move.y >= JUMP_HEIGHT)
		{
			//�W�����v������Ԃɂ���
			m_bJump = true;
			m_bLand = false;
		}
	}

	if (pInputKeyboard->GetRelease(DIK_SPACE) == true && m_bJump == false)
	{
		//�W�����v������Ԃɂ���
		m_bJump = true;
		m_bLand = false;
	}
}

//==============================================================
//�v���C���[�̃L�[�{�[�h�̃_�b�V�����쏈��(��O��)
//==============================================================
void CPlayer::ControlFrontKeyboardDash(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputKeyboard->GetPress(DIK_D) == true)
	{//�E

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//��
			if (pInputKeyboard->GetTrigger(DIK_J) == true)
			{
				m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
				m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
				m_bDash = true;			//�_�b�V��������Ԃɂ���
				m_bJump = true;			//�W�����v������Ԃɂ���

				if (m_bCollisionAlpha == true)
				{//�����_�b�V��

					m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;

					m_nDashCounter++;		//�_�b�V���񐔉��Z

				}
			}
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//��
			if (pInputKeyboard->GetTrigger(DIK_J) == true)
			{
				m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
				m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
				m_bDash = true;			//�_�b�V��������Ԃɂ���
				m_bJump = true;			//�W�����v������Ԃɂ���

				if (m_bCollisionAlpha == true)
				{//�����_�b�V��

					m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if(m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//�_�b�V���񐔉��Z

				}
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_J) == true)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.z = 0.0f;		//�ړ��ʃ��Z�b�g
			m_bDash = true;			//�_�b�V��������Ԃɂ���
			m_bJump = true;			//�W�����v������Ԃɂ���

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z

			}
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{//��
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//��
			if (pInputKeyboard->GetTrigger(DIK_J) == true)
			{
				m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
				m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
				m_bDash = true;			//�_�b�V��������Ԃɂ���
				m_bJump = true;			//�W�����v������Ԃɂ���

				if (m_bCollisionAlpha == true)
				{//�����_�b�V��

					m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//�_�b�V���񐔉��Z
				}
			}
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//��
			if (pInputKeyboard->GetTrigger(DIK_J) == true)
			{
				m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
				m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
				m_bDash = true;			//�_�b�V��������Ԃɂ���
				m_bJump = true;			//�W�����v������Ԃɂ���

				if (m_bCollisionAlpha == true)
				{//�����_�b�V��

					m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//�_�b�V���񐔉��Z
				}
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_J) == true)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.z = 0.0f;		//�ړ��ʃ��Z�b�g
			m_bDash = true;			//�_�b�V��������Ԃɂ���
			m_bJump = true;			//�W�����v������Ԃɂ���

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{//��
		if (pInputKeyboard->GetTrigger(DIK_J) == true)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
			m_bDash = true;			//�_�b�V��������Ԃɂ���
			m_bJump = true;			//�W�����v������Ԃɂ���

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_UP) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_UP) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_DASH_MOVE;
				m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{//��
		if (pInputKeyboard->GetTrigger(DIK_J) == true)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
			m_bDash = true;			//�_�b�V��������Ԃɂ���
			m_bJump = true;			//�W�����v������Ԃɂ���

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_DOWN) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_DOWN) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_DASH_MOVE;
				m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{//J�L�[�������������Ƃ�

		if (m_rot.y > 0)
		{//�v���C���[�̌���������������

			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.z = 0.0f;		//�ړ��ʃ��Z�b�g

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_RL) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}
		else if (m_rot.y <= 0)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.z = 0.0f;		//�ړ��ʃ��Z�b�g

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}

		m_bDash = true;		//�_�b�V��������Ԃɂ���
		m_bJump = true;		//�W�����v������Ԃɂ���
	}

	if (m_bDashAuto == true)
	{
		//�p�[�e�B�N���̐���
		CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PARTICLETYPE_MOVE, 20, 30.0f);
	}

	if (m_bDash == true && m_bJump == true)
	{
		//�p�[�e�B�N���̐���
		CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PARTICLETYPE_MOVE, 20, 20.0f);

	}
}

//==============================================================
//�v���C���[�̃p�b�h���쏈��(��O��)
//==============================================================
void CPlayer::ControlFrontJoyPad(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//�ړ�����
	CPlayer::ControlFrontJoyPadMove();

	//�W�����v����
	//CPlayer::ControlFrontJoyPadJump();

	////�_�b�V������
	//if (m_nDashCounter < MAX_DASH)
	//{//�ő�W�����v������ && �I�[�g�_�b�V�����Ă��

	//	CPlayer::ControlFrontJoyPadDash();
	//}
}

//==============================================================
//�v���C���[�̃p�b�h�̃W�����v���쏈��(��O��)
//==============================================================
void CPlayer::ControlFrontJoyPadJump(void)
{
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//�p�b�h�̏��擾
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputJoyPad->GetPress(pInputJoyPad->BUTTON_A, 0) == true && m_bJump == false && m_move.y <= JUMP_HEIGHT)
	{
		//�W�����v����
		m_move.y += ADD_MOVE_Y;

		m_nPressCounter++;		//�t���[�������Z

		if (m_move.y >= JUMP_HEIGHT)
		{
			//�W�����v������Ԃɂ���
			m_bJump = true;
			m_bLand = false;
		}
	}

	if (pInputJoyPad->GetRelease(pInputJoyPad->BUTTON_A, 0) == true && m_bJump == false)
	{
		//�W�����v������Ԃɂ���
		m_bJump = true;
		m_bLand = false;
	}
}

//==============================================================
//�v���C���[�̃p�b�h�̃_�b�V�����쏈��(��O��)
//==============================================================
void CPlayer::ControlFrontJoyPadDash(void)
{
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//�p�b�h�̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (pInputJoyPad->GetPressLX(0).x > 0.0f)
	{//�E

		if (pInputJoyPad->GetPressLX(0).y > 0.0f)
		{//��
			if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
			{
				m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
				m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
				m_bDash = true;			//�_�b�V��������Ԃɂ���
				m_bJump = true;			//�W�����v������Ԃɂ���

				if (m_bCollisionAlpha == true)
				{//�����_�b�V��

					m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//�_�b�V���񐔉��Z

				}
			}
		}
		else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
		{//��
			if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
			{
				m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
				m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
				m_bDash = true;			//�_�b�V��������Ԃɂ���
				m_bJump = true;			//�W�����v������Ԃɂ���

				if (m_bCollisionAlpha == true)
				{//�����_�b�V��

					m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//�_�b�V���񐔉��Z

				}
			}
		}
		else if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.z = 0.0f;		//�ړ��ʃ��Z�b�g
			m_bDash = true;			//�_�b�V��������Ԃɂ���
			m_bJump = true;			//�W�����v������Ԃɂ���

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z

			}
		}
	}
	else if (pInputJoyPad->GetPressLX(0).x < 0.0f)
	{//��
		if (pInputJoyPad->GetPressLX(0).y > 0.0f)
		{//��
			if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
			{
				m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
				m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
				m_bDash = true;			//�_�b�V��������Ԃɂ���
				m_bJump = true;			//�W�����v������Ԃɂ���

				if (m_bCollisionAlpha == true)
				{//�����_�b�V��

					m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//�_�b�V���񐔉��Z
				}
			}
		}
		else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
		{//��
			if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
			{
				m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
				m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
				m_bDash = true;			//�_�b�V��������Ԃɂ���
				m_bJump = true;			//�W�����v������Ԃɂ���

				if (m_bCollisionAlpha == true)
				{//�����_�b�V��

					m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

					m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;
					m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
				}
				else if (m_bCollisionAlpha == false)
				{
					m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;
					m_move.y += cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * FRONT_DASH_MOVE;

					//BGM�Đ�
					pSound->Play(pSound->SOUND_LABEL_SE_DASH);

					m_nDashCounter++;		//�_�b�V���񐔉��Z
				}
			}
		}
		else if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.z = 0.0f;		//�ړ��ʃ��Z�b�g
			m_bDash = true;			//�_�b�V��������Ԃɂ���
			m_bJump = true;			//�W�����v������Ԃɂ���

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}
	}
	else if (pInputJoyPad->GetPressLX(0).y > 0.0f)
	{//��
		if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
			m_bDash = true;			//�_�b�V��������Ԃɂ���
			m_bJump = true;			//�W�����v������Ԃɂ���

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_UP) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_UP) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_DASH_MOVE;
				m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}
	}
	else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
	{//��
		if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.y = 0.0f;		//�ړ��ʃ��Z�b�g
			m_bDash = true;			//�_�b�V��������Ԃɂ���
			m_bJump = true;			//�W�����v������Ԃɂ���

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_DOWN) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_DOWN) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_DASH_MOVE;
				m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}
	}
	else if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_X, 0) == true)
	{//J�L�[�������������Ƃ�

		if (m_rot.y > 0)
		{//�v���C���[�̌���������������

			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.z = 0.0f;		//�ړ��ʃ��Z�b�g

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * -CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * -CURVE_RL) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}
		else if (m_rot.y <= 0)
		{
			m_move.x = 0.0f;		//�ړ��ʃ��Z�b�g
			m_move.z = 0.0f;		//�ړ��ʃ��Z�b�g

			if (m_bCollisionAlpha == true)
			{//�����_�b�V��

				m_bDashAuto = true;		//�����_�b�V��������Ԃɂ���

				m_moveSave.x = sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_moveSave.y = cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH_AUTO);
			}
			else if (m_bCollisionAlpha == false)
			{
				m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_DASH_MOVE;
				m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;

				//BGM�Đ�
				pSound->Play(pSound->SOUND_LABEL_SE_DASH);

				m_nDashCounter++;		//�_�b�V���񐔉��Z
			}
		}

		m_bDash = true;		//�_�b�V��������Ԃɂ���
		m_bJump = true;		//�W�����v������Ԃɂ���
	}
}

//==============================================================
//�v���C���[�̃p�b�h�̈ړ����쏈��(��O��)
//==============================================================
void CPlayer::ControlFrontJoyPadMove(void)
{
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//�p�b�h�̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾

	//�ړ�
	if (pInputJoyPad->GetPressLX(0).x > 0.0f)
	{//�E

		if (pInputJoyPad->GetPressLX(0).y > 0.0f)
		{//��

			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * FRONT_MOVE;
			m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.75f;

			m_bMove = true;		//��������
		}
		else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
		{//��

			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * FRONT_MOVE;
			m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.25f;

			m_bMove = true;		//��������
		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;
		}

		m_bMove = true;		//��������
	}
	else if (pInputJoyPad->GetPressLX(0).x < 0.0f)
	{//��

		if (pInputJoyPad->GetPressLX(0).y > 0.0f)
		{//��

			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * FRONT_MOVE;
			m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.75f;

			m_bMove = true;		//��������
		}
		else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
		{//��

			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * FRONT_MOVE;
			m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.25f;

			m_bMove = true;		//��������
		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * FRONT_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;
		}

		m_bMove = true;		//��������
	}
	else if (pInputJoyPad->GetPressLX(0).y > 0.0f)
	{//��

		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_MOVE;
		m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * FRONT_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_UP;

		m_bMove = true;		//��������
	}
	else if (pInputJoyPad->GetPressLX(0).y < 0.0f)
	{//��

		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_MOVE;
		m_move.y += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * FRONT_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_UP;

		m_bMove = true;		//��������
	}

	if ((m_move.x <= STOP_MOVE && m_move.x >= -STOP_MOVE) && (m_move.z <= STOP_MOVE && m_move.z >= -STOP_MOVE))
	{//�����ĂȂ��Ƃ�

		m_bMove = false;		//�����ĂȂ���Ԃɂ���
	}
}

//==============================================================
//�v���C���[�̎����_�b�V������
//==============================================================
void CPlayer::SetDashAuto(bool bDashAuto)
{ 
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾

	m_bCollisionAlpha = bDashAuto;

	if (m_bCollisionAlpha == true)
	{//�����u���b�N�ɓ���������

		m_nDashCounter = 0;		//�_�b�V���J�E���^�[�����Z�b�g
	}
	else if (m_bCollisionAlpha == false)
	{//�����u���b�N�ɓ������ĂȂ��Ƃ�

		m_bDashAuto = false;		//�����_�b�V�����Ȃ���Ԃɂ���
	}
}

//==============================================================
//��ʊO���菈��
//==============================================================
void CPlayer::Screen(void)
{
	if (m_pos.y <= 0.0f && m_pos.x <= 35.0f)
	{//�ǒʂ蔲������

		m_pos.x = 35.0f;
		m_move.x = 0.0f;
	}

	if (m_pos.y <= 0.0f && m_pos.x >= 1330.0f)
	{//�ǒʂ蔲������

		m_pos.x = 1300.0f;
		m_move.x = 0.0f;
	}

	if (m_pos.y <= -6000.0f)
	{//��ԉ��܂ōs������

		CFade *pFade = CManager::GetInstance()->GetFade();			//�t�F�[�h�̏��擾
		CDeathUI *pDeathUI = CGame::GetDeathUI();
		CItemUI *pItemUI = CGame::GetItemUI();

		//�X�R�A�ۑ�
		CManager::GetInstance()->SetNumDeath(pDeathUI->Get());
		CManager::GetInstance()->SetNumItem(pItemUI->Get());

		//���U���g�̔���
		CManager::GetInstance()->SetResult(true);

		//�Q�[�����
		pFade->SetFade(CScene::MODE_RESULT);
	}
}

//==============================================================
//�v���C���[�̕`�揈��
//==============================================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X��������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//�v���C���[�̕`��
		m_apModel[nCntPlayer]->Draw();
	}
}

//==============================================================
//�v���C���[�̃q�b�g����
//==============================================================
void CPlayer::Hit(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();
	CDeathUI *pDeathUI = CGame::GetDeathUI();
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (m_state != CObject::STATE_DAMAGE && m_state != CObject::STATE_APPEAR && m_nCntDamage <= 0 && m_nCntHit <= 0 &&
		pFade->GetFade() != CFade::FADE_IN)
	{
		//BGM�Đ�
		pSound->Play(pSound->SOUND_LABEL_SE_DAMAGE001);

		m_state = CObject::STATE_APPEAR;

		m_nCntDamage = HIT_CNT;				//�_���[�W��Ԃ�ۂ��Ԑݒ�
		m_nCntHit = 60;				//�U��������܂ł̃J�E���^�[

		pDeathUI->Add(1);		//���S�����Z

		//�t�F�[�h������
		pFade->SetNormalFade();

		for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
		{
			//�v���C���[�̐F�ݒ�
			m_apModel[nCntPlayer]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}

		//�p�[�e�B�N������
		//CParticle::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PARTICLETYPE_EXPLOSION, 30, 40);

		//��Ԑݒ�
		for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
		{
			m_apModel[nCntPlayer]->SetState(m_state);		//�_���[�W��Ԃɂ���
		}
	}
}

//==============================================================
//���f���t�@�C���ǂݍ��ݏ���
//==============================================================
void CPlayer::LoadFile(void)
{
	FILE *pFile;				//�t�@�C���|�C���^
	char aString[MAX_STR];		//�����ǂݍ���
	int nIndex = 0, nParent = 0;	//�p�[�cNo.,�e�ԍ�
	D3DXVECTOR3 pos, rot;

	//�t�@�C���J��
	pFile = fopen(FILE_HUMAN, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (strcmp(&aString[0], "CHARACTERSET") != 0)
		{//[CHARACTERSET]����܂ł̊�

			fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���
		}

		if (strcmp(&aString[0], "CHARACTERSET") == 0)
		{//[CHARACTERSET]��������

			while (strcmp(&aString[0], "END_CHARACTERSET") != 0)
			{//[END_CHARACTERSET]������܂ł̊�

				fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

				if (strcmp(&aString[0], "PARTSSET") == 0)
				{//[PARTSSET]��������

					while (strcmp(&aString[0], "END_PARTSSET") != 0)
					{//[END_PARTSSET]������܂ł̊�

						fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

						if (strcmp(&aString[0], "INDEX") == 0)
						{//�p�[�cNo.

							fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���
							fscanf(pFile, "%d", &nIndex);			//�p�[�cNo.�ǂݍ���

						}
						else if (strcmp(&aString[0], "PARENT") == 0)
						{//�e���

							fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���
							fscanf(pFile, "%d", &nParent);			//�e�ԍ��ǂݍ���

							if (nParent == -1)
							{//�e�����Ȃ�������

								m_apModel[nIndex]->SetParent(NULL);		//NULL������
							}
							else
							{//�e��������

								m_apModel[nIndex]->SetParent(m_apModel[nParent]);		//�e�ԍ������
							}
						}
						else if (strcmp(&aString[0], "POS") == 0)
						{//�ʒu���

							fscanf(pFile, "%s", &aString[0]);	//�����ǂݍ���

							fscanf(pFile, "%f", &pos.x);		//�ʒu�ǂݍ���
							fscanf(pFile, "%f", &pos.y);		//�ʒu�ǂݍ���
							fscanf(pFile, "%f", &pos.z);		//�ʒu�ǂݍ���

							m_apModel[nIndex]->SetPosition(pos);		//�ʒu�ݒ�
							m_apModel[nIndex]->SetDefaultPosition(pos);	//�����ʒu�ݒ�

						}
						else if (strcmp(&aString[0], "ROT") == 0)
						{//�������

							fscanf(pFile, "%s", &aString[0]);	//�����ǂݍ���

							fscanf(pFile, "%f", &rot.x);		//�����ǂݍ���
							fscanf(pFile, "%f", &rot.y);		//�����ǂݍ���
							fscanf(pFile, "%f", &rot.z);		//�����ǂݍ���

							m_apModel[nIndex]->SetRotation(rot);		//�����ݒ�
							m_apModel[nIndex]->SetDefaultRotation(rot);	//���������ݒ�
						}
					}
				}
			}
		}

		//�t�@�C������
		fclose(pFile);
	}
}