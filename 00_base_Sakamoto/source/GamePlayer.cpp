//============================================
//
//	�`���[�g���A���̃v���C���[ [TutorialPlayer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "GamePlayer.h"
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
CListManager<CGamePlayer>* CGamePlayer::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CGamePlayer::CGamePlayer(int nPriority) : CPlayer(nPriority)
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CGamePlayer::~CGamePlayer()
{

}

//====================================================================
//��������
//====================================================================
CGamePlayer* CGamePlayer::Create(int PlayNumber)
{
	CGamePlayer* pPlayer = new CGamePlayer();

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
HRESULT CGamePlayer::Init(int PlayNumber)
{
	CPlayer::Init(PlayNumber);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CGamePlayer::Uninit(void)
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
void CGamePlayer::Update(void)
{
	CPlayer::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CGamePlayer::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CPlayer::Draw();
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CGamePlayer>* CGamePlayer::GetList(void)
{
	return m_pList;
}
