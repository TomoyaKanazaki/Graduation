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
CFire::CFire(int nPriority) : CObjectX(nPriority),
m_pEffect(nullptr)
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

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetMultiMatrix(true);

	// ���̗̑�
	m_nLife = FIRE_LIFE;

	// �G�t�F�N�g�𐶐�����
	m_pEffect = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_FIRE, true, useful::CalcMatrix(m_pos, m_rot, GetUseMultiMatrix()), m_rot, D3DXVECTOR3(50.0f, 50.0f, 50.0f));

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

	// �G�t�F�N�g������
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
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
	
	// �G�t�F�N�g�𓮂���
	if (m_pEffect != nullptr)
	{
		D3DXMATRIX mat = GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(m_pos, m_rot, GetUseMultiMatrix());
		m_pEffect->SetPosition(ef);
	}

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
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CEnemy* pEnemy : list)
	{
		D3DXVECTOR3 pos = pEnemy->GetPos();
		D3DXVECTOR3 Size = pEnemy->GetSize();

		// �~�̓����蔻��
		if (useful::CollisionCircle(m_pos, pos, Size.x) == true)
		{// �e����������
			pEnemy->Hit(1);

			// �폜
			Uninit();

			return;
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
