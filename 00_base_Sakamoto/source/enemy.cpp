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
#include "objectcharacter.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "CubeBlock.h"
#include "slowManager.h"
#include "score.h"
#include "devil.h"
#include "DevilHole.h"
#include "objmeshField.h"
#include "sound.h"
#include "wall.h"
#include "move.h"

#include "enemyBonbon.h"
#include "enemyMedaman.h"
#include "enemyYoungDevil.h"
#include "friedegg.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(35.0f, 60.0f, 35.0f);		//���̓����蔻��
	const float LIFE = 1.0f;	// �̗�
	const float GRIT_OK = 45.0f;			//�ړ��\�ȃO���b�g�͈͓̔�

	const float DISTANCE_RECEDE = 200.0f;	//�߂Â�����
	const float DISTANCE_APPROACH = 100.0f;	//�������鋗��

	const float COORDDINATE_RATE[] = // �o�H�T�����s���Ԋu
	{
		5.0f,
		3.0f,
		1.0f
	};
	const float TARGET_DIFF = 10.0f; // ���e�͈�
	const float MOVE_ASTAR = 150.0f; // �ǐՎ��̈ړ����x

	const CMyEffekseer::TYPE EFFECT_TYPE[] = // �o�H�T�����s���Ԋu
	{
		CMyEffekseer::TYPE_RESPAWN_MEDAMAN,
		CMyEffekseer::TYPE_RESPAWN_BONBON,
		CMyEffekseer::TYPE_RESPAWN_BONBON // TODO : �q�f�r��
	};
}

