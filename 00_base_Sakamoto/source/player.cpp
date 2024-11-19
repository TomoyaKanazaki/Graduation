//============================================
//
//	�v���C���[�̏��� [player.cpp]
//	Author:sakamoto kai
//
//============================================
#include "player.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "enemy.h"
#include "objGauge2D.h"
#include "CubeBlock.h"
#include "slowManager.h"
#include "Number.h"
#include "MapModel.h"
#include "effect.h"
#include "sound.h"
#include "LifeUi.h"
#include "cross.h"
#include "bowabowa.h"
#include "fire.h"
#include "DevilHole.h"
#include "devil.h"
#include "bible.h"
#include "RailBlock.h"
#include "objmeshField.h"
#include "CubeBlock.h"
#include "RollRock.h"

#include "MyEffekseer.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	const int LIFE_MAX = 2;	//�������C�t��
	const int FIRE_STOPTIME = 30;	//�U�����̈ړ���~����
	const D3DXVECTOR3 RESPAWN_POS = D3DXVECTOR3(-100.0f, 2000.0f, 100.0f); // �����ʒu
	const float RESPAWN_GRAVITY = 0.3f;			//���̏d��
	const int INVINCIBLE_TIME = 120;			//���G����

	const float GRIT_OK = 45.0f;			//�ړ��\�ȃO���b�g�͈͓̔�
	const float PLAYER_SPEED = 5.0f;		//�v���C���[�̈ړ����x
	const float OBJDISTANCE = 10000.0f;		// �I�u�W�F�N�g�̋���

	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(35.0f, 40.0f, 35.0f);		//���̓����蔻��
	const D3DXVECTOR3 LIFE_POS = D3DXVECTOR3(50.0f, 650.0f, 0.0f);

	const float CROSS_TIME = 10.0f; // �\���˂��������Ă����鎞��

	const float EGG_GRAVITY = 0.98f;	 //�ړ��ʂ̌������x
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //�ړ��ʂ̌������x
	const float EGG_ROT = D3DX_PI * 0.006f;		//��]���x
	const float EGG_MOVE_DEL = 0.9f;			//�ړ��ʂ̌������x
	const float EGG_COLOR_DEL_A = 0.01f;		//�s�����x�̌������x
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CPlayer>* CPlayer::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CPlayer::CPlayer(int nPriority) : CObject(nPriority),
m_size(INITVECTOR3),
m_pos(INITVECTOR3),
m_move(INITVECTOR3),
m_Objmove(INITVECTOR3),
m_rot(INITVECTOR3),
m_AutoMoveRot(INITVECTOR3),
m_bJump(false),
m_nActionCount(0),
m_Action(ACTION_NONE),
m_AtkAction(ACTION_NONE),
m_State(STATE_EGG),
m_nStateCount(0),
m_AtkPos(INITVECTOR3),
m_CollisionRot(0.0f),
m_pCharacter(nullptr),
m_OKL(true),
m_OKR(true),
m_OKU(true),
m_OKD(true),
m_bInput(false),
m_pLifeUi(nullptr),
m_nLife(0),
m_eItemType(TYPE_NONE),
m_MoveState(MOVE_STATE_NONE),
m_Grid(0, 0),
m_bGritCenter(true),
m_bPressObj(false),
m_fCrossTimer(0.0f),
m_pUpEgg(nullptr),
m_pDownEgg(nullptr),
m_EggMove(INITVECTOR3),
m_bInvincible(true),
m_nInvincibleCount(0)
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CPlayer::~CPlayer()
{

}

