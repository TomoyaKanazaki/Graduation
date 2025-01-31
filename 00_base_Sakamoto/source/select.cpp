
//============================================
//
//	�I����� [select.cpp]
//	Author:sakamoto kai
//
//============================================
#include "select.h"
#include "fade.h"
#include "object2D.h"
#include "texture.h"
#include "objmeshDome.h"
#include "objmeshField.h"
#include "light.h"
#include "Devil.h"
#include "MapMove.h"
#include "sound.h"
#include "number.h"
#include "MapSystem.h"

// �萔��`
namespace
{
	const char* MAP_BLOCK_PASS("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	const char* MAP_XMODEL_PASS("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	const char* MAP_GIMMICK_PASS("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");

	const D3DXVECTOR3 STAGE_POS = D3DXVECTOR3(490.0f, 80.0f, 0.0f);		// �I�����ڂ̈ʒu
	const D3DXVECTOR3 MAP_POS = D3DXVECTOR3(640.0f, 250.0f, 0.0f);			// �T���l�C���̈ʒu
	const D3DXVECTOR2 STAGE_SIZE = D3DXVECTOR2(130.0f, 160.0f);				// �I�����ڂ̑傫��
	const D3DXVECTOR2 MAP_SIZE = D3DXVECTOR2(400.0f, 300.0f);				// �T���l�C���̑傫��
	const D3DXVECTOR2 NUMBER_SIZE = D3DXVECTOR2(35.0f, 35.0f);				// �T���l�C���̑傫��
	const D3DXVECTOR2 STAGE_DISTANCE = D3DXVECTOR2(300.0f + (STAGE_SIZE.x * 1.0f), 0.0f + (STAGE_SIZE.y * 0.0f));	// �I�����ڂ̕�
	const float NUMBER_DISTANCE = 80.0f;	// �X�e�[�W�Ɛ����̕�

	const D3DXVECTOR3 SCROOL_POS = D3DXVECTOR3(440.0f, 500.0f, 0.0f);		// �I�����ڂ̈ʒu
	const D3DXVECTOR2 SCROOL_SIZE = D3DXVECTOR2(300.0f, 50.0f);				// �I�����ڂ̑傫��
	const D3DXVECTOR2 SCROOL_DISTANCE = D3DXVECTOR2(100.0f + (STAGE_SIZE.x * 1.0f), 0.0f + (STAGE_SIZE.y * 0.0f));	// �I�����ڂ̕�

	const D3DXCOLOR SELECT_COLOR_TRUE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �I����ԍ��ڂ̐F
	const D3DXCOLOR SELECT_COLOR_FALSE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// �I�����ĂȂ����ڂ̐F

	const D3DXVECTOR3 BUTTON_POS = D3DXVECTOR3(640.0f, 670.0f, 0.0f);		// �{�^���̈ʒu
	const D3DXVECTOR2 BUTTON_SIZE = { 300.0f, 60.0f };						// �{�^���̑傫��

	const float DOME_ROT_SPEED = 0.001f;	// ���b�V���h�[���̉�]���x

	const float SCROOL_SPEED = 0.05f; // �X�N���[���̑��x
	const float SCROOL_SCALE = 0.1f; // �X�N���[���̔{��
}

//�ÓI�����o�ϐ��錾
CSelect* CSelect::m_pSelect = nullptr;
std::vector<CSelect::SStageSelect> CSelect::m_StageSelect = {};	// �}�b�v�̏��

//====================================================================
//�R���X�g���N�^
//====================================================================
CSelect::CSelect()
{
	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		m_pScrollSelect[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		m_pTexScroll[nCnt] = nullptr;
	}
	m_pTitleButton = nullptr;
	m_pMeshDome = nullptr;

	m_fCountFade = 0.0f;
	m_bStart = false;
	m_bTurn = false;
	m_bSlash = false;
	m_nSelect = 0;
	m_nStep = 0;
	m_nSetStage = 0;
	m_nTime = 0;
	m_fTex[0] = 0.0f;
	m_fTex[1] = 0.0f;
	m_Type = SCROLL_NONE;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSelect::~CSelect()
{
	m_StageSelect.clear();
}


//====================================================================
//�C���X�^���X�擾
//====================================================================
CSelect* CSelect::GetInstance(void)
{
	if (m_pSelect == nullptr)
	{
		m_pSelect = new CSelect;
	}
	return m_pSelect;
}

//====================================================================
//����������
//====================================================================
HRESULT CSelect::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �}�b�v���擾
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	std::vector<CMapSystem::MapInfo> MapInfo = pMapSystem->GetMapInfo();
	int nMax = MapInfo.size();

