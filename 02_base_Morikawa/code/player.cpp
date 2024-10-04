//========================================
//
// �v���C���[[player.cpp]
// Author�F�X��x��
//
//========================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "particle.h"
#include "useful.h"
#include "gauge.h"
#include "main.h"
#include "model.h"
#include "motion.h"
#include "effect.h"
#include "object2D.h"
#include "object3D.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "calculation.h"

#include "stageobj.h"
#include "obstacle.h"
#include "reverse.h"
#include "timer.h"

//========================================
// �萔��`
//========================================
namespace
{
	const int LIFE = 3;			// �̗�
	const float SPEED = 500.0f;		// ���x
	const float SPEED_SCALE = 75.0f; // �����x
	const float JUMP_MOVE = 1500.0f;	// �W�����v��
	const float JUMP_SAB = JUMP_MOVE * 0.04f;	// �W�����v����
	const float ROLL_TIME = 0.3f;	// �]����p������
	const float RADIUS = 90.0f;		// ���a
	const float HEIGHT = 200.0f;	// �g��
	const float HEIGHT_SCALE = 0.5f;	// �]���蒆�̐g���{��
	const int DAMAGE_COUNT = 80;		// �_���[�W�J�E���g

	const char* RABBIT_PASS = "data\\FILE\\rabbit.txt"; // �e�p�X
	const char* TURTLE_PASS = "data\\FILE\\turtle.txt"; // �T�p�X

	const float WALK_COUNT = 0.3f; // ���s���̃C���^�[�o��
}

//========================================
// �ÓI�����o�ϐ�
//========================================
CPlayer* CPlayer::m_pPlayer = nullptr;

//========================================
//�R���X�g���N�^
//========================================
CPlayer::CPlayer(int nPriority) : CCharacter(nPriority),
m_apNumModel	(0),			// ���f���̑���
m_nLife			(0),			// �̗�
m_nOldMotion	(0),			// �O��̃��[�V����
m_WalkCounter	(0),			// ���s���G�t�F�N�g�o���p�̃J�E���^�[
m_State			(STATE_NONE),	// ���
m_fRadius		(0.0f),			// ���a
m_fHeight		(0.0f),			// �g��
m_fRollTime		(0.0f),			// �]���莞��
m_fSpeedScale	(0.0f),			// �����x
m_fWalkSound	(0.0f),			// ���s��
m_bJump			(false),		// �W�����v�t���O
m_bRoll			(false),		// �]����t���O
m_pEffect		(nullptr),		// �G�t�F�N�g�̃|�C���^
m_pGauge		(nullptr),		// �Q�[�W�̃|�C���^
m_type			(TYPE_NONE),	// �^�C�v
m_typeDefault	(TYPE_NONE)		// �f�t�H���g�^�C�v
{
	memset(&m_apModel[0], 0, sizeof(m_apModel));	//���f�����
}

//========================================
//�f�X�g���N�^
//========================================
CPlayer::~CPlayer()
{
}

//========================================
//�v���C���[�̐���
//========================================
CPlayer *CPlayer::Create(std::string pfile)
{
	if (m_pPlayer == nullptr)
	{
		//�v���C���[����
		m_pPlayer = new CPlayer;

		//������
		m_pPlayer->Init(pfile);
	}

	//�|�C���^��Ԃ�
	return m_pPlayer;
}

//========================================
// ������
//========================================
HRESULT CPlayer::Init(std::string pfile)
{
	// �L�����̏�����
	CCharacter::Init(pfile);

	// �v���C���[��Ԃ̏�����
	m_State = STATE_NORMAL;
	m_typeDefault = m_type = TYPE_TURTLE;

	// �ʒu�ݒ�
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �����ݒ�
	SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	// �ړ��ʐݒ�
	SetMove(D3DXVECTOR3(SPEED, 0.0f, 0.0));

	// ���s���̃J�E���^�[
	m_WalkCounter = 0;

	// ����������
	m_nLife = LIFE;
	m_fRadius = RADIUS;
	m_fHeight = HEIGHT;

	// �Q�[�W����
	m_pGauge = CGauge::Create(m_nLife);

	// �ʒu�ݒ�
	m_pGauge->SetPos(D3DXVECTOR3(50.0f, 650.0f, 0.0f));

	// �T�C�Y�ݒ�
	m_pGauge->SetSize(50.0f, 50.0f);

	return S_OK;
}

//========================================
//�I��
//========================================
void CPlayer::Uninit(void)
{
	if (m_pGauge != nullptr)
	{
		m_pGauge->Uninit();
		m_pGauge = nullptr;
	}

	// �I��
	CCharacter::Uninit();
	m_pPlayer = nullptr;
}

