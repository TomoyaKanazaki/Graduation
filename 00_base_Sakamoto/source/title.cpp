//============================================
//
//	タイトル画面 [title.cpp]
//	Author:sakamoto kai
//
//============================================
#include "title.h"
#include "fade.h"
#include "object2D.h"
#include "texture.h"
#include "objmeshDome.h"
#include "objmeshField.h"
#include "light.h"
#include "sound.h"

// 定数定義
namespace
{
	const char* MAP_BLOCK_PASS("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	const char* MAP_XMODEL_PASS("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	const char* MAP_GIMMICK_PASS("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");

	const D3DXVECTOR3 TITLE_LOGO_POS = D3DXVECTOR3(640.0f, 150.0f, 0.0f);	// タイトルロゴの位置
	const D3DXVECTOR2 TITLE_LOGO_SIZE = { 600.0f, 240.0f };				// タイトルロゴの大きさ

	const D3DXVECTOR3 SELECT_POS = D3DXVECTOR3(640.0f, 300.0f, 0.0f);		// 選択項目の位置
	const D3DXVECTOR2 SELECT_SIZE = D3DXVECTOR2(300.0f, 80.0f);				// 選択項目の大きさ
	const D3DXVECTOR2 SELECT_DISTANCE = D3DXVECTOR2(0.0f + (SELECT_SIZE.x * 0.0f), 5.0f + (SELECT_SIZE.y * 1.0f));	// 選択項目の幅
	const D3DXCOLOR SELECT_COLOR_TRUE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 選択状態項目の色
	const D3DXCOLOR SELECT_COLOR_FALSE = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// 選択してない項目の色

	const D3DXVECTOR3 BUTTON_POS = D3DXVECTOR3(640.0f, 670.0f, 0.0f);		// ボタンの位置
	const D3DXVECTOR2 BUTTON_SIZE = { 300.0f, 160.0f };						// ボタンの大きさ

	const float FADE_TIME = 5.0f; // 自動で遷移するまでの時間
}

//静的メンバ変数宣言
CTitle* CTitle::m_pTitle = nullptr;

//====================================================================
//コンストラクタ
//====================================================================
CTitle::CTitle() :
	m_fTimer(0.0f)
{
	for (int nCnt = 0; nCnt < MAX_SELECT; nCnt++)
	{
		m_pModeSelect[nCnt] = nullptr;
	}
	m_pLogo = nullptr;
	m_pTitleButton = nullptr;
	m_pMeshDome = nullptr;

	m_fCountFade = 0.0f;
	m_bStart = false;
	m_bTurn = false;
	m_bSlash = false;
	m_nSelect = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CTitle::~CTitle()
{

}


//====================================================================
//インスタンス取得
//====================================================================
CTitle* CTitle::GetInstance(void)
{
	if (m_pTitle == nullptr)
	{
		m_pTitle = new CTitle;
	}
	return m_pTitle;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CTitle::Init(void)
{
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// タイトル文字
	m_pLogo = CObject2D::Create();
	m_pLogo->SetPos(TITLE_LOGO_POS);
	m_pLogo->SetSize(D3DXVECTOR3(TITLE_LOGO_SIZE.x, TITLE_LOGO_SIZE.y, 0.0f));
	if (m_pLogo != nullptr)
	{
		m_pLogo->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\devilins.png"));
	}

	for (int nCnt = 0; nCnt < MAX_SELECT; nCnt++)
	{
		if (m_pModeSelect[nCnt] == nullptr)
		{
			// ボタン
			m_pModeSelect[nCnt] = CObject2D::Create();
			m_pModeSelect[nCnt]->SetPos(D3DXVECTOR3(SELECT_POS.x + (SELECT_DISTANCE.x * nCnt), SELECT_POS.y + (SELECT_DISTANCE.y * nCnt), SELECT_POS.z));
			m_pModeSelect[nCnt]->SetSize(D3DXVECTOR3(BUTTON_SIZE.x, BUTTON_SIZE.y, 0.0f));
		}

		if (m_pModeSelect[nCnt] != nullptr)
		{
			switch (nCnt)
			{
			case 0:
				m_pModeSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\singleplay.png"));
				break;
			case 1:
				m_pModeSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\multiplay.png"));
				break;
			case 2:
				m_pModeSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\tutorial.png"));
				break;
			case 3:
				m_pModeSelect[nCnt]->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\ranking.png"));
				break;
			}

			if (m_nSelect == nCnt)
			{
				m_pModeSelect[nCnt]->SetColor(SELECT_COLOR_TRUE);
			}
			else
			{
				m_pModeSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);
			}
		}
	}

	// ボタン
	if (m_pTitleButton == nullptr)
	{
		m_pTitleButton = CObject2D::Create();
		m_pTitleButton->SetPos(BUTTON_POS);
		m_pTitleButton->SetSize(D3DXVECTOR3(600.0f, BUTTON_SIZE.y, 0.0f));
	}

	if (m_pTitleButton != nullptr)
	{
		m_pTitleButton->SetIdx(pTexture->Regist("data\\TEXTURE\\UI\\pressbotton.png"));
	}
	
	// ライトの初期化
	CManager::GetInstance()->GetLight()->Init();

	// タイマーの初期化
	m_fTimer = 0.0f;

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CTitle::Uninit(void)
{
	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	if (m_pTitle != nullptr)
	{
		delete m_pTitle;
		m_pTitle = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CTitle::Update(void)
{	
	if (CManager::GetInstance()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		// 選択処理
		Select();

		// 決定処理
		Button();

		// 自動遷移
		AutoFade();
	}
}

//====================================================================
//描画処理
//====================================================================
void CTitle::Draw(void)
{

}

//====================================================================
//選択処理
//====================================================================
void CTitle::Select(void)
{
	// 下入力処理
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_S) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_DOWN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_DOWN, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_DOWN, 0) == true)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		m_nSelect++;

		if (m_nSelect >= MAX_SELECT)
		{
			m_nSelect = 0;
		}

		// タイマーの初期化
		m_fTimer = 0.0f;
	}

	// 上入力処理
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_W) == true ||
		CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_UP) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_UP, 0) ||
		CManager::GetInstance()->GetInputJoyPad()->Get_LStick_Trigger(CInputJoypad::JOYPAD_LSTICK::LSTICK_UP, 0) == true)
	{
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

		m_nSelect--;

		if (m_nSelect < 0)
		{
			m_nSelect = MAX_SELECT - 1;
		}

		// タイマーの初期化
		m_fTimer = 0.0f;
	}


	for (int nCnt = 0; nCnt < MAX_SELECT; nCnt++)
	{
		if (m_pModeSelect[nCnt] != nullptr)
		{
			if (m_nSelect == nCnt)
			{
				m_pModeSelect[nCnt]->SetColor(SELECT_COLOR_TRUE);
			}
			else
			{
				m_pModeSelect[nCnt]->SetColor(SELECT_COLOR_FALSE);
			}
		}
	}
}

