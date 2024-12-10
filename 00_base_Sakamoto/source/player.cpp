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
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "camera.h"
#include "input.h"
#include "enemy.h"
#include "slowManager.h"
#include "Number.h"
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
#include "RollRock.h"
#include "shadow.h"
#include "score.h"
#include "mask.h"
#include "wall.h"
#include "objectBillboard.h"
#include "move.h"
#include "MapMove.h"

#include "MyEffekseer.h"
#include "footprint.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	const int LIFE_MAX = 2;	//�������C�t��
	const int FIRE_STOPTIME = 30;	//�U�����̈ړ���~����
	const D3DXVECTOR3 RESPAWN_POS = D3DXVECTOR3(-100.0f, 2000.0f, 100.0f); // �����ʒu
	const float RESPAWN_GRAVITY = 0.03f;			//���̏d��
	const int INVINCIBLE_TIME = 120;			//���G����(��ŏ���)

	const float GRIT_OK = 45.0f;			//�ړ��\�ȃO���b�g�͈͓̔�
	const float OBJDISTANCE = 10000.0f;		// �I�u�W�F�N�g�̋���

	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(35.0f, 40.0f, 35.0f);		//���̓����蔻��

	const D3DXVECTOR3 LIFE_POS00 = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
	const D3DXVECTOR3 LIFE_POS01 = D3DXVECTOR3(900.0f, 650.0f, 0.0f);

	const float CROSS_TIME = 10.0f; // �\���˂��������Ă����鎞��

	const float EGG_GRAVITY = 0.98f;	 //�ړ��ʂ̌������x
	const D3DXVECTOR3 EGG_MOVE = D3DXVECTOR3(10.0f, 10.0f, 10.0f);	 //�ړ��ʂ̌������x(��ŏ���)
	const float EGG_ROT = D3DX_PI * 0.006f;		//��]���x
	const float EGG_MOVE_DEL = 0.9f;			//�ړ��ʂ̌������x
	const float EGG_COLOR_DEL_A = 0.01f;		//�s�����x�̌������x

	const float SHADOW_SIZE = 50.0f;			// �ۉe�̑傫��

	const D3DXVECTOR2 NUMBER_SIZE = { 280.0f , 170.0f };
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CPlayer>* CPlayer::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CPlayer::CPlayer(int nPriority) : CObjectCharacter(nPriority),
m_AutoMoveRot(INITVECTOR3),
m_bJump(false),
m_nActionCount(0),
m_Action(ACTION_WAIT),
m_AtkAction(ACTION_WAIT),
//state(STATE_EGG),
m_nStateCount(0),
m_AtkPos(INITVECTOR3),
m_CollisionRot(0.0f),
m_bInput(false),
m_pLifeUi(nullptr),
m_nLife(0),
m_eItemType(TYPE_NONE),
m_OldGrid(0, 0),
m_bGritCenter(true),
m_bPressObj(false),
m_fCrossTimer(0.0f),
m_pUpEgg(nullptr),
m_pDownEgg(nullptr),
m_EggMove(INITVECTOR3),
m_bInvincible(true),
m_nInvincibleCount(0),
m_pScore(nullptr),
m_nTime(0),
m_pEffectEgg(nullptr),
m_pEffectSpeed(nullptr),
m_pP_NumUI(nullptr),
m_pEffectGuide(nullptr),
m_pEffectItem(nullptr),
m_pMoveState(nullptr)
{
	// �ړ��̐i�s��
	m_Progress.bOKD = true;
	m_Progress.bOKL = true;
	m_Progress.bOKR = true;
	m_Progress.bOKU = true;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CPlayer::~CPlayer()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CPlayer::Init(int PlayNumber)
{
	// �l���擾
	D3DXVECTOR3 posThis = GetPos();			// �ʒu
	D3DXVECTOR3 posOldThis = GetPosOld();	// �O��̈ʒu
	D3DXVECTOR3 rotThis = GetRot();			// ����
	D3DXVECTOR3 sizeThis = GetSize();		// �傫��

	// �v���C���[�ԍ���ݒ�
	m_nPlayNumber = PlayNumber;

	// �L�����N�^�[�N���X�̏������i�p���j
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// �L�����N�^�[�e�L�X�g�ǂݍ��ݏ���
	switch (m_nPlayNumber)
	{
	case 0:

		CObjectCharacter::SetTxtCharacter("data\\TXT\\motion_tamagon1P.txt", 1);

		break;

	case 1:

		CObjectCharacter::SetTxtCharacter("data\\TXT\\motion_tamagon2P.txt", 1);

		break;
	}

	// �L�����N�^�[�̃}�g���b�N�X�ݒ�
	CObjectCharacter::SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
	CObjectCharacter::SetUseStencil(true);
	CObjectCharacter::SetUseShadowMtx(true);

	CMapSystem* pMapSystem = CMapSystem::GetInstance();		// �}�b�v�V�X�e���̏��

	// �v���C���[�̈ʒu�擾
	posThis = pMapSystem->GetPlayerPos(PlayNumber);

	// �T�C�Y�̐ݒ�
	sizeThis = COLLISION_SIZE;

	// �����̐ݒ�
	rotThis = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	// �A�N�V�����̐ݒ�
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;

	//��ސݒ�
	SetType(CObject::TYPE_PLAYER3D);

	// �̗͂̐ݒ�
	m_nLife = LIFE_MAX;

	//��������UI�̐���
	UI_Create();

	//��������UI�̏�����
	UI_Init();

	// �A�C�e����Ԃ�ݒ�
	SetItemType(CPlayer::TYPE_NONE);

	// �X���[�̐���
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	// �l�X�V
	SetPos(posThis);		// �ʒu
	SetPosOld(posOldThis);	// �O��̈ʒu
	SetRot(rotThis);		// ����
	SetSize(sizeThis);		// �傫��

	// ��Ԃ̐ݒ�
	SetState(STATE_EGG);

	// �ړ���Ԃ̐ݒ�
	if (m_pMoveState == nullptr)
	{
		m_pMoveState = new CStateStop();	// ��~���
		m_pMoveState->ControlStop(this);	// �����Ԃɂ���
	}
	// ������Ԃ̐ݒ�
	m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);

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

	// �L�����N�^�[�N���X�̏I���i�p���j
	CObjectCharacter::Uninit();

	// �X�R�A�̍폜
	if (m_pScore != nullptr)
	{
		m_pScore = nullptr;
	}

	// �ړ���Ԃ̔j��
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	// �G�t�F�N�g�̍폜
	if (m_pEffectEgg != nullptr)
	{
		m_pEffectEgg->SetDeath();
		m_pEffectEgg = nullptr;
	}
	if (m_pEffectSpeed != nullptr)
	{
		m_pEffectSpeed->SetDeath();
		m_pEffectSpeed = nullptr;
	}
	if (m_pEffectItem != nullptr)
	{
		m_pEffectItem->SetDeath();
		m_pEffectItem = nullptr;
	}
	if (m_pEffectGuide != nullptr)
	{
		m_pEffectGuide->SetDeath();
		m_pEffectGuide = nullptr;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CPlayer::Update(void)
{
	// �l���擾
	D3DXVECTOR3 posThis = GetPos();			// �ʒu
	D3DXVECTOR3 posOldThis = GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 rotThis = GetRot();			// ����
	D3DXVECTOR3 sizeThis = GetSize();			// �傫��
	STATE state = GetState();				// ���
	STATE oldstate = GetOldState();			// ���

	// �ߋ��̈ʒu�ɑ��
	posOldThis = posThis;
	m_OldGrid = m_Grid;

	if (state != STATE_DEATH)
	{
		//�ǂ����邩���f
		SearchWall(posThis);

		if (
			(state != STATE_EGG && CollisionStageIn(posThis) == true &&
				CMapSystem::GetInstance()->GetGritBool(m_Grid.x, m_Grid.z) == false) ||
			(state == STATE_EGG && CollisionStageIn(posThis) == true &&
				CMapSystem::GetInstance()->GetGritBool(m_Grid.x, m_Grid.z) == false &&
				m_bGritCenter == true && posThis.y <= 0.0f)
			)
		{// �X�e�[�W���ɂ��� ���� �u���b�N�̖����O���b�h��̎�

			// ������
			//m_pMoveState->ControlStop(this);

			// �ړ�����
			m_pMoveState->Move(this, posThis, rotThis);

			// ���f����`�悷��
			if (GetState() != STATE_EGG && oldstate == STATE_EGG)
			{
				SetItemType(TYPE_NONE);
			}

			// �ړ�����
			//Move(posThis,rotThis);
		}

		// �����ړ�����
		Rot(rotThis);

		if (m_eItemType != TYPE_NONE)
		{
			Attack(posThis,rotThis);
		}

		// �\���˂������Ă���ꍇ
		if (m_eItemType == TYPE_CROSS)
		{
			// �^�C�}�[�����Z
			m_fCrossTimer += DeltaTime::Get();
			DebugProc::Print(DebugProc::POINT_CENTER, "�c�莞�� : %f", m_fCrossTimer);

			// �\���˂̏����\���Ԃ𒴉߂����ꍇ
			if (m_fCrossTimer >= CROSS_TIME)
			{
				// �������ԃ^�C�}�[�����Z�b�g
				m_fCrossTimer = 0.0f;

				// �G�t�F�N�g�𐶐�
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_CRASH_CROSS, false, ef, rotThis);

				// �A�C�e�����������Ă��Ȃ���Ԃɂ���
				SetItemType(TYPE_NONE);
			}
		}

		// �J�����X�V����
		CameraPosUpdate(posThis);

		if (state == STATE_WALK)
		{
			// �ʒu�X�V����
			PosUpdate(posThis,posOldThis,sizeThis);
		}

		ObjPosUpdate(posThis,posOldThis,sizeThis);

		if (state != STATE_EGG && state != STATE_DEATH)
		{
			//��ʊO����
			CollisionStageOut(posThis);

			if (m_pP_NumUI != nullptr)
			{
				m_pP_NumUI->SetPos(D3DXVECTOR3(
					posThis.x,
					posThis.y + 50.0f,
					posThis.z + 50.0f));

				m_pP_NumUI->SetAppear(true);
			}
		}

		// �G�̔���
		CollisionEnemy(posThis);
	}

	// �v���C���[���}�b�v�̂ǂ̃}�X�ɑ��݂��Ă��邩�ݒ肷��
	m_Grid.x = CMapSystem::GetInstance()->CMapSystem::CalcGridX(posThis.x);
	m_Grid.z = CMapSystem::GetInstance()->CMapSystem::CalcGridZ(posThis.z);

	//��Ԃ̊Ǘ�
	StateManager(posThis, rotThis);

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
	EggMove(posThis, rotThis);

	// �L�����N�^�[�N���X�̍X�V�i�p���j
	CObjectCharacter::Update();

	//���[�V�����̊Ǘ�
	ActionState();

	//�f�o�b�O�L�[�̏����Ɛݒ�
	DebugKey();

	// �X�N���[���ɍ��킹�Ĉړ�����
	CMapSystem::GetInstance()->GetMove()->FollowScroll(posThis);

	//�f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]�ʒu %f : %f : %f\n", posThis.x, posThis.y, posThis.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]���� %f : %f : %f\n", rotThis.x, rotThis.y, rotThis.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]�� %d : �c %d\n", m_Grid.x, m_Grid.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[����]��� : ");
	auto str = magic_enum::enum_name(state);
	DebugProc::Print(DebugProc::POINT_LEFT, str.data());
	DebugProc::Print(DebugProc::POINT_LEFT, "\n");

	// �l�X�V
	SetPos(posThis);		// �ʒu
	SetPosOld(posOldThis);	// �O��̈ʒu
	SetRot(rotThis);		// ����
	SetSize(sizeThis);		// �傫��

	// �G�t�F�N�g�̑���
	ControlEffect(m_pEffectEgg);	// ���̃G�t�F�N�g
	ControlEffect(m_pEffectSpeed);	// �������̃G�t�F�N�g
	ControlEffect(m_pEffectItem);	// �A�C�e�������G�t�F�N�g
	if (m_pShadow != nullptr)
	{
		ControlEffect(m_pEffectGuide, &m_pShadow->GetPos()); // �����ʒu�̃K�C�h�G�t�F�N�g
	}
	PrintFoot(rotThis);
}

//====================================================================
//�`�揈��
//====================================================================
void CPlayer::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();
}

