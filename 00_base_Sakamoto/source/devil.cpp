//============================================
//
//	�f�r���̏���(��) [devil.cpp]
//	Author:sakamoto kai
//
//============================================
#include "devil.h"
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
#include "bowabowa.h"
#include "Cross.h"
#include "MapSystem.h"
#include "debugproc.h"
#include "objmeshField.h"
#include "railblock.h"
#include "sound.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	float SCROOL_SPEED = 1.5f;					// �X�N���[���̈ړ����x
	float STAGE_ROT_LIMIT = D3DX_PI * 0.25f;	// �X�N���[���̈ړ����x
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CDevil>* CDevil::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CDevil::CDevil(int nPriority) : CObject(nPriority)
{
	SetSize(D3DXVECTOR3(750.0f, 0.0f, 550.0f));
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_fActionCount = 0.0f;
	m_Action = ACTION_NEUTRAL;
	m_AtkAction = ACTION_NEUTRAL;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_pMotion = nullptr;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CDevil::~CDevil()
{

}

//====================================================================
//��������
//====================================================================
CDevil* CDevil::Create()
{
	CDevil* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//�v���C���[�̐���
		pPlayer = new CDevil();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pPlayer->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//����������
//====================================================================
HRESULT CDevil::Init(void)
{
	//��ސݒ�
	SetType(CObject::TYPE_DEVIL);

	//���f���̐���
	LoadLevelData("data\\TXT\\MOTION\\01_enemy\\motion_devil.txt");

	//���[�V�����̐���
	if (m_pMotion == nullptr)
	{
		//���[�V�����̐���
		m_pMotion = new CMotion;
	}

	//����������
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\MOTION\\01_enemy\\motion_devil.txt");

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
	}

	// �X���[�̐���
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PLAYER, CSlowManager::TAG_PLAYER);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CDevil>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CDevil::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = nullptr;
	}

	//���[�V�����̏I������
	if (m_pMotion != nullptr)
	{
		//���[�V�����̔j��
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CDevil::Update(void)
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
void CDevil::TitleUpdate(void)
{
	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CDevil::GameUpdate(void)
{
	// TODO : �����_���ړ���߂�
#ifndef _DEBUG
		// �J�E���^�[�̃��Z�b�g
		if (m_fActionCount >= 5.0f)
		{
			ACTION_TYPE action = m_Action;

			do
			{
				m_Action = (ACTION_TYPE)(rand() % ACTION_MAX);
			} while (m_Action == action);

			m_fActionCount = 0.0f;
		}

		// �J�E���^�[�̉��Z
		m_fActionCount += DeltaTime::Get();
		DebugProc::Print(DebugProc::POINT_CENTER, "�ړ��J�E���^�[ : %f\n", m_fActionCount);

		// �ړ�
		switch (m_Action)
		{
		case ACTION_NEUTRAL:
			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			DebugProc::Print(DebugProc::POINT_CENTER, "�ҋ@\n");
			break;
		case ACTION_SIGNAL_UP:
			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED));
			DebugProc::Print(DebugProc::POINT_CENTER, "��\n");
			break;
		case ACTION_SIGNAL_DOWN:
			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED));
			DebugProc::Print(DebugProc::POINT_CENTER, "��\n");
			break;
		case ACTION_SIGNAL_LEFT:
			ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED, 0.0f, 0.0f));
			DebugProc::Print(DebugProc::POINT_CENTER, "��\n");
			break;
		case ACTION_SIGNAL_RIGHT:
			ObjectScroll(D3DXVECTOR3(SCROOL_SPEED, 0.0f, 0.0f));
			DebugProc::Print(DebugProc::POINT_CENTER, "�E\n");
			break;
		default:
			assert(false);
			break;
		}
