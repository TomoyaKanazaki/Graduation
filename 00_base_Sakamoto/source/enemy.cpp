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
#include "character.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "CubeBlock.h"
#include "LevelModelEffect.h"
#include "slowManager.h"
#include "2DEffect.h"
#include "score.h"
#include "modelEffect.h"
#include "Effect.h"
#include "devil.h"
#include "DevilHole.h"
#include "objmeshField.h"
#include "sound.h"

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
	const float TARGET_DIFF = 5.0f; // ���e�͈�
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
CEnemy::CEnemy(int nPriority) :CObject(nPriority),
m_pPath(nullptr),
m_fCoordinateTimer(0.0f),
m_nTargetIndex(0),
m_nNumCoordinate(0)
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

	m_pCharacter = nullptr;

	m_size = COLLISION_SIZE;
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

	m_nBugCounter = 0;
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

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetMultiMatrix(true);

	// �X���[�̐���(�z���A�^�O�̐ݒ�)
	m_pSlow = CSlowManager::Create(m_pSlow->CAMP_ENEMY, m_pSlow->TAG_ENEMY);

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

	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Uninit();
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CEnemy::Update(void)
{
	// �ߋ��̈ʒu���L�^
	m_posOld = m_pos;

	//�ǂ̍��G����
	SearchWall();

	// ��Ԃ̍X�V
	//StateManager();

	// �ړ���������
	Rot();

	// �ʒu�X�V����
	//UpdatePos();

	// �v���C���[�ւ̍ŒZ�o�H�T��
	Coordinate();

	// �ŒZ�n�I�����ǂ�
	Route();

	// �����̔ԍ���ݒ�
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(m_pos);

	//���̔���
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	// �L�����N�^�[�̍X�V
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}

	// �f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "[�G]�� %d : �c %d\n", m_Grid.x, m_Grid.z);
}

//====================================================================
//�`�揈��
//====================================================================
void CEnemy::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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

	// �L�����N�^�[�̕`��
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}

	//�X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
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
// �L�����N�^�[�̎擾����
//====================================================================
CCharacter* CEnemy::GetCharacter(void)
{
	if (m_pCharacter != nullptr)
	{
		return m_pCharacter;
	}

	assert(("�L�����N�^�[�̎擾���s", false));
	return nullptr;
}

//====================================================================
// ���f���֘A�̏���������
//====================================================================
HRESULT CEnemy::InitModel(const char* pFilename)
{
	// �L�����N�^�[�̍X�V
	if (m_pCharacter == nullptr)
	{
		if (m_pCharacter == nullptr)
		{
			m_pCharacter = CCharacter::Create(pFilename);

			if (m_pCharacter == nullptr)
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

//====================================================================
// �ʒu�X�V����
//====================================================================
void CEnemy::UpdatePos(void)
{
	if (m_pCharacter == nullptr)
	{
		return;
	}

	// ���f���̎擾
	CMotion* pMotion = m_pCharacter->GetMotion();

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

	CDevil* pDevil = CGame::GetDevil();

	//Y���̈ʒu�X�V
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed() * fSpeed;

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_Y);
	CollisionDevilHole(useful::COLLISION_Y);

	//X���̈ʒu�X�V
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeX(m_move.x);

	// �ǂƂ̓����蔻��
	CollisionWall(useful::COLLISION_X);
	CollisionDevilHole(useful::COLLISION_X);

	//Z���̈ʒu�X�V
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed() * fSpeed * pDevil->MoveSlopeZ(m_move.z);

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

	m_rot.y = atan2f(-m_move.x, -m_move.z);

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CEnemy::CollisionWall(useful::COLLISION XYZ)
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
		bool bNullJump;

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, posOld, Move, Size, &m_pos, m_posOld, &m_move, &m_Objmove, m_size, &bNullJump, XYZ) == true)
		{
			//�ҋ@��Ԃɂ���
			m_State = E_STATE_WAIT;

			m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
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
void CEnemy::CollisionOut()
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CDevil::GetList() == nullptr) { return; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CDevil* pDevil : list)
	{
		D3DXVECTOR3 Pos = pDevil->GetDevilPos();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize();

		// �X�e�[�W�O�̓����蔻��
		if (Pos.x + MapSize.x < m_pos.x) // �E
		{
			m_pos.x = Pos.x -MapSize.x - GritSize;
		}
		if (Pos.x - MapSize.x - GritSize > m_pos.x) // ��
		{
			m_pos.x = Pos.x + MapSize.x;
		}
		if (Pos.z + MapSize.z + GritSize < m_pos.z) // ��
		{
			m_pos.z = Pos.z - MapSize.z;
		}
		if (Pos.z - MapSize.z > m_pos.z) // ��
		{
			m_pos.z = Pos.z + MapSize.z + GritSize;
		}
	}
}

//====================================================================
// ���S����
//====================================================================
void CEnemy::Death(void)
{
	if (m_pCharacter == nullptr)
	{
		return;
	}

	// ���f���̎擾
	CMotion* pMotion = m_pCharacter->GetMotion();

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

		if (abs(m_move.x) < 0.01f && abs(m_move.z) < 0.01f)
		{
			m_nBugCounter++;

			if (m_nBugCounter > 180)
			{
				m_pos = CMapSystem::GetInstance()->GetGritPos(m_Grid);
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
	D3DXVECTOR3 MyGritPos = CMapSystem::GetInstance()->GetGritPos(m_Grid);;
	float MapGritSize = pMapSystem->GetGritSize();

	DebugProc::Print(DebugProc::POINT_LEFT, "�G�̈ʒu %f %f %f\n", MyGritPos.x, MyGritPos.y, MyGritPos.z);

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - GRIT_OK) &&
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - GRIT_OK))
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
	D3DXVECTOR3 path = CMapSystem::GetInstance()->GetGritPos(m_pPath[m_nTargetIndex]);

	// ���Ɍ������O���b�h�ɏd�Ȃ����炻�̎��̖ڕW��ݒ�
	if (fabsf(path.x - pos.x) <= TARGET_DIFF &&
		fabsf(path.z - pos.z) <= TARGET_DIFF) // ���͈͓��ł����
	{
		// �C���f�b�N�X�ԍ������Z
		m_nTargetIndex++;
		path = CMapSystem::GetInstance()->GetGritPos(m_pPath[m_nTargetIndex]);
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
	D3DXVECTOR3 rot = GetRot();

	// �G�t�F�N�g�𐶐�
	MyEffekseer::EffectCreate(EFFECT_TYPE[m_EnemyType], false, useful::CalcMatrix(pos, rot, GetUseMultiMatrix()), rot);
}

//====================================================================
// ���X�g�擾
//====================================================================
CListManager<CEnemy>* CEnemy::GetList(void)
{
	return m_pList;
}
