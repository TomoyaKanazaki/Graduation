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
	const char* MODEL_PASS = "data\\MODEL\\zyuzika.x"; // ���f���p�X
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
//����������
//====================================================================
HRESULT CCross::Init()
{
	SetType(CObject::TYPE_CROSS);

	CItem::Init(MODEL_PASS);

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
bool CCross::Hit(CPlayer* pPlayer)
{
	if (pPlayer->GetItemType() != CPlayer::TYPE_NONE
		&& pPlayer->GetItemType() != CPlayer::TYPE_CROSS)
	{
		return false;
	}

	// �v���C���[�̃A�C�e����ݒ�
	pPlayer->SetItemType(CPlayer::TYPE_CROSS);
	return true;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CCross>* CCross::GetList(void)
{
	return m_pList;
}
