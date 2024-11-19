//============================================
//
//	マネージャ [manager.cpp]
//	Author:sakamoto kai
//
//============================================
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "XModel.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "tutorial.h"
#include "logo.h"
#include "MapSystem.h"
#include "MyEffekseer.h"

#ifdef _DEBUG
#define SET_MODE (CScene::MODE_GAME)
#else
#define SET_MODE (CScene::MODE_GAME)
#endif // _DEBUG

//静的メンバ変数宣言
CManager* CManager::pManager = nullptr;
CScene::MODE CScene::m_mode = SET_MODE;

//====================================================================
//コンストラクタ
//====================================================================
CManager::CManager()
{
	//静的メンバ変数宣言
	m_bEdit = false;
	m_bStop = false;
	m_SetTutorial = false;
	m_Pause = false;
	m_SetScoreResult = false;
	m_SetJoyPad = false;
	m_PauseOK = false;
	m_bPad = false;
	m_EndScore = 0;
	m_bGameClear = false;
	m_bState = true;
	pManager = nullptr;
	m_pRenderer = nullptr;
	m_pInputKeyboard = nullptr;
	m_pInputJoyPad = nullptr;
	m_pInputMouse = nullptr;
	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pTexture = nullptr;
	m_pXModel = nullptr;
	m_pScene = nullptr;
	m_LevelUP = nullptr;
	m_Fade = nullptr;
	m_pSound = nullptr;
	m_pRanking = nullptr;
	m_pBlockManager = nullptr;
	m_typeInput = TYPE_INPUT::TYPE_NONE;
	m_nStage = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CManager::~CManager()
{

}

//====================================================================
//インスタンス取得
//====================================================================
CManager *CManager::GetInstance(void)
{
	if (pManager == nullptr)
	{
		pManager = new CManager;
	}
	return pManager;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	GameSpeed = 1.00000000f;

	if (m_pRenderer == nullptr)
	{
		//レンダラーの生成
		m_pRenderer = new CRenderer;
	}

	//レンダラーの初期化処理
	if (FAILED(m_pRenderer->Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pSound == nullptr)
	{
		//サウンドの生成
		m_pSound = new CSound;
	}

	//サウンドの初期化処理
	if (FAILED(m_pSound->InitSound(hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pInputKeyboard == nullptr)
	{
		//キーボードの生成
		m_pInputKeyboard = new CInputKeyboard;
	}
	//キーボードの初期化処理
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pInputJoyPad == nullptr)
	{
		//ジョイパッドの生成
		m_pInputJoyPad = new CInputJoypad;
	}
	//ジョイパッドの初期化処理
	if (FAILED(m_pInputJoyPad->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pInputMouse == nullptr)
	{
		//マウスの生成
		m_pInputMouse = new CInputMouse;
	}
	//キーボードの初期化処理
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pCamera == nullptr)
	{
		//カメラの生成
		m_pCamera = new CCamera;
	}

	//カメラの初期化処理
	if (FAILED(m_pCamera->Init()))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	if (m_pLight == nullptr)
	{
		//ライトの生成
		m_pLight = new CLight;
	}

	//ライトの初期化処理
	if (FAILED(m_pLight->Init()))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	CMapSystem::GetInstance()->Init();

	if (m_pTexture == nullptr)
	{
		//テクスチャの生成
		m_pTexture = new CTexture;
	}

	//全てのテクスチャの読み込み
	if (FAILED(m_pTexture->Load()))
	{//読み込みが失敗した場合
		return E_FAIL;
	}


	if (m_pXModel == nullptr)
	{
		//Xモデルの生成
		m_pXModel = new CXModel;
	}

	////アイテムマネージャの読み込み処理
	//CItemManager::Load();

	//全てのXモデルの読み込み
	if (FAILED(m_pXModel->Load()))
	{//読み込みが失敗した場合
		return E_FAIL;
	}

	if (m_Fade == nullptr)
	{
		//フェードの生成
		m_Fade = new CFade;

		if (m_Fade != nullptr)
		{
			m_Fade->Init(SET_MODE);
		}
	}

	m_PauseOK = true;

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CManager::Uninit(void)
{
	//BGMの停止
	m_pSound->StopSound();

	if (m_Fade != nullptr)
	{
		//フェードの終了処理
		m_Fade->Uninit();

		delete m_Fade;
		m_Fade = nullptr;
	}

	if (m_pScene != nullptr)
	{
		//シーンの終了処理
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = nullptr;
	}

	////アイテムマネージャの読み込み処理
	//CItemManager::UnLoad();

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	CMapSystem::GetInstance()->Uninit();

	if (m_pTexture != nullptr)
	{
		//テクスチャの終了処理
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pXModel != nullptr)
	{
		//Xモデルの終了処理
		m_pXModel->Unload();

		delete m_pXModel;
		m_pXModel = nullptr;
	}

	if (m_pLight != nullptr)
	{
		//ライトの終了処理
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pCamera != nullptr)
	{
		//カメラの終了処理
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pInputMouse != nullptr)
	{
		//ジョイパッドの終了処理
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	if (m_pInputJoyPad != nullptr)
	{
		//ジョイパッドの終了処理
		m_pInputJoyPad->Uninit();

		delete m_pInputJoyPad;
		m_pInputJoyPad = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{
		//キーボードの終了処理
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pRenderer != nullptr)
	{
		//レンダラーの終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pSound != nullptr)
	{
		//サウンドの終了処理
		m_pSound->UninitSound();

		delete m_pSound;
		m_pSound = nullptr;
	}

	// エフェクシアの終了
	CMyEffekseer::GetInstance()->Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CManager::Update(void)
{
	//デバッグ表示
	DebugProc::Print(DebugProc::POINT_LEFT, "-----デバッグ表示-----\n");
	DebugProc::Print(DebugProc::POINT_CENTER, "FPS : %d\n", GetFps());

	//カメラの更新処理
	m_pCamera->Update();

	//ライトの更新処理
	m_pLight->Update();

	//キーボードの更新処理
	m_pInputKeyboard->Update();

	//ジョイパッドの更新処理
	m_pInputJoyPad->Update();

	//マウスの更新処理
	m_pInputMouse->Update();

#ifdef _DEBUG

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{
		if (CScene::GetMode() == CScene::MODE_GAME ||
			CScene::GetMode() == CScene::MODE_TUTORIAL)
		{
			//条件？ 処理１：処理２;
			m_bEdit = m_bEdit ? false : true;
			CObject::DeleteBlock();
		}
	}

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{
		bool Set = CManager::GetInstance()->GetRenderer()->GetAfterImage();
		Set = !Set;
		CManager::GetInstance()->GetRenderer()->SetAfterImage(Set);
	}

#endif

	if ((CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_P) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_START, 0) == true) &&
		m_PauseOK == true &&
		m_Fade->GetFade() == CFade::FADE_NONE)
	{
		//条件？ 処理１：処理２;
		m_Pause = m_Pause ? false : true;
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_DASH);
	}

	//シーンの更新処理
	m_pScene->Update();

	//レンダラーの更新処理
	m_pRenderer->Update();

	//フェードの更新処理
	m_Fade->Update();

	// エフェクシアの更新
	CMyEffekseer::GetInstance()->Update();
}

//====================================================================
//描画処理
//====================================================================
void CManager::Draw(void)
{
	//レンダラーの描画処理
	m_pRenderer->Draw();
}

//====================================================================
//生成処理
//====================================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = nullptr;

	if (pScene == nullptr)
	{
		//シーンの生成
		switch (mode)
		{
		case MODE_LOGO:
			pScene = new CLogo();
			break;
		case MODE_TITLE:
			pScene = new CTitle();
			break;
		case MODE_GAME:
			pScene = new CGame();
			break;
		case MODE_RESULT:
			pScene = new CResult();
			break;
		case CScene::MODE_TUTORIAL:
			pScene = new CTutorial();
			break;
		}
	}

	CManager::GetInstance()->GetInstance()->GetCamera()->ResetCamera();

	CManager::GetInstance()->GetInstance()->SetEdit(false);

	m_mode = mode;

	if (m_mode == MODE_GAME)
	{

	}

	//オブジェクトの初期化処理
	if (FAILED(pScene->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pScene;
}

//====================================================================
//コンストラクタ
//====================================================================
CScene::CScene()
{

}

//====================================================================
//デストラクタ
//====================================================================
CScene::~CScene()
{
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CScene::Init(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Init();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CScene::Uninit(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CScene::Update(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Update();
}

//====================================================================
//描画処理
//====================================================================
void CScene::Draw(void)
{
	CScene *m_pScene = CManager::GetInstance()->GetScene();

	m_pScene->Draw();
}

//====================================================================
//モード設定処理
//====================================================================
void CScene::SetMode(MODE mode)
{
	CScene *pScene = CManager::GetInstance()->GetScene();
	CManager::GetInstance()->GetSound()->StopSound();

	if (pScene != nullptr)
	{
		//シーンの終了処理
		pScene->Uninit();

		delete pScene;
		pScene = nullptr;
	}

	//モードの生成
	pScene = Create(mode);

	CManager::GetInstance()->GetInstance()->SetScene(pScene);
}