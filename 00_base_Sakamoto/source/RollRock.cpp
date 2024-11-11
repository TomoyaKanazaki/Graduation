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
#include "Devil.h"
#include "MapSystem.h"
#include "CubeBlock.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(45.0f, 45.0f, 45.0f);		//�����蔻��
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CRollRock>* CRollRock::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CRollRock::CRollRock(int nPriority) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
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
CRollRock* CRollRock::Create(char* pModelName)
{
	CRollRock* pSample = nullptr;

	if (pSample == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CRollRock();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init(pModelName)))
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
	SetType(CObject::TYPE_ENEMY3D);

	CObjectX::Init(pModelName);

	SetMultiMatrix(true);

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

	// �ړ��ʂ��ʒu�ɔ��f
	m_pos.x += m_move.x;

	// X���̓����蔻��
	CollisionWall(useful::COLLISION_X);

	// �ړ�������������]
	m_rot.z -= (m_pos.x - m_posOld.x) * D3DX_PI * 0.01f;

	// �ړ��ʂ��ʒu�ɔ��f
	m_pos.z += m_move.z;

	// Z���̓����蔻��
	CollisionWall(useful::COLLISION_Z);

	// �ړ�������������]
	m_rot.x += (m_pos.z - m_posOld.z) * D3DX_PI * 0.01f;

	//�ʒu�X�V
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//���_���̍X�V
	CObjectX::Update();
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
	D3DXVECTOR3 SlopeRot = CGame::GetDevil()->GetDevilRot();

	// �X���ɂ��ړ��ʐݒ�
	m_move.x = -SlopeRot.z * 10.0f;
	m_move.z = SlopeRot.x * 10.0f;
}

//====================================================================
// �ǂƂ̓����蔻��
//====================================================================
void CRollRock::CollisionWall(useful::COLLISION XYZ)
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
//���X�g�擾
//====================================================================
CListManager<CRollRock>* CRollRock::GetList(void)
{
	return m_pList;
}