//====================================================================
//��������UI�̐���
//====================================================================
void CPlayer::UI_Create(void)
{
	//�̗�UI�̐���
	if (m_pLifeUi == nullptr)
	{
		m_pLifeUi = CLifeUi::Create();
	}

	//�X�R�A�̐���
	if (m_pScore == nullptr)
	{
		m_pScore = CScore::Create();
	}

	//�v���C���[�ԍ�UI�̐���
	if (m_pP_NumUI == nullptr && CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
	{
		m_pP_NumUI = CObjectBillboard::Create();
	}
}

//====================================================================
//��������UI�̏�����
//====================================================================
void CPlayer::UI_Init(void)
{
	switch (m_nPlayNumber)
	{
	case 0:
		if (m_pLifeUi != nullptr)
		{
			// �����̈ʒu
			m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS00.x + 200.0f, LIFE_POS00.y, LIFE_POS00.z));

			// �̗�
			m_pLifeUi->SetPos(LIFE_POS00);
			m_pLifeUi->GetNumber()->SetNumber(m_nLife);
		}

		if (m_pScore != nullptr)
		{
			m_pScore->SetPos(D3DXVECTOR3(50.0f, 40.0f, 0.0f));
		}

		if (m_pP_NumUI != nullptr)
		{
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetWidth(NUMBER_SIZE.x);
			m_pP_NumUI->SetHeight(NUMBER_SIZE.y);
			m_pP_NumUI->SetTexture("data\\TEXTURE\\UI\\1p.png");
		}

		break;

	case 1:

		if (m_pLifeUi != nullptr)
		{
			// �����̈ʒu
			m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS01.x + 200.0f, LIFE_POS01.y, LIFE_POS01.z));

			// �̗�
			m_pLifeUi->SetPos(LIFE_POS01);
			m_pLifeUi->GetNumber()->SetNumber(m_nLife);
		}

		if (m_pScore != nullptr)
		{
			m_pScore->SetPos(D3DXVECTOR3(1050.0f, 40.0f, 0.0f));
		}

		if (m_pP_NumUI != nullptr)
		{
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetWidth(NUMBER_SIZE.x);
			m_pP_NumUI->SetHeight(NUMBER_SIZE.y);
			m_pP_NumUI->SetTexture("data\\TEXTURE\\UI\\2p.png");
		}

		break;
	}
}

