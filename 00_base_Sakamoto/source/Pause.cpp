//============================================
//
//	ポーズ処理 [Pause.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Pause.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"

//静的メンバ変数宣言
bool CPause::m_bColor = false;

namespace
{
	const D3DXVECTOR3 PAUSE_SIZE = { 400.0f , 150.0f ,0.0f};
}

//====================================================================
//コンストラクタ
//====================================================================
CPause::CPause()
{
	m_PauseSelect = 0;
	m_Appear = false;
	for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		m_pPauseUI[nCnt] = nullptr;
	}
	m_pPauseFG = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CPause::~CPause()
{

}

//====================================================================
//生成処理
//====================================================================
CPause *CPause::Create()
{
	CPause *pPause = nullptr;

	if (pPause == nullptr)
	{
		//敵の生成
		pPause = new CPause();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPause->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pPause;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CPause::Init(void)
{
	if (m_pPauseFG == nullptr)
	{
		m_pPauseFG = CObject2D::Create(7);
		m_pPauseFG->SetType(CObject::TYPE_TUTORIALUI);
		m_pPauseFG->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
		m_pPauseFG->SetSize(D3DXVECTOR3(1280.0f, 720.0f, 0.0f));
		m_pPauseFG->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
		m_pPauseFG->SetAppear(false);
	}

	for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		if (m_pPauseUI[nCnt] == nullptr)
		{
			m_pPauseUI[nCnt] = CObject2D::Create(7);
			m_pPauseUI[nCnt]->SetPos(D3DXVECTOR3(640.0f, 200.0f + nCnt * 200.0f, 0.0f));
			m_pPauseUI[nCnt]->SetSize(PAUSE_SIZE);
			m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pPauseUI[nCnt]->SetType(CObject::TYPE_TUTORIALUI);
			m_pPauseUI[nCnt]->SetAppear(false);
		}
	}
	m_pPauseUI[0]->SetTexture("data\\TEXTURE\\UI\\pause_game.png");
	m_pPauseUI[1]->SetTexture("data\\TEXTURE\\UI\\pause_restart.png");
	m_pPauseUI[2]->SetTexture("data\\TEXTURE\\UI\\pause_title.png");

	m_Appear = false;
	CManager::GetInstance()->SetPause(false);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CPause::Uninit(void)
{
	if (m_pPauseFG != nullptr)
	{
		m_pPauseFG->Uninit();
		m_pPauseFG = nullptr;
	}

	for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		if (m_pPauseUI[nCnt] != nullptr)
		{
			m_pPauseUI[nCnt]->Uninit();
			m_pPauseUI[nCnt] = nullptr;
		}
	}
}

//====================================================================
//更新処理
//====================================================================
void CPause::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad *pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	if (CManager::GetInstance()->GetPause() == true)
	{
		if (pInputKeyboard->GetTrigger(DIK_W) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_UP, 0) == true ||
			pInputJoypad->Get_LStick_Trigger(CInputJoypad::LSTICK_UP, 0) == true)
		{
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);
			m_PauseSelect--;

			if (m_PauseSelect < 0)
			{
				m_PauseSelect = MAX_PAUSE - 1;
			}
		}

		if (pInputKeyboard->GetTrigger(DIK_S) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_DOWN, 0) == true ||
			pInputJoypad->Get_LStick_Trigger(CInputJoypad::LSTICK_DOWN, 0) == true)
		{
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);
			m_PauseSelect++;

			if (m_PauseSelect > MAX_PAUSE - 1)
			{
				m_PauseSelect = 0;
			}
		}

		for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
		{
			if (m_pPauseUI[nCnt] != nullptr)
			{
				m_pPauseUI[nCnt]->SetAppear(true);
				m_pPauseUI[nCnt]->SetPos(D3DXVECTOR3(640.0f, 200.0f + nCnt * 200.0f, 0.0f));
				//m_pPauseUI[nCnt]->SetSize(D3DXVECTOR3(150.0f, 50.0f, 0.0f));

				if (nCnt == m_PauseSelect)
				{
					m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.75f));
				}
			}
		}

		if (m_pPauseFG != nullptr)
		{
			m_pPauseFG->SetAppear(true);
		}

		if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
			CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true ||
			CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 0) == true)
		{
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

			switch (m_PauseSelect)
			{
			case 0:
				break;
			case 1:
				CFade::SetFade(CScene::GetMode());
				break;
			case 2:
				CFade::SetFade(CScene::MODE_TITLE);
				break;
			default:
				CFade::SetFade(CScene::MODE_GAME);
				break;
			}
			CManager::GetInstance()->SetPause(false);
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
		{
			if (m_pPauseUI[nCnt] != nullptr)
			{
				m_pPauseUI[nCnt]->SetAppear(false);
			}
		}

		if (m_pPauseFG != nullptr)
		{
			m_pPauseFG->SetAppear(false);
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CPause::Draw(void)
{

}