//==========================================
//  �ÓI�x������
//==========================================
static_assert(NUM_ARRAY(COORDDINATE_RATE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");
static_assert(NUM_ARRAY(EFFECT_TYPE) == CEnemy::ENEMY_MAX, "ERROR : Type Count Missmatch");

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CEnemy>* CEnemy::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemy::CEnemy(int nPriority) :CObjectCharacter(nPriority),
m_pPath(nullptr),
m_fCoordinateTimer(0.0f),
m_nTargetIndex(0),
m_nNumCoordinate(0),
m_pEffect(nullptr)
{
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_nActionCount = 0;
	SetSize(COLLISION_SIZE);
	m_nStateCount = 0;

	m_ColorA = 1.0f;

	m_EnemyType = ENEMY_MEDAMAN;
	m_State = E_STATE_WAIT;
	m_pSlow = nullptr;
	m_SelectMove = SELECT_MOVE_MAX;

	m_OKL = true;
	m_OKR = true;
	m_OKU = true;
	m_OKD = true;

	m_Grid.x = 0;
	m_Grid.z = 0;

	m_SelectGrid.x = 0;
	m_SelectGrid.z = 0;

	m_nBugCounter = 0;

	m_pMoveState = nullptr;
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
CEnemy* CEnemy::Create(const ENEMY_TYPE eType, const CMapSystem::GRID& grid)
{
	// �|�C���^�錾
	CEnemy* pEnemy = nullptr;

	// �^�C�v���Ƃ̃C���X�^���X����
	switch (eType)
	{
	case ENEMY_MEDAMAN: // �߂��܂�
		pEnemy = new CEnemyMedaman;
		break;

	case ENEMY_BONBON: // �ڂ�ڂ�
		pEnemy = new CEnemyBonbon;
		break;

	case ENEMY_LITTLEDEVIL: // ���łт�
		pEnemy = new CEnemyYoungDevil;
		break;

	default:
		assert(false);
		break;
	}

	//����������
	if (FAILED(pEnemy->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	// ���W��ݒ�
	pEnemy->SetGrid(grid);
	pEnemy->SetPos(CMapSystem::GetInstance()->GetGritPos(grid));

	// �G�̃^�C�v��ݒ�
	pEnemy->m_EnemyType = eType;

	// �G�t�F�N�g�𐶐�
	pEnemy->Effect();

	return pEnemy;
}

//====================================================================
//����������
//====================================================================
HRESULT CEnemy::Init(void)
{
	//��ސݒ�
	SetType(CObject::TYPE_ENEMY3D);

	// �L�����N�^�[�N���X�̏������i�p���j
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// �}�g���b�N�X�ݒ�
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());	// �}�b�v�}�g���b�N�X�Ɗ|�����킹
	SetUseStencil(true);	// �X�e���V��

	// �X���[�̐���(�z���A�^�O�̐ݒ�)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

	// �ړ���Ԑݒ�
	//if (m_pMoveState == nullptr)
	//{ // �ړ���Ԑݒ�
	//	m_pMoveState = new CStateStop();		// ��~���
	//	m_pMoveState->RandomStop(this);			// �����_�����s���
	//}

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEnemy>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CEnemy::Uninit(void)
{
	// ���������폜
	if (m_pPath != nullptr) { delete[] m_pPath; };

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �G�t�F�N�g������
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}

	// �ړ���Ԃ̔j��
	/*if (m_pMoveState != nullptr)
	{
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}*/

	// �L�����N�^�[�N���X�̏I���i�p���j
	CObjectCharacter::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CEnemy::Update(void)
{
	// �l���擾
	D3DXVECTOR3 posMy = GetPos();			// �ʒu
	D3DXVECTOR3 posOldMy = GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 rotMy = GetRot();			// ����
	D3DXVECTOR3 sizeMy = GetSize();			// �傫��

	// �ߋ��̈ʒu���L�^
	posOldMy = posMy;

	////�ǂ̍��G����
	//SearchWall(posMy);

	//// �����_���ړ�
	//MoveSelect();

	// ��Ԃ̍X�V
	StateManager(posMy);

	// �ړ���������
	Rot(rotMy);

	// �ʒu�X�V����
	UpdatePos(posMy,posOldMy,sizeMy);

	// �ǐՏ�Ԃɂ���
	//m_pMoveState->RandomAStar(this);

	// �v���C���[�ւ̍ŒZ�o�H�T��
	Coordinate();

	//// �ŒZ�o�H�����ǂ�
	Route();

	// �����̔ԍ���ݒ�
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(posMy);

	//���̔���
	if (posMy.y <= 0.0f)
	{
		posMy.y = 0.0f;
		m_move.y = 0.0f;
	}

	// �G�t�F�N�g�𓮂���
	if (m_pEffect != nullptr)
	{
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 pos = posMy;
		pos.y += 0.5f;
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rotMy, *GetUseMultiMatrix());
		m_pEffect->SetPosition(ef);
	}

	// �L�����N�^�[�N���X�̍X�V�i�p���j
	CObjectCharacter::Update();

	// �f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "[�G]�� %d : �c %d\n", m_Grid.x, m_Grid.z);

	// �l�X�V
	SetPos(posMy);			// �ʒu
	SetPosOld(posOldMy);	// �O��̈ʒu
	SetRot(rotMy);			// ����
	SetSize(sizeMy);		// �傫��
}

//====================================================================
//�`�揈��
//====================================================================
void CEnemy::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();
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

		// �ڋʏĂ��𐶐�
		CFriedEgg::Create(m_EnemyType, m_Grid);
	}

	return true;
}

//====================================================================
// ���f���֘A�̏���������
//====================================================================
HRESULT CEnemy::InitModel(const char* pFilename)
{
	// �L�����N�^�[�e�L�X�g�ǂݍ��ݏ���
	CObjectCharacter::SetTxtCharacter(pFilename);
	
	return S_OK;
}

//====================================================================
// �ʒu�X�V����
//====================================================================
void CEnemy::UpdatePos(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy)
{
	// ���[�V�����̎擾
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	//�d��
	m_move.y -= 0.5f;

	// �ϐ��錾
	float fSpeed = 1.0f;	// �X���[�p default1.0f�ŏ�����
	if (m_pSlow)
	{
		fSpeed = m_pSlow->GetValue();

		if (pMotion)
		{
			pMotion->SetSlowVaule(fSpeed);
		}
	}

	CDevil* pDevil = CDevil::GetListTop();

	//Y���̈ʒu�X�V
	posMy.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	posMy.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	//CollisionWall(posMy,posOldMy,sizeMy,useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X���̈ʒu�X�V
	posMy.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);
	posMy.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);

	// �ǂƂ̓����蔻��
	//CollisionWall(posMy, posOldMy, sizeMy, useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	posMy.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);
	posMy.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);

	// �ǂƂ̓����蔻��
	//CollisionWall(posMy, posOldMy, sizeMy, useful::COLLISION_Z);
	CollisionDevilHole(useful::COLLISION_Z);

	//�X�e�[�W�O�Ƃ̓����蔻��
	CollisionOut(posMy);
}

