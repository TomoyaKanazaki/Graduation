//============================================
//
//	�]�����̏��� [RollRock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RollRock.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "game.h"
#include "tutorial.h"
#include "Devil.h"
#include "MapSystem.h"
#include "CubeBlock.h"
#include "objmeshField.h"
#include "wall.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(40.0f, 40.0f, 40.0f);		//�����蔻��
	const float GRIT_OK = 45.0f;			//�ړ��\�ȃO���b�g�͈͓̔�
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CRollRock>* CRollRock::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CRollRock::CRollRock(int nPriority) : CObjectX(nPriority),
m_pEffect(nullptr)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_Grid.x = 0;
	m_Grid.z = 0;

	m_OKL = false;
	m_OKR = false;
	m_OKU = false;
	m_OKD = false;
}

//====================================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//====================================================================
CRollRock::CRollRock(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_Grid = gridCenter;		// �O���b�h

	m_OKL = false;
	m_OKR = false;
	m_OKU = false;
	m_OKD = false;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CRollRock::~CRollRock()
{

}

//====================================================================
//��������
//====================================================================
CRollRock* CRollRock::Create(CMapSystem::GRID gridCenter)
{
	CRollRock* pSample = nullptr;

	if (pSample == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CRollRock(3, gridCenter);
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init("data\\MODEL\\rock.x")))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pSample;
}

//====================================================================
//����������
//====================================================================
HRESULT CRollRock::Init(char* pModelName)
{
	m_pEffect = nullptr;
	// �ʒu
	m_pos = m_Grid.ToWorld();
	m_pos.y = 50.0f;

	SetType(CObject::TYPE_ENEMY3D);

	CObjectX::Init(pModelName);

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// �ʒu�ݒ�
	CObjectX::SetPos(m_pos);

	SetSize(SAMPLE_SIZE);

	//���[�h���Ƃɏ����l��ݒ�o����
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

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CRollRock>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);


	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRollRock::Uninit(void)
{
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

	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CRollRock::Update(void)
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
void CRollRock::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//�ʒu�X�V
	pos += m_move;

	SetPos(pos);

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CRollRock::GameUpdate(void)
{
	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	m_posOld = m_pos;
	CObjectX::SetPosOld(m_pos);

	Move();

	//// �ړ��ʂ��ʒu�ɔ��f
	//m_pos.x += m_move.x;

	// X���̓����蔻��
	CollisionWall(useful::COLLISION_X);

	// �ړ�������������]
	m_rot.z -= (m_pos.x - m_posOld.x) * D3DX_PI * 0.01f;

	//// �ړ��ʂ��ʒu�ɔ��f
	//m_pos.z += m_move.z;

	// Z���̓����蔻��
	CollisionWall(useful::COLLISION_Z);

	// �ړ�������������]
	m_rot.x += (m_pos.z - m_posOld.z) * D3DX_PI * 0.01f;

	// �X�e�[�W�O�Ƃ̓����蔻��
	CollisionOut();

	// �O���b�g�ԍ��̐ݒ�
	m_Grid = CMapSystem::GetInstance()->CalcGrid(m_pos);

	//�ʒu�X�V
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//���_���̍X�V
	CObjectX::Update();

	DebugProc::Print(DebugProc::POINT_RIGHT, "[��]�� %d : �c %d\n", m_Grid.x, m_Grid.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[��]�� %d\n", m_OKL);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[��]�E %d\n", m_OKR);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[��]�� %d\n", m_OKU);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[��]�� %d\n", m_OKD);
}

