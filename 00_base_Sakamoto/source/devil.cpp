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
#include "objectcharacter.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "camera.h"
#include "input.h"
#include "enemy.h"
#include "CubeBlock.h"
#include "slowManager.h"
#include "Number.h"
#include "MapModel.h"
#include "bowabowa.h"
#include "Cross.h"
#include "objmeshField.h"
#include "railblock.h"
#include "RollRock.h"
#include "sound.h"
#include "scrollarrow.h"
#include "fire.h"
#include "Scene.h"
#include "tile.h"
#include "wall.h"
#include "MapSystem.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "effect.h"
#include "signal.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	int SCROOL_TIME = 300;						// �X�N���[������

	int SCROOL_MOVEGRID_01 = 3;					// �X�N���[���̈ړ��}�X��
	float SCROOL_SPEED_01 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_01 / SCROOL_TIME);				// �X�N���[���̈ړ����x

	int SCROOL_COUNT_02 = 12;					// �X�N���[���̈ړ���
	int SCROOL_MOVEGRID_02 = 3;					// �X�N���[���̈ړ��}�X��
	float SCROOL_SPEED_02 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_02) / SCROOL_COUNT_02;				// �X�N���[���̈ړ����x

	int SLOPE_TIME = 300;						// �X�����쎞��
	int SLOPE_RAND = 25;						// �X�������m��
	float STAGE_ROT_LIMIT = D3DX_PI * 0.15f;	// �X���̊p�x����

	float SLOPE_SPEED01 = 0.00075f;				// �X���̈ړ����x

	float SLOPE_SPEED02 = 0.0125f;				// �X���̈ړ����x
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CDevil>* CDevil::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CDevil::CDevil(int nPriority) : CObjectCharacter(nPriority)
{
	SetSize(D3DXVECTOR3(750.0f, 0.0f, 550.0f));
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_fActionCount = 0.0f;
	m_Action = ACTION_NEUTRAL;
	m_AtkAction = ACTION_NEUTRAL;
	m_State = STATE_WAIT;
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
	m_bSlope = false;
	m_MinGrid = CMapSystem::GRID(0, 0);
	m_MaxGrid = CMapSystem::GRID(NUM_WIGHT - 1, NUM_HEIGHT - 1);
	m_DevilArrow = 0;
	m_ScrollArrowOld = 0;
	m_SlopwArrowOld = 0;
	m_ScrollArrow[0] = nullptr;
	m_ScrollArrow[1] = nullptr;
	m_ScrollType = SCROLL_TYPE_NORMAL;
	m_pSignal[0] = nullptr;
	m_pSignal[1] = nullptr;
	m_nStateNum = 0;
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

	// �e��s�g�p�ɐݒ�
	SetShadow(false);

	// �L�����N�^�[�e�L�X�g�̐ݒ菈��
	CObjectCharacter::Init("data\\TXT\\MOTION\\01_enemy\\motion_devil.txt");
	
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

	if (m_ScrollArrow[0] == nullptr)
	{
		m_ScrollArrow[0] = CScrollArrow::Create();
		m_ScrollArrow[0]->SetPos((D3DXVECTOR3(80.0f, 120.0f, 0.0f)));
	}

	if (m_ScrollArrow[1] == nullptr)
	{
		m_ScrollArrow[1] = CScrollArrow::Create();
		m_ScrollArrow[1]->SetPos((D3DXVECTOR3(1200.0f, 120.0f, 0.0f)));
	}

	// ��󐶐�
	if (m_pSignal[0] == nullptr)
	{
		m_pSignal[0] = CSignal::Create("data\\MODEL\\signal.x");
		m_pSignal[0]->SetPos(D3DXVECTOR3(-1000.0f,200.0f,500.0f));
	}

	if (m_pSignal[1] == nullptr)
	{
		m_pSignal[1] = CSignal::Create("data\\MODEL\\signal.x");
		m_pSignal[1]->SetPos(D3DXVECTOR3(1000.0f, 200.0f, 500.0f));
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

	// �L�����N�^�[�N���X�̏I���i�p���j
	CObjectCharacter::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CDevil::Update(void)
{
	// �l���擾
	D3DXVECTOR3 posMy = GetPos();			// �ʒu
	D3DXVECTOR3 posOldMy = GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 rotMy = GetRot();			// ����
	D3DXVECTOR3 sizeMy = GetSize();			// �傫��

	// �ߋ��̈ʒu�ɑ��
	posOldMy = posMy;

	// �}�b�v�̌X��
	m_DevilRot = CObjmeshField::GetListTop()->GetRot();

	// ���̉�]
	SignalScroll();

	//��Ԃ̊Ǘ�
	StateManager();

	//�f�o�b�O�L�[�̏����Ɛݒ�
	DebugKey();

	//�X�e�[�W�O�ɂ���I�u�W�F�N�g�̏���
	CollisionOut();

	// �L�����N�^�[�N���X�̍X�V�i�p���j
	CObjectCharacter::Update();

	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	pTestEffect = CEffect::Create();
	//
	//	switch (nCnt)
	//	{
	//	case 0:
	//		pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x, m_DevilPos.y, m_DevilPos.z + MapSize.z));
	//		break;
	//	case 1:
	//		pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x, m_DevilPos.y, m_DevilPos.z + MapSize.z));
	//		break;
	//	case 2:
	//		pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x, m_DevilPos.y, m_DevilPos.z - MapSize.z));
	//		break;
	//	case 3:
	//		pTestEffect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x, m_DevilPos.y, m_DevilPos.z - MapSize.z));
	//		break;
	//	}
	//
	//	pTestEffect->SetLife(20);
	//	pTestEffect->SetRadius(20.0f);
	//	pTestEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//}

	//���[�V�����̊Ǘ�
	ActionState();

	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	//�f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v]�@�@�@�ʒu %f : %f\n", MapPos.x, MapPos.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̍���]�ʒu %f : %f\n", m_MapDifference.x, m_MapDifference.z);
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̃X�N���[��]�F���L�[\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X��X]�F[ X+ : 6 ] [ X- : 7 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X��Z]�F[ Z+ : 8 ] [ Z- : 9 ]\n");
	DebugProc::Print(DebugProc::POINT_LEFT, "[�}�b�v�̌X�����Z�b�g]�F[ 5 ]\n");

	DebugProc::Print(DebugProc::POINT_RIGHT, "[�ŏ��ԍ�]�� %d : �� %d\n", m_MinGrid.x, m_MinGrid.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[�ő�ԍ�]�E %d : �� %d\n", m_MaxGrid.x, m_MaxGrid.z);

	// �l�X�V
	SetPos(posMy);			// �ʒu
	SetPosOld(posOldMy);	// �O��̈ʒu
	SetRot(rotMy);			// ����
	SetSize(sizeMy);		// �傫��
}

//====================================================================
//�`�揈��
//====================================================================
void CDevil::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();
}

//====================================================================
//�ړ�����
//====================================================================
void CDevil::Move(int Arroow)
{
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	switch (m_ScrollType)
	{
	case CDevil::SCROLL_TYPE_NORMAL:

		switch (Arroow)
		{
		case 0:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED_01));
			m_move.z = SCROOL_SPEED_01;

			break;
		case 1:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED_01));
			m_move.z = -SCROOL_SPEED_01;

			break;
		case 2:

			ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED_01, 0.0f, 0.0f));
			m_move.x = -SCROOL_SPEED_01;

			break;
		case 3:

			ObjectScroll(D3DXVECTOR3(SCROOL_SPEED_01, 0.0f, 0.0f));
			m_move.x = SCROOL_SPEED_01;

			break;
		}

		break;

	case CDevil::SCROLL_TYPE_RETRO:

		switch (Arroow)
		{
		case 0:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED_02));
			m_move.z = SCROOL_SPEED_02;

			break;
		case 1:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED_02));
			m_move.z = -SCROOL_SPEED_02;

			break;
		case 2:

			ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED_02, 0.0f, 0.0f));
			m_move.x = -SCROOL_SPEED_02;

			break;
		case 3:

			ObjectScroll(D3DXVECTOR3(SCROOL_SPEED_02, 0.0f, 0.0f));
			m_move.x = SCROOL_SPEED_02;

			break;
		}

		break;

	default:

		switch (Arroow)
		{
		case 0:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, SCROOL_SPEED_01));
			m_move.z = SCROOL_SPEED_01;

			break;
		case 1:

			ObjectScroll(D3DXVECTOR3(0.0f, 0.0f, -SCROOL_SPEED_01));
			m_move.z = -SCROOL_SPEED_01;

			break;
		case 2:

			ObjectScroll(D3DXVECTOR3(-SCROOL_SPEED_01, 0.0f, 0.0f));
			m_move.x = -SCROOL_SPEED_01;

			break;
		case 3:

			ObjectScroll(D3DXVECTOR3(SCROOL_SPEED_01, 0.0f, 0.0f));
			m_move.x = SCROOL_SPEED_01;

			break;
		}

		break;
	}
}