//====================================================================
//�ړ���������
//====================================================================
void CEnemy::Rot(D3DXVECTOR3& rotMy)
{
	//�L�[�{�[�h�̎擾
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//�ړ������Ɍ��������킹�鏈��
	float fRotMove, fRotDest;
	fRotMove = rotMy.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	rotMy.y = atan2f(-m_move.x, -m_move.z);

	useful::NormalizeAngle(&rotMy);
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CEnemy::CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& size,useful::COLLISION XYZ)
{
	// �ǂ̃��X�g�\����������Δ�����
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // ���X�g���擾

	// �ǃ��X�g�̒��g���m�F����
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 posOld = pWall->GetPosOld();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();
		bool bNullJump;

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, posOld, Move, Size, &posMy, posOldMy, &m_move, &m_Objmove, size, &bNullJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			m_State = E_STATE_WAIT;

			posMy = m_Grid.ToWorld();
		}
	}
}

//====================================================================
// �}�b�v���f���̓����蔻��
//====================================================================
void CEnemy::CollisionDevilHole(useful::COLLISION XYZ)
{
	//// �f�r���z�[���̃��X�g�\����������Δ�����
	//if (CDevilHole::GetList() == nullptr) { return; }
	//std::list<CDevilHole*> list = CDevilHole::GetList()->GetList();    // ���X�g���擾

	//// �f�r���z�[�����X�g�̒��g���m�F����
	//for (CDevilHole* pDevilHole : list)
	//{
	//	D3DXVECTOR3 pos = pDevilHole->GetPos();
	//	D3DXVECTOR3 posOld = pDevilHole->GetPosOld();
	//	D3DXVECTOR3 Size = pDevilHole->GetSize();
	//	bool bNullJump;

	//	// ��`�̓����蔻��
	//	if (useful::CollisionBlock(pos, posOld, INITVECTOR3, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
	//	{
	//		//�ҋ@��Ԃɂ���
	//		m_State = E_STATE_WAIT;
	//	}
	//}
}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CEnemy::CollisionOut(D3DXVECTOR3& posMy)
{
	// //�L���[�u�u���b�N�̃��X�g�\����������Δ�����
	//if (CDevil::GetList() == nullptr) { return; }
	//std::list<CDevil*> list = CDevil::GetList()->GetList();    // ���X�g���擾

	// //�L���[�u�u���b�N���X�g�̒��g���m�F����
	//for (CDevil* pDevil : list)
	//{
	//	D3DXVECTOR3 Pos = pDevil->GetDevilPos();
	//	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	//	float GritSize = CMapSystem::GetInstance()->GetGritSize();

	//	// �X�e�[�W�O�̓����蔻��
	//	if (Pos.x + MapSize.x < posMy.x) // �E
	//	{
	//		posMy.x = Pos.x -MapSize.x - GritSize;
	//	}
	//	if (Pos.x - MapSize.x - GritSize > posMy.x) // ��
	//	{
	//		posMy.x = Pos.x + MapSize.x;
	//	}
	//	if (Pos.z + MapSize.z + GritSize < posMy.z) // ��
	//	{
	//		posMy.z = Pos.z - MapSize.z;
	//	}
	//	if (Pos.z - MapSize.z > posMy.z) // ��
	//	{
	//		posMy.z = Pos.z + MapSize.z + GritSize;
	//	}
	//}
}

//====================================================================
// ���S����
//====================================================================
void CEnemy::Death(void)
{
	// ���[�V�����̎擾
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	if (pMotion->GetFinish() == true)
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
void CEnemy::StateManager(D3DXVECTOR3& posMy)
{
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

		if (abs(m_move.x) < 0.01f && abs(m_move.z) < 0.01f)
		{
			m_nBugCounter++;

			if (m_nBugCounter > 180)
			{
				posMy = m_Grid.ToWorld();
				m_nBugCounter = 0;
			}
		}

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

		m_SelectGrid = m_Grid;
	}
}

