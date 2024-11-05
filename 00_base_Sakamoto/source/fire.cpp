//============================================
//
//	�I�u�W�F�N�g�w���f���̃T���v�� [SampleObjX.cpp]
//	Author:morikawa shunya
//
//============================================
#include "fire.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "enemy.h"

//==========================================
// �萔��`
//==========================================
namespace
{
	const int FIRE_LIFE = 60;			// ���̗̑�
	const float FIRE_SPEED = 10.0f;		// ���̑��x
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CFire>* CFire::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CFire::CFire(int nPriority) : CObjectX(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = 0;			//�}�e���A���̐�
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_nLife = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CFire::~CFire()
{

}

//====================================================================
//��������
//====================================================================
CFire* CFire::Create(char* pModelName, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	CFire* pFire = nullptr;

	if (pFire == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pFire = new CFire();

		pFire->SetPos(pos);
		pFire->SetRot(rot);
		pFire->m_rot = rot;
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pFire->Init(pModelName)))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pFire;
}

//====================================================================
//����������
//====================================================================
HRESULT CFire::Init(char* pModelName)
{
	// �I�u�W�F�N�g�̎�ސݒ�
	SetType(CObject::TYPE_FIRE);

	// �p���N���X�̏�����
	CObjectX::Init(pModelName);

	// ���̗̑�
	m_nLife = FIRE_LIFE;

	// ���̑��x
	D3DXVECTOR3 move = -D3DXVECTOR3(FIRE_SPEED * sinf(m_rot.y), 0.0f, FIRE_SPEED * cosf(m_rot.y));
	SetMove(move);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFire>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CFire::Uninit(void)
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
void CFire::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	GameUpdate();

	//�ʒu�X�V
	pos += m_move;

	// �ʒu�E�ړ��ʐݒ�
	SetPos(pos);
	
	// �G�Ƃ̔���
	CollisionEnemy();

	// ���Z
	m_nLife--;

	if (m_nLife < 0)
	{// �̗�0�ȉ��ɂȂ���
		//�j������
		Uninit();
	}
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CFire::GameUpdate(void)
{
	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	m_posOld = m_pos;

	//�ʒu�X�V
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//��ʊO����
	if (m_pos.y < 0.0f)
	{
		Uninit();
	}

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//��ԊǗ�
	StateManager();

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CFire::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
// �G�Ƃ̔���
//====================================================================
void CFire::CollisionEnemy()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			// ��ނ��擾
			CObject::TYPE type = pObj->GetType();

			if (type == TYPE_ENEMY3D)
			{//��ނ��G�̎�
				// �G�̏��̎擾
				CEnemy* pEnemy = (CEnemy*)pObj;	

				D3DXVECTOR3 pos = pEnemy->GetPos();
				D3DXVECTOR3 Size = pEnemy->GetSize();

				// �~�̓����蔻��
				if (useful::CollisionCircle(m_pos, pos, Size.x) == true)
				{// �e����������
					pEnemy->Hit(1);

					Uninit();

					return;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CFire::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CFire>* CFire::GetList(void)
{
	return m_pList;
}
