//============================================
//
//	�Q�[���̃v���C���[ [TutorialPlayer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "GamePlayer.h"
#include "object.h"
#include "manager.h"
#include "MyEffekseer.h"
#include "Item.h"
#include "sound.h"
#include "game.h"
#include "LifeUi.h"
#include "number.h"
#include "score.h"
#include "objectBillboard.h"
#include "crossUI.h"
#include "popUI.h"
#include "popUIBg.h"
#include "bowabowa.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	const D3DXVECTOR2 NUMBER_SIZE = { 280.0f , 170.0f };

	const D3DXVECTOR3 LIFE_POS00 = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
	const D3DXVECTOR3 LIFE_POS01 = D3DXVECTOR3(900.0f, 650.0f, 0.0f);
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CGamePlayer::CGamePlayer(int nPriority) : CPlayer(nPriority),
m_pScore(nullptr),
m_pLifeUi(nullptr),
m_pCrossUI(nullptr),
m_pCrossUIBg(nullptr),
m_pPopUIBg(nullptr),
m_pPopUI(nullptr),
m_pP_NumUI(nullptr)
{
	m_Grid.x = 0;
	m_Grid.z = 0;
	m_fVariableSizePopUI = 0.4f;
	m_fSizePopUI = 0.0f;
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

	//�̗�UI�̐���
	if (m_pLifeUi == nullptr)
	{
		m_pLifeUi = CLifeUi::Create();
	}

	//�̗�UI�̐���
	if (m_pScore == nullptr)
	{
		m_pScore = CScore::Create();
	}

	//�\���˃A�C�e���̕ێ����UI�̐���
	if (m_pCrossUI == nullptr)
	{
		m_pCrossUI = CCrossUi::Create();
	}

	//�\���˃A�C�e���̕ێ����UI�w�i�̐���
	if (m_pCrossUIBg == nullptr)
	{
		m_pCrossUIBg = CCrossUi::Create();
	}
	
	//�����o��UI�w�i�̐���
	if (m_pPopUIBg == nullptr)
	{
		m_pPopUIBg = CPopUiBg::Create();
	}

	//�����o��UI�̐���
	if (m_pPopUI == nullptr)
	{
		m_pPopUI = CPopUi::Create();
	}

	//��������UI�̏�����
	InitUI();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CGamePlayer::Uninit(void)
{
	// �L�����N�^�[�N���X�̏I���i�p���j
	CPlayer::Uninit();

	// �X�R�A�̍폜
	if (m_pScore != nullptr)
	{
		m_pScore = nullptr;
	}

	// ���C�t�̍폜
	if (m_pLifeUi != nullptr)
	{
		m_pLifeUi = nullptr;
	}

	// �\����UI�̍폜
	if (m_pCrossUI != nullptr)
	{
		m_pCrossUI = nullptr;
	}

	// �\����UIh�w�i�̍폜
	if (m_pCrossUIBg != nullptr)
	{
		m_pCrossUIBg = nullptr;
	}

	// �����o��UI�w�i�̍폜
	if (m_pPopUIBg != nullptr)
	{
		m_pPopUIBg = nullptr;
	}

	// �����o��UI�̍폜
	if (m_pPopUI != nullptr)
	{
		m_pPopUI = nullptr;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CGamePlayer::Update(void)
{
	float fCrossStatePercent;

	CPlayer::Update();

	//�\����UI�̍X�V
	if (m_pCrossUI != nullptr && m_pCrossUIBg != nullptr)
	{
		if (GetItemType() != TYPE_NONE)
		{//�A�C�e���ێ����
			fCrossStatePercent = GetCrossStateParcent();
		}
		else
		{//�A�C�e����ێ����
			fCrossStatePercent = 0.0f;
		}

		//�\����UI�̃Q�[�W�������f����
		D3DXVECTOR2 SizeDef = m_pCrossUI->GetSizeDefault();

		m_pCrossUI->SetSize(D3DXVECTOR3(SizeDef.x, SizeDef.y * fCrossStatePercent, 0.0f));
		m_pCrossUI->SetPos(D3DXVECTOR3(m_posDefCrossUI.x, m_posDefCrossUI.y + (SizeDef.y - SizeDef.y * fCrossStatePercent) / 2, 0.0f));

		m_pCrossUI->SetAnim(D3DXVECTOR2(0.0f, 1.0 + fCrossStatePercent), D3DXVECTOR2(1.0f, 1.0f));
	}

	//�����o��UI�w�i
	if (m_pPopUIBg != nullptr)
	{
		//�v���C���[�ʒu�ւ̒Ǐ]�X�V
		D3DXVECTOR3 PlayerPos = GetPos();
		m_pPopUIBg->SetPos(D3DXVECTOR3(PlayerPos.x + 20.0f, PlayerPos.y + 100.0f, PlayerPos.z));
		
		m_fSizePopUI += m_fVariableSizePopUI;

		if (m_fSizePopUI >= 15.0f|| m_fSizePopUI<=0.0f)
		{
			m_fVariableSizePopUI = m_fVariableSizePopUI * -1;
		}
		
		m_pPopUIBg->SetHeight(75.0f + m_fSizePopUI);
		m_pPopUIBg->SetWidth(75.0f + m_fSizePopUI);

		if (GetState() == CObjectCharacter::STATE_EGG)
		{
			m_pPopUIBg->SetColorA(0.0f);
		}
		else
		{
			m_pPopUIBg->SetColorA(1.0f);
		}
		
	}

	//�����o��UI
	if (m_pPopUI != nullptr)
	{
		//�v���C���[�ʒu�ւ̒Ǐ]�X�V
		D3DXVECTOR3 PlayerPos = GetPos();
		m_pPopUI->SetPos(D3DXVECTOR3(PlayerPos.x + 20.0f, PlayerPos.y + 100.0f, PlayerPos.z));


		//����Ԃł̓�����
		if (GetState() == CObjectCharacter::STATE_EGG)
		{
			m_pPopUI->SetColorA(0.0f);
		}
		else
		{
			m_pPopUI->SetColorA(1.0f);
		}

		//�\�����e�̍X�V
		if (GetItemType() == TYPE_CROSS)
		{//�A�C�e���ێ����
			m_pPopUI->SetpopUIType(CPopUi::TYPE_BOWABOWA);
		}
		else if(GetItemType() == TYPE_BIBLE)
		{//�A�C�e����ێ����
			m_pPopUI->SetpopUIType(CPopUi::TYPE_DEVILHOLE);
		}
		else
		{
			if (CBowabowa::GetList() != nullptr)
			{
				m_pPopUI->SetpopUIType(CPopUi::TYPE_CROSS);
			}
			else
			{
				m_pPopUI->SetpopUIType(CPopUi::TYPE_BIBLE);
			}
		}
	}

	//�f�o�b�O�L�[�̏����Ɛݒ�
	DebugKey();
}

//====================================================================
//�`�揈��
//====================================================================
void CGamePlayer::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CPlayer::Draw();
}

//====================================================================
//�_���[�W����
//====================================================================
void CGamePlayer::Death(void)
{
	if (GetState() != STATE_EGG && GetState() != STATE_DEATH)
	{
		CPlayer::Death();

		// ���V�G�t�F�N�g
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(GetPos(), GetRot(), mat);
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ACENTION, false, ef, GetRot(), D3DXVECTOR3(100.0f, 100.0f, 100.0f));

		SetLife(GetLife() - 1);

		// �������������Ă���Ƃ��ɂ��̏�ɐ����𗎂Ƃ�
		if (GetItemType() == TYPE_BIBLE)
		{
			// ��������
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GetGrid()));
		}

		if (GetLife() < 0)
		{
			// ���S��
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATH);

			CGame::GetInstance()->SetGameEnd(true);
			CGame::GetInstance()->SetGameClear(false);
			CManager::GetInstance()->SetStage(0);
		}
		else
		{
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
		}

		// �A�C�e�����������Ă��Ȃ���Ԃɂ���
		SetItemType(TYPE_NONE);
	}
}