//====================================================================
// �ǂ̍��G����
//====================================================================
void CEnemy::SearchWall(D3DXVECTOR3& posMy)
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
	D3DXVECTOR3 MyGritPos = m_Grid.ToWorld();
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "�G�̈ʒu %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if ((posMy.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		posMy.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK)) &&
		(m_Grid.x != m_SelectGrid.x || m_Grid.z != m_SelectGrid.z))
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
}

//==========================================
//  �ŒZ�o�H�T��
//==========================================
void CEnemy::Coordinate()
{
	// �T���^�C�}�[�����Z
	m_fCoordinateTimer += DeltaTime::Get();

	// �T���̃^�C�~���O�łȂ��ꍇ�֐��𔲂���
	if (m_fCoordinateTimer < COORDDINATE_RATE[m_EnemyType]) { return; }

	// �ŒZ�o�H�̎��̖ڕW�����Z�b�g
	m_nTargetIndex = 1;

	// �^�C�}�[�̃��Z�b�g
	m_fCoordinateTimer -= COORDDINATE_RATE[m_EnemyType];

	// �ŒZ�o�H���擾
	AStar::CoordinateList Path = AStar::Generator::GetInstance()->FindPlayer({ m_Grid.x, m_Grid.z });
	m_nNumCoordinate = Path.size();

	// ���������폜
	if (m_pPath != nullptr) { delete[] m_pPath; };

	// �ŒZ�o�H�ɕK�v�ȃO���b�h�������������m��
	m_pPath = new CMapSystem::GRID[m_nNumCoordinate];

	// �m�ۂ����������ɍŒZ�o�H�̃O���b�h���i�[
	for (int i = 0; i < m_nNumCoordinate; ++i)
	{
		m_pPath[i] = Path.at(i);
	}
}

//==========================================
// �ŒZ�o�H�����ǂ�
//==========================================
void CEnemy::Route()
{
	// ���g�̈ʒu�E�ړ��ʎ擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRot();

	// �ŒZ�o�H�������Ƃ�
	if (m_pPath == nullptr)
	{
		return;
	}

	// �ڕW�n�_�̍��W�����߂�
	D3DXVECTOR3 path = m_pPath[m_nTargetIndex].ToWorld();

	// ���Ɍ������O���b�h�ɏd�Ȃ����炻�̎��̖ڕW��ݒ�
	if (fabsf(path.x - pos.x) <= TARGET_DIFF &&
		fabsf(path.z - pos.z) <= TARGET_DIFF) // ���͈͓��ł����
	{
		// �C���f�b�N�X�ԍ������Z
		m_nTargetIndex++;
		path = m_pPath[m_nTargetIndex].ToWorld();
	}

	// ���̖ڕW�����݂��Ȃ�������֐��𔲂���
	if (m_nTargetIndex >= m_nNumCoordinate)
	{
		return;
	}

	// ���̖ڕW�ʒu�Ƃ̊p�x
	float RotDest = atan2f(path.z - pos.z, path.x - pos.x);

	// ���̖ڕW�ʒu�Ɉړ�
	move = path - pos;
	D3DXVec3Normalize(&move, &move);
	move *= DeltaTime::Get() * MOVE_ASTAR;

	// �ʒu�X�V
	pos += move;

	//�ړI�̌���
	float DiffRot = (RotDest - rot.y) * 0.1f;
	rot.y += DiffRot;

	// �ʒu�E�ړ��ʐݒ�
	SetPos(pos);
	SetMove(move);
	SetRot(rot);
}

//==========================================
//  �G�t�F�N�g�̐���
//==========================================
void CEnemy::Effect()
{
	// �����擾
	D3DXVECTOR3 pos = GetPos();
	pos.y += 0.5f;
	D3DXVECTOR3 rot = GetRot();
	D3DXMATRIX mtx = *GetUseMultiMatrix();
	pos = useful::CalcMatrix(pos, rot, mtx);
	rot = useful::CalcMatrixToRot(mtx);

	// �G�t�F�N�g�𐶐�
	m_pEffect = MyEffekseer::EffectCreate(EFFECT_TYPE[m_EnemyType], false, pos, rot, D3DXVECTOR3(10.0f, 10.0f, 10.0f));
}

//====================================================================
// ���X�g�擾
//====================================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	return m_pList;
}