//====================================================================
//��������
//====================================================================
CPlayer* CPlayer::Create()
{
	CPlayer* pPlayer = new CPlayer();

	// �������̊m�ۂɎ��s�����ꍇnull��Ԃ�
	if (pPlayer == nullptr) { assert(false); return nullptr; }

	// �����������Ɏ��s�����ꍇnull��Ԃ�
	if (FAILED(pPlayer->Init()))
	{
		assert(false);
		delete pPlayer;
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//����������
//====================================================================
HRESULT CPlayer::Init(void)
{
	// �T�C�Y�̐ݒ�
	m_size = COLLISION_SIZE;

	// �����̐ݒ�
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	// �A�N�V�����̐ݒ�
	m_Action = ACTION_EGG;
	m_AtkAction = ACTION_EGG;

	//��ސݒ�
	SetType(CObject::TYPE_PLAYER3D);

	// �̗͂̐ݒ�
	m_nLife = LIFE_MAX;

	// ��Ԃ̐ݒ�
	m_MoveState = MOVE_STATE_WAIT;

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetMultiMatrix(true);

	if (m_pCharacter == nullptr)
	{
		m_pCharacter = CCharacter::Create("data\\TXT\\motion_tamagon1P.txt");
	}

	m_pLifeUi = CLifeUi::Create();

	// ���l
	m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS.x + 200.0f, LIFE_POS.y, LIFE_POS.z));

	// �A�C�e����Ԃ�ݒ�
	SetItemType(CPlayer::TYPE_NONE);

	// �̗�
	m_pLifeUi->SetPos(LIFE_POS);
	m_pLifeUi->GetNumber()->SetNumber(m_nLife);

	// �X���[�̐���
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CPlayer::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �L�����N�^�[�̏I������
	if (m_pCharacter != nullptr)
	{
		// �L�����N�^�[�̔j��
		m_pCharacter->Uninit();
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CPlayer::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_TUTORIAL:
		TutorialUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CPlayer::TitleUpdate(void)
{
	// �L�����N�^�[�X�V����
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CPlayer::GameUpdate(void)
{
	// �ߋ��̈ʒu�ɑ��
	m_posOld = m_pos;

	if (m_State != STATE_DEATH)
	{
		//�ǂ����邩���f
		SearchWall();

		if (
			(m_State != STATE_EGG && CollisionStageIn() == true && 
			CMapSystem::GetInstance()->GetGritBool(m_Grid.x,m_Grid.z) == false)||
			(m_State == STATE_EGG && CollisionStageIn() == true &&
			CMapSystem::GetInstance()->GetGritBool(m_Grid.x, m_Grid.z) == false &&
			m_bGritCenter == true && m_pos.y <= 0.0f)
			)
		{// �X�e�[�W���ɂ��� ���� �u���b�N�̖����O���b�h��̎�
			// �ړ�����
			Move();
		}

		// �����ړ�����
		Rot();

		if (m_eItemType != TYPE_NONE)
		{
			Attack();
		}

		// �\���˂������Ă���ꍇ
		if (m_eItemType == TYPE_CROSS)
		{
			// �^�C�}�[�����Z
			m_fCrossTimer += DeltaTime::Get();

			// �\���˂̏����\���Ԃ𒴉߂����ꍇ
			if (m_fCrossTimer >= CROSS_TIME)
			{
				// �������ԃ^�C�}�[�����Z�b�g
				m_fCrossTimer = 0.0f;

				// �A�C�e�����������Ă��Ȃ���Ԃɂ���
				SetItemType(TYPE_NONE);
			}
		}

		// �J�����X�V����
		CameraPosUpdate();

		if (m_State == STATE_WALK)
		{
			// �ʒu�X�V����
			PosUpdate();
		}

		ObjPosUpdate();

		if (m_State != STATE_EGG)
		{
			//��ʊO����
			CollisionStageOut();
		}

		// �G�̔���
		CollisionEnemy();
	}

	// �v���C���[���}�b�v�̂ǂ̃}�X�ɑ��݂��Ă��邩�ݒ肷��
	m_Grid.x = CMapSystem::GetInstance()->CMapSystem::CalcGridX(m_pos.x);
	m_Grid.z = CMapSystem::GetInstance()->CMapSystem::CalcGridZ(m_pos.z);

	//��Ԃ̊Ǘ�
	StateManager();

	if (m_nInvincibleCount > 0)
	{
		m_nInvincibleCount--;
	}
	else
	{
		m_bInvincible = false;
	}

	if (m_bInvincible)
	{
		SetModelColor(CModel::COLORTYPE_TRUE_ALL, D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
	}
	else
	{
		SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//���̓���
	EggMove();

	// �L�����N�^�[�X�V����
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}

	//���[�V�����̊Ǘ�
	ActionState();

	//�f�o�b�O�L�[�̏����Ɛݒ�
	DebugKey();

	//�f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]�ʒu %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]���� %f : %f : %f\n", m_rot.x, m_rot.y, m_rot.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]�� %d : �c %d\n", m_Grid.x, m_Grid.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]��� : ");
	auto str = magic_enum::enum_name(m_State);
	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");

}

//====================================================================
//�`���[�g���A���ł̍X�V����
//====================================================================
void CPlayer::TutorialUpdate(void)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CPlayer::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMATRIX mtxShadow;	// �V���h�E�}�g���b�N�X
	D3DLIGHT9 light;		// ���C�g���
	D3DXVECTOR4 posLight;	// ���C�g�̈ʒu
	D3DXVECTOR3 pos, normal;	// ���ʏ�̔C�ӂ̓_�A�@���x�N�g��
	D3DXPLANE plane;		// ���ʏ��

	// ���C�g�̈ʒu�ݒ�
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.05f);

	// ���ʏ��𐶐�
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// �V���h�E�}�g���b�N�X�̏�����
	
	
	// �V���h�E�}�g���b�N�X�̍쐬
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// �V���h�E�}�g���b�N�X�̐ݒ�


	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bMultiMatrix)
	{
		SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

		//�Z�o�����}�g���N�X���������킹��
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_UseMultiMatrix);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//�X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�̐ݒ� => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	//�X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//�X�e���V���o�b�t�@�̔�r���@ => (�Q�ƒl => �X�e���V���o�b�t�@�̎Q�ƒl)�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//�X�e���V���e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �L�����N�^�[�`�揈��
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}

	//�X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
//�ړ�����
//====================================================================
void CPlayer::Move(void)
{
	if (m_pCharacter == nullptr)
	{
		assert(("�L�����N�^�[���Ȃ�", false));
		return;
	}

	m_bInput = false;

	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	NormarizeMove = MoveInputKey(NormarizeMove);

	NormarizeMove = MoveInputPadStick(NormarizeMove);

	NormarizeMove = MoveInputPadKey(NormarizeMove);

	if (m_bInput && m_State != STATE_ATTACK)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;

		//�ړ��ʂ���
		m_move = NormarizeMove;

		if (m_State == STATE_EGG)
		{
			// ���f�����̎擾
			int nNumModel = m_pCharacter->GetNumModel();

			for (int nCnt = 0; nCnt < nNumModel; nCnt++)
			{
				CModel* pModel = m_pCharacter->GetModel(nCnt);
				
				if (pModel != nullptr)
				{
					pModel->SetDisp(true);
				}
			}

			m_EggMove.y = EGG_MOVE.y;

			switch (m_MoveState)
			{
			case CPlayer::MOVE_STATE_LEFT:
				m_EggMove.x = EGG_MOVE.x;
				m_EggMove.z = 0.0f;
				break;
			case CPlayer::MOVE_STATE_RIGHT:
				m_EggMove.x = -EGG_MOVE.x;
				m_EggMove.z = 0.0f;
				break;
			case CPlayer::MOVE_STATE_UP:
				m_EggMove.x = 0.0f;
				m_EggMove.z = -EGG_MOVE.z;
				break;
			case CPlayer::MOVE_STATE_DOWN:
				m_EggMove.x = 0.0f;
				m_EggMove.z = EGG_MOVE.z;
				break;
			}

			m_bInvincible = true;
			m_nInvincibleCount = INVINCIBLE_TIME;
		}

		SetItemType(m_eItemType);

		//�ړ���Ԃɂ���
		m_State = STATE_WALK;
	}
}

//====================================================================
//�ړ����̓L�[�{�[�h
//====================================================================
D3DXVECTOR3 CPlayer::MoveInputKey(D3DXVECTOR3 Move)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�L�[�{�[�h�̈ړ�����
	if ((pInputKeyboard->GetPress(DIK_W) && m_OKU && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_W) && m_MoveState == MOVE_STATE_DOWN))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_UP;
	}
	else if ((pInputKeyboard->GetPress(DIK_S) && m_OKD && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_S) && m_MoveState == MOVE_STATE_UP))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_DOWN;
	}
	else if ((pInputKeyboard->GetPress(DIK_A) && m_OKL && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_A) && m_MoveState == MOVE_STATE_RIGHT))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_LEFT;
	}
	else if ((pInputKeyboard->GetPress(DIK_D) && m_OKR && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_D) && m_MoveState == MOVE_STATE_LEFT))
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_RIGHT;
	}

	return Move;
}

