//============================================
//
//	�`���[�g���A���̃v���C���[ [TutorialPlayer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "TutorialPlayer.h"
#include "object.h"
#include "manager.h"
#include "MyEffekseer.h"

//===========================================
// �萔��`
//===========================================
namespace
{

}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CTutorialPlayer>* CTutorialPlayer::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CTutorialPlayer::CTutorialPlayer(int nPriority) : CPlayer(nPriority)
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CTutorialPlayer::~CTutorialPlayer()
{

}

//====================================================================
//��������
//====================================================================
CTutorialPlayer* CTutorialPlayer::Create(int PlayNumber)
{
	CTutorialPlayer* pPlayer = new CTutorialPlayer();

	// �������̊m�ۂɎ��s�����ꍇnull��Ԃ�
	if (pPlayer == nullptr) { assert(false); return nullptr; }

	// �����������Ɏ��s�����ꍇnull��Ԃ�
	if (FAILED(pPlayer->Init(PlayNumber)))
	{
		assert(false);
		delete pPlayer;
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//����������
//====================================================================
HRESULT CTutorialPlayer::Init(int PlayNumber)
{
	CPlayer::Init(PlayNumber);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CTutorialPlayer::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �L�����N�^�[�N���X�̏I���i�p���j
	CPlayer::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CTutorialPlayer::Update(void)
{
	CPlayer::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CTutorialPlayer::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CPlayer::Draw();
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CTutorialPlayer>* CTutorialPlayer::GetList(void)
{
	return m_pList;
}
