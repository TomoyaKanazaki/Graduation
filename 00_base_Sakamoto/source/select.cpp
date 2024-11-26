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
#include "sound.h"

// �萔��`
namespace
{
	const char* MAP_BLOCK_PASS("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	const char* MAP_XMODEL_PASS("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	const char* MAP_GIMMICK_PASS("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");

	const D3DXVECTOR3 STAGE_POS = D3DXVECTOR3(240.0f, 250.0f, 0.0f);		// �I�����ڂ̈ʒu
	const D3DXVECTOR2 STAGE_SIZE = D3DXVECTOR2(300.0f, 300.0f);				// �I�����ڂ̑傫��
	const D3DXVECTOR2 STAGE_DISTANCE = D3DXVECTOR2(50.0f + (STAGE_SIZE.x * 1.0f), 0.0f + (STAGE_SIZE.y * 0.0f));	// �I�����ڂ̕�

	const D3DXVECTOR3 SCROOL_POS = D3DXVECTOR3(440.0f, 500.0f, 0.0f);		// �I�����ڂ̈ʒu
	const D3DXVECTOR2 SCROOL_SIZE = D3DXVECTOR2(300.0f, 50.0f);				// �I�����ڂ̑傫��
	const D3DXVECTOR2 SCROOL_DISTANCE = D3DXVECTOR2(100.0f + (STAGE_SIZE.x * 1.0f), 0.0f + (STAGE_SIZE.y * 0.0f));	// �I�����ڂ̕�

	const D3DXCOLOR SELECT_COLOR_TRUE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �I����ԍ��ڂ̐F
	const D3DXCOLOR SELECT_COLOR_FALSE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// �I�����ĂȂ����ڂ̐F

	const D3DXVECTOR3 BUTTON_POS = D3DXVECTOR3(640.0f, 670.0f, 0.0f);		// �{�^���̈ʒu
	const D3DXVECTOR2 BUTTON_SIZE = { 300.0f, 60.0f };						// �{�^���̑傫��

	const float DOME_ROT_SPEED = 0.001f;	// ���b�V���h�[���̉�]���x
}

//�ÓI�����o�ϐ��錾
CSelect* CSelect::m_pSelect = nullptr;

//====================================================================
//�R���X�g���N�^
//====================================================================
CSelect::CSelect()
{
	for (int nCnt = 0; nCnt < NUM_STAGE; nCnt++)
	{
		m_pStageSelect[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		m_pScrollSelect[nCnt] = nullptr;
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
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSelect::~CSelect()
{

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
	//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �X�J�C�h�[��
	m_pMeshDome = CObjmeshDome::Create();
	if (m_pMeshDome)
	{
		//m_pMeshDome->SetTexture("data\\TEXTURE\\sky.jpg");
	}

	// ���b�V���t�B�[���h
	CObjmeshField* pMeshField = CObjmeshField::Create(21, 21);
	pMeshField->SetPos(INITVECTOR3);
	//pMeshField->SetTexture("data\\TEXTURE\\field00.jpg");

	for (int nCnt = 0; nCnt < NUM_STAGE; nCnt++)
	{
		if (m_pStageSelect[nCnt] == nullptr)
		{
			// �{�^��
			m_pStageSelect[nCnt] = CObject2D::Create();
			m_pStageSelect[nCnt]->SetPos(D3DXVECTOR3(STAGE_POS.x + (SCROOL_DISTANCE.x * nCnt), STAGE_POS.y + (SCROOL_DISTANCE.y * nCnt), STAGE_POS.z));
			m_pStageSelect[nCnt]->SetSize(D3DXVECTOR3(500.0f, STAGE_SIZE.y, 0.0f));
		}

		if (m_pStageSelect[nCnt] != nullptr)
		{
			switch (nCnt)
			{
			case 0:
				m_pStageSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\stage00.png"));
				break;
			case 1:
				m_pStageSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\stage01.png"));
				break;
			case 2:
				m_pStageSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\stage02.png"));
				break;
			}
		}
	}

	for (int nCnt = 0; nCnt < NUM_SCROLLTYPE; nCnt++)
	{
		if (m_pScrollSelect[nCnt] == nullptr)
		{
			// �{�^��
			m_pScrollSelect[nCnt] = CObject2D::Create();
			m_pScrollSelect[nCnt]->SetPos(D3DXVECTOR3(SCROOL_POS.x + (STAGE_DISTANCE.x * nCnt), SCROOL_POS.y + (STAGE_DISTANCE.y * nCnt), SCROOL_POS.z));
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

	// �{�^��
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
		m_pSelect = nullptr;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CSelect::Update(void)
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
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_D) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 0))
	{
		m_nSelect++;

		if (m_nSelect >= NUM_STAGE)
		{
			m_nSelect = 0;
		}
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_A) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_LEFT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 0))
	{
		m_nSelect--;

		if (m_nSelect < 0)
		{
			m_nSelect = NUM_STAGE - 1;
		}
	}


	for (int nCnt = 0; nCnt < NUM_STAGE; nCnt++)
	{
		if (m_pStageSelect[nCnt] != nullptr)
		{
			if (m_nSelect == nCnt)
			{
				m_pStageSelect[nCnt]->SetColor(SELECT_COLOR_TRUE);
			}
			else
			{
				m_pStageSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);
			}
		}
	}
}

//====================================================================
//���菈��
//====================================================================
void CSelect::StageButton(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		m_nSetStage = m_nSelect;
		m_nSelect = 0;
		m_nStep++;
	}
}

//====================================================================
//�I������
//====================================================================
void CSelect::ScrollSelect(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_D) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_RIGHT, 0))
	{
		m_nSelect++;

		if (m_nSelect >= NUM_SCROLLTYPE)
		{
			m_nSelect = 0;
		}
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_A) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_LEFT) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_LEFT, 0))
	{
		m_nSelect--;

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
			}
			else
			{
				m_pScrollSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);
			}
		}
	}
}

//====================================================================
//���菈��
//====================================================================
void CSelect::ScrollButton(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_MNK);	// ���̓^�C�v�F�L�[�}�E
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);

		switch (m_nSelect)
		{
		case 0:
			CManager::GetInstance()->SetScrollType(CDevil::SCROLL_TYPE_NORMAL);
			CFade::SetFade(CScene::MODE_GAME);
			break;
		case 1:
			CManager::GetInstance()->SetScrollType(CDevil::SCROLL_TYPE_RETRO);
			CFade::SetFade(CScene::MODE_GAME);
			break;
		}
	}
	else if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 0) == true)
	{
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