//====================================================================
//�ړ����̓p�b�h�X�e�B�b�N
//====================================================================
D3DXVECTOR3 CPlayer::MoveInputPadStick(D3DXVECTOR3 Move)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (nCnt == m_nPlayNumber)
		{
			//�L�[�{�[�h�̈ړ�����
			if ((pInputJoypad->Get_Stick_Left(nCnt).y > 0.0f && m_OKU && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).y > 0.0f && m_MoveState == MOVE_STATE_DOWN))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

				Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_UP;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).y < 0.0f && m_OKD && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).y < 0.0f && m_MoveState == MOVE_STATE_UP))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

				Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_DOWN;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).x < 0.0f && m_OKL && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).x < 0.0f && m_MoveState == MOVE_STATE_RIGHT))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

				Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_LEFT;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).x > 0.0f && m_OKR && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).x > 0.0f && m_MoveState == MOVE_STATE_LEFT))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

				Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_RIGHT;
			}
		}
	}

	return Move;
}

//====================================================================
//�ړ����̓p�b�h�L�[
//====================================================================
D3DXVECTOR3 CPlayer::MoveInputPadKey(D3DXVECTOR3 Move)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (nCnt == m_nPlayNumber)
		{
			//�L�[�{�[�h�̈ړ�����
			if ((pInputJoypad->GetPress(CInputJoypad::BUTTON_UP, nCnt) && m_OKU && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_UP, nCnt) && m_MoveState == MOVE_STATE_DOWN))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

				Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_UP;
			}
			else if ((pInputJoypad->GetPress(CInputJoypad::BUTTON_DOWN, nCnt) && m_OKD && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_DOWN, nCnt) && m_MoveState == MOVE_STATE_UP))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

				Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_DOWN;
			}
			else if ((pInputJoypad->GetPress(CInputJoypad::BUTTON_LEFT, nCnt) && m_OKL && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_LEFT, nCnt) && m_MoveState == MOVE_STATE_RIGHT))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

				Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_LEFT;
			}
			else if ((pInputJoypad->GetPress(CInputJoypad::BUTTON_RIGHT, nCnt) && m_OKR && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_RIGHT, nCnt) && m_MoveState == MOVE_STATE_LEFT))
			{
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

				Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_RIGHT;
			}
		}
	}

	return Move;
}

