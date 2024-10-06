//============================================
//
//	ポーズ処理 [Pause.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Pause.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"

//静的メンバ変数宣言
CObject2D *CPause::m_pPauseUI[MAX_PAUSE] = {};
CObject2D *CPause::m_pPauseFG[MAX_FG] = {};
CObject2D *CPause::m_pPauseMark = NULL;
bool CPause::m_bColor = false;

//====================================================================
//コンストラクタ
//====================================================================
CPause::CPause()
{
	m_PauseSelect = 0;
	m_Appear = false;
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
	CPause *pPause = NULL;

	if (pPause == NULL)
	{
		//敵の生成
		pPause = new CPause();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPause->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pPause;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CPause::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_FG; nCnt++)
	{
		m_pPauseFG[nCnt] = CObject2D::Create(7);
		m_pPauseFG[nCnt]->SetType(CObject::TYPE_TUTORIALUI);

		switch (nCnt)
		{
		case 0:
			m_pPauseFG[nCnt]->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
			m_pPauseFG[nCnt]->SetWidth(1280.0f);
			m_pPauseFG[nCnt]->SetHeight(720.0f);
			m_pPauseFG[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			break;

		case 1:
			m_pPauseFG[nCnt]->SetPos(D3DXVECTOR3(220.0f, 400.0f, 0.0f));
			m_pPauseFG[nCnt]->SetWidth(295.0f);
			m_pPauseFG[nCnt]->SetHeight(600.0f);
			m_pPauseFG[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_pPauseFG[nCnt]->SetTexture("data\\TEXTURE\\pause\\paper00.png");
			break;

		case 2:
			m_pPauseFG[nCnt]->SetPos(D3DXVECTOR3(220.0f, 400.0f, 0.0f));
			m_pPauseFG[nCnt]->SetWidth(90.0f);
			m_pPauseFG[nCnt]->SetHeight(500.0f);
			m_pPauseFG[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_pPauseFG[nCnt]->SetTexture("data\\TEXTURE\\pause\\pause_03.png");
			break;

		case 3:
			m_pPauseFG[nCnt]->SetPos(D3DXVECTOR3(220.0f, 100.0f, 0.0f));
			m_pPauseFG[nCnt]->SetWidth(360.0f);
			m_pPauseFG[nCnt]->SetHeight(100.0f);
			m_pPauseFG[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_pPauseFG[nCnt]->SetTexture("data\\TEXTURE\\pause\\makimono00.png");
			break;

		default:
			break;
		}
	}

	for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		m_pPauseUI[nCnt] = CObject2D::Create(7);
		m_pPauseUI[nCnt]->SetType(CObject::TYPE_TUTORIALUI);
		switch (nCnt)
		{
		case 0:
			m_pPauseUI[nCnt]->SetWidth(600.0f);
			m_pPauseUI[nCnt]->SetHeight(150.0f);
			break;
		case 1:
			m_pPauseUI[nCnt]->SetWidth(600.0f);
			m_pPauseUI[nCnt]->SetHeight(150.0f);
			break;
		case 2:
			m_pPauseUI[nCnt]->SetWidth(600.0f);
			m_pPauseUI[nCnt]->SetHeight(150.0f);
			break;
		}
		m_pPauseUI[nCnt]->SetPos(D3DXVECTOR3(1080.0f, 200.0f + nCnt * 200.0f, 0.0f));
		m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

		m_InitPos[nCnt] = m_pPauseUI[nCnt]->GetPos();
	}
	m_pPauseUI[0]->SetTexture("data\\TEXTURE\\pause\\pause_00.png");
	m_pPauseUI[1]->SetTexture("data\\TEXTURE\\pause\\pause_01.png");
	m_pPauseUI[2]->SetTexture("data\\TEXTURE\\pause\\pause_02.png");

	m_pPauseMark = CObject2D::Create(7);
	m_pPauseMark->SetType(CObject::TYPE_TUTORIALUI);
	m_pPauseMark->SetWidth(140.0f);
	m_pPauseMark->SetHeight(140.0f);
	m_pPauseMark->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPauseMark->SetPos(INITVECTOR3);
	m_pPauseMark->SetTexture("data\\TEXTURE\\pause\\shuriken00.png");

	m_MoveRot = 0.04f;
	m_MarkRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CPause::Uninit(void)
{

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
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT_PUSH);
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
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT_PUSH);
			m_PauseSelect++;

			if (m_PauseSelect > MAX_PAUSE - 1)
			{
				m_PauseSelect = 0;
			}
		}

		for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
		{
			if (nCnt == m_PauseSelect)
			{
				m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_pPauseUI[nCnt]->SetWidth(900.0f);
				m_pPauseUI[nCnt]->SetHeight(225.0f);

				m_pPauseUI[nCnt]->SetPos(D3DXVECTOR3(m_InitPos[nCnt].x - 150.0f, m_InitPos[nCnt].y, m_InitPos[nCnt].z));
				m_pPauseMark->SetPos(D3DXVECTOR3(m_InitPos[nCnt].x - 610.0f, m_InitPos[nCnt].y - 57.0f, m_InitPos[nCnt].z));

				m_MarkRot.z -= m_MoveRot;

				if (m_MarkRot.z >= D3DX_PI)
				{
					m_MarkRot.z -= D3DX_PI * 2;
				}
				else if (m_MarkRot.z <= -D3DX_PI)
				{
					m_MarkRot.z += D3DX_PI * 2;
				}

				m_pPauseMark->SetRot(m_MarkRot);
			}
			else
			{
				m_pPauseUI[nCnt]->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.75f));
				m_pPauseUI[nCnt]->SetWidth(600.0f);
				m_pPauseUI[nCnt]->SetHeight(150.0f);
			
				m_pPauseUI[nCnt]->SetPos(D3DXVECTOR3(m_InitPos[nCnt].x, m_InitPos[nCnt].y, m_InitPos[nCnt].z));
			}
		}

		if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
			CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true ||
			CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_B, 0) == true)
		{
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);

			switch (m_PauseSelect)
			{
			case 0:
				break;
			case 1:

				switch (CScene::GetMode())
				{
				case CScene::MODE_GAME:

					CFade::SetFade(CScene::MODE_GAME);

					break;
				case CScene::MODE_TUTORIAL:

					CFade::SetFade(CScene::MODE_TUTORIAL);

					break;
				}
				break;
			case 2:
				CFade::SetFade(CScene::MODE_TITLE);
				break;
			default:
				CFade::SetFade(CScene::MODE_GAME);
				break;
			}
			CManager::GetInstance()->SetPause(false);
			m_Appear = false;
		}
	}

	if (m_Appear == true)
	{
		for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
		{
			m_pPauseUI[nCnt]->SetAppear(true);		
		}
		for (int nCnt = 0; nCnt < MAX_FG; nCnt++)
		{
			m_pPauseFG[nCnt]->SetAppear(true);
		}
		m_pPauseMark->SetAppear(true);

		if (!m_bColor)
		{
			m_pPauseFG[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
			m_pPauseFG[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pPauseFG[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pPauseFG[3]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		m_bColor = true;
	}
	else
	{
		for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
		{
			m_pPauseUI[nCnt]->SetAppear(false);
		}
		for (int nCnt = 0; nCnt < MAX_FG; nCnt++)
		{
			m_pPauseFG[nCnt]->SetAppear(false);
		}
		m_pPauseMark->SetAppear(false);

		if (m_bColor)
		{
			m_pPauseFG[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_pPauseFG[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pPauseFG[2]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pPauseFG[3]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		m_bColor = false;
	}
}

//====================================================================
//描画処理
//====================================================================
void CPause::Draw(void)
{

}