//========================================
//�X�V
//========================================
void CPlayer::Update(void)
{
	// ��Ԃ�ۑ�����
	PLAYERTYPE type = m_type;

	// ���g�̏����擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	// �f���^�^�C���̎擾
	const float fDeltaTime = CManager::GetInstance()->GetDeltaTime();

	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �J�����̒Ǐ]�ݒ�
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
#ifndef _DEBUG
	pCamera->Following(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
#endif // !_DEBUG
#if 1
	pCamera->Following(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
#endif // 0

	// �ړ�����
	Move(pos, move, fDeltaTime);

	// ���n����
	Land(pos, move);

	// �_���[�W�J�E���^�[
	DamageCounter();

	// �X�e�[�W�I�u�W�F�Ƃ͈̔̓`�F�b�N
	CheckStageObjRange();

	// ���]�I�u�W�F�̓����蔻��
	CollisionReverseObj();

	// �Q�[�W�ɑ̗͐ݒ�
	m_pGauge->SetLife(m_nLife);

	// ��Ԃ��ς���Ă����ꍇ���f����ύX����
	if (type != m_type)
	{

		// �ω�
		Myparticle::Create(Myparticle::TYPE::TYPE_CHANGE, GetPos());

		// �T�E���h�Đ�
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_CHANGE);

		// ���[�V�������̎擾
		CMotion* pMotion = GetMotion();

		// ���[�V�������폜
		if (pMotion != nullptr)
		{//���[�V�����j��
			pMotion->Uninit();
			delete pMotion;
			pMotion = nullptr;
		}

		// ���[�V��������
		pMotion = CMotion::Create();

		switch (m_type)
		{
		case TYPE_RABBIT:
			// ���[�V�����ǂݍ���
			pMotion->Load(RABBIT_PASS);
			break;

		case TYPE_TURTLE:
			// ���[�V�����ǂݍ���
			pMotion->Load(TURTLE_PASS);
			break;

		default:
			break;
		}
	}

	// ���[�V����
	Motion();

	// ����ݒ�
	SetPos(pos);
	SetMove(move);

	// �����蔻�菈��
	Collision();

	D3DXVECTOR3 rot = GetRot();
	// �f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "�v���C���[�̈ʒu�F%f�A%f�A%f\n", pos.x, pos.y, pos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "�v���C���[�̈ړ��ʁF%f�A%f�A%f\n", move.x, move.y, move.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "�v���C���[�̌����F%f�A%f�A%f\n", rot.x, rot.y, rot.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "�v���C���[�̃T�C�Y�F%f�A%f�A%f\n", m_fRadius, m_fHeight, m_fRadius);
	DebugProc::Print(DebugProc::POINT_LEFT, "�v���C���[�̗̑́F%d\n", m_nLife);
}

//========================================
//�`��
//========================================
void CPlayer::Draw(void)
{
	// �`��
	CCharacter::Draw();
}

//========================================
// �_���[�W�J�E���^�[
//========================================
void CPlayer::DamageCounter()
{
	// �_���[�W�J�E���g��0�ɂ���
	m_nDamageCount--;

	if (m_nDamageCount <= 0)
	{ // �J�E���g�����l�ȉ��̏ꍇ

		// �J�E���g��␳����
		m_nDamageCount = 0;
	}
}

//========================================
// ���[�V�����Ǘ�
//========================================
void CPlayer::Motion()
{
	// ���[�V�������擾
	CMotion* pMotion = GetMotion();

	// ���[�V������null�̏ꍇ�֐��𔲂���
	if (pMotion == nullptr)
	{
		assert(false);
		return;
	}

	// ���[�V������ݒ�
	if (m_bJump) // �W�����v���
	{
		// �W�����v���[�V������ݒ�
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_JUMP);
		DebugProc::Print(DebugProc::POINT_CENTER, "�W�����v���\n");
	}
	else if (m_bRoll) // �]������
	{
		// �]���胂�[�V������ݒ�
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_ROLL);
		DebugProc::Print(DebugProc::POINT_CENTER, "�]������\n");
	}
	else // ���̑��̏��
	{
		// ���s���[�V������ݒ�
		pMotion->Set(CMotion::PLAYER_MOTIONTYPE_WALK);
	}

	// ���[�V�������X�V
	pMotion->Update();
}