//====================================================================
//�`�揈��
//====================================================================
void CRollRock::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//�X�����̈ړ�����
//====================================================================
void CRollRock::Move(void)
{
	D3DXVECTOR3 SlopeRot = INITVECTOR3;

	SlopeRot = CDevil::GetListTop()->GetDevilRot();

	// �X���ɂ��ړ��ʐݒ�
	m_move.x = -SlopeRot.z * 10.0f;
	m_move.z = SlopeRot.x * 10.0f;

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = m_Grid.ToWorld();
	float MapGritSize = CMapSystem::GetInstance()->GetGritSize();

	if (useful::CollisionCircle(MyGritPos, D3DXVECTOR3(m_pos.x, MyGritPos.y, m_pos.z), 5.0f) == true)
	{// �u���b�N�̒��S�ɂ��鎞�ɏ㉺�����E�̂ǂ��炩�ɂȂ�܂łɈړ�����

		m_pos.x += m_move.x;
		m_pos.z += m_move.z;
	}
	else
	{// �u���b�N�̒��S�ɂȂ��Ƃ�

		//�㉺�ړ�
		if (MyGritPos.x - m_pos.x >= -5.0f && MyGritPos.x - m_pos.x <= 5.0f)
		{
			m_pos.z += m_move.z;
		}
		else
		{
			m_pos.z = MyGritPos.z;
		}

		//���E�ړ�
		if (MyGritPos.z - m_pos.z >= -5.0f && MyGritPos.z - m_pos.z <= 5.0f)
		{
			m_pos.x += m_move.x;
		}
		else
		{
			m_pos.x = MyGritPos.x;
		}
	}

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - (GRIT_OK * m_OKR)) &&	//��
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - (GRIT_OK * m_OKL)) &&	//�E
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f)) &&	//��
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f)))	//��
	{// �O���b�g�̒��S�ʒu�ɗ����Ă��邩

		int nRGridX = m_Grid.x + 1;
		int nLGridX = m_Grid.x - 1;

		nRGridX = useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, nRGridX);
		nLGridX = useful::RangeNumber(CMapSystem::GetInstance()->GetWightMax(), 0, nLGridX);

		if (CMapSystem::GetInstance()->GetGritBool(nRGridX, m_Grid.z) == true)
		{//�E
			m_OKR = false;
		}
		else
		{
			m_OKR = true;
		}

		if (CMapSystem::GetInstance()->GetGritBool(nLGridX, m_Grid.z) == true)
		{//��
			m_OKL = false;
		}
		else
		{
			m_OKL = true;
		}
	}

	if (m_pos.x <= MyGritPos.x + ((MapGritSize * 0.5f)) &&	//��
		m_pos.x >= MyGritPos.x - ((MapGritSize * 0.5f)) &&	//�E
		m_pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - (GRIT_OK * m_OKD)) &&	//��
		m_pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - (GRIT_OK * m_OKU)))	//��
	{// �O���b�g�̒��S�ʒu�ɗ����Ă��邩

		int nUGridZ = m_Grid.z - 1;
		int nDGridZ = m_Grid.z + 1;

		nUGridZ = useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, nUGridZ);
		nDGridZ = useful::RangeNumber(CMapSystem::GetInstance()->GetHeightMax(), 0, nDGridZ);

		if (CMapSystem::GetInstance()->GetGritBool(m_Grid.x, nUGridZ) == true)
		{//��
			m_OKU = false;
		}
		else
		{
			m_OKU = true;
		}

		if (CMapSystem::GetInstance()->GetGritBool(m_Grid.x, nDGridZ) == true)
		{//��
			m_OKD = false;
		}
		else
		{
			m_OKD = true;
		}
	}

	if (!m_OKR && m_move.x > 0.0f)
	{
		if (m_pos.x > MyGritPos.x)
		{
			m_pos.x = MyGritPos.x;
			m_move.x = 0.0f;
		}
	}
	if (!m_OKL && m_move.x < 0.0f)
	{
		if (m_pos.x < MyGritPos.x)
		{
			m_pos.x = MyGritPos.x;
			m_move.x = 0.0f;
		}
	}
	if (!m_OKU && m_move.z > 0.0f)
	{
		if (m_pos.z > MyGritPos.z)
		{
			m_pos.z = MyGritPos.z;
			m_move.z = 0.0f;
		}
	}
	if (!m_OKD && m_move.z < 0.0f)
	{
		if (m_pos.z < MyGritPos.z)
		{
			m_pos.z = MyGritPos.z;
			m_move.z = 0.0f;
		}
	}

	// �G�t�F�N�g�̐���
	if (m_pEffect == nullptr && (fabsf(m_move.x) > 0.1f || fabsf(m_move.z) > 0.1f))
	{
		D3DXVECTOR3 rot = INITVECTOR3;
		rot.y = atan2f(-m_move.x, -m_move.z);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(m_pos, rot, mat);
		m_pEffect = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ROLL, true, ef, rot);
	}

	// �G�t�F�N�g���ړ�
	if (m_pEffect != nullptr)
	{
		// �v�Z�Ɏg�p����l�̎擾
		D3DXMATRIX mat = *GetUseMultiMatrix();

		// ���W�ƌ����Ƀ}�g���b�N�X�𔽉f
		D3DXVECTOR3 pos = useful::CalcMatrix(m_pos, m_rot, mat);

		// �G�t�F�N�g�ɏ���K�p
		m_pEffect->SetPosition(pos);
	}

	// �G�t�F�N�g������
	if (m_pEffect != nullptr && fabsf(m_move.x) <= 0.1f && fabsf(m_move.z) <= 0.1f)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CRollRock::CollisionWall(useful::COLLISION XYZ)
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CWall::GetList() == nullptr) { return; }
	std::list<CWall*> list = CWall::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CWall* pWall : list)
	{
		D3DXVECTOR3 pos = pWall->GetPos();
		D3DXVECTOR3 posOld = pWall->GetPosOld();
		D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Size = pWall->GetSize();

		D3DXVECTOR3 ObjMove = INITVECTOR3;
		D3DXVECTOR3 MySize = GetSize();
		bool Jump = false;

		// ��`�̓����蔻��
		if (useful::CollisionBlock(pos, pos, Move, Size, &m_pos, m_posOld, &m_move, &ObjMove, MySize, &Jump, XYZ) == true)
		{

		}
	}
}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CRollRock::CollisionOut()
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
			m_pos.x = Pos.x - MapSize.x - GritSize;
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
//���X�g�擾
//====================================================================
CListManager<CRollRock>* CRollRock::GetList(void)
{
	return m_pList;
}