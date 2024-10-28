//============================================
//
//	�G�̏��� [enemy.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemy.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "CubeBlock.h"
#include "CubeColl.h"
#include "LevelModelEffect.h"
#include "slowManager.h"
#include "2DEffect.h"
#include "score.h"
#include "modelEffect.h"
#include "Effect.h"
#include "devil.h"
#include "DevilHole.h"
#include "MapSystem.h"
#include "objmeshField.h"
#include "sound.h"

namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(45.0f, 60.0f, 45.0f);		//���̓����蔻��
	const float LIFE = 1.0f;	// �̗�

	const float DISTANCE_RECEDE = 200.0f;	//�߂Â�����
	const float DISTANCE_APPROACH = 100.0f;	//�������鋗��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemy::CEnemy(int nPriority) :CObject(nPriority)
{
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;

	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_nActionCount = 0;
	SetSize(COLLISION_SIZE);
	m_nStateCount = 0;

	m_ColorA = 1.0f;

	for (int nCnt = 0; nCnt < MODEL_NUM; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
		m_aModelName[nCnt] = "";
	}

	m_size = COLLISION_SIZE;
	m_pMotion = nullptr;
	m_nNumModel = 0;
	m_EnemyType = ENEMY_MEDAMAN;
	m_State = E_STATE_WAIT;
	m_pSlow = nullptr;
	m_SelectMove = SELECT_MOVE_MAX;

	m_OKL = true;
	m_OKR = true;
	m_OKU = true;
	m_OKD = true;

	m_nMapWight = 0;
	m_nMapHeight = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemy::~CEnemy()
{

}

//====================================================================
//��������
//====================================================================
CEnemy* CEnemy::Create(char* aModelName)
{
	// ����
	CEnemy* pInstance = new CEnemy();

	//����������
	if (FAILED(pInstance->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}
	
	// ���f���֘A����������
	pInstance->InitModel(aModelName);

	return pInstance;
}

//====================================================================
//����������
//====================================================================
HRESULT CEnemy::Init(void)
{
	//��ސݒ�
	SetType(CObject::TYPE_ENEMY3D);

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetMultiMatrix(true);

	// �X���[�̐���(�z���A�^�O�̐ݒ�)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CEnemy::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CEnemy::Uninit(void)
{
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
void CEnemy::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CEnemy::TitleUpdate(void)
{
	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CEnemy::GameUpdate(void)
{
	// �ߋ��̈ʒu���L�^
	m_posOld = m_pos;

	//�ǂ̍��G����
	SearchWall();

	// ��Ԃ̍X�V
	StateManager();

	// �ړ���������
	Rot();

	// �ʒu�X�V����
	UpdatePos();

	// �����̔ԍ���ݒ�
	MapSystemNumber();

	//���̔���
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	if (m_pMotion != nullptr)
	{
		//���[�V�����̍X�V
		m_pMotion->Update();
	}

	DebugProc::Print(DebugProc::POINT_LEFT, "[�G]�� %d : �c %d\n", m_nMapWight, m_nMapHeight);
}

//====================================================================
//�`�揈��
//====================================================================
void CEnemy::Draw(void)
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

	if (m_bMultiMatrix)
	{
		SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

		//�Z�o�����}�g���N�X���������킹��
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_UseMultiMatrix);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���̕`��(�S�p�[�c)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//====================================================================
// �q�b�g����
//====================================================================
bool CEnemy::Hit(int nLife)
{
	m_nLife -= nLife;

	if (m_nLife < 0)
	{// �̗�0�ȉ�
		Uninit();
	}

	return true;
}

//====================================================================
// ���f���֘A�̏���������
//====================================================================
void CEnemy::InitModel(const char* pFilename)
{
	strcpy(&m_cFileName[0], pFilename);

	//���f���̐���
	LoadLevelData(pFilename);

	//���[�V�����̐���
	if (m_pMotion == nullptr)
	{
		//���[�V�����̐���
		m_pMotion = new CMotion;
	}

	//����������
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData(pFilename);
}

//====================================================================
// �ʒu�X�V����
//====================================================================
void CEnemy::UpdatePos(void)
{
	//�d��
	m_move.y -= 0.5f;

	// �ϐ��錾
	float fSpeed = 1.0f;	// �X���[�p default1.0f�ŏ�����
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();

		if (m_pMotion)
		{
			m_pMotion->SetSlowVaule(fSpeed);
		}
	}

	//Y���̈ʒu�X�V
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X���̈ʒu�X�V
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Z);
	CollisionDevilHole(useful::COLLISION_Z);

	//�X�e�[�W�O�Ƃ̓����蔻��
	CollisionOut();
}

//====================================================================
//�ړ���������
//====================================================================
void CEnemy::Rot(void)
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

	m_rot.y = atan2f(m_move.z, -m_move.x);

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CEnemy::CollisionWall(useful::COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_CUBEBLOCK)
			{//��ނ��u���b�N�̎�

				CCubeBlock* pBlock = (CCubeBlock*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pBlock->GetPos();
				D3DXVECTOR3 posOld = pBlock->GetPosOld();
				D3DXVECTOR3 Move = pBlock->GetMove();
				D3DXVECTOR3 Size = pBlock->GetSize();
				bool bNullJump;

				// ��`�̓����蔻��
				if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
				{
					m_State = E_STATE_WAIT;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// �}�b�v���f���̓����蔻��
//====================================================================
void CEnemy::CollisionDevilHole(useful::COLLISION XYZ)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_DEVILHOLE)
			{//��ނ��f�r���z�[���̎�

				CDevilHole* pDevilHole = (CDevilHole*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 pos = pDevilHole->GetPos();
				D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
				D3DXVECTOR3 Size = pDevilHole->GetSize();
				bool bNullJump;

				// ��`�̓����蔻��
				if (useful::CollisionBlock(pos, posOld, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
				{
					//�ҋ@��Ԃɂ���
					m_State = E_STATE_WAIT;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CEnemy::CollisionOut()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_DEVIL)
			{//��ނ��u���b�N�̎�

				CDevil* pDevil = (CDevil*)pObj;	// �u���b�N���̎擾

				D3DXVECTOR3 Pos = pDevil->GetDevilPos();
				D3DXVECTOR3 Size = pDevil->GetDevilSize();

				// �X�e�[�W�O�̓����蔻��
				if (Pos.x + Size.x < m_pos.x)
				{
					m_pos.x = -Size.x + Pos.x;
				}
				if (Pos.x - Size.x > m_pos.x)
				{
					m_pos.x = Size.x + Pos.x;
				}
				if (Pos.z + Size.z < m_pos.z)
				{
					m_pos.z = -Size.z + Pos.z;
				}
				if (Pos.z - Size.z > m_pos.z)
				{
					m_pos.z = Size.z + Pos.z;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// ���S����
//====================================================================
void CEnemy::Death(void)
{
	if (m_pMotion->GetFinish() == true)
	{
		m_ColorA -= 0.005f;

		if (m_ColorA <= 0.0f)
		{
			Uninit();
		}
	}
}

//====================================================================
// ��ԍX�V
//====================================================================
void CEnemy::StateManager()
{
	int nRand = 0;

	//��Ԃ̊Ǘ�
	switch (m_State)
	{
	case CEnemy::E_STATE_WAIT:

		MoveSelect();
		m_State = E_STATE_WALK;

		break;

	case CEnemy::E_STATE_TRUN:

		m_move.x = m_move.x * -1.0f;
		m_move.z = m_move.z * -1.0f;

		break;

	case CEnemy::E_STATE_WALK:
		break;

	case CEnemy::E_STATE_EGG:
		break;

	case CEnemy::E_STATE_DEATH:

		m_move.x = 0.0f;
		m_move.z = 0.0f;

		break;

	default:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// �ړ������̑I��
//====================================================================
void CEnemy::MoveSelect()
{
	float OKRot[4];
	int RotNumber = 0;

	if (m_OKL && m_SelectMove != SELECT_MOVE_RIGHT)
	{
		OKRot[RotNumber] = D3DX_PI * -0.5f;
		RotNumber++;
	}
	if (m_OKR && m_SelectMove != SELECT_MOVE_LEFT)
	{
		OKRot[RotNumber] = D3DX_PI * 0.5f;
		RotNumber++;
	}
	if (m_OKU && m_SelectMove != SELECT_MOVE_DOWN)
	{
		OKRot[RotNumber] = D3DX_PI * 0.0f;
		RotNumber++;
	}
	if (m_OKD && m_SelectMove != SELECT_MOVE_UP)
	{
		OKRot[RotNumber] = D3DX_PI * 1.0f;
		RotNumber++;
	}

	if (RotNumber != 0)
	{
		int nRand = rand() % RotNumber;

		m_move.x = sinf(OKRot[nRand]) * 3.0f;
		m_move.z = cosf(OKRot[nRand]) * 3.0f;

		if (m_move.x >= 3.0f)
		{
			m_SelectMove = SELECT_MOVE_RIGHT;
		}
		else if (m_move.x <= -3.0f)
		{
			m_SelectMove = SELECT_MOVE_LEFT;
		}
		else if (m_move.z >= 3.0f)
		{
			m_SelectMove = SELECT_MOVE_UP;
		}
		else if (m_move.z <= -3.0f)
		{
			m_SelectMove = SELECT_MOVE_DOWN;
		}
	}
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CEnemy::SearchWall(void)
{
	bool OKR = true;	//�E
	bool OKL = true;	//��
	bool OKU = true;	//��
	bool OKD = true;	//��

	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	int nMapWightMax = pMapSystem->GetWightMax();
	int nMapHeightMax = pMapSystem->GetHeightMax();
	D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

	int nRNumber = m_nMapWight + 1;
	int nLNumber = m_nMapWight - 1;
	int nUNumber = m_nMapHeight - 1;
	int nDNumber = m_nMapHeight + 1;

	nRNumber = useful::RangeNumber(nMapWightMax, 0, nRNumber);
	nLNumber = useful::RangeNumber(nMapWightMax, 0, nLNumber);
	nUNumber = useful::RangeNumber(nMapHeightMax, 0, nUNumber);
	nDNumber = useful::RangeNumber(nMapHeightMax, 0, nDNumber);

	OKR = !pMapSystem->GetGritBool(nRNumber, m_nMapHeight);
	OKL = !pMapSystem->GetGritBool(nLNumber, m_nMapHeight);
	OKU = !pMapSystem->GetGritBool(m_nMapWight, nUNumber);
	OKD = !pMapSystem->GetGritBool(m_nMapWight, nDNumber);

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = CMapSystem::GetInstance()->GetGritPos(m_nMapWight, m_nMapHeight);;
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "�������� %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - m_size.x) &&
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - m_size.x) &&
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - m_size.z) &&
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - m_size.z))
	{// �O���b�g�̒��S�ʒu�ɗ����Ă���Ȃ瑀����󂯕t����

		if (!m_OKR && OKR)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKL && OKL)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKU && OKU)
		{
			m_State = E_STATE_WAIT;
		}
		if (!m_OKD && OKD)
		{
			m_State = E_STATE_WAIT;
		}

		m_OKR = OKR;	//�E
		m_OKL = OKL;	//��
		m_OKU = OKU;	//��
		m_OKD = OKD;	//��
	}
	else
	{
		m_OKR = false;	//�E
		m_OKL = false;	//��
		m_OKU = false;	//��
		m_OKD = false;	//��
	}

	//bool OKR = true;	//�E
	//bool OKL = true;	//��
	//bool OKU = true;	//��
	//bool OKD = true;	//��

	//for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	//{
	//	//�I�u�W�F�N�g���擾
	//	CObject* pObj = CObject::GetTop(nCntPriority);

	//	while (pObj != nullptr)
	//	{
	//		CObject* pObjNext = pObj->GetNext();

	//		CObject::TYPE type = pObj->GetType();			//��ނ��擾

	//		if (type == TYPE_CUBEBLOCK)
	//		{//��ނ��u���b�N�̎�

	//			CCubeBlock* pBlock = (CCubeBlock*)pObj;	// �u���b�N���̎擾

	//			D3DXVECTOR3 pos = pBlock->GetPos();
	//			D3DXVECTOR3 posOld = pBlock->GetPosOld();
	//			D3DXVECTOR3 Move = pBlock->GetMove();
	//			D3DXVECTOR3 Size = pBlock->GetSize();

	//			D3DXVECTOR3 MyPos = INITVECTOR3;

	//			for (int nCnt = 0; nCnt < 4; nCnt++)
	//			{
	//				switch (nCnt)
	//				{
	//				case 0:
	//					MyPos = D3DXVECTOR3(m_pos.x + Size.x * 2.0f, m_pos.y, m_pos.z);	//�E
	//					break;
	//				case 1:
	//					MyPos = D3DXVECTOR3(m_pos.x - Size.x * 2.0f, m_pos.y, m_pos.z);	//��
	//					break;
	//				case 2:
	//					MyPos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + Size.z * 2.0f);	//��
	//					break;
	//				case 3:
	//					MyPos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - Size.z * 2.0f);	//��
	//					break;
	//				}

	//				// ��`�̓����蔻��
	//				if (useful::CollisionRectangle2D(MyPos, pos, COLLISION_SIZE, Size, useful::COLLISION::COLLISION_ZX) == true)
	//				{
	//					switch (nCnt)
	//					{
	//					case 0:
	//						OKR = false;
	//						break;
	//					case 1:
	//						OKL = false;
	//						break;
	//					case 2:
	//						OKU = false;
	//						break;
	//					case 3:
	//						OKD = false;
	//						break;
	//					}

	//					CEffect* pEffect = CEffect::Create();
	//					pEffect->SetPos(MyPos);
	//				}
	//			}
	//		}

	//		if (type == TYPE_DEVILHOLE)
	//		{//��ނ��f�r���z�[���̎�

	//			CDevilHole* pDevilHole = (CDevilHole*)pObj;	// �u���b�N���̎擾

	//			D3DXVECTOR3 pos = pDevilHole->GetPos();
	//			D3DXVECTOR3 Size = pDevilHole->GetSize();

	//			D3DXVECTOR3 MyPos = INITVECTOR3;

	//			for (int nCnt = 0; nCnt < 4; nCnt++)
	//			{
	//				switch (nCnt)
	//				{
	//				case 0:
	//					MyPos = D3DXVECTOR3(m_pos.x + Size.x * 2.0f, m_pos.y, m_pos.z);	//�E
	//					break;
	//				case 1:
	//					MyPos = D3DXVECTOR3(m_pos.x - Size.x * 2.0f, m_pos.y, m_pos.z);	//��
	//					break;
	//				case 2:
	//					MyPos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + Size.z * 2.0f);	//��
	//					break;
	//				case 3:
	//					MyPos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - Size.z * 2.0f);	//��
	//					break;
	//				}

	//				// ��`�̓����蔻��
	//				if (useful::CollisionRectangle2D(MyPos, pos, COLLISION_SIZE, Size, useful::COLLISION::COLLISION_ZX) == true)
	//				{
	//					switch (nCnt)
	//					{
	//					case 0:
	//						OKR = false;
	//						break;
	//					case 1:
	//						OKL = false;
	//						break;
	//					case 2:
	//						OKU = false;
	//						break;
	//					case 3:
	//						OKD = false;
	//						break;
	//					}

	//					CEffect* pEffect = CEffect::Create();
	//					pEffect->SetPos(MyPos);
	//				}
	//			}
	//		}

	//		pObj = pObjNext;
	//	}
	//}

	//if (!m_OKR && OKR)
	//{
	//	m_State = E_STATE_WAIT;
	//}
	//if (!m_OKL && OKL)
	//{
	//	m_State = E_STATE_WAIT;
	//}
	//if (!m_OKU && OKU)
	//{
	//	m_State = E_STATE_WAIT;
	//}
	//if (!m_OKD && OKD)
	//{
	//	m_State = E_STATE_WAIT;
	//}

	//m_OKR = OKR;	//�E
	//m_OKL = OKL;	//��
	//m_OKU = OKU;	//��
	//m_OKD = OKD;	//��
}

//====================================================================
// �}�b�v�̂ǂ̃}�X�ɑ��݂��Ă��邩�ݒ肷��
//====================================================================
void CEnemy::MapSystemNumber(void)
{
	m_nMapWight = CMapSystem::GetInstance()->GetGritWightNumber(m_pos.x);
	m_nMapHeight = CMapSystem::GetInstance()->GetGritHeightNumber(m_pos.z);
}

//====================================================================
//���[�h����
//====================================================================
void CEnemy::LoadLevelData(const char* pFilename)
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
						m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_FALSE);
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
								fscanf(pFile, "%f", &ModelRot.x);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.y);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.z);				////�����̏����ݒ�

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

