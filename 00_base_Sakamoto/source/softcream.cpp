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
	const int APPEA = 4;	// �o�����̍ő吔
	const char* MODEL_PASS = "data\\MODEL\\02_item\\holybible.x"; // ���f���p�X
	const float MOVE_SCALE = sqrtf(50.0f * 50.0f * 2.0f); // �ړ���
	const float DELTE_TIME = 600.0f;	// �����ŏ����鎞��
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CSoftCream>* CSoftCream::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CSoftCream::CSoftCream(int nPriority) : CItem(nPriority),
m_nAppea(0),	// �o����
m_fMove(0.0f),	// �ړ���
m_fConuter(0.0f)	// �폜�J�E���^�[
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
void CSoftCream::Move()
{
	// �t�B�[���h�̍��W���擾
	D3DXVECTOR3 posField = CGame::GetMapField()->GetPos();

	// ���g�̍��W���擾 
	D3DXVECTOR3 posThis = GetPos();

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