//====================================================================
//�ړ���������
//====================================================================
void CDevil::BackSlope(void)
{
	CObjmeshField* pMapField = CObjmeshField::GetListTop();
	D3DXVECTOR3 MapRot = pMapField->GetRot();
	bool bBackOK = false;

	switch (m_ScrollType)
	{
	case CDevil::SCROLL_TYPE_NORMAL:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED01;
		}

		break;

	case CDevil::SCROLL_TYPE_RETRO:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x <= 0.0f)
			{
				bBackOK = true;
			}
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x >= 0.0f)
			{
				bBackOK = true;
			}
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z <= 0.0f)
			{
				bBackOK = true;
			}
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z >= 0.0f)
			{
				bBackOK = true;
			}
		}

		break;

	default:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED01;
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED01;
		}

		break;
	}

	if (bBackOK)
	{
		STATE_WAIT;
		m_nStateCount = 120;
		MapRot.x = 0.0f;
		MapRot.z = 0.0f;
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
//�ړ���������
//====================================================================
void CDevil::Slope(int Arroow)
{	
	CObjmeshField *pMapField = CObjmeshField::GetListTop();
	if (pMapField == nullptr) { assert(false); }

	D3DXVECTOR3 MapRot = pMapField->GetRot();

	switch (m_ScrollType)
	{
	case CDevil::SCROLL_TYPE_NORMAL:

		switch (Arroow)
		{
		case 0:

			MapRot.x += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x > STAGE_ROT_LIMIT)
			{
				MapRot.x = STAGE_ROT_LIMIT;
			}

			break;
		case 1:

			MapRot.x -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x < -STAGE_ROT_LIMIT)
			{
				MapRot.x = -STAGE_ROT_LIMIT;
			}

			break;
		case 2:

			MapRot.z += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z > STAGE_ROT_LIMIT)
			{
				MapRot.z = STAGE_ROT_LIMIT;
			}

			break;
		case 3:

			MapRot.z -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z < -STAGE_ROT_LIMIT)
			{
				MapRot.z = -STAGE_ROT_LIMIT;
			}

			break;
		}

		break;

	case CDevil::SCROLL_TYPE_RETRO:

		switch (Arroow)
		{
		case 0:

			MapRot.x += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x > STAGE_ROT_LIMIT)
			{
				MapRot.x = STAGE_ROT_LIMIT;
			}

			break;
		case 1:

			MapRot.x -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x < -STAGE_ROT_LIMIT)
			{
				MapRot.x = -STAGE_ROT_LIMIT;
			}

			break;
		case 2:

			MapRot.z += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z > STAGE_ROT_LIMIT)
			{
				MapRot.z = STAGE_ROT_LIMIT;
			}

			break;
		case 3:

			MapRot.z -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z < -STAGE_ROT_LIMIT)
			{
				MapRot.z = -STAGE_ROT_LIMIT;
			}

			break;
		}

		break;

	default:

		switch (Arroow)
		{
		case 0:

			MapRot.x += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x > STAGE_ROT_LIMIT)
			{
				MapRot.x = STAGE_ROT_LIMIT;
			}

			break;
		case 1:

			MapRot.x -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x < -STAGE_ROT_LIMIT)
			{
				MapRot.x = -STAGE_ROT_LIMIT;
			}

			break;
		case 2:

			MapRot.z += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z > STAGE_ROT_LIMIT)
			{
				MapRot.z = STAGE_ROT_LIMIT;
			}

			break;
		case 3:

			MapRot.z -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z < -STAGE_ROT_LIMIT)
			{
				MapRot.z = -STAGE_ROT_LIMIT;
			}

			break;
		}

		break;
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CDevil::CollisionOut()
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CEnemy* pEnemy : list)
	{
		CMapSystem::GRID EnemyGrid = pEnemy->GetGrid();
		D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
		D3DXVECTOR3 EnemyMove = pEnemy->GetMove();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		//m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//��
		//m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//�E
		//m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize - 1.0f);	//��
		//m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//��

		//if (EnemyGrid.x == m_MinGrid.x)		//��
		//{
		//	if (EnemyMove.x < 0.0f || m_DevilArrow == 3)
		//	{
		//		EnemyPos = CMapSystem::GRID(m_MaxGrid.x, pEnemy->GetGrid().z).ToWorld();
		//		EnemyPos.x += GritSize * 2;
		//	}
		//}
		//if (EnemyGrid.x == m_MaxGrid.x)		//�E
		//{
		//	if (EnemyMove.x > 0.0f || m_DevilArrow == 2)
		//	{
		//		EnemyPos = CMapSystem::GRID(m_MinGrid.x, pEnemy->GetGrid().z).ToWorld();
		//		EnemyPos.x -= GritSize * 2;
		//	}
		//}

		//if (EnemyGrid.z == m_MinGrid.z)		//��
		//{
		//	if (EnemyMove.z > 0.0f || m_DevilArrow == 0)
		//	{
		//		EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MaxGrid.z).ToWorld();
		//		EnemyPos.z -= GritSize * 2;
		//	}
		//}
		//if (EnemyGrid.z == m_MaxGrid.z)		//��
		//{
		//	if (EnemyMove.z < 0.0f || m_DevilArrow == 1)
		//	{
		//		EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MinGrid.z).ToWorld();
		//		EnemyPos.z += GritSize * 2;
		//	}
		//}

		m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//��
		m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//��
		m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//�E
		m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//��

		CEffect *Effect = CEffect::Create();
		Effect->SetPos(CMapSystem::GetInstance()->GetGritPos(m_MinGrid));
		//Effect->SetPos(D3DXVECTOR3(m_DevilPos.x - MapSize.x - GritSize, 0.0f, m_DevilPos.z + MapSize.z + GritSize));
		Effect->SetRadius(50.0f);

		Effect = CEffect::Create();
		Effect->SetPos(CMapSystem::GetInstance()->GetGritPos(m_MaxGrid));
		//Effect->SetPos(D3DXVECTOR3(m_DevilPos.x + MapSize.x - GritSize, 0.0f, m_DevilPos.z - MapSize.z + GritSize));
		Effect->SetRadius(50.0f);

		if (EnemyGrid.x == -1)
		{
			if (EnemyPos.x > 0.0f)
			{
				m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//��
				EnemyPos = CMapSystem::GRID(m_MinGrid.x, pEnemy->GetGrid().z).ToWorld();
			}
			else
			{
				m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//�E
				EnemyPos = CMapSystem::GRID(m_MaxGrid.x, pEnemy->GetGrid().z).ToWorld();
			}
		}
		else if (EnemyGrid.z == -1)
		{
			if (EnemyPos.z < 0.0f)
			{
				m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//��
				EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MinGrid.z).ToWorld();
			}
			else
			{
				m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//��
				EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MaxGrid.z).ToWorld();
			}
		}

		pEnemy->SetPos(EnemyPos);
	}
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CDevil::ActionState(void)
{
	// �ヂ�[�V����
	if (m_DevilArrow == 0)
	{
		if (m_Action != ACTION_SIGNAL_UP)
		{
			m_Action = ACTION_SIGNAL_UP;
			m_nStateNum = ACTION_SIGNAL_UP;
			GetMotion()->Set(ACTION_SIGNAL_UP, 5);
		}
	}
	// �����[�V����
	if (m_DevilArrow == 1)
	{
		if (m_Action != ACTION_SIGNAL_DOWN)
		{
			m_Action = ACTION_SIGNAL_DOWN;
			m_nStateNum = ACTION_SIGNAL_DOWN;
			GetMotion()->Set(ACTION_SIGNAL_DOWN, 5);
		}
	}
	// �����[�V����
	if (m_DevilArrow == 2)
	{
		if (m_Action != ACTION_SIGNAL_LEFT)
		{
			m_Action = ACTION_SIGNAL_LEFT;
			m_nStateNum = ACTION_SIGNAL_LEFT;
			GetMotion()->Set(ACTION_SIGNAL_LEFT, 5);
		}
	}
	// �E���[�V����
	if (m_DevilArrow == 3)
	{
		if (m_Action != ACTION_SIGNAL_RIGHT)
		{
			m_Action = ACTION_SIGNAL_RIGHT;
			m_nStateNum = ACTION_SIGNAL_RIGHT;
			GetMotion()->Set(ACTION_SIGNAL_RIGHT, 5);
		}
	}
	// �j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_NEUTRAL)
		{
			m_Action = ACTION_NEUTRAL;
			GetMotion()->Set(ACTION_NEUTRAL, 5);
		}
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CDevil::StateManager(void)
{
	switch (m_State)
	{
	case STATE_WAIT:

		// ��ԏI����
		if (m_nStateCount <= 0)
		{
			// 100����rand()���܂킷
			int nRand = rand() % 101;

			// �X���̎w�聓�̎�
			if (nRand <= SLOPE_RAND)
			{
				// �X����Ԑݒ�
				m_State = STATE_SLOPE;

				// �X���Ă����Ԃ��ǂ�����؂�ւ���
				m_bSlope = !m_bSlope;

				if (m_bSlope)
				{// �X����Ԃ̎�

					//�X�������w�菈��
					m_DevilArrow = rand() % 2;

					if (m_SlopwArrowOld == 0 || m_SlopwArrowOld == 1)
					{// �O��̌X�����������E�������ꍇ

						// ����̌X�������͏㉺�ɂ���
						if (m_DevilArrow == 0)
						{
							m_DevilArrow = 2;
						}
						else if (m_DevilArrow == 1)
						{
							m_DevilArrow = 3;
						}
					}

					// ����̌X���������L�^����
					m_SlopwArrowOld = m_DevilArrow;

					m_nStateCount = SLOPE_TIME;

					m_nStateNum = m_DevilArrow;

					// �X�����u�̃��X�g�\����������Δ�����
					if (CSlopeDevice::GetList() == nullptr) { return; }
					std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // ���X�g���擾

					// �X�����u�̃��X�g�̒��g���m�F����
					for (CSlopeDevice* pSlopeDevice : list)
					{
						// �����̌X�����u���㏸��ԂɕύX
						pSlopeDevice->SetState(CSlopeDevice::STATE_ASCENT, (CScrollArrow::Arrow)m_DevilArrow);
					}
				}
				else
				{// �X���߂���Ԃ̎�

					// �X����߂��������{�̎��Ԃ��w�肵�A�߂�؂�����X����Ԃ��I���Ƃ���
					m_nStateCount = SLOPE_TIME * 2;

					// �X�����u�̃��X�g�\����������Δ�����
					if (CSlopeDevice::GetList() == nullptr) { return; }
					std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // ���X�g���擾

					// �X�����u�̃��X�g�̒��g���m�F����
					for (CSlopeDevice* pSlopeDevice : list)
					{
						// �����̌X�����u���㏸��ԂɕύX
						pSlopeDevice->SetState(CSlopeDevice::STATE_DESCENT, (CScrollArrow::Arrow)m_SlopwArrowOld);
					}
				}
			}
			else
			{// �X���̎w�聓����Ȃ���

				// �X�N���[����Ԑݒ�
				m_State = STATE_SCROLL;

				// �X�N���[�����Ԑݒ�
				m_nStateCount = SCROOL_TIME;

				// �X�N���[�������w��
				m_DevilArrow = rand() % 2;

				if (m_ScrollArrowOld == 0 || m_ScrollArrowOld == 1)
				{// �O��̌X�����������E�������ꍇ

					// ����̌X�������͏㉺�ɂ���
					if (m_DevilArrow == 0)
					{
						m_DevilArrow = 2;
					}
					else if (m_DevilArrow == 1)
					{
						m_DevilArrow = 3;
					}
				}
				m_ScrollArrowOld = m_DevilArrow;
				m_nStateNum = m_DevilArrow;

				// �}�b�v�ړ����u�̃��X�g�\����������Δ�����
				if (CScrollDevice::GetList() == nullptr) { return; }
				std::list<CScrollDevice*> list = CScrollDevice::GetList()->GetList();    // ���X�g���擾

				// �}�b�v�ړ����u�̃��X�g�̒��g���m�F����
				for (CScrollDevice* pScrollDevice : list)
				{
					// ��]��ԂɕύX
					pScrollDevice->SetState(CScrollDevice::STATE_ROTATE);
				}
			}

			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				if (m_ScrollArrow[nCnt] != nullptr)
				{
					switch (m_DevilArrow)
					{
					case 0:
						m_ScrollArrow[nCnt]->SetState(CScrollArrow::Arrow::STATE_UP);

						break;

					case 1:
						m_ScrollArrow[nCnt]->SetState(CScrollArrow::Arrow::STATE_DOWN);
						break;

					case 2:
						m_ScrollArrow[nCnt]->SetState(CScrollArrow::Arrow::STATE_LEFT);
						break;

					case 3:
						m_ScrollArrow[nCnt]->SetState(CScrollArrow::Arrow::STATE_RIGHT);
						break;
					}
				}
			}
		}

		break;

	case STATE_SCROLL:

		switch (m_ScrollType)
		{
		case CDevil::SCROLL_TYPE_NORMAL:
			Move(m_DevilArrow);
			break;

		case CDevil::SCROLL_TYPE_RETRO:

			if (m_nStateCount % (SCROOL_TIME / SCROOL_COUNT_02) == 0)
			{
				Move(m_DevilArrow);
			}

			break;

		default:

			break;
		}

		if (m_nStateCount <= 0)
		{
			m_State = STATE_WAIT;
			m_nStateCount = 120;

			// �}�b�v�ړ����u�̃��X�g�\����������Δ�����
			if (CScrollDevice::GetList() == nullptr) { return; }
			std::list<CScrollDevice*> list = CScrollDevice::GetList()->GetList();    // ���X�g���擾

			// �}�b�v�ړ����u�̃��X�g�̒��g���m�F����
			for (CScrollDevice* pScrollDevice : list)
			{
				// �ʏ��ԂɕύX
				pScrollDevice->SetState(CScrollDevice::STATE_NORMAL);
			}
		}

		break;

	case STATE_SLOPE:

		switch (m_ScrollType)
		{
		case CDevil::SCROLL_TYPE_NORMAL:

			if (m_bSlope)
			{
				Slope(m_DevilArrow);
			}
			else
			{
				BackSlope();
			}
			break;

		case CDevil::SCROLL_TYPE_RETRO:

			if (m_nStateCount % 25 == 0)
			{
				if (m_bSlope)
				{
					Slope(m_DevilArrow);
				}
				else
				{
					BackSlope();
				}
			}
			break;

		default:

			break;
		}

		if (m_nStateCount <= 0)
		{
			m_State = STATE_WAIT;
			m_nStateCount = 120;
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

	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//�L�[�{�[�h�̈ړ�����
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		m_DevilArrow = 0;
		Move(0);

	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		m_DevilArrow = 1;
		Move(1);
	}

	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		m_DevilArrow = 2;
		Move(2);
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		m_DevilArrow = 3;
		Move(3);
	}

	if (pInputKeyboard->GetPress(DIK_5))
	{
		CObjmeshField* pMapField = CGame::GetInstance()->GetMapField();
		D3DXVECTOR3 MapRot = pMapField->GetRot();
		MapRot = INITVECTOR3;
		pMapField->SetRot(MapRot);
	}

	if (pInputKeyboard->GetPress(DIK_6))
	{
		Slope(0);
	}
	if (pInputKeyboard->GetPress(DIK_7))
	{
		Slope(1);
	}
	if (pInputKeyboard->GetPress(DIK_8))
	{
		Slope(2);
	}
	if (pInputKeyboard->GetPress(DIK_9))
	{
		Slope(3);
	}
#endif // !_DEBUG
}

