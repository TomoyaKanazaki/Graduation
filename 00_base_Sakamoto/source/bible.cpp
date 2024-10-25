//============================================
//
//	�����̏��� [bible.cpp]
//	Author:morikawa shunya
//
//============================================
#include "bible.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "MapSystem.h"
#include "effect.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CBible>* CBible::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CBible::CBible(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_fMove = 0.0f;
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CBible::~CBible()
{

}

//====================================================================
// ����
//====================================================================
CBible* CBible::Create(char* pModelName)
{
	// �C���X�^���X����
	CBible* pBible = new CBible;

	// ������
	pBible->Init(pModelName);

	return pBible;
}

//====================================================================
// ������
//====================================================================
HRESULT CBible::Init(char* pModelName)
{
	// �p���N���X�̏�����
	CItem::Init(pModelName);

	SetType(CObject::TYPE_BIBLE);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBible>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CBible::Uninit(void)
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
void CBible::Update(void)
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
// �^�C�g���ł̍X�V
//====================================================================
void CBible::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	//�ʒu�X�V
	pos += move;

	SetPos(pos);

	//���_���̍X�V
	CItem::Update();
}

//====================================================================
// �Q�[���ł̍X�V
//====================================================================
void CBible::GameUpdate(void)
{
	//��ԊǗ�
	StateManager();

	//�e�N���X�̍X�V
	CItem::Update();

	// �v���C���[�Ƃ̔���
	CollisionPlayer();
}

//====================================================================
// �`��
//====================================================================
void CBible::Draw(void)
{
	// �p���N���X�̕`��
	CItem::Draw();
}

//====================================================================
// �v���C���[�Ƃ̔���
//====================================================================
bool CBible::CollisionPlayer()
{
	if (!CItem::CollisionPlayer())
	{
		return false;
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();	// ��ނ��擾

			if (type != TYPE_PLAYER3D)
			{
				pObj = pObjNext;
				continue;
			}

			CPlayer* pPlayer = (CPlayer*)pObj;		// �A�C�e���̏��̎擾

			if (pPlayer->GetItemType() == CPlayer::ITEM_TYPE::TYPE_BIBLE ||
				pPlayer->GetState() == CPlayer::STATE_DEATH)
			{
				pObj = pObjNext;
				continue;
			}

			D3DXVECTOR3 pos = pObj->GetPos();
			D3DXVECTOR3 Size = pObj->GetSize();

			// �����ɐݒ�
			pPlayer->SetItemType(CPlayer::TYPE_BIBLE);

			//// �A�C�e���̈ʒu���v���C���[�Ɠ����ʒu�ɐݒ�
			//SetPos(pos);

			Uninit();
			return true;

			pObj = pObjNext;
		}
	}

	return true;
}

//====================================================================
// �����̐���
//====================================================================
void CBible::Move()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosold();
	D3DXVECTOR3 rot = GetRot();

	m_fMove += D3DX_PI * 0.01f;

	D3DXVECTOR3 GritPos = INITVECTOR3;
	float MaxX = 0.0f;
	float MaxZ = 0.0f;

	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	posOld = pos;

	switch (m_PosType)
	{
	case CBible::POS_TYPE_LEFTUP:

		GritPos = CMapSystem::GetInstance()->GetStartGritPos(1.5f, 1.5f);

		pos.x = GritPos.x - sinf(m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;
		pos.y = 0.0f;
		pos.z = GritPos.z + sinf(m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		break;
	case CBible::POS_TYPE_RIGHTUP:

		MaxX = (float)CMapSystem::GetInstance()->GetWightMax();
		GritPos = CMapSystem::GetInstance()->GetStartGritPos(MaxX - 1.5f, 1.5f);

		pos.x = GritPos.x + sinf(m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;
		pos.y = 0.0f;
		pos.z = GritPos.z + sinf(m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		break;
	case CBible::POS_TYPE_LEFTDOWN:

		MaxZ = (float)CMapSystem::GetInstance()->GetHeightMax();
		GritPos = CMapSystem::GetInstance()->GetStartGritPos(1.5f, MaxZ - 1.5f);

		pos.x = GritPos.x + sinf(-m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;
		pos.y = 0.0f;
		pos.z = GritPos.z + sinf(-m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		break;
	case CBible::POS_TYPE_RIGHTDOWN:

		MaxX = (float)CMapSystem::GetInstance()->GetWightMax();
		MaxZ = (float)CMapSystem::GetInstance()->GetHeightMax();
		GritPos = CMapSystem::GetInstance()->GetStartGritPos(MaxX - 1.5f, MaxZ - 1.5f);

		pos.x = GritPos.x - sinf(-m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;
		pos.y = 0.0f;
		pos.z = GritPos.z + sinf(-m_fMove) * CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		break;

	default:
		break;
	}

	float Scaling = GetScaling();

#ifdef _DEBUG

	CEffect* pEffect = CEffect::Create();
	pEffect->SetPos(pos);
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pEffect->SetRadius(50.0f);

#endif // _DEBUG

	//�ʒu�X�V
	CItem::SetPos(pos);
	CObjectX::SetPos(pos);
	CObjectX::SetRot(rot);

	//�傫���̐ݒ�
	SetScaling(Scaling);
}

//====================================================================
//��ԊǗ�
//====================================================================
void CBible::StateManager(void)
{
	CItem::STATE State = GetState();

	int nStateCounter = GetStateCounter();

	switch (State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (nStateCounter > 0)
	{
		nStateCounter--;
	}
}