	// �S�ẴX�e�[�W���̑I��������
	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		// �I����������₷
		m_StageSelect.emplace_back();

		// �}�b�v�T���l�C��
		m_StageSelect.back().m_pMapTex = CObject2D::Create();
		m_StageSelect.back().m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x + (STAGE_DISTANCE.x * nCnt), MAP_POS.y + (STAGE_DISTANCE.y * nCnt), MAP_POS.z));
		m_StageSelect.back().m_pMapTex->SetSize(D3DXVECTOR3(MAP_SIZE.x, MAP_SIZE.y, 0.0f));
		m_StageSelect.back().m_pMapTex->SetIdx(pTexture->Regist(MapInfo[nCnt].texture));

		// �{�^��
		m_StageSelect.back().m_pSelectUI = CObject2D::Create();
		m_StageSelect.back().m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x + (STAGE_DISTANCE.x * nCnt), STAGE_POS.y + (STAGE_DISTANCE.y * nCnt), STAGE_POS.z));
		m_StageSelect.back().m_pSelectUI->SetSize(D3DXVECTOR3(STAGE_SIZE.x, STAGE_SIZE.y, 0.0f));

		// ����(�P)
		m_StageSelect.back().m_pSelectNumber01 = CNumber::Create();		// ����
		m_StageSelect.back().m_pSelectNumber01->SetPos(D3DXVECTOR3(		// �ʒu
			m_StageSelect.back().m_pSelectUI->GetPos().x + NUMBER_DISTANCE + 32.0f,
			m_StageSelect.back().m_pSelectUI->GetPos().y,
			m_StageSelect.back().m_pSelectUI->GetPos().z));
		m_StageSelect.back().m_pSelectNumber01->SetSize(D3DXVECTOR3(NUMBER_SIZE.x, NUMBER_SIZE.y, 0.0f));		// �T�C�Y
		m_StageSelect.back().m_pSelectNumber01->SetNumber(nCnt + 1);							// ����

		// ����(�P�O)
		m_StageSelect.back().m_pSelectNumber10 = CNumber::Create();		// ����
		m_StageSelect.back().m_pSelectNumber10->SetPos(D3DXVECTOR3(		// �ʒu
			m_StageSelect.back().m_pSelectUI->GetPos().x + NUMBER_DISTANCE,
			m_StageSelect.back().m_pSelectUI->GetPos().y,
			m_StageSelect.back().m_pSelectUI->GetPos().z));
		m_StageSelect.back().m_pSelectNumber10->SetSize(D3DXVECTOR3(NUMBER_SIZE.x, NUMBER_SIZE.y, 0.0f));		// �T�C�Y
		m_StageSelect.back().m_pSelectNumber10->SetNumber((nCnt + 1) / 10);						// ����

		if (m_StageSelect.back().m_pSelectUI != nullptr)
		{ // �{�^��UI����������Ă���

			// Stage�e�N�X�`��
			m_StageSelect.back().m_pSelectUI->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\stage00.png"));
		}
	}

	// �Ō�̑I�����̔ԍ�
	int nLast = nMax - 1;

	// �Ō���ŏ��Ɏ����Ă���
	m_StageSelect[nLast].m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x - STAGE_DISTANCE.x, MAP_POS.y - STAGE_DISTANCE.y, MAP_POS.z));
	m_StageSelect[nLast].m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x - STAGE_DISTANCE.x, STAGE_POS.y - STAGE_DISTANCE.y, STAGE_POS.z));

	// ���[�h�p�I����
	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		if (m_pScrollSelect[nCnt] == nullptr)
		{
			// �{�^��
			m_pScrollSelect[nCnt] = CObject2D::Create();
			m_pScrollSelect[nCnt]->SetPos(D3DXVECTOR3(300.0f + (500.0f * nCnt), SCROOL_POS.y + (SCROOL_DISTANCE.y * nCnt), SCROOL_POS.z));
			m_pScrollSelect[nCnt]->SetSize(D3DXVECTOR3(400.0f, 200.0f, 0.0f));
			m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);
		}
		if (m_pScrollSelect[nCnt] != nullptr)
		{
			switch (nCnt)
			{
			case 0:
				m_pScrollSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\ScrollSelect00.png"));
				break;
			case 1:
				m_pScrollSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\ScrollSelect01.png"));
				break;
			}
		}
	}

	// ���[�h�p�̎Q�l�摜
	for (int nCnt = 0; nCnt < NUM_SCROLL; nCnt++)
	{
		if (m_pTexScroll[nCnt] == nullptr)
		{
			// �X�N���[��
			m_pTexScroll[nCnt] = CObject2D::Create();
			m_pTexScroll[nCnt]->SetPos(D3DXVECTOR3(550.0f+ (470.0f * nCnt), SCROOL_POS.y + (SCROOL_DISTANCE.y * nCnt), SCROOL_POS.z));
			m_pTexScroll[nCnt]->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		}
		// �X�N���[��
		if (m_pTexScroll[nCnt] != nullptr)
		{
			switch (nCnt)
			{
			case 0:
				m_pTexScroll[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\scroll.png"));
				break;
			case 1:
				m_pTexScroll[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\scroll.png"));
				break;
			}
		}
	}

	// ����{�^��
	if (m_pTitleButton == nullptr)
	{
		m_pTitleButton = CObject2D::Create();
		m_pTitleButton->SetPos(BUTTON_POS);
		m_pTitleButton->SetSize(D3DXVECTOR3(600.0f, 200.0f, 0.0f));
		//m_pTitleButton->SetColor(SELECT_COLOR_TRUE);
	}

	if (m_pTitleButton != nullptr)
	{
		m_pTitleButton->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\pressbotton.png"));
	}

	// ���C�g�̏�����
	CManager::GetInstance()->GetLight()->Init();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CSelect::Uninit(void)
{
	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	if (m_pSelect != nullptr)
	{
		delete m_pSelect;
		m_pSelect = nullptr;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CSelect::Update(void)
{
	// �X�N���[������
	m_fTex[0] += SCROOL_SPEED * SCROOL_SCALE;

	// �e�N�X�`��[0]�ړ��ʉ��Z
	m_pTexScroll[0]->SetScroll(D3DXVECTOR2(m_fTex[0],1.0f));

	m_nTime++;

	if (m_nTime >= 20)
	{
		// �e�N�X�`��[1]�ړ��ʉ��Z
		m_fTex[1] += SCROOL_SPEED;
		m_pTexScroll[1]->SetScroll(D3DXVECTOR2(m_fTex[1], 1.0f));

		m_nTime = 0;
	}

	if (CManager::GetInstance()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (m_nStep == 0)
		{
			// �I������
			StageSelect();

			// ���菈��
			StageButton();
		}
		else
		{
			// �I������
			ScrollSelect();

			// ���菈��
			ScrollButton();
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CSelect::Draw(void)
{

}

//====================================================================
//�I������
//====================================================================
void CSelect::StageSelect(void)
{
	int nMax = CMapSystem::GetInstance()->GetMapInfo().size();

	// �E
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_D) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_RIGHT, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 1) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_RIGHT, 1) == true)
	{
		m_nSelect++;

		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		// �ő�z������ŏ��ɖ߂�
		if (m_nSelect >= nMax)
		{
			m_nSelect = 0;
		}
	}
	// ��
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_A) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_LEFT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_LEFT, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 1) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_LEFT, 1) == true)
	{
		m_nSelect--;

		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		// �ŏ���菬����������ő�ɖ߂�
		if (m_nSelect < 0)
		{
			m_nSelect = nMax - 1;
		}
	}

	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		if (m_nSelect == nCnt)
		{
			m_StageSelect[nCnt].m_pSelectUI->SetColor(SELECT_COLOR_TRUE);
			m_StageSelect[nCnt].m_pMapTex->SetColor(SELECT_COLOR_TRUE);

		}
		else
		{
			m_StageSelect[nCnt].m_pSelectUI->SetColor(SELECT_COLOR_FALSE);
			m_StageSelect[nCnt].m_pMapTex->SetColor(SELECT_COLOR_FALSE);
		}
	}

	int nLast = nMax - 1;		// �Ō�̑I�����̔ԍ�
	int nStart = -nLast;		// �ŏ��̑I�����̔ԍ�

	// �ʒu�Ƃ�
	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		int nNumber = nCnt - m_nSelect;
		//if (m_nSelect - 2 <= nCnt && nCnt <= m_nSelect + 2)
		//{//�d���Ȃ������p
			if (m_StageSelect[nCnt].m_pSelectUI != nullptr)
			{
				// �{�^��
				m_StageSelect[nCnt].m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x + (STAGE_DISTANCE.x * (nNumber)), STAGE_POS.y + (STAGE_DISTANCE.y * nCnt), STAGE_POS.z));
				//m_pStageSelect[nCnt].m_pSelectUI->SetSize(D3DXVECTOR3(500.0f, STAGE_SIZE.y, 0.0f));
			}

			if (m_StageSelect[nCnt].m_pMapTex != nullptr)
			{
				// �T���l�C��
				m_StageSelect[nCnt].m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x + (STAGE_DISTANCE.x * (nNumber)), MAP_POS.y + (STAGE_DISTANCE.y * nCnt), MAP_POS.z));
				//m_pStageSelect[nCnt].m_pSelectUI->SetSize(D3DXVECTOR3(500.0f, STAGE_SIZE.y, 0.0f));
				
				if (nNumber == nLast)
				{ // �ŏ����I������Ă�ꍇ�A�Ō���ŏ��̍���

					// �Ō���ŏ��Ɏ����Ă���
					m_StageSelect[nLast].m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x - STAGE_DISTANCE.x, MAP_POS.y - STAGE_DISTANCE.y, MAP_POS.z));
					m_StageSelect[nLast].m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x - STAGE_DISTANCE.x, STAGE_POS.y - STAGE_DISTANCE.y, STAGE_POS.z));
				}
				else if (nNumber == nStart)
				{ // �Ōオ�I������Ă�ꍇ�A�ŏ����Ō�̉E��

					// �ŏ����Ō�Ɏ����Ă���
					m_StageSelect[0].m_pMapTex->SetPos(D3DXVECTOR3(MAP_POS.x + (STAGE_DISTANCE.x * 1), MAP_POS.y + (STAGE_DISTANCE.y * nCnt), MAP_POS.z));
					m_StageSelect[0].m_pSelectUI->SetPos(D3DXVECTOR3(STAGE_POS.x + (STAGE_DISTANCE.x * 1), STAGE_POS.y + (STAGE_DISTANCE.y * nCnt), STAGE_POS.z));
				}
			}

			if (m_StageSelect[nCnt].m_pSelectNumber01 != nullptr)
			{
				m_StageSelect[nCnt].m_pSelectNumber01->SetPos(D3DXVECTOR3(
					m_StageSelect[nCnt].m_pSelectUI->GetPos().x + NUMBER_DISTANCE + 32.0f,
					m_StageSelect[nCnt].m_pSelectUI->GetPos().y,
					m_StageSelect[nCnt].m_pSelectUI->GetPos().z));

				if (m_nSelect == nCnt)
				{
					m_StageSelect[nCnt].m_pSelectNumber01->SetColor(SELECT_COLOR_TRUE);
				}
				else
				{
					m_StageSelect[nCnt].m_pSelectNumber01->SetColor(SELECT_COLOR_FALSE);
				}
			}

			if (m_StageSelect[nCnt].m_pSelectNumber10 != nullptr)
			{
				m_StageSelect[nCnt].m_pSelectNumber10->SetPos(D3DXVECTOR3(
					m_StageSelect[nCnt].m_pSelectUI->GetPos().x + NUMBER_DISTANCE,
					m_StageSelect[nCnt].m_pSelectUI->GetPos().y,
					m_StageSelect[nCnt].m_pSelectUI->GetPos().z));

				if (m_nSelect == nCnt)
				{
					m_StageSelect[nCnt].m_pSelectNumber10->SetColor(SELECT_COLOR_TRUE);
				}
				else
				{
					m_StageSelect[nCnt].m_pSelectNumber10->SetColor(SELECT_COLOR_FALSE);
				}
			}
		//}
	}
}