//====================================================================
//�ړ���������
//====================================================================
void CPlayer::Rot(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//�ړ������Ɍ��������킹�鏈��
	float fRotMove, fRotDest;
	fRotMove = m_rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	if (m_State == STATE_WALK)
	{
		m_rot.y = atan2f(-m_move.x, -m_move.z);
	}

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
//�U������
//====================================================================
void CPlayer::Attack(void)
{
	if (m_State == STATE_WALK)
	{
		//�L�[�{�[�h�̎擾
		CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
		CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

		if (pInputKeyboard->GetTrigger(DIK_SPACE) || pInputJoypad->GetTrigger(CInputJoypad::BUTTON_B, 0))
		{
			// �Ή�����
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_FIRE);

			//MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HIT, false, useful::CalcMatrix(m_pos, m_rot, m_UseMultiMatrix), m_rot);

			CFire::Create("data\\model\\fireball.x", m_pos, m_rot);
			m_State = STATE_ATTACK;
			m_nStateCount = FIRE_STOPTIME;
		}
	}
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CPlayer::ActionState(void)
{
	// ���f�����̎擾
	if (m_pCharacter == nullptr)
	{
		assert(("�L�����N�^�[���Ȃ�", false));
		return;
	}

	CMotion* pMotion = m_pCharacter->GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	//�ړ����[�V����
	if (m_State == STATE_DEATH)
	{
		if (m_Action != ACTION_DEATH)
		{
			m_Action = ACTION_DEATH;
			pMotion->Set(ACTION_DEATH, 5);
		}
	}
	//�����[�V����
	else if (m_State == STATE_EGG)
	{
		if (m_Action != ACTION_EGG)
		{
			m_Action = ACTION_EGG;
			pMotion->Set(ACTION_EGG, 5);
		}
	}
	//�ړ����[�V����
	else if (m_State == STATE_ATTACK)
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			pMotion->Set(ACTION_WAIT, 5);
		}
	}
	//�ړ����[�V����
	else if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			pMotion->Set(ACTION_MOVE, 5);
		}
	}
	//�j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			pMotion->Set(ACTION_WAIT, 5);
		}
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CPlayer::StateManager(void)
{
	switch (m_State)
	{
	case STATE_WAIT:
		//	�X���[��default��
		CSlowManager::SetValueDefault();
		break;

	case STATE_WALK:
		break;

	case STATE_ATTACK:

		if (m_nStateCount == 0)
		{
			m_State = STATE_WALK;
		}

		break;

	case STATE_DEATH:
		if (m_nStateCount == 0)
		{
			//�w��ʒu����u���b�N�����݂��Ȃ��O���b�h���������Ă��̏ꏊ�ɕ������鏈��
			int WMax = CMapSystem::GetInstance()->GetWightMax();
			int HMax = CMapSystem::GetInstance()->GetHeightMax();
			CMapSystem::GRID ReivelPos = CMapSystem::GRID(0, 0);
			ReivelPos.x = CMapSystem::GetInstance()->CalcGridX(RESPAWN_POS.x);
			ReivelPos.z = CMapSystem::GetInstance()->CalcGridZ(RESPAWN_POS.z);

			for (int nSetW = ReivelPos.x, nCntW = 0; nCntW < WMax; nSetW++, nCntW++)
			{
				if (nSetW >= WMax)
				{
					nSetW = nSetW - WMax;
				}

				for (int nSetH = ReivelPos.z, nCntH = 0; nCntH < HMax; nCntH++, nCntH++)
				{
					if (nSetH >= HMax)
					{
						nSetH = nSetH - HMax;
					}

					if (CMapSystem::GetInstance()->GetGritBool(nSetW, nSetH) == false)
					{
						SetGrid(CMapSystem::GRID(nSetW, nSetH));
						m_pos = CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(nSetW, nSetH));
						m_pos.y = RESPAWN_POS.y;
						m_State = STATE_EGG;
						return;
					}

				}
			}
		}

		break;

	case STATE_EGG:

		// ���f�����̎擾
		int nNumModel = m_pCharacter->GetNumModel();

		for (int nCnt = 0; nCnt < nNumModel; nCnt++)
		{
			CModel* pModel = m_pCharacter->GetModel(nCnt);

			if (pModel != nullptr)
			{
				pModel->SetDisp(false);
			}
		}

		if (m_pUpEgg == nullptr)
		{
			m_pUpEgg = CObjectX::Create("data\\MODEL\\00_Player\\1P\\upper_egg.x");
			m_pUpEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));
			m_pUpEgg->SetMultiMatrix(true);
		}

		if (m_pDownEgg == nullptr)
		{
			m_pDownEgg = CObjectX::Create("data\\MODEL\\00_Player\\1P\\downer_egg.x");
			m_pDownEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));
			m_pDownEgg->SetMultiMatrix(true);
		}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CPlayer::CollisionWall(useful::COLLISION XYZ)
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CCubeBlock* pCubeBlock : list)
	{
		D3DXVECTOR3 pos = pCubeBlock->GetPos();
		D3DXVECTOR3 posOld = pCubeBlock->GetPosOld();
		D3DXVECTOR3 Move = pCubeBlock->GetMove();
		D3DXVECTOR3 Size = pCubeBlock->GetSize();

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, pos, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		}
	}
}

//====================================================================
// �ǂƂ̈�������
//====================================================================
void CPlayer::CollisionPressWall(useful::COLLISION XYZ)
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CCubeBlock* pCubeBlock : list)
	{
		D3DXVECTOR3 pos = pCubeBlock->GetPos();
		D3DXVECTOR3 posOld = pCubeBlock->GetPosOld();
		D3DXVECTOR3 Move = pCubeBlock->GetMove();
		D3DXVECTOR3 Size = pCubeBlock->GetSize();

		// ��`�̓����蔻��
		if (useful::CollisionRectangle2D(m_pos, pos, m_size, Size, XYZ) == true)
		{
			Death();
		}
	}
}

//====================================================================
// �~�܂��Ă��郌�[���u���b�N�Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionWaitRailBlock(useful::COLLISION XYZ)
{
	if (m_bPressObj == true)
	{
		return;
	}

	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // ���X�g���擾

	// ���[���u���b�N���X�g�̒��g���m�F����
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = pRailBlock->GetPos();
		D3DXVECTOR3 posOld = pRailBlock->GetPosOld();
		D3DXVECTOR3 Move = (pos - posOld);
		D3DXVECTOR3 Size = pRailBlock->GetSize();

		if (abs(Move.x) > 0.01f)
		{
			return;
		}
		if (abs(Move.z) > 0.01f)
		{
			return;
		}

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, pos, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		}
	}
}

