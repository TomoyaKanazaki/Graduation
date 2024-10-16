//============================================
//
//	�A�C�e���̏��� [item.cpp]
//	Author:morikawa shunya
//
//============================================
#include "item.h"
#include "cross.h"
#include "bowabowa.h"

//====================================================================
// �R���X�g���N�^
//====================================================================
CItem::CItem(int nPriority) : CObjectX(nPriority)
{
	m_eState = STATE_NORMAL;	// ���
	m_eType = TYPE_NONE;		// ���
	m_nIdxXModel = 0;			// X���f���ԍ�
	m_nIdxTexture = -1;			// �e�N�X�`���ԍ�
	m_nStateCount = 0;			// ��ԊǗ�
	m_fColorA = 0.0f;			// �s�����x
	m_Scaling = 0.0f;			// �傫��
	m_bCollision = false;		// �����蔻��
	m_CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ������W
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ߋ��̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CItem::~CItem()
{

}

//====================================================================
// ����
//====================================================================
CItem* CItem::Create(TYPE eType, const D3DXVECTOR3& pos)
{
	// �C���X�^���X����
	CItem* pItem = nullptr;

	if (pItem == nullptr)
	{
		switch (eType)
		{
		case CItem::TYPE_CROSS:		// �\����
			pItem = new CCross;
			break;

		case CItem::TYPE_BIBLE:		// ����
			// pItem = new CBible;
			break;

		case CItem::TYPE_BOWABOWA:	// �ڂ�ڂ�
			pItem = new CBowabowa;
			break;

		default:
			assert(false);
			break;
		}
	}

	// �ʒu
	pItem->SetPos(pos);

	return pItem;
}

//====================================================================
// ������
//====================================================================
HRESULT CItem::Init(char* pModelName)
{
	// �p���N���X�̏�����
	CObjectX::Init(pModelName);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CItem::Uninit()
{
	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//====================================================================
// �X�V
//====================================================================
void CItem::Update()
{
	// �p���N���X�̍X�V
	CObjectX::Update();
}

//====================================================================
// �`��
//====================================================================
void CItem::Draw()
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}

//====================================================================
// �v���C���[�Ƃ̓����蔻��
//====================================================================
void CItem::CollisionPlayer()
{

}
