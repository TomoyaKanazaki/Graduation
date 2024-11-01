//============================================
//
//	�{���{���̏��� [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "bowabowa.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "game.h"
#include "score.h"
#include "player.h"

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
CListManager<CBowabowa>* CBowabowa::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CBowabowa::CBowabowa(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CBowabowa::~CBowabowa()
{

}

//====================================================================
//��������
//====================================================================
CBowabowa* CBowabowa::Create(char* pModelName)
{
	CBowabowa* pBowabowa = nullptr;

	if (pBowabowa == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pBowabowa = new CBowabowa();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pBowabowa->Init(pModelName)))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pBowabowa;
}

//====================================================================
//����������
//====================================================================
HRESULT CBowabowa::Init(char* pModelName)
{
	SetType(CObject::TYPE_BOWABOWA);

	CItem::Init(pModelName);

	//���[�h���Ƃɏ����l��ݒ�o����
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:
		CGame::AddBowabowa(1);
		break;

	case CScene::MODE_RESULT:
		break;
	}

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBowabowa>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CBowabowa::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	CItem::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CBowabowa::Update(void)
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
void CBowabowa::TitleUpdate(void)
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
//�Q�[���ł̍X�V����
//====================================================================
void CBowabowa::GameUpdate(void)
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
}

//====================================================================
//�`�揈��
//====================================================================
void CBowabowa::Draw(void)
{
	CItem::Draw();
}

//====================================================================
//��ԊǗ�
//====================================================================
void CBowabowa::StateManager(void)
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

//====================================================================
//��ԊǗ�
//====================================================================
void CBowabowa::Take(void)
{
	CGame::AddBowabowa(-1);
	CScore *pScore = CGame::GetScore();
	pScore->AddScore(100);
	Uninit();
}

//==========================================
// �q�b�g����
//==========================================
void CBowabowa::Hit(CPlayer* pPlayer)
{
	// �\���˂������ĂȂ��ꍇ�֐��𔲂���
	if (pPlayer->GetItemType() != CPlayer::TYPE_CROSS)
	{
		return;
	}

	// �폜
	Uninit();
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CBowabowa>* CBowabowa::GetList(void)
{
	return m_pList;
}
