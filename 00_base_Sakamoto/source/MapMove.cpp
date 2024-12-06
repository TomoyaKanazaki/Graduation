//============================================
//
//	�}�b�v�̓��� [MapMove.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MapMove.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"
#include "objmeshField.h"
#include "enemy.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
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
	float SCROOL_SPEED_02 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_02) / SCROOL_COUNT_02;			// �X�N���[���̈ړ����x

	int SLOPE_TIME = 300;						// �X�����쎞��
	int SLOPE_RAND = 75;						// �X�������m��
	float STAGE_ROT_LIMIT = D3DX_PI * 0.15f;	// �X���̊p�x����

	float SLOPE_SPEED01 = 0.00075f;				// �X���̈ړ����x

	float SLOPE_SPEED02 = 0.0125f;				// �X���̈ړ����x
}

//�ÓI�����o�ϐ��錾

//====================================================================
//�R���X�g���N�^
//====================================================================
CMapMove::CMapMove()
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMapMove::~CMapMove()
{

}

//====================================================================
//��������
//====================================================================
CMapMove* CMapMove::Create()
{
	CMapMove* pPause = nullptr;

	if (pPause == nullptr)
	{
		//�G�̐���
		pPause = new CMapMove();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pPause->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pPause;
}

//====================================================================
//����������
//====================================================================
HRESULT CMapMove::Init(void)
{

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CMapMove::Uninit(void)
{

}

//====================================================================
//�X�V����
//====================================================================
void CMapMove::Update(void)
{
#ifdef _DEBUG
	//m_ScrollType = SCROLL_TYPE_MAX;
#endif // _DEBUG

	// �}�b�v�̌X��
	m_DevilRot = CObjmeshField::GetListTop()->GetRot();

	// ���̉�]
	SignalManager();

	//��Ԃ̊Ǘ�
	StateManager();

	//�f�o�b�O�L�[�̏����Ɛݒ�
	DebugKey();

	//�X�e�[�W�O�ɂ���I�u�W�F�N�g�̏���
	CollisionOut();

	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
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
}

//====================================================================
//�`�揈��
//====================================================================
void CMapMove::Draw(void)
{

}

//====================================================================
//�ړ�����
//====================================================================
void CMapMove::Move(int Arroow)
{
	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:

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

	case CMapMove::SCROLL_TYPE_RETRO:

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
void CMapMove::BackSlope(void)
{
	CObjmeshField* pMapField = CObjmeshField::GetListTop();
	D3DXVECTOR3 MapRot = pMapField->GetRot();
	bool bBackOK = false;

	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x <= 0.0f)
			{
				MapRot.x = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x >= 0.0f)
			{
				MapRot.x = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z <= 0.0f)
			{
				MapRot.z = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z >= 0.0f)
			{
				MapRot.z = 0.0f;
				bBackOK = true;
			}
		}

		break;

	case CMapMove::SCROLL_TYPE_RETRO:

		if (MapRot.x > 0.0f)
		{
			MapRot.x -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x <= 0.0f)
			{
				MapRot.x = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.x < 0.0f)
		{
			MapRot.x += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.x >= 0.0f)
			{
				MapRot.x = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.z > 0.0f)
		{
			MapRot.z -= D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z <= 0.0f)
			{
				MapRot.z = 0.0f;
				bBackOK = true;
			}
		}

		if (MapRot.z < 0.0f)
		{
			MapRot.z += D3DX_PI * SLOPE_SPEED02;

			if (MapRot.z >= 0.0f)
			{
				MapRot.z = 0.0f;
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
		MOVE_WAIT;
		m_nStateCount = 60;
		MapRot.x = 0.0f;
		MapRot.z = 0.0f;
	}

	pMapField->SetRot(MapRot);
}

//====================================================================
//�ړ���������
//====================================================================
void CMapMove::Slope(int Arroow)
{
	CObjmeshField* pMapField = CObjmeshField::GetListTop();
	if (pMapField == nullptr) { assert(false); }

	D3DXVECTOR3 MapRot = pMapField->GetRot();

	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:

		switch (Arroow)
		{
		case 0:

			MapRot.x += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x > STAGE_ROT_LIMIT)
			{
				MapRot.x = STAGE_ROT_LIMIT;
			}
			// �E��
			m_SlopeType = 0;

			break;
		case 1:

			MapRot.x -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.x < -STAGE_ROT_LIMIT)
			{
				MapRot.x = -STAGE_ROT_LIMIT;
			}
			// ����
			m_SlopeType = 1;

			break;
		case 2:

			MapRot.z += D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z > STAGE_ROT_LIMIT)
			{
				MapRot.z = STAGE_ROT_LIMIT;
			}
			// ����
			m_SlopeType = 2;

			break;
		case 3:

			MapRot.z -= D3DX_PI * SLOPE_SPEED01;

			if (MapRot.z < -STAGE_ROT_LIMIT)
			{
				MapRot.z = -STAGE_ROT_LIMIT;
			}
			// ��O��
			m_SlopeType = 3;

			break;
		}

		break;

	case CMapMove::SCROLL_TYPE_RETRO:

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
void CMapMove::CollisionOut()
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

		m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//��
		m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//��
		m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//�E
		m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//��

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
//��ԊǗ�
//====================================================================
void CMapMove::StateManager(void)
{
	switch (m_State)
	{
	case MOVE_WAIT:

		// ��ԏI����
		if (m_nStateCount <= 0)
		{
			// 100����rand()���܂킷
			int nRand = rand() % 101;

			// �X���̎w�聓�̎�
			if (nRand <= SLOPE_RAND)
			{
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

					// ��Ԑݒ�
					switch (m_DevilArrow)
					{
					case 0:
						m_State = MOVE_SLOPE_UP;
						break;
					case 1:	  
						m_State = MOVE_SLOPE_DOWN;
						break;
					case 2:	   
						m_State = MOVE_SLOPE_RIGHT;
						break;
					case 3:	   
						m_State = MOVE_SLOPE_LEFT;
						break;
					}

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
						// �����̌X�����u�����~��ԂɕύX
						pSlopeDevice->SetState(CSlopeDevice::STATE_DESCENT, (CScrollArrow::Arrow)m_SlopwArrowOld);
					}
				}
			}
			else
			{// �X���̎w�聓����Ȃ���

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

				// ��Ԑݒ�
				switch (m_DevilArrow)
				{
				case 0:
					m_State = MOVE_SCROLL_UP;
					break;
				case 1:
					m_State = MOVE_SCROLL_DOWN;
					break;
				case 2:
					m_State = MOVE_SCROLL_RIGHT;
					break;
				case 3:
					m_State = MOVE_SCROLL_LEFT;
					break;
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
		}

		break;

	case MOVE_SCROLL_UP:
	case MOVE_SCROLL_DOWN:
	case MOVE_SCROLL_RIGHT:
	case MOVE_SCROLL_LEFT:

		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:
			Move(m_DevilArrow);
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

			if (m_nStateCount % (SCROOL_TIME / SCROOL_COUNT_02) == 0)
			{
				Move(m_DevilArrow);
			}

			break;

		default:

			break;
		}

		if (m_nStateCount <= 1)
		{
			m_State = MOVE_WAIT;
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

	case MOVE_SLOPE_UP:
	case MOVE_SLOPE_DOWN:
	case MOVE_SLOPE_RIGHT:
	case MOVE_SLOPE_LEFT:

		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:

			if (m_bSlope)
			{
				Slope(m_DevilArrow);

				// ���X��
				SignalManager();
			}
			else
			{
				BackSlope();
			}
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

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
			m_State = MOVE_WAIT;
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
void CMapMove::DebugKey(void)
{
#ifdef _DEBUG

	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

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
		CObjmeshField* pMapField = CObjmeshField::GetListTop();
		D3DXVECTOR3 MapRot = pMapField->GetRot();
		MapRot = INITVECTOR3;
		pMapField->SetRot(MapRot);
	}

	if (pInputKeyboard->GetPress(DIK_6))
	{
		m_pSignal[0]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));

		Slope(0);
	}
	if (pInputKeyboard->GetPress(DIK_7))
	{
		m_pSignal[0]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));

		Slope(1);
	}
	if (pInputKeyboard->GetPress(DIK_8))
	{
		m_pSignal[0]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));

		Slope(2);
	}
	if (pInputKeyboard->GetPress(DIK_9))
	{
		m_pSignal[0]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));


		Slope(3);
	}