//====================================================================
// �����Ă��郌�[���u���b�N�Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionMoveRailBlock(useful::COLLISION XYZ)
{
	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // ���X�g���擾

	// ���[���u���b�N���X�g�̒��g���m�F����
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float G_Size = CMapSystem::GetInstance()->GetGritSize();

		D3DXVECTOR3 Mypos = pRailBlock->GetPos();

		if (Mypos.x < D_pos.x + MapSize.x - G_Size &&
			Mypos.x > D_pos.x - MapSize.x + G_Size &&
			Mypos.z < D_pos.z + MapSize.z - G_Size &&
			Mypos.z > D_pos.z - MapSize.z + G_Size)
		{
			D3DXVECTOR3 MyposOld = pRailBlock->GetPosOld();
			D3DXVECTOR3 MyMove = (Mypos - MyposOld);
			float MySize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

			D3DXVECTOR3 pos = m_pos;
			D3DXVECTOR3 posOld = m_posOld;
			D3DXVECTOR3 Size = m_size;
			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// ��`�̓����蔻��
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &a, XYZ) == true)
			{
				m_bPressObj = true;
				return;
			}
		}
	}
}

//====================================================================
// �~�܂��Ă����Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionWaitRock(useful::COLLISION XYZ)
{
	if (m_bPressObj == true)
	{
		return;
	}

	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // ���X�g���擾

	// ���[���u���b�N���X�g�̒��g���m�F����
	for (CRollRock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(pRailBlock->GetPos().x, 0.0f, pRailBlock->GetPos().z);
		D3DXVECTOR3 posOld = pRailBlock->GetPosOld();
		D3DXVECTOR3 Move = (pos - posOld);
		D3DXVECTOR3 Size = pRailBlock->GetSize();

		switch (XYZ)
		{
		case useful::COLLISION_X:
			if (abs(Move.x) > 0.0f)
			{
				return;
			}
			break;

		case useful::COLLISION_Z:
			if (abs(Move.z) > 0.0f)
			{
				return;
			}
			break;
		}

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, pos, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		}
	}
}

//====================================================================
// �����Ă����Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionMoveRock(useful::COLLISION XYZ)
{
	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // ���X�g���擾

	// ���[���u���b�N���X�g�̒��g���m�F����
	for (CRollRock* pRock : list)
	{
		D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float G_Size = CMapSystem::GetInstance()->GetGritSize();

		D3DXVECTOR3 Mypos = pRock->GetPos();

		if (Mypos.x < D_pos.x + MapSize.x - G_Size &&
			Mypos.x > D_pos.x - MapSize.x + G_Size &&
			Mypos.z < D_pos.z + MapSize.z - G_Size &&
			Mypos.z > D_pos.z - MapSize.z + G_Size)
		{
			D3DXVECTOR3 MyposOld = pRock->GetPosOld();
			D3DXVECTOR3 MyMove = pRock->GetMove();
			float MySize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

			D3DXVECTOR3 pos = m_pos;
			D3DXVECTOR3 posOld = m_posOld;
			D3DXVECTOR3 Size = m_size;
			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// ��`�̓����蔻��
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &a, XYZ) == true)
			{
				//m_Objmove.x = MyMove.x;
				//m_move.x = 0.0f;
				//m_bPressObj = true;
				return;
			}
		}
	}
}

//====================================================================
// �㉺���E�ɕǂ����݂��邩�̔���
//====================================================================
void CPlayer::SearchWall(void)
{
	bool OKR = true;	//�E
	bool OKL = true;	//��
	bool OKU = true;	//��
	bool OKD = true;	//��

	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = m_Grid.x + 1;
	int nLNumber = m_Grid.x - 1;
	int nUNumber = m_Grid.z - 1;
	int nDNumber = m_Grid.z + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapHeightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapHeightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, m_Grid.z);
	OKL = !pMapSystem->GetGritBool(nLNumber, m_Grid.z);
	OKU = !pMapSystem->GetGritBool(m_Grid.x, nUNumber);
	OKD = !pMapSystem->GetGritBool(m_Grid.x, nDNumber);

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "����������O���b�g�̒��S�ʒu %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if ((m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)) ||
		m_State == STATE_WAIT)
	{// �O���b�g�̒��S�ʒu�ɗ����Ă���Ȃ瑀����󂯕t����
		m_OKR = OKR;	//�E
		m_OKL = OKL;	//��
		m_OKU = OKU;	//��
		m_OKD = OKD;	//��

		m_bGritCenter = true;
	}
	else
	{
		m_OKR = false;	//�E
		m_OKL = false;	//��
		m_OKU = false;	//��
		m_OKD = false;	//��

		m_bGritCenter = false;
	}
}

//====================================================================
// �f�r���z�[���Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionDevilHole(useful::COLLISION XYZ)
{
	// �f�r���z�[���̃��X�g�\����������Δ�����
	if (CDevilHole::GetList() == nullptr) { return; }
	std::list<CDevilHole*> list = CDevilHole::GetList()->GetList();    // ���X�g���擾

	// �f�r���z�[�����X�g�̒��g���m�F����
	for (CDevilHole* pDevilHole : list)
	{
		D3DXVECTOR3 pos = pDevilHole->GetPos();
		D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
		D3DXVECTOR3 Size = pDevilHole->GetSize();

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, pos, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &m_bJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			m_State = STATE_WAIT;
			m_MoveState = MOVE_STATE_WAIT;
			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		}
	}
}

