//============================================
//
//	�I�u�W�F�N�g�w���f���̃T���v�� [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "cross.h"
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
	const int CROSS_DELETTIME = 600;	// �\���ˏ��Ŏ���
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 200.0f);		//�����蔻��
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CCross>* CCross::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CCross::CCross(int nPriority) : CItem(nPriority)
{
	m_nDeletCont = 0;
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CCross::~CCross()
{

}

//====================================================================
//��������
//====================================================================
CCross* CCross::Create(char* pModelName)
{
	CCross* pCross = nullptr;

	if (pCross == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pCross = new CCross();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pCross->Init(pModelName)))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pCross;
}

//====================================================================
//����������
//====================================================================
HRESULT CCross::Init(char* pModelName)
{
	m_nDeletCont = 0;

	SetType(CObject::TYPE_CROSS);

	CItem::Init(pModelName);

	//���[�h���Ƃɏ����l��ݒ�o����
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
	}

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CCross>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CCross::Uninit(void)
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
void CCross::Update(void)
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
void CCross::TitleUpdate(void)
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
void CCross::GameUpdate(void)
{
	// �傫���擾
	float Scaling = GetScaling();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosold();
	D3DXVECTOR3 rot = GetRot();

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
void CCross::Draw(void)
{
	CItem::Draw();
}

//====================================================================
//�@�q�b�g����
//====================================================================
void CCross::Hit(CPlayer* pPlayer)
{
	if (pPlayer->GetItemType() != CPlayer::TYPE_NONE
		&& pPlayer->GetItemType() != CPlayer::TYPE_CROSS)
	{
		return;
	}

	// �v���C���[�̃A�C�e����ݒ�
	pPlayer->SetItemType(CPlayer::TYPE_CROSS);
}

//====================================================================
//��ԊǗ�
//====================================================================
void CCross::StateManager(void)
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

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CCross>* CCross::GetList(void)
{
	return m_pList;
}