#if 0
//====================================================================
//�ړ�����
//====================================================================
void CPlayer::Move(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	m_bInput = false;

	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���͏���
	NormarizeMove = MoveInputKey(posThis,rotThis,NormarizeMove);

	NormarizeMove = MoveInputPadStick(posThis, rotThis, NormarizeMove);

	NormarizeMove = MoveInputPadKey(posThis, rotThis, NormarizeMove);

	if (m_bInput && m_State != STATE_ATTACK)
	{
		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.z *= PLAYER_SPEED;

		//�ړ��ʂ���
		m_move = NormarizeMove;

		if (m_State == STATE_EGG)
		{
			// ���f�����̎擾
			int nNumModel = GetNumModel();

			for (int nCnt = 0; nCnt < nNumModel; nCnt++)
			{
				// ���f���̎擾
				CModel* pModel = GetModel(nCnt);
				
				if (pModel != nullptr)
				{
					pModel->SetDisp(true);
				}
			}

			// �A�C�e����j��
			SetItemType(TYPE_NONE);

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

			//���G��Ԃ̐ݒ�
			m_bInvincible = true;
			m_nInvincibleCount = INVINCIBLE_TIME;
		}

		//�ړ���Ԃɂ���
		m_State = STATE_WALK;
	}
}

//====================================================================
//�ړ����̓L�[�{�[�h
//====================================================================
D3DXVECTOR3 CPlayer::MoveInputKey(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis, D3DXVECTOR3 Move)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�L�[�{�[�h�̈ړ�����
	if ((pInputKeyboard->GetPress(DIK_W) && m_Progress.bOKU && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_W) && m_MoveState == MOVE_STATE_DOWN))
	{
		Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_UP;
	}
	else if (((pInputKeyboard->GetPress(DIK_S) && m_Progress.bOKD && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_S) && m_MoveState == MOVE_STATE_UP)) &&
		pInputKeyboard->GetPress(DIK_W) == false)
	{
		Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_DOWN;
	}
	else if ((pInputKeyboard->GetPress(DIK_A) && m_Progress.bOKL && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_A) && m_MoveState == MOVE_STATE_RIGHT))
	{
		Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

		m_bInput = true;
		m_MoveState = MOVE_STATE_LEFT;
	}
	else if (((pInputKeyboard->GetPress(DIK_D) && m_Progress.bOKR && m_bGritCenter) ||
		(pInputKeyboard->GetPress(DIK_D) && m_MoveState == MOVE_STATE_LEFT)) &&
		pInputKeyboard->GetPress(DIK_A) == false)
	{
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
D3DXVECTOR3 CPlayer::MoveInputPadStick(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis, D3DXVECTOR3 Move)
{
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (nCnt == m_nPlayNumber)
		{
			//�X�e�B�b�N�̈ړ�����
			if ((pInputJoypad->Get_Stick_Left(nCnt).y > 0.0f && m_Progress.bOKU && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).y > 0.0f && m_MoveState == MOVE_STATE_DOWN))
			{
				Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_UP;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).y < 0.0f && m_Progress.bOKD && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).y < 0.0f && m_MoveState == MOVE_STATE_UP))
			{
				Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_DOWN;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).x < 0.0f && m_Progress.bOKL && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).x < 0.0f && m_MoveState == MOVE_STATE_RIGHT))
			{
				Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_LEFT;
			}
			else if ((pInputJoypad->Get_Stick_Left(nCnt).x > 0.0f && m_Progress.bOKR && m_bGritCenter) ||
				(pInputJoypad->Get_Stick_Left(nCnt).x > 0.0f && m_MoveState == MOVE_STATE_LEFT))
			{
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
D3DXVECTOR3 CPlayer::MoveInputPadKey(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis, D3DXVECTOR3 Move)
{
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (nCnt == m_nPlayNumber)
		{
			//�L�[�{�[�h�̈ړ�����
			if ((pInputJoypad->GetPress(CInputJoypad::BUTTON_UP, nCnt) && m_Progress.bOKU && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_UP, nCnt) && m_MoveState == MOVE_STATE_DOWN))
			{
				Move.z += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_UP;
			}
			else if (((pInputJoypad->GetPress(CInputJoypad::BUTTON_DOWN, nCnt) && m_Progress.bOKD && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_DOWN, nCnt) && m_MoveState == MOVE_STATE_UP)) &&
				pInputJoypad->GetPress(CInputJoypad::BUTTON_UP, nCnt) == false)
			{
				Move.z += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.x += -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_DOWN;
			}
			else if ((pInputJoypad->GetPress(CInputJoypad::BUTTON_LEFT, nCnt) && m_Progress.bOKL && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_LEFT, nCnt) && m_MoveState == MOVE_STATE_RIGHT))
			{
				Move.x += -1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= -1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_LEFT;
			}
			else if (((pInputJoypad->GetPress(CInputJoypad::BUTTON_RIGHT, nCnt) && m_Progress.bOKR && m_bGritCenter) ||
				(pInputJoypad->GetPress(CInputJoypad::BUTTON_RIGHT, nCnt) && m_MoveState == MOVE_STATE_LEFT)) &&
				pInputJoypad->GetPress(CInputJoypad::BUTTON_LEFT, nCnt) == false)
			{
				Move.x += 1.0f * cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
				Move.z -= 1.0f * sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;

				m_bInput = true;
				m_MoveState = MOVE_STATE_RIGHT;
			}
		}
	}

	return Move;
}