//====================================================================
// �G�̓����蔻��
//====================================================================
void CPlayer::CollisionEnemy(void)
{
	// �G�̃��X�g�\����������Δ�����
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // ���X�g���擾

	// �G�̃��X�g�̒��g���m�F����
	for (CEnemy* pEnemy : list)
	{
		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 posOld = pEnemy->GetPosOld();
		D3DXVECTOR3 Size = pEnemy->GetSize();

		// �~�̓����蔻��
		if (useful::CollisionCircle(m_pos, pos, 30.0f) == true)
		{
			if (!m_bInvincible)
			{
				Death();
			}
		}
	}
}

//====================================================================
// �X�e�[�W�O�̓����蔻��
//====================================================================
void CPlayer::CollisionStageOut(void)
{
	D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();

	if (m_pos.x + G_Size > D_pos.x + MapSize.x)	// �E
	{
		m_pos.x = D_pos.x + MapSize.x - G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (m_pos.x - G_Size < D_pos.x - MapSize.x)	// ��
	{
		m_pos.x = D_pos.x - MapSize.x + G_Size;
		m_State = STATE_WAIT;
		m_move.x = 0.0f;
	}
	if (m_pos.z + G_Size > D_pos.z + MapSize.z)	// ��
	{
		m_pos.z = D_pos.z + MapSize.z - G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
	if (m_pos.z - G_Size < D_pos.z - MapSize.z)	// ��
	{
		m_pos.z = D_pos.z - MapSize.z + G_Size;
		m_State = STATE_WAIT;
		m_move.z = 0.0f;
	}
}

//====================================================================
// �X�e�[�W���ɂ��邩�ǂ���
//====================================================================
bool CPlayer::CollisionStageIn(void)
{
	D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();

	if (m_pos.x + G_Size <= D_pos.x + MapSize.x &&
		m_pos.x - G_Size >= D_pos.x - MapSize.x &&
		m_pos.z + G_Size <= D_pos.z + MapSize.z &&
		m_pos.z - G_Size >= D_pos.z - MapSize.z)
	{
		return true;
	}

	return false;
}

//====================================================================
// ��ʊO�Ƃ̈�������
//====================================================================
void CPlayer::CollisionPressStageOut(void)
{
	if (m_bPressObj == true)
	{
		D3DXVECTOR3 D_pos = CGame::GetDevil()->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float G_Size = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		if (m_pos.x + G_Size > D_pos.x + MapSize.x)
		{
			Death();
		}
		if (m_pos.x - G_Size < D_pos.x - MapSize.x)
		{
			Death();
		}
		if (m_pos.z + G_Size > D_pos.z + MapSize.z)
		{
			Death();
		}
		if (m_pos.z - G_Size < D_pos.z - MapSize.z)
		{
			Death();
		}
	}
}

//====================================================================
//�J�����ʒu�X�V����
//====================================================================
void CPlayer::CameraPosUpdate(void)
{
	//�J�����ʒu�̍X�V
	m_CameraPos.x = m_pos.x;
	m_CameraPos.z = m_pos.z;

	if (m_bJump == false)
	{
		m_CameraPos.y = m_pos.y;
	}
}

//====================================================================
//�ʒu�X�V����
//====================================================================
void CPlayer::PosUpdate(void)
{
	//�����W��
	//m_move.x = m_move.x * 0.5f;
	//m_move.z = m_move.z * 0.5f;

	if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
	{
		m_move.x = 0.0f;
	}

	if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
	{
		m_move.z = 0.0f;
	}

	if (m_bJump == true)
	{
		m_Objmove.x = m_Objmove.x * 0.25f;
		if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
		{
			m_Objmove.x = 0.0f;
		}

		m_Objmove.z = m_Objmove.z * 0.25f;
		if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
		{
			m_Objmove.z = 0.0f;
		}
	}

	// �ϐ��錾
	float fSpeed = 1.0f;	// �X���[�p default1.0f�ŏ�����
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();
	}

	CDevil* pDevil = CGame::GetDevil();

	//Y���̈ʒu�X�V
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X���̈ʒu�X�V
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);
	CollisionWaitRailBlock(useful::COLLISION_X);
	CollisionWaitRock(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Z);
	CollisionDevilHole(useful::COLLISION_Z);
	CollisionWaitRailBlock(useful::COLLISION_Z);
	CollisionWaitRock(useful::COLLISION_Z);
}

//====================================================================
//�I�u�W�F�N�g�ɂ��ʒu�X�V����
//====================================================================
void CPlayer::ObjPosUpdate(void)
{
	if (m_bJump == true)
	{
		m_Objmove.x = m_Objmove.x * 0.25f;
		if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
		{
			m_Objmove.x = 0.0f;
		}

		m_Objmove.z = m_Objmove.z * 0.25f;
		if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
		{
			m_Objmove.z = 0.0f;
		}
	}

	// �ϐ��錾
	float fSpeed = 1.0f;	// �X���[�p default1.0f�ŏ�����
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();
	}

	CDevil* pDevil = CGame::GetDevil();

	//Y���̈ʒu�X�V
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	//X���̈ʒu�X�V
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// ���[���u���b�N�Ƃ̓����蔻��
	CollisionMoveRailBlock(useful::COLLISION_X);
	CollisionMoveRock(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	CollisionMoveRailBlock(useful::COLLISION_Z);
	CollisionMoveRock(useful::COLLISION_Z);

	//// �ǂƂ̓����蔻��
	CollisionPressWall(useful::COLLISION_ZX);
}

