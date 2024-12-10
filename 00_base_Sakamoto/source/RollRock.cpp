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
#include "objmeshField.h"
#include "wall.h"
#include "MapMove.h"

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
	m_move = INITVECTOR3;
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
CRollRock::CRollRock(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority),
m_Grid(gridCenter),
m_OldGrid(gridCenter)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_move = INITVECTOR3;

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
	if (FAILED(pSample->Init("data\\MODEL\\00_rock.x")))
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
	D3DXVECTOR3 pos = m_Grid.ToWorld();
	pos.y = 50.0f;

	SetType(CObject::TYPE_ENEMY3D);

	CObjectX::Init(pModelName);

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// �ʒu�ݒ�
	CObjectX::SetPos(pos);

	SetSize(SAMPLE_SIZE);

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
	// �l���擾
	D3DXVECTOR3 posThis = GetPos();			// �ʒu
	D3DXVECTOR3 posOldThis = GetPosOld();	// �O��̈ʒu
	D3DXVECTOR3 rotThis = GetRot();			// ����
	D3DXVECTOR3 sizeThis = GetSize();		// �傫��

	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	posOldThis = posThis;
	m_OldGrid = m_Grid;
	CObjectX::SetPosOld(posThis);

	Move(posThis, rotThis);

	// �ړ�������������]
	rotThis.z -= (posThis.x - posOldThis.x) * D3DX_PI * 0.01f;

	// �ړ�������������]
	rotThis.x += (posThis.z - posOldThis.z) * D3DX_PI * 0.01f;

	// �X�e�[�W�O�Ƃ̓����蔻��
	CollisionOut(posThis);

	// �O���b�h����ݒ�
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(posThis);

	// A*�����ݒ�
	Coodinate();

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	// �X�N���[���ɍ��킹�Ĉړ�����
	CMapSystem::GetInstance()->GetMove()->FollowScroll(posThis);

	// �l�X�V
	SetPos(posThis);		// �ʒu
	SetPosOld(posOldThis);	// �O��̈ʒu
	SetRot(rotThis);		// ����
	SetSize(sizeThis);		// �傫��

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
void CRollRock::Move(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	D3DXVECTOR3 SlopeRot = INITVECTOR3;

	SlopeRot = CMapMove::GetListTop()->GetDevilRot();

	// �X���ɂ��ړ��ʐݒ�
	m_move.x = -SlopeRot.z * 10.0f;
	m_move.z = SlopeRot.x * 10.0f;

	//�����̗����Ă���O���b�g�̒��S�ʒu�����߂�
	D3DXVECTOR3 MyGritPos = m_Grid.ToWorld();
	float MapGritSize = CMapSystem::GetInstance()->GetGritSize();

	if (useful::CollisionCircle(MyGritPos, D3DXVECTOR3(pos.x, MyGritPos.y, pos.z), 5.0f) == true)
	{// �u���b�N�̒��S�ɂ��鎞�ɏ㉺�����E�̂ǂ��炩�ɂȂ�܂łɈړ�����

		pos.x += m_move.x;
		pos.z += m_move.z;
	}
	else
	{// �u���b�N�̒��S�ɂȂ��Ƃ�

		//�㉺�ړ�
		if (MyGritPos.x - pos.x >= -5.0f && MyGritPos.x - pos.x <= 5.0f)
		{
			pos.z += m_move.z;
		}
		else
		{
			pos.z = MyGritPos.z;
		}

		//���E�ړ�
		if (MyGritPos.z - pos.z >= -5.0f && MyGritPos.z - pos.z <= 5.0f)
		{
			pos.x += m_move.x;
		}
		else
		{
			pos.x = MyGritPos.x;
		}
	}

	if (pos.x <= MyGritPos.x + ((MapGritSize * 0.5f) - (GRIT_OK * m_OKR)) &&	//��
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f) - (GRIT_OK * m_OKL)) &&	//�E
		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f)) &&	//��
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f)))	//��
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

	if (pos.x <= MyGritPos.x + ((MapGritSize * 0.5f)) &&	//��
		pos.x >= MyGritPos.x - ((MapGritSize * 0.5f)) &&	//�E
		pos.z <= MyGritPos.z + ((MapGritSize * 0.5f) - (GRIT_OK * m_OKD)) &&	//��
		pos.z >= MyGritPos.z - ((MapGritSize * 0.5f) - (GRIT_OK * m_OKU)))	//��
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
		if (pos.x > MyGritPos.x)
		{
			pos.x = MyGritPos.x;
			m_move.x = 0.0f;
		}
	}
	if (!m_OKL && m_move.x < 0.0f)
	{
		if (pos.x < MyGritPos.x)
		{
			pos.x = MyGritPos.x;
			m_move.x = 0.0f;
		}
	}
	if (!m_OKU && m_move.z > 0.0f)
	{
		if (pos.z > MyGritPos.z)
		{
			pos.z = MyGritPos.z;
			m_move.z = 0.0f;
		}
	}
	if (!m_OKD && m_move.z < 0.0f)
	{
		if (pos.z < MyGritPos.z)
		{
			pos.z = MyGritPos.z;
			m_move.z = 0.0f;
		}
	}

	// �G�t�F�N�g�̐���
	if (m_pEffect == nullptr && (fabsf(m_move.x) > 0.1f || fabsf(m_move.z) > 0.1f))
	{
		D3DXVECTOR3 rotEf = INITVECTOR3;
		rotEf.y = atan2f(-m_move.x, -m_move.z);
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rotEf, mat);
		m_pEffect = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ROLL, true, ef, rotEf);
	}

	// �G�t�F�N�g���ړ�
	if (m_pEffect != nullptr)
	{
		// �v�Z�Ɏg�p����l�̎擾
		D3DXMATRIX mat = *GetUseMultiMatrix();

		// ���W�ƌ����Ƀ}�g���b�N�X�𔽉f
		D3DXVECTOR3 posEf = useful::CalcMatrix(pos, rot, mat);

		// �G�t�F�N�g�ɏ���K�p
		m_pEffect->SetPosition(posEf);
	}

	// �G�t�F�N�g������
	if (m_pEffect != nullptr && fabsf(m_move.x) <= 0.1f && fabsf(m_move.z) <= 0.1f)
	{
		m_pEffect->SetDeath();
		m_pEffect = nullptr;
	}
}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CRollRock::CollisionOut(D3DXVECTOR3& pos)
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
		if (Pos.x + MapSize.x < pos.x) // �E
		{
			pos.x = Pos.x - MapSize.x - GritSize;
		}
		if (Pos.x - MapSize.x - GritSize > pos.x) // ��
		{
			pos.x = Pos.x + MapSize.x;
		}
		if (Pos.z + MapSize.z + GritSize < pos.z) // ��
		{
			pos.z = Pos.z - MapSize.z;
		}
		if (Pos.z - MapSize.z > pos.z) // ��
		{
			pos.z = Pos.z + MapSize.z + GritSize;
		}
	}
}

//==========================================
//  A*�E�F�C�g�̕ύX����
//==========================================
void CRollRock::Coodinate()
{
	// �O��̃O���b�h�ƍ���̃O���b�h����v���Ă���ꍇ�֐��𔲂���
	if (m_Grid == m_OldGrid) { return; }

	// �o�H�T���p�̏����擾
	auto generator = AStar::Generator::GetInstance();
	if (generator == nullptr)
	{
		assert(false);
		generator = AStar::Generator::Create();
	}

	// �}�b�v�����擾
	CMapSystem* pMapSystem = CMapSystem::GetInstance();

	// �O��̃O���b�h���ړ��\�n�_�ɐݒ�
	generator->removeCollision(m_OldGrid.ToAStar());
	pMapSystem->SetGritBool(m_OldGrid, false);

	// ���݂̃O���b�h���ړ��s�n�_�ɐݒ�
	generator->addCollision(m_Grid.ToAStar());
	pMapSystem->SetGritBool(m_Grid, true);
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CRollRock>* CRollRock::GetList(void)
{
	return m_pList;
}