//====================================================================
// UI�̏�����
//====================================================================
void CGamePlayer::InitUI()
{
	switch (GetPlayNumber())
	{
	case 0:
		if (m_pLifeUi != nullptr)
		{
			// �����̈ʒu
			m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS00.x + 200.0f, LIFE_POS00.y, LIFE_POS00.z));

			// �̗�
			m_pLifeUi->SetPos(LIFE_POS00);
			m_pLifeUi->GetNumber()->SetNumber(GetLife());
		}

		if (m_pScore != nullptr)
		{
			m_pScore->SetPos(D3DXVECTOR3(50.0f, 40.0f, 0.0f));
		}

		//�\����UI
		if (m_pCrossUI != nullptr)
		{
			m_pCrossUI->SetPos(D3DXVECTOR3(LIFE_POS00.x+25, LIFE_POS00.y-110.0f, LIFE_POS00.z));
			m_posDefCrossUI = D3DXVECTOR2(LIFE_POS00.x+25, LIFE_POS00.y - 110.0f);
		}

		//�\����UI�w�i
		if (m_pCrossUIBg != nullptr)
		{
			m_pCrossUIBg->SetPos(D3DXVECTOR3(LIFE_POS00.x + 25, LIFE_POS00.y - 110.0f, LIFE_POS00.z));
			m_pCrossUIBg->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		}

		//�����o��UI�w�i
		if (m_pPopUIBg != nullptr)
		{
			D3DXVECTOR3 PlayerPos=GetPos();
			m_pPopUIBg->SetPos(D3DXVECTOR3(PlayerPos.x+20.0f, PlayerPos.y - 100.0f, PlayerPos.z));
		}

		//�����o��UI
		if (m_pPopUI != nullptr)
		{
			D3DXVECTOR3 PlayerPos = GetPos();
			m_pPopUI->SetPos(D3DXVECTOR3(PlayerPos.x + 20.0f, PlayerPos.y - 100.0f, PlayerPos.z));
		}

		if (m_pP_NumUI != nullptr)
		{
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetWidth(NUMBER_SIZE.x);
			m_pP_NumUI->SetHeight(NUMBER_SIZE.y);
			m_pP_NumUI->SetTexture("data\\TEXTURE\\UI\\1p.png");
		}

		break;

	case 1:

		if (m_pLifeUi != nullptr)
		{
			// �����̈ʒu
			m_pLifeUi->GetNumber()->SetPos(D3DXVECTOR3(LIFE_POS01.x + 200.0f, LIFE_POS01.y, LIFE_POS01.z));

			// �̗�
			m_pLifeUi->SetPos(LIFE_POS01);
			m_pLifeUi->GetNumber()->SetNumber(GetLife());
		}

		if (m_pScore != nullptr)
		{
			m_pScore->SetPos(D3DXVECTOR3(1050.0f, 40.0f, 0.0f));
		}

		if (m_pP_NumUI != nullptr)
		{
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetPos(GetPos());
			m_pP_NumUI->SetWidth(NUMBER_SIZE.x);
			m_pP_NumUI->SetHeight(NUMBER_SIZE.y);
			m_pP_NumUI->SetTexture("data\\TEXTURE\\UI\\2p.png");
		}

		break;
	}
}
//====================================================================
// �f�o�b�O�{�^��
//====================================================================
void CGamePlayer::DebugKey(void)
{
#ifdef _DEBUG

	int nLife = GetLife();

	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�L�[�{�[�h�̈ړ�����
	if (pInputKeyboard->GetTrigger(DIK_3))
	{
		++nLife;
		m_pLifeUi->GetNumber()->SetNumber(nLife);
	}
	if (pInputKeyboard->GetTrigger(DIK_4))
	{
		--nLife;
		m_pLifeUi->GetNumber()->SetNumber(nLife);
	}

	SetLife(nLife);

#endif // !_DEBUG
}