#endif // !_DEBUG
}

//====================================================================
// �I�u�W�F�N�g�̃X�N���[��
//====================================================================
void CMapMove::ObjectScroll(D3DXVECTOR3 Move)
{
	// �O���b�h�̃X�N���[��
	GritScroll(Move);

	// �O���b�h�̃T�C�Y
	float m_GritSize = CMapSystem::GetInstance()->GetGritSize();
}

//====================================================================
// �O���b�g�̃X�N���[��
//====================================================================
void CMapMove::GritScroll(D3DXVECTOR3 Move)
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
}

//====================================================================
// ��󂪃}�l�[�W���[
//====================================================================
void CMapMove::SignalManager(void)
{
	switch (m_State)
	{
	case MOVE_SCROLL_UP:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		break;

	case MOVE_SCROLL_DOWN:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;

	case MOVE_SCROLL_RIGHT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		break;

	case MOVE_SCROLL_LEFT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		break;

	case MOVE_SLOPE_UP:
		m_pSignal[0]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		break;

	case MOVE_SLOPE_DOWN:
		m_pSignal[0]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(1.75f, 0.0f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(-1.75f, 0.0f, 0.0f));
		break;

	case MOVE_SLOPE_LEFT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));

		break;

	case MOVE_SLOPE_RIGHT:
		m_pSignal[0]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[1]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[2]->SetRot(D3DXVECTOR3(1.75f, D3DX_PI * 0.5f, 0.0f));
		m_pSignal[3]->SetRot(D3DXVECTOR3(-1.75f, D3DX_PI * 0.5f, 0.0f));
		break;
	}
}

//====================================================================
// �v���C���[���ׂ���鎞�̏���
//====================================================================
void CMapMove::CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size)
{
	//// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	//if (CCubeBlock::GetList() == nullptr) { return; }
	//std::list<CCubeBlock*> list = CCubeBlock::GetList()->GetList();    // ���X�g���擾

	//// �L���[�u�u���b�N�̃��X�g�̒��g���m�F����
	//for (CCubeBlock* pCubeBlock : list)
	//{
	//	D3DXVECTOR3 Blockpos = pCubeBlock->GetPos();
	//	D3DXVECTOR3 BlockSize = pCubeBlock->GetSize();

	//	if (useful::CollisionRectangle2D(pos, Blockpos, Size, BlockSize, useful::COLLISION::COLLISION_ZX))
	//	{
	//		pPlayer->Death();
	//		return;
	//	}
	//}
}

//====================================================================
//�X�����̈ړ��ʕϓ�
//====================================================================
float CMapMove::MoveSlopeX(float Move)
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