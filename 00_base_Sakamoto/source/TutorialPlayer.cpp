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
	const char* CHECK_MARKER_TEX = "data\\TEXTURE\\UI\\tutorial_check.png";	// �`�F�b�N�}�[�J�[�e�N�X�`��

	const D3DXVECTOR3 MARKER_TEX_POS = D3DXVECTOR3(50.0f, 160.0f, 0.0f);	// �}�[�J�[�ʒu
	const D3DXVECTOR3 MARKER_TEX_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);	// �}�[�J�[�T�C�Y

}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================

//====================================================================
//�R���X�g���N�^
//====================================================================
CTutorialPlayer::CTutorialPlayer(int nPriority) : CPlayer(nPriority),
m_pCheckMaker(nullptr)
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

	switch (GetItemType())
	{
	case TYPE_CROSS:

		if (m_pCheckMaker == nullptr)
		{// �`�F�b�N�}�[�J�[����
			m_pCheckMaker = CObject2D::Create();
		}
		if (m_pCheckMaker != nullptr)
		{// �e�N�X�`�������E�ʒu�E�T�C�Y�ݒ�
			m_pCheckMaker->SetTexture(CHECK_MARKER_TEX);
			m_pCheckMaker->SetPos(MARKER_TEX_POS);
			m_pCheckMaker->SetSize(MARKER_TEX_SIZE);
		}

		break;

	case TYPE_BIBLE:

		if (m_pCheckMaker == nullptr)
		{// �`�F�b�N�}�[�J�[����
			m_pCheckMaker = CObject2D::Create();
		}
		if (m_pCheckMaker != nullptr)
		{// �e�N�X�`�������E�ʒu�E�T�C�Y�ݒ�
			m_pCheckMaker->SetTexture(CHECK_MARKER_TEX);
			m_pCheckMaker->SetPos(D3DXVECTOR3(50.0f, 0.0f, 0.0f));
			m_pCheckMaker->SetSize(MARKER_TEX_SIZE);
		}

		break;

	default:
		break;
	}
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

		if (GetLifeUI() != nullptr)
		{
			GetLifeUI()->GetNumber()->SetNumber(GetLife());
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