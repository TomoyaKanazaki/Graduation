//============================================
//
//  �\�t�g�N���[���̏��� [softcream.cpp]
//	Author:morikawa shunya
//
//============================================
#include "softcream.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "MapSystem.h"
#include "effect.h"
#include "game.h"
#include "objmeshField.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* MODEL_PASS = "data\\MODEL\\02_item\\softcream.x"; // ���f���p�X
	const float MOVE_SCALE_X = 1050.0f; // �ړ���
	const float MOVE_SCALE_Z = 650.0f; // �ړ���

	// TODO : �{�d�l�̎��ɕK������
	float fMove = 0;
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CSoftCream>* CSoftCream::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CSoftCream::CSoftCream(int nPriority) : CItem(nPriority)
{
	SetSize(COLLISION_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CSoftCream::~CSoftCream()
{

}

//====================================================================
// ������
//====================================================================
HRESULT CSoftCream::Init()
{
	// �e�N���X�̏�����
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetType(CObject::TYPE_SOFTCREAM);

	// �X�N���[���̑Ώۂ���O��
	SetMapScroll(false);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CSoftCream>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CSoftCream::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �p���N���X�̏I��
	CItem::Uninit();
}

//====================================================================
// �X�V
//====================================================================
void CSoftCream::Update(void)
{
	DebugProc::Print(DebugProc::POINT_CENTER, "�\�t�g�N���[�����W : %f, %f, %f\n", GetPos().x, GetPos().y, GetPos().z);

	//�e�N���X�̍X�V
	CItem::Update();
}

//====================================================================
// �`��
//====================================================================
void CSoftCream::Draw(void)
{
	// �p���N���X�̕`��
	CItem::Draw();
}

//====================================================================
// �����̐���
//====================================================================
void CSoftCream::Move(D3DXVECTOR3& pos)
{
	// �ړ��ʉ��Z
	pos.x += MOVE_SCALE_X * DeltaTime::Get() * sinf(fMove);
	pos.z += MOVE_SCALE_Z * DeltaTime::Get() * cosf(fMove);

	// TODO : �{�d�l�̎��ɕK������
	fMove += DeltaTime::Get();
}

//==========================================
//  �}�b�v�ԍ��̐ݒ�
//==========================================
void CSoftCream::SetGrid(const CMapSystem::GRID& pos)
{
	// �e�N���X�̐ݒ菈�����Ăяo��
	CItem::SetGrid(pos);

	// �O���b�h��񂩂玩�g�̍��W���Z�o����
	D3DXVECTOR3 posThis = CMapSystem::GetInstance()->GetGritPos(pos.x, pos.z);

	// �ʒu��ݒ�
	CItem::SetPos(posThis);
	CObjectX::SetPos(posThis);
}

//====================================================================
// ��ԊǗ�
//====================================================================
bool CSoftCream::Hit(CPlayer* pPlayer)
{
	if (pPlayer->GetItemType() != CPlayer::TYPE_NONE
		&& pPlayer->GetItemType() != CPlayer::TYPE_BIBLE)
	{
		return false;
	}

	// ���g�̍폜
	Uninit();
	return true;
}

//==========================================
// ���X�g�̎擾
//==========================================
CListManager<CSoftCream>* CSoftCream::GetList(void)
{
	return m_pList;
}
