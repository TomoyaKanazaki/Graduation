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
#include "fade.h"
#include "sound.h"
#include "MapSystem.h"
#include "MyEffekseer.h"
#include "Scene.h"

#ifdef _DEBUG
#define SET_MODE (CScene::MODE_GAME)
#define SET_PLAY_MODE (GAME_MODE::MODE_SINGLE)
#else
#define SET_MODE (CScene::MODE_TITLE)
#define SET_PLAY_MODE (GAME_MODE::MODE_SINGLE)
#endif // _DEBUG

//静的メンバ変数宣言
CManager* CManager::pManager = nullptr;

//====================================================================
//コンストラクタ
//====================================================================
CManager::CManager() :
	m_pEffect(nullptr)
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
	m_pFade = nullptr;
	m_pSound = nullptr;
	m_pRanking = nullptr;
	m_pBlockManager = nullptr;
	m_typeInput = TYPE_INPUT::TYPE_NONE;
	m_nStage = 0;
	m_ScrollType = 0;
	m_End1PScore = 0;
	m_End2PScore = 0;
	m_GameMode = SET_PLAY_MODE;
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
	if (FAILED(m_pSound->Init(hWnd)))
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

	 //エフェクトの生成
	if (m_pEffect == nullptr)
	{
		m_pEffect = new CMyEffekseer;
		m_pEffect->Init();
	}

	// マップシステムの初期化
	CMapSystem::GetInstance();
	
	// テクスチャの初期化	
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

	//全てのXモデルの読み込み
	if (FAILED(m_pXModel->Load()))
	{//読み込みが失敗した場合
		return E_FAIL;
	}

	if (m_pFade == nullptr)
	{
		//フェードの生成
		m_pFade = new CFade;

		if (m_pFade != nullptr)
		{
			m_pFade->Init(SET_MODE);
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
	m_pSound->Stop();

	if (m_pFade != nullptr)
	{
		//フェードの終了処理
		m_pFade->Uninit();

		delete m_pFade;
		m_pFade = nullptr;
	}

	if (m_pScene != nullptr)
	{
		//シーンの終了処理
		m_pScene->Uninit();

		m_pScene = nullptr;
	}

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
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = nullptr;
	}

	// エフェクシアの終了
	if (m_pEffect != nullptr)
	{
		// エフェクシアの終了処理
		m_pEffect->Uninit();
		delete m_pEffect;
		m_pEffect = nullptr;
	}
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
	(m_pCamera != nullptr) ? m_pCamera->Update() : assert(false);

	//ライトの更新処理
	m_pLight != nullptr ? m_pLight->Update() : assert(false);

	//キーボードの更新処理
	m_pInputKeyboard != nullptr ? m_pInputKeyboard->Update() : assert(false);

	//ジョイパッドの更新処理
	m_pInputJoyPad != nullptr ? m_pInputJoyPad->Update() : assert(false);

	//マウスの更新処理
	m_pInputMouse != nullptr ? m_pInputMouse->Update() : assert(false);

#ifdef _DEBUG

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{
		if (CScene::GetMode() == CScene::MODE_GAME ||
			CScene::GetMode() == CScene::MODE_TUTORIAL)
		{
			//条件？ 処理１：処理２;
			m_bEdit = m_bEdit ? false : true;
			CObject::DeleteBlock();
			CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_CONTROL);
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
		m_pFade->GetFade() == CFade::FADE_NONE)
	{
		//条件？ 処理１：処理２;
		m_Pause = m_Pause ? false : true;
	}

	//シーンの更新処理
	m_pScene != nullptr ? m_pScene->Update() : assert(false);

	//レンダラーの更新処理
	m_pRenderer != nullptr ? m_pRenderer->Update() : assert(false);

	//フェードの更新処理
	m_pFade != nullptr ? m_pFade->Update() : assert(false);

	// エフェクシアの更新
	m_pEffect != nullptr ? m_pEffect->Update() : assert(false);
}

//====================================================================
//描画処理
//====================================================================
void CManager::Draw(void)
{
	//レンダラーの描画処理
	m_pRenderer != nullptr ? m_pRenderer->Draw() : assert(false);
}