#endif // !_DEBUG

	// �ߋ��̈ʒu�ɑ��
	m_posOld = m_pos;

	// �ړ�����
	Move();

	// �����ړ�����
	Rot();

	// �}�b�v�̌X��
	m_DevilRot = CGame::GetMapField()->GetRot();

	//��Ԃ̊Ǘ�
	StateManager();

	if (m_pMotion != nullptr)
	{
		//���[�V�����̍X�V
		m_pMotion->Update();
	}

	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	CEffect* pTestEffect = nullptr;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pTestEffect = CEffect::Create();

		switch (nCnt)
		{
		case 0:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x, m_DevilPos.y, m_DevilPos.z + MapSize.z));
			break;
		case 1:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x, m_DevilPos.y, m_DevilPos.z + MapSize.z));
			break;
		case 2:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x, m_DevilPos.y, m_DevilPos.z - MapSize.z));
			break;
		case 3:
			pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x, m_DevilPos.y, m_DevilPos.z - MapSize.z));
			break;
		}

		pTestEffect->SetLife(20);
		pTestEffect->SetRadius(20.0f);
		pTestEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	//���[�V�����̊Ǘ�
	ActionState();

	//�f�o�b�O�L�[�̏����Ɛݒ�
	DebugKey();

	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	//�f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v]�@�@�@�ʒu %f : %f\n", MapPos.x, MapPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̍���]�ʒu %f : %f\n", m_MapDifference.x, m_MapDifference.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̃X�N���[��]�F���L�[\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X��X]�F[ X+ : 6 ] [ X- : 7 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X��Z]�F[ Z+ : 8 ] [ Z- : 9 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X�����Z�b�g]�F[ 5 ]\n");
}

