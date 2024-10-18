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

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
}

//====================================================================
// �R���X�g���N�^
//====================================================================
CBible::CBible(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
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

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CBible::Uninit(void)
{
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
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosold();
	D3DXVECTOR3 rot = GetRot();

	float Scaling = GetScaling();

	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	posOld = pos;

	//�ʒu�X�V
	CObjectX::SetPos(pos);
	CObjectX::SetRot(rot);

	//��ʊO����
	if (pos.y < 0.0f)
	{

	}

	//�傫���̐ݒ�
	SetScaling(Scaling);

	//��ԊǗ�
	StateManager();

	//���_���̍X�V
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

			D3DXVECTOR3 pos = pObj->GetPos();
			D3DXVECTOR3 Size = pObj->GetSize();

			// �������ǂ���
			pPlayer->SetItemType(CPlayer::TYPE_BIBLE);

			// �w��p�[�c�\��
			pPlayer->SetPartsDisp(0, true);

			// �A�C�e���̈ʒu���v���C���[�Ɠ����ʒu�ɐݒ�
			SetPos(pos);

			pObj = pObjNext;
		}
	}

	return true;
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