//====================================================================
// ���f���̃��[�V�����ݒ�
//====================================================================
void CDevil::SetAction(ACTION_TYPE Action, float BlendTime)
{
	// ���[�V�����̎擾����
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
void CDevil::SetModelDisp(bool Sst)
{
	// ���f�������擾
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// ���f���̎擾����
		CModel* pModel = GetModel(nCnt);

		if (pModel != nullptr)
		{
			// �\���ݒ�
			pModel->SetDisp(Sst);
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

	// �]�����̃X�N���[��
	RollRockScroll(Move, m_GritSize);

	// �v���C���[�̃X�N���[��
	PlayerScroll(Move, m_GritSize);

	// �t�@�C�A�{�[���̃X�N���[��
	FireScroll(Move, m_GritSize);

	// ���̃X�N���[��
	TileScroll(Move, m_GritSize);

	// �ǂ̃X�N���[��
	WallScroll(Move, m_GritSize);
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
		// �}�b�v�ϐ�
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		
		// �I�u�W�F�ϐ�
		D3DXVECTOR3 pos = pCross->GetPos();
		D3DXVECTOR3 Size = pCross->GetSize();

		// �ړ�	
		pos += Move;

		// x���W����
		if (Move.x > 0.0f)
		{// �ړ��ʂ��v���X
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

		// z���W����
		if (Move.z > 0.0f)
		{// �ړ��ʂ��v���X
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
// �]�����̃X�N���[��
//====================================================================
void CDevil::RollRockScroll(D3DXVECTOR3 Move, float GritSize)
{
	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRollRock::GetList() == nullptr) { return; }
	std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // ���X�g���擾

	// ���[���u���b�N�̃��X�g�̒��g���m�F����
	for (CRollRock* pRailBlock : list)
	{
		D3DXVECTOR3 pos = pRailBlock->GetPos();
		pos += Move;
		pRailBlock->SetPos(pos);
	}
}

//==========================================
//  �t�@�C�A�{�[���̃X�N���[��
//==========================================
void CDevil::FireScroll(D3DXVECTOR3 Move, float GritSize)
{
	// �t�@�C�A�{�[���̃��X�g��������Δ�����
	if (CFire::GetList() == nullptr) { return; }
	std::list<CFire*> list = CFire::GetList()->GetList();    // ���X�g���擾

	// �t�@�C�A�{�[���̃��X�g�̒��g���m�F����
	for (CFire* fire : list)
	{
		D3DXVECTOR3 pos = fire->GetPos();
		pos += Move;
		fire->SetPos(pos);
	}
}

//====================================================================
// ���̃X�N���[��
//====================================================================
void CDevil::TileScroll(D3DXVECTOR3 Move, float GritSize)
{
	// �\���˂̃��X�g�\����������Δ�����
	if (CTile::GetList() == nullptr) { return; }
	std::list<CTile*> list = CTile::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N�̃��X�g�̒��g���m�F����
	for (CTile* pTile : list)
	{
		// �c���̃i���o�[�ƍ�����ݒ肷��
		D3DXVECTOR3 pos = INITVECTOR3;

		//�O���b�g�ԍ����ʒu�ɕϊ�
		pos = pTile->GetGrid().ToWorld();
		pos.y = 0.0f;

		pTile->SetPos(pos);
	}
}

//====================================================================
// �ǂ̃X�N���[��
//====================================================================
void CDevil::WallScroll(D3DXVECTOR3 Move, float GritSize)
{
	// �\���˂̃��X�g�\����������Δ�����
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N�̃��X�g�̒��g���m�F����
	for (CWall* pWall : list)
	{
		// �c���̃i���o�[�ƍ�����ݒ肷��
		D3DXVECTOR3 pos = INITVECTOR3;

		//�O���b�g�ԍ����ʒu�ɕϊ�
		pos = pWall->GetGrid().ToWorld();
		pos.y = 0.0f;

		pWall->SetPos(pos);
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
			float G_Size = CMapSystem::GetInstance()->GetGritSize();

			pos += Move;

			if (Move.x > 0.0f)
			{
				if (pos.x + G_Size > m_DevilPos.x + MapSize.x)	// �E
				{
					pos.x = m_DevilPos.x + MapSize.x - G_Size;
					CollisionPressPlayer(pPlayer, pos, Size);
				}
			}
			if (Move.x < 0.0f)
			{
				if (pos.x - G_Size < m_DevilPos.x - MapSize.x)	// ��
				{
					pos.x = m_DevilPos.x - MapSize.x + G_Size;
					CollisionPressPlayer(pPlayer, pos, Size);
				}
			}
			if (Move.z > 0.0f)
			{
				if (pos.z + G_Size > m_DevilPos.z + MapSize.z)	// ��
				{
					pos.z = m_DevilPos.z + MapSize.z - G_Size;
					CollisionPressPlayer(pPlayer, pos, Size);
				}
			}
			if (Move.z < 0.0f)
			{
				if (pos.z - G_Size < m_DevilPos.z - MapSize.z)	// ��
				{
					pos.z = m_DevilPos.z - MapSize.z + G_Size;
					CollisionPressPlayer(pPlayer, pos, Size);
				}
			}

			pPlayer->SetPos(pos);
		}
		else
		{// ����Ԃ̎��A�u���b�N�����݂��Ȃ��ʒu�Ƀz�[�~���O����

			CMapSystem::GRID Grit = pPlayer->GetGrid();;
			if (!CMapSystem::GetInstance()->GetGritBool(Grit))
			{
				D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
				D3DXVECTOR3 AnswerPos = INITVECTOR3;
				AnswerPos = Grit.ToWorld();

				if (pPlayer->GetGritCenter())
				{
					AnswerPos.y = PlayerPos.y;
					pPlayer->SetPos(AnswerPos);
				}
				else
				{
					PlayerPos.x += (AnswerPos.x - PlayerPos.x);
					PlayerPos.z += (AnswerPos.z - PlayerPos.z);
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

#ifdef _DEBUG

	////�@�O���b�g�̈ʒu�ɃG�t�F�N�g��\��
	//for (int nCntW = 0; nCntW < CMapSystem::GetInstance()->GetWightMax(); nCntW++)
	//{
	//	for (int nCntH = 0; nCntH < CMapSystem::GetInstance()->GetHeightMax(); nCntH++)
	//	{
	//		if (CMapSystem::GetInstance()->GetGritBool(nCntW, nCntH))
	//		{// �u���b�N�����݂���O���b�g�̂݃G�t�F�N�g��\��

	//			CEffect* pEffect = CEffect::Create();
	//			pEffect->SetPos(CMapSystem::GetInstance()->GetGritPos(CMapSystem::GRID(nCntW, nCntH)));
	//			pEffect->SetRadius(20.0f);
	//			pEffect->SetLife(10);
	//		}
	//	}
	//}

#endif // _DEBUG

	CMapSystem::GetInstance()->SetMapPos(MapPos);

	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CCubeBlock::GetList() == nullptr) { return; }
	std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N�̃��X�g�̒��g���m�F����
	for (CCubeBlock* pCubeBlock : list)
	{
		// �c���̃i���o�[�ƍ�����ݒ肷��
		D3DXVECTOR3 pos = INITVECTOR3;
		CMapSystem::GRID grid = CMapSystem::GRID
		(
			pCubeBlock->GetWightNumber(),
			pCubeBlock->GetHeightNumber()
		);

		//�O���b�g�ԍ����ʒu�ɕϊ�
		pos = grid.ToWorld();
		pos.y = 50.0f;

		pCubeBlock->SetPos(pos);
	}
}

//====================================================================
// ��󂪉�]���鏈��
//====================================================================
void CDevil::SignalScroll(void)
{
	switch (m_nStateNum)
	{
	case ACTION_SIGNAL_UP:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI,0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		break;

	case ACTION_SIGNAL_DOWN:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); 
		break;

	case ACTION_SIGNAL_RIGHT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		break;

	case ACTION_SIGNAL_LEFT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f)); 
		break;
	}
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
float CDevil::MoveSlopeX(float Move)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = m_DevilRot;

	if (Move > 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z));
	}
	else if (Move < 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z));
	}

	return fSlopeMove;
}

//====================================================================
//�X�����̈ړ��ʕϓ�
//====================================================================
float CDevil::MoveSlopeZ(float Move)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = m_DevilRot;

	if (Move > 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x));
	}
	else if (Move < 0.0f)
	{
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x));
	}

	return fSlopeMove;
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CDevil>* CDevil::GetList(void)
{
	return m_pList;
}

//====================================================================
//���X�g�̐擪�擾
//====================================================================
CDevil* CDevil::GetListTop(void)
{
	if (CDevil::GetList() == nullptr) { return nullptr; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // ���X�g���擾
	CDevil* pDevil = list.front();

	return pDevil;
}