//====================================================================
//���菈��
//====================================================================
void CSelect::StageButton(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 1) == true)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		m_nSetStage = m_nSelect;
		m_nSelect = 0;
		m_nStep++;
	}
	else if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 1) == true)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);
		CScene::SetMode(MODE_TITLE);
	}
}

//====================================================================
//�I������
//====================================================================
void CSelect::ScrollSelect(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_D) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_RIGHT, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 1) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_RIGHT, 1) == true)
	{
		m_nSelect++;

		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		if (m_nSelect >= NUM_SCROLLTYPE)
		{
			m_nSelect = 0;
		}
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_A) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_LEFT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_LEFT, 0) == true || 
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 1) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_LEFT, 1) == true)
	{
		m_nSelect--;

		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		if (m_nSelect < 0)
		{
			m_nSelect = NUM_SCROLLTYPE - 1;
		}
	}

	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		if (m_pScrollSelect[nCnt] != nullptr)
		{
			if (m_nSelect == nCnt)
			{
				m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_TRUE);

				// �X���[�Y�^�C�v�ɕύX
				m_Type = SCROLL_LAGGY;
			}
			else
			{
				m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);

				// �J�N�J�N�^�C�v�ɕύX
				m_Type = SCROLL_SMOOTH;
			}
		}
	}
}

//====================================================================
//���菈��
//====================================================================
void CSelect::ScrollButton(void)
{
	CMapSystem* pMapSystem = CMapSystem::GetInstance();

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 1) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_MNK);	// ���̓^�C�v�F�L�[�}�E
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		// �w��̃}�b�v�ԍ���ݒ�
		pMapSystem->SetSelectMap(m_nSetStage);

		switch (m_nSelect)
		{
		case 0:
			CManager::GetInstance()->SetScrollType(CMapMove::SCROLL_TYPE_NORMAL);
			CFade::SetFade(CScene::MODE_GAME);
			break;
		case 1:
			CManager::GetInstance()->SetScrollType(CMapMove::SCROLL_TYPE_RETRO);
			CFade::SetFade(CScene::MODE_GAME);
			break;
		}

		CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_TITLE);
	}
	else if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 1) == true)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
		{
			if (m_pScrollSelect[nCnt] != nullptr)
			{
				m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);
			}
		}

		m_nSelect = 0;
		m_nStep--;
		m_nSelect = m_nSetStage;
	}
}