//====================================================================
//�`���[�g���A���ł̍X�V����
//====================================================================
void CDevil::TutorialUpdate(void)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CDevil::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���̕`��(�S�p�[�c)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//====================================================================
//�ړ�����
//====================================================================
void CDevil::Move(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�L�[�{�[�h�̈ړ�����
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED));

	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED));
	}

	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED, 0.0f, 0.0f));
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		ObjectScroll(D3DXVECTOR3(SCROOL_SPEED, 0.0f, 0.0f));
	}

	if (pInputKeyboard->GetPress(DIK_UP) == false && pInputKeyboard->GetPress(DIK_LEFT) == false && pInputKeyboard->GetPress(DIK_DOWN) == false && pInputKeyboard->GetPress(DIK_RIGHT) == false)
	{
		//���X�e�B�b�N�ɂ��O��ړ�	
		m_DevilPos.z += pInputJoypad->Get_Stick_Left(0).y * SCROOL_SPEED;
		m_DevilPos.x += pInputJoypad->Get_Stick_Left(0).y * SCROOL_SPEED;

		//���X�e�B�b�N�ɂ�鍶�E�ړ�
		m_DevilPos.x += pInputJoypad->Get_Stick_Left(0).x * SCROOL_SPEED;
		m_DevilPos.z -= pInputJoypad->Get_Stick_Left(0).x * SCROOL_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_LEFT) == true || pInputKeyboard->GetPress(DIK_DOWN) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= SCROOL_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= SCROOL_SPEED;
	}

	m_move += NormarizeMove;

}

//====================================================================
//�ړ���������
//====================================================================
void CDevil::Rot(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	
	CObjmeshField *pMapField = CGame::GetMapField();
	D3DXVECTOR3 MapRot = pMapField->GetRot();

	if (pInputKeyboard->GetPress(DIK_5))
	{
		MapRot = INITVECTOR3;
	}

	if (pInputKeyboard->GetPress(DIK_6))
	{
		MapRot.x += D3DX_PI * 0.005f;

		if (MapRot.x > STAGE_ROT_LIMIT)
		{
			MapRot.x = STAGE_ROT_LIMIT;
		}
	}
	if (pInputKeyboard->GetPress(DIK_7))
	{
		MapRot.x -= D3DX_PI * 0.005f;

		if (MapRot.x < -STAGE_ROT_LIMIT)
		{
			MapRot.x = -STAGE_ROT_LIMIT;
		}
	}

	if (pInputKeyboard->GetPress(DIK_8))
	{
		MapRot.z += D3DX_PI * 0.005f;

		if (MapRot.z > STAGE_ROT_LIMIT)
		{
			MapRot.z = STAGE_ROT_LIMIT;
		}
	}
	if (pInputKeyboard->GetPress(DIK_9))
	{
		MapRot.z -= D3DX_PI * 0.005f;

		if (MapRot.z < -STAGE_ROT_LIMIT)
		{
			MapRot.z = -STAGE_ROT_LIMIT;
		}
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CDevil::ActionState(void)
{
	// �ړ����[�V����
	//if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	//{
	//	if (m_Action != ACTION_SIGNAL_UP)
	//	{
	//		m_Action = ACTION_SIGNAL_UP;
	//		m_pMotion->Set(ACTION_SIGNAL_UP, 5);
	//	}
	//}
	//// �j���[�g�������[�V����
	//else
	//{
	//	if (m_Action != ACTION_NEUTRAL)
	//	{
	//		m_Action = ACTION_NEUTRAL;
	//		m_pMotion->Set(ACTION_NEUTRAL, 5);
	//	}
	//}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CDevil::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		//	�X���[��default��
		CSlowManager::SetValueDefault();
		break;

	case STATE_DEATH:
		break;

	case STATE_WAIT:
		break;

	case STATE_DAMAGE:
		if (m_nStateCount == 0)
		{
			m_State = STATE_NORMAL;
		}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// �f�o�b�O�{�^��
//====================================================================
void CDevil::DebugKey(void)
{
#ifdef _DEBUG

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_WHEEL))
	{
		HitDamage(10.0f);
	}

#endif // !_DEBUG
}

//====================================================================
// ���f���̃��[�V�����ݒ�
//====================================================================
void CDevil::SetAction(ACTION_TYPE Action, float BlendTime)
{
	if (m_Action != Action)
	{
		m_Action = Action;
		m_pMotion->Set(Action, BlendTime);
	}
}

//====================================================================
// ���f���\���̐ݒ�
//====================================================================
void CDevil::SetModelDisp(bool Sst)
{
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->SetDisp(Sst);
		}
	}
}

//====================================================================
// �I�u�W�F�N�g�̃X�N���[��
//====================================================================
void CDevil::ObjectScroll(D3DXVECTOR3 Move)
{
	// �O���b�h�̃X�N���[��
	GritScroll(Move);

	// �O���b�h�̃T�C�Y
	float m_GritSize = CMapSystem::GetInstance()->GetGritSize();

	// �\���˂̃X�N���[��
	CrossScroll(Move, m_GritSize);

	// �{���{���̃X�N���[��
	BowabowaScroll(Move, m_GritSize);

	// �G�̃X�N���[��
	EnemyScroll(Move, m_GritSize);

	// ���[���u���b�N�̃X�N���[��
	RailBlockScroll(Move, m_GritSize);

	// �v���C���[�̃X�N���[��
	PlayerScroll(Move, m_GritSize);
}

//====================================================================
// �\���˂̃X�N���[��
//====================================================================
void CDevil::CrossScroll(D3DXVECTOR3 Move, float GritSize)
{
	// �\���˂̃��X�g�\����������Δ�����
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // ���X�g���擾

	// �\���˂̃��X�g�̒��g���m�F����
	for (CCross* pCross : list)
	{
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		D3DXVECTOR3 pos = pCross->GetPos();
		D3DXVECTOR3 Size = pCross->GetSize();

		pos += Move;

		if (Move.x > 0.0f)
		{
			if (m_DevilPos.x + MapSize.x < pos.x - GritSize)
			{
				pos.x = -MapSize.x + m_DevilPos.x - GritSize + Move.x;
			}
		}
		if (Move.x < 0.0f)
		{
			if (m_DevilPos.x - MapSize.x > pos.x + GritSize)
			{
				pos.x = MapSize.x + m_DevilPos.x + GritSize + Move.x;
			}
		}
		if (Move.z > 0.0f)
		{
			if (m_DevilPos.z + MapSize.z < pos.z - GritSize)
			{
				pos.z = -MapSize.z + m_DevilPos.z - GritSize + Move.z;
			}
		}
		if (Move.z < 0.0f)
		{
			if (m_DevilPos.z - MapSize.z > pos.z + GritSize)
			{
				pos.z = MapSize.z + m_DevilPos.z + GritSize + Move.z;
			}
		}

		// �ʒu�ݒ�
		pCross->SetPos(pos);
	}
}

//====================================================================
// �{���{���̃X�N���[��
//====================================================================
void CDevil::BowabowaScroll(D3DXVECTOR3 Move, float GritSize)
{
	// �{���{���̃��X�g�\����������Δ�����
	if (CBowabowa::GetList() == nullptr) { return; }
	std::list<CBowabowa*> list = CBowabowa::GetList()->GetList();    // ���X�g���擾

	// �{���{���̃��X�g�̒��g���m�F����
	for (CBowabowa* pBowabowa : list)
	{
		D3DXVECTOR3 pos = pBowabowa->GetPos();
		D3DXVECTOR3 Size = pBowabowa->GetSize();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

		pos += Move;

		if (Move.x > 0.0f)
		{
			if (MapSize.x < pos.x)
			{
				pos.x = -MapSize.x + Move.x - 100.0f;
			}
		}
		if (Move.x < 0.0f)
		{
			if (-MapSize.x > pos.x)
			{
				pos.x = MapSize.x + Move.x + 100.0f;
			}
		}
		if (Move.z > 0.0f)
		{
			if (MapSize.z < pos.z)
			{
				pos.z = -MapSize.z + Move.z - 100.0f;
			}
		}
		if (Move.z < 0.0f)
		{
			if (-MapSize.z > pos.z)
			{
				pos.z = MapSize.z + Move.z + 100.0f;
			}
		}

		pBowabowa->SetPos(pos);
	}
}

//====================================================================
// �G�̃X�N���[��
//====================================================================
void CDevil::EnemyScroll(D3DXVECTOR3 Move, float GritSize)
{
	// �G�̃��X�g�\����������Δ�����
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // ���X�g���擾

	// �G�̃��X�g�̒��g���m�F����
	for (CEnemy* pEnemy : list)
	{
		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 Size = pEnemy->GetSize();

		pos += Move;

		pEnemy->SetPos(pos);
	}
}

//====================================================================
// ���[���u���b�N�̃X�N���[��
//====================================================================
void CDevil::RailBlockScroll(D3DXVECTOR3 Move, float GritSize)
{
	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRailBlock::GetList() == nullptr) { return; }
	std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // ���X�g���擾

	// ���[���u���b�N�̃��X�g�̒��g���m�F����
	for (CRailBlock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = pRailBlock->GetPos();
		pos += Move;
		pRailBlock->SetPos(pos);
	}
}

//====================================================================
// �v���C���[�̃X�N���[��
//====================================================================
void CDevil::PlayerScroll(D3DXVECTOR3 Move, float GritSize)
{
	// �v���C���[�̃��X�g�\����������Δ�����
	if (CPlayer::GetList() == nullptr) { return; }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // ���X�g���擾

	// �v���C���[�̃��X�g�̒��g���m�F����
	for (CPlayer* pPlayer : list)
	{
		if (pPlayer->GetState() != CPlayer::STATE_EGG)
		{
			D3DXVECTOR3 pos = pPlayer->GetPos();
			D3DXVECTOR3 Size = pPlayer->GetSize();
			D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

			pos += Move;

			if (Move.x > 0.0f)
			{
				if (pos.x + (GritSize * 0.5f) > m_DevilPos.x + MapSize.x)
				{
					pos.x = m_DevilPos.x + MapSize.x - (GritSize * 0.5f) + Move.x;
					CollisionPressPlayer(pPlayer, pos, Size);

					CEffect* pEffect = CEffect::Create();
					pEffect->SetPos(pos);
					pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					pEffect->SetRadius(100.0f);
				}
			}
			if (Move.x < 0.0f)
			{
				if (pos.x - (GritSize * 0.5f) < m_DevilPos.x - MapSize.x)
				{
					pos.x = m_DevilPos.x - MapSize.x + (GritSize * 0.5f) + Move.x;
					CollisionPressPlayer(pPlayer, pos, Size);

					CEffect* pEffect = CEffect::Create();
					pEffect->SetPos(pos);
					pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					pEffect->SetRadius(100.0f);
				}
			}
			if (Move.z > 0.0f)
			{
				if (pos.z + (GritSize * 0.5f) > m_DevilPos.z + MapSize.z)
				{
					pos.z = m_DevilPos.z + MapSize.z - (GritSize * 0.5f) + Move.x;
					CollisionPressPlayer(pPlayer, pos, Size);

					CEffect* pEffect = CEffect::Create();
					pEffect->SetPos(pos);
					pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					pEffect->SetRadius(100.0f);
				}
			}
			if (Move.z < 0.0f)
			{
				if (pos.z - (GritSize * 0.5f) < m_DevilPos.z - MapSize.z)
				{
					pos.z = m_DevilPos.z - MapSize.z + (GritSize * 0.5f) + Move.z;
					CollisionPressPlayer(pPlayer, pos, Size);

					CEffect* pEffect = CEffect::Create();
					pEffect->SetPos(pos);
					pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
					pEffect->SetRadius(100.0f);
				}
			}

			pPlayer->SetPos(pos);
		}
		else
		{// ����Ԃ̎��A�u���b�N�����݂��Ȃ��ʒu�Ƀz�[�~���O����

			CMapSystem::GRID Grit = pPlayer->GetGrid();;
			if (!CMapSystem::GetInstance()->GetGritBool(Grit.x, Grit.z))
			{
				D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
				D3DXVECTOR3 AnswerPos = INITVECTOR3;
				AnswerPos = CMapSystem::GetInstance()->GetGritPos(Grit.x, Grit.z);

				if (pPlayer->GetGritCenter())
				{
					pPlayer->SetPos(AnswerPos);
				}
				else
				{
					PlayerPos += (AnswerPos - PlayerPos) * 0.5f;
					pPlayer->SetPos(PlayerPos);
				}
			}
		}
	}
}

//====================================================================
// �O���b�g�̃X�N���[��
//====================================================================
void CDevil::GritScroll(D3DXVECTOR3 Move)
{
	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();
	int MapWightMax = CMapSystem::GetInstance()->GetWightMax();
	int MapHeightMax = CMapSystem::GetInstance()->GetHeightMax();
	float MapGrit = CMapSystem::GetInstance()->GetGritSize();
	MapPos += Move;
	m_MapDifference = -(InitPos - MapPos);
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

	if ((InitPos.x - MapPos.x) > 0.0f)
	{// ���͈�
		MapPos.x = InitPos.x + (MapSize.x * 2.0f) + MapGrit + Move.x;
	}
	if ((InitPos.x - MapPos.x + MapGrit) < (-MapSize.x * 2.0f))
	{// �E�͈�
		MapPos.x = InitPos.x + Move.x;
	}

	if ((InitPos.z - MapPos.z) < 0.0f)
	{// ��͈�
		MapPos.z = InitPos.z + (-MapSize.z * 2.0f) - MapGrit + Move.z;
	}
	if ((InitPos.z - MapPos.z - MapGrit) > (MapSize.z * 2.0f))
	{// ���͈�
		MapPos.z = InitPos.z + Move.z;
	}

	CMapSystem::GetInstance()->SetMapPos(MapPos);

	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N�̃��X�g�̒��g���m�F����
	for (CCubeBlock* pCubeBlock : list)
	{
		// �c���̃i���o�[�ƍ�����ݒ肷��
		D3DXVECTOR3 pos = INITVECTOR3;
		int BlockWight = pCubeBlock->GetWightNumber();
		int BlockHeight = pCubeBlock->GetHeightNumber();

		//�O���b�g�ԍ����ʒu�ɕϊ�
		pos = CMapSystem::GetInstance()->GetGritPos(BlockWight, BlockHeight);
		pos.y = 50.0f;

		pCubeBlock->SetPos(pos);
	}

#ifdef _DEBUG

	////�@�O���b�g�̈ʒu�ɃG�t�F�N�g��\��
	//for (int nCntW = 0; nCntW < MapWightMax; nCntW++)
	//{
	//	for (int nCntH = 0; nCntH < MapHeightMax; nCntH++)
	//	{
	//		//�O���b�g�ԍ����ʒu�ɕϊ�
	//		D3DXVECTOR3 CountPos = CMapSystem::GetInstance()->GetGritPos(nCntW, nCntH);
	//		CountPos.y = 50.0f;

	//		if (CMapSystem::GetInstance()->GetGritBool(nCntW, nCntH))
	//		{// �u���b�N�����݂���O���b�g�̂݃G�t�F�N�g��\��

	//			CEffect* pEffect = CEffect::Create();
	//			pEffect->SetPos(CountPos);
	//			pEffect->SetLife(10);
	//		}
	//	}
	//}

#endif // _DEBUG
}

//====================================================================
// �v���C���[���ׂ���鎞�̏���
//====================================================================
void CDevil::CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size)
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N�̃��X�g�̒��g���m�F����
	for (CCubeBlock* pCubeBlock : list)
	{
		D3DXVECTOR3 Blockpos = pCubeBlock->GetPos();
		D3DXVECTOR3 BlockSize = pCubeBlock->GetSize();

		if (useful::CollisionRectangle2D(pos, Blockpos, Size, BlockSize, useful::COLLISION::COLLISION_ZX))
		{
			pPlayer->Death();
			return;
		}
	}
}

//====================================================================
//�X�����̈ړ��ʕϓ�
//====================================================================
float CDevil::MoveSlopeX(void)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		DevilRot = m_DevilRot;
	}

	if (m_move.x > 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z));
	}
	else if (m_move.x < 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z));
	}

	return fSlopeMove;
}

//====================================================================
//�X�����̈ړ��ʕϓ�
//====================================================================
float CDevil::MoveSlopeZ(void)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		DevilRot = CGame::GetDevil()->GetDevilRot();
	}

	if (m_move.z > 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x));
	}
	else if (m_move.z < 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x));
	}

	return fSlopeMove;
}

//====================================================================
//���[�h����
//====================================================================
void CDevil::LoadLevelData(const char* pFilename)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//�S�~��
		char aMessage[128] = {};			//�X�^�[�g�ƃG���h�̃��b�Z�[�W
		char aBool[128] = {};				//bool�ϊ��p���b�Z�[�W

		// �ǂݍ��݊J�n-----------------------------------------------------
		while (1)
		{//�uSCRIPT�v��T��
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// ���f�����ǂݍ���-----------------------------------------------------
				while (1)
				{//�uNUM_MODEL�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &m_nNumModel);		//���f�����̐ݒ�
						break;
					}
				}

				//���f���t�@�C���̓ǂݍ���
				while (1)
				{//�uMODEL_FILENAME�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelName[0]);		//�ǂݍ��ރ��f���̃p�X���擾

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// �L�����N�^�[���ǂݍ���-----------------------------------------------------
				while (1)
				{//�uPARTSSET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nCntModel);	//�C���f�b�N�X��ݒ�
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//�e���f���̃C���f�b�N�X��ݒ�

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(nullptr);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.y);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.z);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				//�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.y);				//�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.z);				//�����̏����ݒ�

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CDevil>* CDevil::GetList(void)
{
	return m_pList;
}