//==========================================
//  �ړ�����
//==========================================
void CPlayer::Move(D3DXVECTOR3& pos, D3DXVECTOR3& move, const float fDeltaTime)
{
	// �C���v�b�g���̎擾
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputPad* pPad = CManager::GetInstance()->GetInputPad();

#ifdef _DEBUG
	// 2�L�[�ŏ����ʒu�ɖ߂�
	if (pKeyboard->GetTrigger(DIK_2))
	{
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		return;
	}

#endif

	// ��Ԃ̐؂�ւ�
	SwitchType(pPad, pKeyboard);

	// ��Ԃ��Ƃ̏���
	switch (m_type)
	{
	case TYPE_RABBIT:
		// �W�����v����
		Jump(move, pPad, pKeyboard);
		DebugProc::Print(DebugProc::POINT_CENTER, "���A�e�ł��I�I�I\n");
		break;

	case TYPE_TURTLE:
		// �]���菈��
		Roll(move, pPad, pKeyboard, fDeltaTime);
		DebugProc::Print(DebugProc::POINT_CENTER, "���A�T�ł��I�I�I\n");
		break;

	default:
		assert(false);
		break;
	}

	// ����
	SpeedUp(move);

	// ���W�Ɉړ��ʂ����Z
	pos += move * fDeltaTime;

	// �T�E���h���Z
	m_fWalkSound += fDeltaTime;

	// �T�E���h�Đ�
	if (m_fWalkSound >= WALK_COUNT)
	{
		CSound* pSound = CManager::GetInstance()->GetSound();
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		m_fWalkSound = 0.0f;
	}
}

//==========================================
//  �W�����v����
//==========================================
void CPlayer::Jump(D3DXVECTOR3& move, CInputPad* pPad, CInputKeyboard* pKeyboard)
{
	// �W�����v���̏ꍇ�֐��𔲂���
	if (m_bJump) { return; }

	// �{�^���������ꂽ��W�����v����
	if (pPad->GetTrigger(CInputPad::BUTTON_A, 0) || pKeyboard->GetTrigger(DIK_SPACE))
	{
		// �W�����v�t���O��ݒ�
		m_bJump = true;

		// �ړ��ʂ����Z
		move.y += JUMP_MOVE;

		// �T�E���h���擾
		CSound* pSound = CManager::GetInstance()->GetSound();

		// �T�E���h��~
		pSound->Stop(CSound::SOUND_LABEL_SE_JUMP);

		// �T�E���h�Đ�
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_JUMP);
	}
}

//==========================================
//  �]���菈��
//==========================================
void CPlayer::Roll(D3DXVECTOR3& move, CInputPad* pPad, CInputKeyboard* pKeyboard, const float fDeltaTime)
{
	// �T�E���h���擾
	CSound* pSound = CManager::GetInstance()->GetSound();

	// �]���蒆�̏ꍇ���Ԃ����Z
	if (m_bRoll)
	{
		// ���Z
		m_fRollTime += fDeltaTime;

		// ��]
		MyLib::Vector3 rot = GetRot();
		rot.y = UtilFunc::Correction::EasingLinear(0.0f, (D3DX_PI * 2.0f) * 10.0f, 0.0f, ROLL_TIME, m_fRollTime);
		rot.y += -D3DX_PI * 0.5f;
		SetRot(rot);

		Myparticle::Create(Myparticle::TYPE::TYPE_ROLLINGTURTLE, GetPos());

		// ��莞�Ԍo�߂��Ă��Ȃ��ꍇ�֐��𔲂���
		if (m_fRollTime < ROLL_TIME) { return; }

		// �]�����Ԃ���������
		m_fRollTime = 0.0f;
		m_bRoll = false;
		m_fHeight = HEIGHT;
		m_State = STATE_NORMAL;

		// �T�E���h��~
		pSound->Stop(CSound::SOUND_LABEL_SE_ROWLING);

		return;
	}

	// �{�^���������ꂽ��]�����ԂɂȂ�
	if (pPad->GetTrigger(CInputPad::BUTTON_A, 0) || pKeyboard->GetTrigger(DIK_SPACE))
	{
		// �t���O��true
		m_bRoll = true;

		// �g�������炷
		m_fHeight *= HEIGHT_SCALE;

		// ��]��Ԃɂ���
		m_State = STATE_ROLL;

		// �T�E���h�Đ�
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ROWLING);
	}
}

//==========================================
//  ��Ԃ̐؂�ւ�
//==========================================
void CPlayer::SwitchType(CInputPad* pPad, CInputKeyboard* pKeyboard)
{
	// �]�����Ԃ������̓W�����v���̏ꍇ�֐��𔲂���
	if (m_bJump || m_bRoll) { return; }

	// �^�C�v���f�t�H���g�ŏ㏑������
	m_type = m_typeDefault;

	// �{�^����������Ă��Ȃ��ꍇ�֐��𔲂���
	if(!pPad->GetPress(CInputPad::BUTTON_LB, 0) && !pKeyboard->GetPress(DIK_LSHIFT)) { return; }

	// �f�t�H���g�^�C�v�����ă^�C�v��ύX����
	m_type = m_typeDefault == TYPE_TURTLE ? TYPE_RABBIT : TYPE_TURTLE;
}