//====================================================================
//�����X�V����
//====================================================================
void CPlayer::RotUpdate(void)
{
	// �p�x�̍������Z�o
	D3DXVECTOR3 rotDiff = m_rotDest - m_rot;

	// ���K��
	useful::NormalizeAngle(&rotDiff);

	// �����̍X�V����
	m_rot += (rotDiff * 0.5);
}

//====================================================================
//���̓���
//====================================================================
void CPlayer::EggMove(void)
{
	if (m_State == STATE_EGG)
	{
		if (m_pos.y > 0.0f)
		{
			//�d��
			m_move.y -= RESPAWN_GRAVITY;
		}

		//Y���̈ʒu�X�V
		m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed();

		//���̗�������
		if (m_pos.y < 0.0f)
		{
			//�������Ɉړ��ʂƈʒu���O�ɂ���
			m_pos.y = 0.0f;
			m_move.y = 0.0f;
		}

		if (m_pUpEgg != nullptr)
		{
			m_pUpEgg->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 65.0f, m_pos.z));
		}
		if (m_pDownEgg != nullptr)
		{
			m_pDownEgg->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 65.0f, m_pos.z));
		}
		m_EggMove = INITVECTOR3;
	}
	else
	{
		if (m_pUpEgg != nullptr)
		{
			D3DXVECTOR3 pos = m_pUpEgg->GetPos();
			D3DXVECTOR3 rot = m_pUpEgg->GetRot();
			float ColorA = m_pUpEgg->GetMatColor().a;

			ColorA -= EGG_COLOR_DEL_A;

			m_EggMove.y -= EGG_GRAVITY;

			pos += m_EggMove;

			rot.z -= m_EggMove.x * EGG_ROT;
			rot.x += m_EggMove.z * EGG_ROT;

			m_EggMove.x = m_EggMove.x * EGG_MOVE_DEL;
			m_EggMove.z = m_EggMove.z * EGG_MOVE_DEL;

			if (pos.y < CGame::GetMapField()->GetPos().y + 30.0f)
			{
				pos.y = CGame::GetMapField()->GetPos().y + 30.0f;
			}
			else
			{
				m_pUpEgg->SetRot(rot);
			}

			m_pUpEgg->SetPos(pos);
			m_pUpEgg->SetMatColorA(ColorA);

			if (ColorA <= 0.0f)
			{
				m_pUpEgg->Uninit();
				m_pUpEgg = nullptr;
			}
		}

		if (m_pDownEgg != nullptr)
		{
			float ColorA = m_pDownEgg->GetMatColor().a;

			ColorA -= EGG_COLOR_DEL_A;

			m_pDownEgg->SetMatColorA(ColorA);

			if (ColorA <= 0.0f)
			{
				m_pDownEgg->Uninit();
				m_pDownEgg = nullptr;
			}
		}
	}
}

//====================================================================
//�_���[�W����
//====================================================================
void CPlayer::Death(void)
{
	if (m_State != STATE_EGG && m_State != STATE_DEATH)
	{
		m_nLife--;

		// �������������Ă���Ƃ��ɂ��̏�ɐ����𗎂Ƃ�
		if (m_eItemType == TYPE_BIBLE)
		{
			// ��������
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(m_Grid.x, m_Grid.z));
		}

		if (m_nLife < 0)
		{
			// ���S��
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATH);

			CGame::SetGameEnd(true);
			CGame::SetGameClear(false);
			CManager::GetInstance()->SetStage(0);
		}
		else
		{
			if (m_pLifeUi != nullptr)
			{
				m_pLifeUi->GetNumber()->SetNumber(m_nLife);
			}

			m_State = STATE_DEATH;
			m_move = INITVECTOR3;
			m_Objmove = INITVECTOR3;
			m_nStateCount = 150;

			// �_���[�W��(��)
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATH);
		}

		// �A�C�e�����������Ă��Ȃ���Ԃɂ���
		SetItemType(TYPE_NONE);
	}
}