#endif

//====================================================================
//�ړ���������
//====================================================================
void CPlayer::Rot(D3DXVECTOR3& rotThis)
{
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera(0)->GetRot();
	STATE state = GetState();		// ���

	//�ړ������Ɍ��������킹�鏈��
	float fRotMove, fRotDest;
	fRotMove = rotThis.y;
	fRotDest = CManager::GetInstance()->GetCamera(0)->GetRot().y;

	if (state == STATE_WALK)
	{
		rotThis.y = atan2f(-m_move.x, -m_move.z);
	}

	useful::NormalizeAngle(&rotThis);
}

//====================================================================
//�U������
//====================================================================
void CPlayer::Attack(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	STATE state = GetState();		// ���

	if (state == STATE_WALK)
	{
		//�L�[�{�[�h�̎擾
		CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
		CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

		if (pInputKeyboard->GetTrigger(DIK_SPACE) || pInputJoypad->GetTrigger(CInputJoypad::BUTTON_B, 0))
		{
			// �Ή�����
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_FIRE);
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);

			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_SMOKE, false, ef, rotThis);

			CFire::Create("data\\model\\fireball.x", posThis, rotThis);
			SetState(STATE_ATTACK);
			m_nStateCount = FIRE_STOPTIME;
		}
	}
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CPlayer::ActionState(void)
{
	// ���[�V�����̎擾
	CMotion* pMotion = GetMotion();
	STATE state = GetState();

	if (pMotion == nullptr)
	{
		return;
	}

	//�ړ����[�V����
	if (state == STATE_DEATH)
	{
		if (m_Action != ACTION_ENEMYDEATH)
		{
			m_Action = ACTION_ENEMYDEATH;
			pMotion->Set(ACTION_ENEMYDEATH, 5);
		}
	}
	//�����[�V����
	else if (state == STATE_EGG)
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			pMotion->Set(ACTION_WAIT, 5);
		}
	}
	//�ړ����[�V����
	else if (state == STATE_ATTACK)
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
void CPlayer::StateManager(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	STATE state = GetState();

	switch (state)
	{
	case STATE_WAIT:
		//	�X���[��default��
		CSlowManager::SetValueDefault();
		break;

	case STATE_WALK:

		m_nTime++;

		if (m_nTime >= 20)
		{
			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_DUSTCLOUD, false, ef, rotThis);

			m_nTime = 0;
		}
		break;

	case STATE_ATTACK:

		if (m_nStateCount == 0)
		{
			SetState(STATE_WALK);
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
						posThis = CMapSystem::GRID(nSetW, nSetH).ToWorld();
						posThis.y = RESPAWN_POS.y;
						SetState(STATE_EGG);
						return;
					}

				}
			}
		}

		break;

	case STATE_EGG:

		// ���f�����̎擾
		int nNumModel = GetNumModel();

		for (int nCnt = 0; nCnt < nNumModel; nCnt++)
		{
			// ���f���̎擾
			CModel* pModel = GetModel(nCnt);

			if (pModel != nullptr)
			{
				pModel->SetDisp(false);
			}
		}

		if (m_pUpEgg == nullptr)
		{
			// �^�}�S�ǂݍ��ݏ���
			switch (m_nPlayNumber)
			{
			case 0:
				m_pUpEgg = CObjectX::Create("data\\MODEL\\00_Player\\1P\\upper_egg.x");
				m_pUpEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));

				m_pUpEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				break;

			case 1:
				m_pUpEgg = CObjectX::Create("data\\MODEL\\00_Player\\2P\\upperegg.x");
				m_pUpEgg->SetMatColor(D3DXCOLOR(0.235294f, 0.715294f, 0.800000f, 1.0f));

				m_pUpEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				break;
			}
		}

		if (m_pDownEgg == nullptr)
		{
			// �^�}�S�ǂݍ��ݏ���
			switch (m_nPlayNumber)
			{
			case 0:
				m_pDownEgg = CObjectX::Create("data\\MODEL\\00_Player\\1P\\downer_egg.x");
				m_pDownEgg->SetMatColor(D3DXCOLOR(0.263529f, 0.570980f, 0.238431f, 1.0f));

				m_pDownEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				break;

			case 1:
				m_pDownEgg = CObjectX::Create("data\\MODEL\\00_Player\\2P\\downeregg.x");
				m_pDownEgg->SetMatColor(D3DXCOLOR(0.235294f, 0.715294f, 0.800000f, 1.0f));

				m_pDownEgg->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
				break;
			}
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
void CPlayer::CollisionWall(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// �ǂ̃��X�g�\����������Δ�����
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // ���X�g���擾

	// �ǃ��X�g�̒��g���m�F����
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 posOld = pWall->GetPosOld();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, pos, Move, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &m_bJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			SetState(STATE_WAIT);
			// ������Ԃ̐ݒ�
			m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
			posThis = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// �ǂƂ̈�������
//====================================================================
void CPlayer::CollisionPressWall(D3DXVECTOR3& posThis, D3DXVECTOR3& sizeThis,useful::COLLISION XYZ)
{
	// �ǂ̃��X�g�\����������Δ�����
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 Size = pWall->GetSize();

		// ��`�̓����蔻��
		if (useful::CollisionRectangle2D(posThis, pos, sizeThis, Size, XYZ) == true)
		{
			Death();
		}
	}
}

//====================================================================
// �~�܂��Ă��郌�[���u���b�N�Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionWaitRailBlock(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
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
		if (useful::CollisionBlock(pos, pos, Move, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &m_bJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			SetState(STATE_WAIT);

			// ������Ԃ̐ݒ�
			m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
			posThis = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// �����Ă��郌�[���u���b�N�Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionMoveRailBlock(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // ���X�g���擾

	// ���[���u���b�N���X�g�̒��g���m�F����
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
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

			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// ��`�̓����蔻��
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &a, XYZ) == true)
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
void CPlayer::CollisionWaitRock(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
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
		if (useful::CollisionBlock(pos, pos, Move, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &m_bJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			SetState(STATE_WAIT);
			// ������Ԃ̐ݒ�
			m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
			posThis = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// �����Ă����Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionMoveRock(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
{
	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // ���X�g���擾

	// ���[���u���b�N���X�g�̒��g���m�F����
	for (CRollRock* pRock : list)
	{
		D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
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

			D3DXVECTOR3 Move = m_move;
			bool a = false;

			// ��`�̓����蔻��
			if (useful::CollisionBlock(Mypos, MyposOld, MyMove, D3DXVECTOR3(MySize, MySize, MySize), &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &a, XYZ) == true)
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
void CPlayer::SearchWall(D3DXVECTOR3& posThis)
{
	STATE state = GetState();

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
	D3DXVECTOR3 MyGritPos = m_Grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "����������O���b�g�̒��S�ʒu %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if ((posThis.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posThis.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		posThis.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posThis.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)) ||
		state == STATE_WAIT)
	{// �O���b�g�̒��S�ʒu�ɗ����Ă���Ȃ瑀����󂯕t����
		m_Progress.bOKR = OKR;	//�E
		m_Progress.bOKL = OKL;	//��
		m_Progress.bOKU = OKU;	//��
		m_Progress.bOKD = OKD;	//��

		m_bGritCenter = true;
	}
	else
	{
		m_Progress.bOKR = false;	//�E
		m_Progress.bOKL = false;	//��
		m_Progress.bOKU = false;	//��
		m_Progress.bOKD = false;	//��

		m_bGritCenter = false;
	}
}

//====================================================================
// �f�r���z�[���Ƃ̓����蔻��
//====================================================================
void CPlayer::CollisionDevilHole(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis, useful::COLLISION XYZ)
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
		if (useful::CollisionBlock(pos, pos, INITVECTOR3, Size, &posThis, posOldThis, &m_move, &m_Objmove, sizeThis, &m_bJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			SetState(STATE_WAIT);
			// ������Ԃ̐ݒ�
			m_pMoveState->SetRotState(CMoveState::ROTSTATE_WAIT);
			posThis = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// �G�̓����蔻��
//====================================================================
void CPlayer::CollisionEnemy(D3DXVECTOR3& posThis)
{
	// �G�̃��X�g�\����������Δ�����
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // ���X�g���擾

	// �G�̃��X�g�̒��g���m�F����
	for (CEnemy* pEnemy : list)
	{
		if (pEnemy->GetHitState() != CEnemy::HIT_STATE_NORMAL)
		{
			continue;
		}

		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 posOld = pEnemy->GetPosOld();
		D3DXVECTOR3 Size = pEnemy->GetSize();

		// �~�̓����蔻��
		if (useful::CollisionCircle(posThis, pos, 30.0f) == true)
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
void CPlayer::CollisionStageOut(D3DXVECTOR3& posThis)
{
	D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();

	if (posThis.x + G_Size > D_pos.x + MapSize.x)	// �E
	{
		posThis.x = D_pos.x + MapSize.x - G_Size;
		SetState(STATE_WAIT);
		m_move.x = 0.0f;
	}
	if (posThis.x - G_Size < D_pos.x - MapSize.x)	// ��
	{
		posThis.x = D_pos.x - MapSize.x + G_Size;
		SetState(STATE_WAIT);
		m_move.x = 0.0f;
	}
	if (posThis.z + G_Size > D_pos.z + MapSize.z)	// ��
	{
		posThis.z = D_pos.z + MapSize.z - G_Size;
		SetState(STATE_WAIT);
		m_move.z = 0.0f;
	}
	if (posThis.z - G_Size < D_pos.z - MapSize.z)	// ��
	{
		posThis.z = D_pos.z - MapSize.z + G_Size;
		SetState(STATE_WAIT);
		m_move.z = 0.0f;
	}
}

//====================================================================
// �X�e�[�W���ɂ��邩�ǂ���
//====================================================================
bool CPlayer::CollisionStageIn(D3DXVECTOR3& posThis)
{
	D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	float G_Size = CMapSystem::GetInstance()->GetGritSize();

	if (posThis.x + G_Size <= D_pos.x + MapSize.x &&
		posThis.x - G_Size >= D_pos.x - MapSize.x &&
		posThis.z + G_Size <= D_pos.z + MapSize.z &&
		posThis.z - G_Size >= D_pos.z - MapSize.z)
	{
		return true;
	}

	return false;
}

//====================================================================
// ��ʊO�Ƃ̈�������
//====================================================================
void CPlayer::CollisionPressStageOut(D3DXVECTOR3& posThis)
{
	if (m_bPressObj == true)
	{
		D3DXVECTOR3 D_pos = CDevil::GetListTop()->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float G_Size = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		if (posThis.x + G_Size > D_pos.x + MapSize.x)
		{
			Death();
		}
		if (posThis.x - G_Size < D_pos.x - MapSize.x)
		{
			Death();
		}
		if (posThis.z + G_Size > D_pos.z + MapSize.z)
		{
			Death();
		}
		if (posThis.z - G_Size < D_pos.z - MapSize.z)
		{
			Death();
		}
	}
}

//====================================================================
//�J�����ʒu�X�V����
//====================================================================
void CPlayer::CameraPosUpdate(D3DXVECTOR3& posThis)
{
	//�J�����ʒu�̍X�V
	m_CameraPos.x = posThis.x;
	m_CameraPos.z = posThis.z;

	if (m_bJump == false)
	{
		m_CameraPos.y = posThis.y;
	}
}

//====================================================================
//�ʒu�X�V����
//====================================================================
void CPlayer::PosUpdate(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis)
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

	CMapMove* pMapMove = CMapMove::GetListTop();

	//Y���̈ʒu�X�V
	posThis.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(posThis,posOldThis,sizeThis,useful::COLLISION_Y);
	CollisionDevilHole(posThis, posOldThis, sizeThis, useful::COLLISION_Y);

	//X���̈ʒu�X�V
	posThis.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeX(m_move.x);

	// �ǂƂ̓����蔻��
	CollisionWall(posThis, posOldThis, sizeThis, useful::COLLISION_X);
	CollisionDevilHole(posThis, posOldThis, sizeThis, useful::COLLISION_X);
	CollisionWaitRailBlock(posThis, posOldThis, sizeThis, useful::COLLISION_X);
	CollisionWaitRock(posThis, posOldThis, sizeThis, useful::COLLISION_X);

	//Z���̈ʒu�X�V
	posThis.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pMapMove->MoveSlopeZ(m_move.z);

	// �ǂƂ̓����蔻��
	CollisionWall(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
	CollisionDevilHole(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
	CollisionWaitRailBlock(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
	CollisionWaitRock(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
}

//====================================================================
//�I�u�W�F�N�g�ɂ��ʒu�X�V����
//====================================================================
void CPlayer::ObjPosUpdate(D3DXVECTOR3& posThis, D3DXVECTOR3& posOldThis, D3DXVECTOR3& sizeThis)
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

	//Y���̈ʒu�X�V
	posThis.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	//X���̈ʒu�X�V
	posThis.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// ���[���u���b�N�Ƃ̓����蔻��
	CollisionMoveRailBlock(posThis, posOldThis, sizeThis, useful::COLLISION_X);
	CollisionMoveRock(posThis, posOldThis, sizeThis, useful::COLLISION_X);

	//Z���̈ʒu�X�V
	posThis.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	CollisionMoveRailBlock(posThis, posOldThis, sizeThis, useful::COLLISION_Z);
	CollisionMoveRock(posThis, posOldThis, sizeThis, useful::COLLISION_Z);

	//// �ǂƂ̓����蔻��
	CollisionPressWall(posThis, sizeThis, useful::COLLISION_ZX);
}

//====================================================================
//�����X�V����
//====================================================================
void CPlayer::RotUpdate(D3DXVECTOR3& rotThis)
{
	// �p�x�̍������Z�o
	D3DXVECTOR3 rotDiff = m_rotDest - rotThis;

	// ���K��
	useful::NormalizeAngle(&rotDiff);

	// �����̍X�V����
	rotThis += (rotDiff * 0.5);
}

//====================================================================
//���̓���
//====================================================================
void CPlayer::EggMove(D3DXVECTOR3& posThis, D3DXVECTOR3& rotThis)
{
	STATE state = GetState();

	if (state == STATE_EGG)
	{
		if (posThis.y > 0.0f)
		{
			//�d��
			m_move.y -= RESPAWN_GRAVITY;

			// �G�t�F�N�g�̐���
			if (m_pEffectEgg == nullptr)
			{
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
				m_pEffectEgg = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_BORN_PLAYER_00 + m_nPlayNumber), true, ef, rotThis);
			}

			// �G�t�F�N�g�̐���
			if (m_pEffectGuide == nullptr && m_pShadow != nullptr)
			{
				D3DXMATRIX mat = *GetUseMultiMatrix();
				D3DXVECTOR3 ef = useful::CalcMatrix(m_pShadow->GetPos(), rotThis, mat);
				m_pEffectGuide = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_BORNGUID1 + m_nPlayNumber), true, ef, rotThis);
			}
		}

		//Y���̈ʒu�X�V
		posThis.y += m_move.y * CManager::GetInstance()->GetGameSpeed();

		//���̗�������
		if (posThis.y < 0.0f)
		{
			//�������Ɉړ��ʂƈʒu���O�ɂ���
			posThis.y = 0.0f;
			m_move.y = 0.0f;

			// �G�t�F�N�g�̐���
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(posThis, rotThis, mat);
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_FALLSMOKE, false, ef, rotThis);

			// �G�t�F�N�g�̍폜
			if (m_pEffectEgg != nullptr)
			{
				m_pEffectEgg->SetDeath();
				m_pEffectEgg = nullptr;
			}

			// �G�t�F�N�g�̍폜
			if (m_pEffectGuide != nullptr)
			{
				m_pEffectGuide->SetDeath();
				m_pEffectGuide = nullptr;
			}
		}

		if (m_pUpEgg != nullptr)
		{
			m_pUpEgg->SetPos(D3DXVECTOR3(posThis.x, posThis.y + 65.0f, posThis.z));
		}
		if (m_pDownEgg != nullptr)
		{
			m_pDownEgg->SetPos(D3DXVECTOR3(posThis.x, posThis.y + 65.0f, posThis.z));
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

			if (pos.y < CObjmeshField::GetListTop()->GetPos().y + 30.0f)
			{
				pos.y = CObjmeshField::GetListTop()->GetPos().y + 30.0f;
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

//==========================================
//  ���Ղ̐ݒu
//==========================================
void CPlayer::PrintFoot(const D3DXVECTOR3& rotThis)
{
	// �O��O���b�h�ƌ��݂̃O���b�h����v���Ă����ꍇ�֐��𔲂���
	if (m_Grid == m_OldGrid) { return; }

	// ���Ղ�ݒ�
	D3DXVECTOR3 rot = rotThis;
	rot.y += D3DX_PI;
	CFootPrint::Create(m_OldGrid, rot);
}

//==========================================
//  �G�t�F�N�g�̈ړ�
//==========================================
void CPlayer::ControlEffect(CEffekseer* pTarget, const D3DXVECTOR3* pPos)
{
	// �����̕ύX
	RotationEffect(pTarget);

	// �ړ�
	MoveEffect(pTarget, pPos);
}

//==========================================
//  �G�t�F�N�g�̉�]
//==========================================
void CPlayer::RotationEffect(CEffekseer* pTarget)
{
	// �Ώۂ̃G�t�F�N�g��null�̏ꍇ�֐��𔲂���
	if (pTarget == nullptr) { return; }

	// �v�Z�Ɏg�p����l�̎擾
	D3DXMATRIX mat = *GetUseMultiMatrix();

	// �����Ƀ}�g���b�N�X�𔽉f
	D3DXVECTOR3 rot = useful::CalcMatrixToRot(mat);

	// �G�t�F�N�g�ɏ���K�p
	pTarget->SetRotation(rot);
}

//==========================================
//  �G�t�F�N�g�̈ړ�
//==========================================
void CPlayer::MoveEffect(CEffekseer* pTarget, const D3DXVECTOR3* pPos)
{
	// �Ώۂ̃G�t�F�N�g��null�̏ꍇ�֐��𔲂���
	if (pTarget == nullptr) { return; }

	// �v�Z�Ɏg�p����l�̎擾
	D3DXMATRIX mat = *GetUseMultiMatrix();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = (pPos != nullptr) ? *pPos : GetPos();

	// ���W�Ƀ}�g���b�N�X�𔽉f
	pos = useful::CalcMatrix(pos, rot, mat);
	rot = useful::CalcMatrixToRot(mat);

	// �G�t�F�N�g�ɏ���K�p
	pTarget->SetPosition(pos);
}

//====================================================================
//�_���[�W����
//====================================================================
void CPlayer::Death(void)
{
	if (m_pP_NumUI != nullptr)
	{
		m_pP_NumUI->SetAppear(false);
	}
}

//====================================================================
// �f�o�b�O�{�^��
//====================================================================
void CPlayer::DebugKey(void)
{
#ifdef _DEBUG

	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�L�[�{�[�h�̈ړ�����
	if (pInputKeyboard->GetTrigger(DIK_3))
	{
		m_nLife++;
		m_pLifeUi->GetNumber()->SetNumber(m_nLife);
	}
	if (pInputKeyboard->GetTrigger(DIK_4))
	{
		m_nLife--;
		m_pLifeUi->GetNumber()->SetNumber(m_nLife);
	}

#endif // !_DEBUG
}

//====================================================================
// ���f���̃��[�V�����ݒ�
//====================================================================
void CPlayer::SetAction(ACTION_TYPE Action, float BlendTime)
{
	// ���[�V�����̎擾
	CMotion* pMotion = GetMotion();

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
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// ���f���̎擾
		CModel* pModel = GetModel(nCnt);

		if (pModel != nullptr)
		{
			pModel->SetDisp(Sst);
		}
	}
}

//==========================================
//  �A�C�e���̐ݒ�
//==========================================
void CPlayer::SetItemType(ITEM_TYPE eType)
{
	// �A�C�e���̃^�C�v��ݒ�
	m_eItemType = eType;

	// �G�t�F�N�g�̐���
	if (eType != TYPE_NONE)
	{
		if (m_pEffectItem == nullptr)
		{
			D3DXMATRIX mat = *GetUseMultiMatrix();
			D3DXVECTOR3 ef = useful::CalcMatrix(GetPos(), GetRot(), mat);
			m_pEffectItem = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_GOT_THEITEM, true, ef, GetRot());
		}
	}

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

		// �G�t�F�N�g�̍폜
		if (m_pEffectItem != nullptr)
		{
			m_pEffectItem->SetDeath();
			m_pEffectItem = nullptr;
		}

		break;
	}
}

//==========================================
// �ړ���ԕύX����
//==========================================
void CPlayer::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
	m_pMoveState->Init();
}

//====================================================================
// �v���C���[�̎w�胂�f������
//====================================================================
void CPlayer::SetPartsDisp(int nParts, bool Set)
{
	// ����ԍ��̃��f���擾
	CModel* pModel = GetModel(nParts);

	if (pModel != nullptr)
	{
		pModel->SetDisp(Set);
	}
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CPlayer>* CPlayer::GetList(void)
{
	return m_pList;
}