//==========================================
//  ���n����
//==========================================
void CPlayer::Land(D3DXVECTOR3& pos, D3DXVECTOR3& move)
{
	// y���W��0.0����������ꍇ�␳����
	if (pos.y < 0.0f)
	{
		// �W�����v�t���O��false
		m_bJump = false;

		// ���W��␳
		pos.y = 0.0f;

		// �ړ��ʂ��폜
		move.y = 0.0f;
	}

	// �W�����v���Ȃ�ړ��ʂ�����������
	if (m_bJump)
	{
		move.y -= JUMP_SAB;
	}
}

//==========================================
// �_���[�W�֐�
//==========================================
void CPlayer::Damage()
{
	// �_���[�W�J�E���g��ݒ肷��
	m_nDamageCount = DAMAGE_COUNT;

	// ���������Z����
	m_nLife--;

	// �p�[�e�B�N�����o��
	Myparticle::Create(Myparticle::TYPE_DEATH, GetPos());
}

//==========================================
// �X�e�[�W�I�u�W�F�͈̔̓`�F�b�N
//==========================================
void CPlayer::CheckStageObjRange()
{
	// �X�e�[�W�I�u�W�F�̃��X�g�擾
	CListManager<CStageObj> list = CStageObj::GetList();

	// �I�[��ۑ�
	std::list<CStageObj*>::iterator itr = list.GetEnd();
	CStageObj* pObj = nullptr;

	// �ʒu�擾
	MyLib::Vector3 pos = GetPos();

	// �I�[�܂Ń��[�v
	while (list.ListLoop(itr))
	{
		CStageObj* pObj = *itr;
		pObj->CollisionRange(pos);
	}
}

//==========================================
// ���]�I�u�W�F�̓����蔻��
//==========================================
void CPlayer::CollisionReverseObj()
{
//#ifndef _DEBUG

	// ���]�I�u�W�F�̃��X�g�擾
	CListManager<CReverse> list = CReverse::GetList();

	// �I�[��ۑ�
	std::list<CReverse*>::iterator itr = list.GetEnd();
	CReverse* pObj = nullptr;

	// �ʒu�擾
	D3DXMATRIX mtx = GetmtxWorld();

	// �I�[�܂Ń��[�v
	while (list.ListLoop(itr))
	{
		CReverse* pObj = *itr;
		if (pObj->Collision(mtx, MyLib::Vector3(RADIUS, HEIGHT, RADIUS),(PLAYERSTATE)(m_State)))
		{
			// ���]
			m_typeDefault = (m_typeDefault == TYPE_RABBIT) ? PLAYERTYPE::TYPE_TURTLE : PLAYERTYPE::TYPE_RABBIT;
			return;
		}
	}

//#endif // !_DEBUG
}

//==========================================
// �����蔻��
//==========================================
void CPlayer::Collision()
{
//#ifndef _DEBUG

	// �_���[�W���󂯂Ȃ���Ԃ̏ꍇ�A���̊֐��𔲂���
	if (m_nDamageCount > 0) { return; }

	// ��Q���̃��X�g�擾
	CListManager<CObstacle> list = CObstacle::GetList();

	// �I�[��ۑ�
	std::list<CObstacle*>::iterator itr = list.GetEnd();
	CObstacle* pObj = nullptr;

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXMATRIX mtx;
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	SetmtxWorld(mtx);

	// �I�[�܂Ń��[�v
	while (list.ListLoop(itr))
	{
		CObstacle* pObj = *itr;

		if (pObj->Collision(mtx, D3DXVECTOR3(RADIUS, HEIGHT, RADIUS), (PLAYERSTATE)(m_State)))
		{ // �����蔻��ɓ��������ꍇ

			// �����𔼕��ɂ���
			m_fSpeedScale *= 0.5f;

			// �_���[�W����
			Damage();
		}
	}

//#endif // !_DEBUG
}

//==========================================
//  ��������
//==========================================
void CPlayer::SpeedUp(D3DXVECTOR3& move)
{
	// �����x�����Z
	m_fSpeedScale += CManager::GetInstance()->GetDeltaTime();

	// �ړ��ʂ����Z
	move.x = SPEED + m_fSpeedScale * SPEED_SCALE;

	DebugProc::Print(DebugProc::POINT_RIGHT, "�����x : %f", m_fSpeedScale);
}