//====================================================================
// �f�o�b�O�{�^��
//====================================================================
void CPlayer::DebugKey(void)
{
#ifdef _DEBUG

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_WHEEL))
	{
		HitDamage(10.0f);
	}
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�L�[�{�[�h�̈ړ�����
	if (pInputKeyboard->GetTrigger(DIK_3))
	{
		m_nLife++;
		m_pLifeUi->GetNumber()->SetNumber(m_nLife);;
	}
	if (pInputKeyboard->GetTrigger(DIK_4))
	{
		m_nLife--;
		m_pLifeUi->GetNumber()->SetNumber(m_nLife);;
	}

#endif // !_DEBUG
}

//====================================================================
// ���f���̃��[�V�����ݒ�
//====================================================================
void CPlayer::SetAction(ACTION_TYPE Action, float BlendTime)
{
	// ���f�����̎擾
	if (m_pCharacter == nullptr)
	{
		assert(("�L�����N�^�[���Ȃ�", false));
		return;
	}

	CMotion* pMotion = m_pCharacter->GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	if (m_Action != Action)
	{
		m_Action = Action;
		pMotion->Set(Action, BlendTime);
	}
}

//====================================================================
// ���f���\���̐ݒ�
//====================================================================
void CPlayer::SetModelDisp(bool Sst)
{
	// ���f�����̎擾
	if (m_pCharacter == nullptr)
	{
		assert(("�L�����N�^�[���Ȃ�",false));
		return;
	}

	int nNumModel = m_pCharacter->GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		CModel* pModel = m_pCharacter->GetModel(nCnt);

		if (pModel != nullptr)
		{
			pModel->SetDisp(Sst);
		}
	}
}

//====================================================================
//�}�b�v�I�u�W�F�N�g�ƕǂƂ̃\�[�g����
//====================================================================
bool CPlayer::SortObject(D3DXVECTOR3 pos)
{
	float fDistance = sqrtf((m_pos.x - pos.x) * (m_pos.x - pos.x) + (m_pos.z - pos.z) * (m_pos.z - pos.z));

	// �G�̃��X�g�\����������Δ�����
	if (CCubeBlock::GetList() == nullptr) { return false; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

	// �G�̃��X�g�̒��g���m�F����
	for (CCubeBlock* pCubeBlock : list)
	{
		if (pCubeBlock->GetPos().y <= 200.0f)
		{
			if (useful::CollisionCircle(m_pos, pCubeBlock->GetPos(), 250.0f) == true)
			{
				D3DXVECTOR3 CrossPos = INITVECTOR3;
				float ObjDistance = OBJDISTANCE;

				//��_�̈ʒu�����߂�
				for (int nCnt = 0; nCnt < 4; nCnt++)
				{
					switch (nCnt)
					{
					case 0:
						CrossPos = useful::CrossIntersection(m_pos, pos,
							D3DXVECTOR3(pCubeBlock->GetPos().x + pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z + pCubeBlock->GetSize().z),
							D3DXVECTOR3(pCubeBlock->GetPos().x - pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z + pCubeBlock->GetSize().z),
							250.0f);
						break;

					case 1:
						CrossPos = useful::CrossIntersection(m_pos, pos,
							D3DXVECTOR3(pCubeBlock->GetPos().x - pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z + pCubeBlock->GetSize().z),
							D3DXVECTOR3(pCubeBlock->GetPos().x - pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z - pCubeBlock->GetSize().z),
							250.0f);
						break;

					case 2:
						CrossPos = useful::CrossIntersection(m_pos, pos,
							D3DXVECTOR3(pCubeBlock->GetPos().x - pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z - pCubeBlock->GetSize().z),
							D3DXVECTOR3(pCubeBlock->GetPos().x + pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z - pCubeBlock->GetSize().z),
							250.0f);
						break;

					case 3:
						CrossPos = useful::CrossIntersection(m_pos, pos,
							D3DXVECTOR3(pCubeBlock->GetPos().x + pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z - pCubeBlock->GetSize().z),
							D3DXVECTOR3(pCubeBlock->GetPos().x + pCubeBlock->GetSize().x, pCubeBlock->GetPos().y, pCubeBlock->GetPos().z + pCubeBlock->GetSize().z),
							250.0f);
						break;
					}

					ObjDistance = sqrtf((m_pos.x - CrossPos.x) * (m_pos.x - CrossPos.x) + (m_pos.z - CrossPos.z) * (m_pos.z - CrossPos.z));

					if (ObjDistance < fDistance)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

//==========================================
//  �A�C�e���̐ݒ�
//==========================================
void CPlayer::SetItemType(ITEM_TYPE eType)
{
	// �A�C�e���̃^�C�v��ݒ�
	m_eItemType = eType;

	// �������Ă���A�C�e���ɂ���ă��f���̕\����؂�ւ���
	switch (eType)
	{
	case TYPE_CROSS:
		SetPartsDisp(9, true);		// �\���˂̃��f���\��
		SetPartsDisp(10, false);	// �����̃��f����\��

		// �������ԃ^�C�}�[�����Z�b�g
		m_fCrossTimer = 0.0f;

		break;

	case TYPE_BIBLE:
		SetPartsDisp(9, false);		// �\���˂̃��f����\��
		SetPartsDisp(10, true);		// �����̃��f���\��
		break;

	default:
		SetPartsDisp(9, false);		// �\���˂̃��f����\��
		SetPartsDisp(10, false);	// �����̃��f����\��
		break;
	}
}

//====================================================================
// �v���C���[�̎w�胂�f������
//====================================================================
void CPlayer::SetPartsDisp(int nParts, bool Set)
{
	// ���f�����̎擾
	if (m_pCharacter == nullptr)
	{
		assert(("�L�����N�^�[���Ȃ�", false));
		return;
	}

	CModel* pModel = m_pCharacter->GetModel(nParts);

	if (pModel != nullptr)
	{
		pModel->SetDisp(Set);
	}
}

//====================================================================
// �v���C���[�̎w�胂�f������
//====================================================================
void CPlayer::SetModelColor(CModel::COLORTYPE Type, D3DXCOLOR Col)
{
	// ���f�����̎擾
	if (m_pCharacter == nullptr)
	{
		assert(("�L�����N�^�[���Ȃ�", false));
		return;
	}

	CModel* pModel = nullptr;

	for (int nCnt = 0; nCnt < m_pCharacter->GetNumModel(); nCnt++)
	{
		pModel = m_pCharacter->GetModel(nCnt);

		if (pModel != nullptr)
		{
			pModel->SetColorType(Type);
			pModel->SetColor(Col);
		}
	}
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CPlayer>* CPlayer::GetList(void)
{
	return m_pList;
}
