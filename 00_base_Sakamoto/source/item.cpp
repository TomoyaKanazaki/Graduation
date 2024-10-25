//============================================
//
//	�A�C�e���̏��� [item.cpp]
//	Author:morikawa shunya
//
//============================================
#include "item.h"
#include "cross.h"
#include "bowabowa.h"
#include "player.h"
#include "useful.h"
#include "effect.h"
#include "MapSystem.h"

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
	m_nMapWight = 1;			// �}�b�v�̉��ԍ�
	m_nMapHeight = 0;			// �}�b�v�̏c�ԍ�
	m_bMapScroll = true;
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

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetMultiMatrix(true);

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
	if (m_bMapScroll == true)
	{
		m_pos = CMapSystem::GetInstance()->GetGritPos(m_nMapWight, m_nMapHeight);
		m_pos.y = 0.0f;

#ifdef _DEBUG

		CEffect* pEffect = CEffect::Create();
		pEffect->SetPos(m_pos);
		pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		pEffect->SetRadius(50.0f);

#endif // _DEBUG

		CObjectX::SetPos(m_pos);

		// �p���N���X�̍X�V
		CObjectX::Update();
	}
	else
	{
		Move();
	}
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
bool CItem::CollisionPlayer()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();	// ��ނ��擾

			if(type != TYPE_PLAYER3D)
			{
				pObj = pObjNext;
				continue;
			}

			CPlayer* pPlayer = (CPlayer*)pObj;		// �A�C�e���̏��̎擾

			D3DXVECTOR3 pos = pObj->GetPos();
			D3DXVECTOR3 Size = pObj->GetSize();

			// ��`�̓����蔻��
			if (useful::CollisionCircle(m_pos, pos, Size.x))
			{
				return true;
			}

			pObj = pObjNext;
		}
	}

	return false;
}
