//========================================
//
// ��Q���̏���[obstacle.cpp]
// Author�F��������
//
//========================================
#include "obstacle.h"
#include "model.h"
#include "calculation.h"

#include "obstacleTree.h"
#include "obstacleRiver.h"
#include "obstacleStone.h"
#include "obstacleHole.h"
#include "obstacleSoftRock.h"

//========================================
//���O���
//========================================
namespace
{
	const char* MODEL[] =		// ���f���̃p�X
	{
		"data\\MODEL\\obstacle\\wood00.x",		// ��
		"data\\MODEL\\obstacle\\stone.x",		// ��
		"data\\MODEL\\obstacle\\hole.x",		// ��
		"data\\MODEL\\obstacle\\wood00.x",		// �_�炩����
	};

	const D3DXVECTOR3 SIZEMAX[] =	// �T�C�Y�̍ő�l
	{
		D3DXVECTOR3(60.0f, 0.0f, 60.0f),		// ��
		D3DXVECTOR3(60.0f, 500.0f, 60.0f),		// ��
		D3DXVECTOR3(40.0f, 0.0f, 40.0f),		// ��
		D3DXVECTOR3(60.0f, 0.0f, 60.0f),		// �_�炩����
	};

	const D3DXVECTOR3 SIZEMIN[] =	// �T�C�Y�̍ŏ��l
	{
		D3DXVECTOR3(-60.0f, 0.0f, -60.0f),		// ��
		D3DXVECTOR3(-60.0f, 0.0f, -60.0f),		// ��
		D3DXVECTOR3(-40.0f, 0.0f, -40.0f),		// ��
		D3DXVECTOR3(-60.0f, 0.0f, -60.0f),		// �_�炩����
	};
}

//========================================
// �ÓI�����o�ϐ��錾
//========================================
CListManager<CObstacle> CObstacle::m_List = {};	// ���X�g

//========================================
//�R���X�g���N�^
//========================================
CObstacle::CObstacle() : CStageObj(),
m_type(TYPE_RIVER)
{//�l���N���A
}

//========================================
//�f�X�g���N�^
//========================================
CObstacle::~CObstacle()
{
}

//========================================
// ������
//========================================
HRESULT CObstacle::Init(void)
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ���f������
	m_pModel = CModel::Create(MODEL[m_type]);

	if (FAILED(CStageObj::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// �I��
//========================================
void CObstacle::Uninit(void)
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I��
	CStageObj::Uninit();
}


//==========================================================================
// �폜
//==========================================================================
void CObstacle::Kill()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I��
	CStageObj::Kill();
}

//========================================
// �X�V
//========================================
void CObstacle::Update(void)
{
	// �X�V
	CStageObj::Update();
}

//========================================
// �v���C���[�Ƃ̓����蔻��
//========================================
bool CObstacle::Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE /*state*/)
{
	D3DXVECTOR3 max = rSize;
	D3DXVECTOR3 min = D3DXVECTOR3(-rSize.x, 0.0f, -rSize.z);

	// ���f���� NULL ����Ȃ��ꍇ
	if (m_pModel == nullptr) { assert(false); return false; }

	// �����蔻���Ԃ�
	return UtilFunc::Collision::IsAABBCollidingWithBox(MyLib::AABB(min, max), MyLib::Matrix(rMtx), MyLib::AABB(SIZEMIN[m_type], SIZEMAX[m_type]), m_pModel->GetMtxWorld());
}

//========================================
// �`��
//========================================
void CObstacle::Draw(void)
{
	// �`��
	CStageObj::Draw();
}

//========================================
// ��������
//========================================
CObstacle* CObstacle::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	CObstacle* pObstacle = nullptr;

	switch (type)
	{
	case CObstacle::TYPE_RIVER:

		pObstacle = new CObstacleRiver;

		break;

	case CObstacle::TYPE_STONE:

		pObstacle = new CObstacleStone;

		break;

	case CObstacle::TYPE_HOLE:

		pObstacle = new CObstacleHole;

		break;

	case CObstacle::TYPE_SOFTROCK:

		pObstacle = new CObstacleSoftRock;

		break;

	default:
		assert(false);
		break;
	}

	// �ʒu�E�����E��ސݒ�
	pObstacle->m_type = type;
	pObstacle->SetPos(rPos);
	pObstacle->SetRot(rRot);

	// �|�C���^��Ԃ�
	return pObstacle;
}
