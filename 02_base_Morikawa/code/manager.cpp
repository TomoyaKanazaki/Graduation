//========================================
//
// 管理[manager.cpp]
// Author：森川駿弥
//
//========================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "XLoad.h"
#include "sound.h"

//========================================
//静的メンバ変数
//========================================
CManager *CManager::m_pManager = nullptr;

//========================================
//コンストラクタ
//========================================
CManager::CManager(void) : 
	m_nCnt(0),	//自動遷移のカウンター
	m_state(0),	//状態変更
	m_NowScene(CScene::MODE_NONE),	//現在のシーン
	m_pRenderer(nullptr),				// レンダラーのポインタ
	m_pInputKeyboard(nullptr),
	m_pInputPad(nullptr),
	m_pInputMouse(nullptr),
	m_pScene(nullptr),
	m_pTexture(nullptr),
	m_pCamera(nullptr),
	m_pLight(nullptr),
	m_pFade(nullptr),
	m_pSound(nullptr),
	m_CurrentTime(0),
	m_OldTime(0),
	m_fDeltaTime(0.0f)
{//値クリア
}

//========================================
//デストラクタ
//========================================
CManager::~CManager(void)
{
}

//========================================
// マネージャーの情報取得
//========================================
CManager *CManager::GetInstance()
{
	if (m_pManager == nullptr)
	{//インスタンス生成
		return m_pManager = new CManager;
	}
	else
	{//ポインタを返す
		return m_pManager;
	}
}

//========================================
//初期化
//========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダラーの生成
	if (m_pRenderer == nullptr)
	{//g_pRendererがnullptrの時
		m_pRenderer = new CRenderer;

		//レンダラーの初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{//初期化処理が失敗した場合
			return -1;
		}
	}

	//キーボードの生成
	if (m_pInputKeyboard == nullptr)
	{//m_pInputKeyboardがnullptrの時
		m_pInputKeyboard = new CInputKeyboard;

		//キーボードの初期化処理
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			return -1;
		}
	}

	//コントローラーの生成
	if (m_pInputPad == nullptr)
	{//m_pInputKeyboardがnullptrの時
		m_pInputPad = new CInputPad;

		//コントローラーの初期化処理
		if (FAILED(m_pInputPad->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			return -1;
		}
	}

	//カメラ生成
	if (m_pCamera == nullptr)
	{//m_pCameraがnullptrの時
		m_pCamera = new CCamera;

		//カメラの初期化処理
		if (FAILED(m_pCamera->Init()))
		{//初期化処理が失敗した場合
			return -1;
		}
	}

	//debugprocの生成
	DebugProc::Init();

	if (m_pLight == nullptr)
	{// m_pLightがnullptrの時
		m_pLight = new CLight;

		if (FAILED(m_pLight->Init()))
		{// 初期化失敗
			return -1;
		}
	}

	if (m_pSound == nullptr)
	{
		// インスタンス生成
		m_pSound = new CSound;

		if (FAILED(m_pSound->Init(hWnd)))
		{// 初期化失敗
			return -1;
		}

	}

	//**********************************
	// Xファイル
	//**********************************
	m_pXLoad = CXLoad::Create();
	if (m_pXLoad == nullptr)
	{
		return E_FAIL;
	}

	//テクスチャ生成
	m_pTexture = CTexture::Create();

	if (m_pFade == nullptr)
	{
		// フェード生成・設定
		m_pFade = CFade::Create(CScene::MODE_TITLE);
		m_pFade->SetFade(CScene::MODE_TITLE);
	}

#ifdef _DEBUG
	{
		m_pFade = CFade::Create(CScene::MODE_GAME);
		m_pFade->SetFade(CScene::MODE_GAME);
	}
#endif

	//成功を返す
	return S_OK;
}

//========================================
//終了
//========================================
void CManager::Uninit(void)
{
	//オブジェクトの破棄
	CObject::ReleaseAll();
	
	if (m_pRenderer != nullptr)
	{//レンダラーの終了
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{//キーボードの破棄
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pInputPad != nullptr)
	{//コントローラーの破棄
		m_pInputPad->Uninit();
		delete m_pInputPad;
		m_pInputPad = nullptr;
	}

	//debugprocの破棄
	DebugProc::Uninit();

	if (m_pTexture != nullptr)
	{//テクスチャ破棄
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pFade != nullptr)
	{// フェード終了
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	if (m_pSound != nullptr)
	{// サウンド終了
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
}

//========================================
//更新
//========================================
void CManager::Update(void)
{
	// 過去の時間保存
	m_OldTime = m_CurrentTime;

	// 経過時間
	m_CurrentTime = timeGetTime();
	m_fDeltaTime = static_cast<float>(m_CurrentTime - m_OldTime) / 1000;

	if (m_pRenderer != nullptr)
	{//g_pRendererがnullptrの時
		m_pRenderer->Update();
	}
	
	if (m_pInputKeyboard != nullptr)
	{//キーボードの更新
		m_pInputKeyboard->Update();
	}
	
	if (m_pInputPad != nullptr)
	{//コントローラーの更新
		m_pInputPad->Update();
	}
	
	//DebugProの更新
	DebugProc::Update();
	
	if (m_pScene != nullptr)
	{//シーンの更新
		m_pScene->Update();
	}

	if (m_pFade != nullptr)
	{// フェード更新
		m_pFade->Update();
	}

	if (m_pCamera != nullptr)
	{// カメラ更新
		m_pCamera->Update();
	}
}

//========================================
//描画
//========================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{//g_pRendererがnullptrの時
		//描画処理
		m_pRenderer->Draw();
	}

	if (m_pScene != nullptr)
	{//シーンの更新
		m_pScene->Draw();
	}
}

//========================================
//モード設定
//========================================
void CManager::SetMode(CScene::MODE mode)
{	
	if (m_pScene != nullptr)
	{//モード破棄
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//全て破棄
	CObject::ReleaseAll();

	//モード生成
	m_pScene = CScene::Create(mode);
}

//========================================
// 勝敗判定
//========================================
bool CManager::bResult()
{
	return m_IsResult;
}

//========================================
//コンストラクタ
//========================================
CScene::CScene() : 
	m_pVtxBuff(nullptr),	//頂点情報を格納
	m_nIdxTexture(0),		//テクスチャ番号
	m_mode(MODE_NONE)		//モード
{//値クリア
}

//========================================
//デストラクタ
//========================================
CScene::~CScene()
{
}

//========================================
//生成
//========================================
CScene* CScene::Create(int nMode)
{
	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();;

	//CSceneのポインタ
	CScene* pScene = nullptr;

	switch (nMode)
	{
	case MODE_TITLE:
		// タイトル生成
		pScene = CTitle::Create();
		break;

	case MODE_TUTORIAL:
		// チュートリアル生成
		pScene = CTutorial::Create();
		break;

	case MODE_GAME:
		// ゲームシーン生成
		pScene = CGame::Create();
		break;

	case MODE_RESULT:
		// リザルト生成
		pScene = CResult::Create();
		break;
	}

	if (pScene != nullptr)
	{
		// シーンの初期化
		pScene->Init();

		// カメラの初期化
		pCamera->Init();
	}

	//ポインタを返す
	return pScene;
}
 