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
#include "game.h"
#include "tutorial.h"
#include "objmeshField.h"
#include "devil.h"
#include "MapMove.h"

//==========================================
// �萔��`
//==========================================
namespace
{
	const int FIRE_LIFE = 120;			// ���̗̑�
	const float FIRE_SPEED = 10.0f;		// ���̑��x
	const float FIRE_HEIGHT = 50.0f;	// ���̍���
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
m_Grid(CMapSystem::GRID(0, 0)),
m_pEffect(nullptr)
{
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
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pFire->Init(pModelName)))
	{//���������������s�����ꍇ
		assert(false);
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

	// ���g�̏����擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �����𒲐�
	pos.y = FIRE_HEIGHT;

	// �O���b�h���W��ݒ�
	m_Grid = CMapSystem::GetInstance()->CalcGrid(pos);

	// ���W��ݒ�
	SetPos(pos);

	// �p���N���X�̏�����
	CObjectX::Init(pModelName);

	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// ���̗̑�
	m_nLife = FIRE_LIFE;

	// �G�t�F�N�g�𐶐�����
	m_pEffect = MyEffekseer::EffectCreate(CMyEffekseer::TYPE_FIRE, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// ���̑��x
	D3DXVECTOR3 move = -D3DXVECTOR3(FIRE_SPEED * sinf(rot.y), 0.0f, FIRE_SPEED * cosf(rot.y));
	SetMove(move);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFire>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// �X�N���[�����I���ɂ���
	SetMapScroll(true);

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
	D3DXVECTOR3 rot = GetRot();

	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	SetPosOld(pos);

	//���_���̍X�V
	CObjectX::Update();

	//�ʒu�X�V
	pos += m_move;

	// �X�N���[���ɍ��킹�Ĉړ�����
	//CMapSystem::GetInstance()->GetMove()->FollowScroll(pos);

	// �ʒu�E�ړ��ʐݒ�
	SetPos(pos);

	// �O���b�h���W��ݒ�
	m_Grid = CMapSystem::GetInstance()->CalcGrid(pos);
	
	// �G�t�F�N�g�𓮂���
	if (m_pEffect != nullptr)
	{
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rot, *GetUseMultiMatrix());
		m_pEffect->SetPosition(ef);
	}

	// �������Z
	m_nLife--;

	// ���Ŕ���
	if (
		CollisionEnemy() ||
		CollisionWall() ||
		m_nLife < 0
		)
	{
		Uninit();
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CFire::Draw(void)
{
	//CObjectX::Draw();
}

//====================================================================
// �G�Ƃ̔���
//====================================================================
bool CFire::CollisionEnemy()
{
	// �G�l�~�[���X�g�\����������Δ�����
	if (CEnemy::GetList() == nullptr) { return false; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CEnemy* pEnemy : list)
	{
		if (pEnemy->GetHitState() != CEnemy::HIT_STATE_NORMAL)
		{
			continue;
		}

		// �~�̓����蔻��
		if (m_Grid == pEnemy->GetGrid())
		{// �e����������
			pEnemy->Hit();

			// �G�t�F�N�g�𐶐�
			D3DXVECTOR3 pos = pEnemy->GetPos();
			D3DXVECTOR3 rot = pEnemy->GetRot();
			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_LITTLEDEVIL)
			{
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HIT_SMALLDEVIL, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);
			}
			else
			{
				MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HIT, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);
			}

			return true;
		}
	}

	return false;
}

//==========================================
//  �ǂ̔���
//==========================================
bool CFire::CollisionWall()
{
	// ���g�̃O���b�h���W���ړ��\�ȏꍇ�֐��𔲂���
	if (!CMapSystem::GetInstance()->GetGritBool(m_Grid)) { return false; }

	// ���g�̏����擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �G�t�F�N�g�𐶐�
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HITTHEWALL, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot, D3DXVECTOR3(25.0f, 25.0f, 25.0f));

	return true;
}

//==========================================
//���X�g�擾
//==========================================
CListManager<CFire>* CFire::GetList(void)
{
	return m_pList;
}