//====================================================================
//決定処理
//====================================================================
void CTitle::Button(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_MNK);	// 入力タイプ：キーマウ
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		switch (m_nSelect)
		{
		case 0:
			// 選択画面遷移
			//CFade::SetFade(CScene::MODE_SELECT);
			CScene::SetMode(MODE_SELECT);
			CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_SINGLE);
			break;
		case 1:
			// 選択画面遷移
			//CFade::SetFade(CScene::MODE_SELECT);
			CScene::SetMode(MODE_SELECT);
			CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_MULTI);
			break;
		case 2:
			// チュートリアル遷移
			CFade::SetFade(CScene::MODE_TUTORIAL);
			CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_MULTI);
			CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_TITLE);
			break;
		case 3:
			// リザルト遷移
			CFade::SetFade(CScene::MODE_RESULT);
			CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_NONE);
			CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_TITLE);
			break;
		}

		// タイマーの初期化
		m_fTimer = 0.0f;
	}
	else if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_JOYPAD);	// 入力タイプ：パッド
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

		switch (m_nSelect)
		{
		case 0:
			// 選択画面遷移
			//CFade::SetFade(CScene::MODE_SELECT);
			CScene::SetMode(MODE_SELECT);
			CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_SINGLE);
			break;
		case 1:
			// 選択画面遷移
			//CFade::SetFade(CScene::MODE_SELECT);
			CScene::SetMode(MODE_SELECT);
			CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_MULTI);
			break;
		case 2:
			// チュートリアル遷移
			CManager::GetInstance()->GetSound()->Stop();
			CFade::SetFade(CScene::MODE_TUTORIAL);
			CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_MULTI);
			CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_TITLE);
			break;
		case 3:
			// リザルト遷移
			CManager::GetInstance()->GetSound()->Stop();
			CFade::SetFade(CScene::MODE_RESULT);
			CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_NONE);
			CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_TITLE);
			break;
		}

		// タイマーの初期化
		m_fTimer = 0.0f;
	}
}

//==========================================
//  自動遷移処理
//==========================================
void CTitle::AutoFade()
{
	// 自動遷移タイマーを加算
	m_fTimer += DeltaTime::Get();

	// 規定時間を超えた場合ランキングに遷移する
	if (m_fTimer >= FADE_TIME)
	{
		CManager::GetInstance()->GetSound()->Stop();
		CFade::SetFade(CScene::MODE_RESULT);
		CManager::GetInstance()->SetGameMode(CManager::GAME_MODE::MODE_NONE);
	}
}
