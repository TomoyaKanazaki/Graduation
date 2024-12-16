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
#include "Item.h"
#include "sound.h"
#include "game.h"
#include "LifeUi.h"
#include "number.h"

//===========================================
// �萔��`
//===========================================
namespace
{
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================

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

//====================================================================
//�_���[�W����
//====================================================================
void CTutorialPlayer::Death(void)
{
	if (GetState() != STATE_EGG && GetState() != STATE_DEATH)
	{
		CPlayer::Death();

		// �������������Ă���Ƃ��ɂ��̏�ɐ����𗎂Ƃ�
		if (GetItemType() == TYPE_BIBLE)
		{
			// ��������
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GetGrid()));
		}

		SetState(STATE_DEATH);
		SetMove(INITVECTOR3);
		SetObjMoveX(0.0f);
		SetObjMoveZ(0.0f);
		SetStateCount(150);

		// �_���[�W��(��)
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATH);

		// �A�C�e�����������Ă��Ȃ���Ԃɂ���
		SetItemType(TYPE_NONE);
	}
}