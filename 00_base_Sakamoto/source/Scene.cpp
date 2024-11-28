//============================================
//
//	マネージャ [manager.cpp]
//	Author:sakamoto kai
//
//============================================
#include "manager.h"
#include "Scene.h"
#include "sound.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "camera.h"

//静的メンバ変数宣言
CScene::MODE CScene::m_mode = MODE_TITLE;

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
//生成処理
//====================================================================
CScene* CScene::Create(MODE mode)
{
	CScene* pScene = nullptr;

	if (pScene == nullptr)
	{
		//シーンの生成
		switch (mode)
		{
		case MODE_TITLE:
			pScene = CTitle::GetInstance();
			break;
		case MODE_SELECT:
			pScene = CSelect::GetInstance();
			break;
		case MODE_GAME:
			pScene = CGame::GetInstance();
			break;
		case MODE_RESULT:
			pScene = CResult::GetInstance();
			break;
		case CScene::MODE_TUTORIAL:
			pScene = CTutorial::GetInstance();
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
//初期化処理
//====================================================================
HRESULT CScene::Init(void)
{
	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CScene::Uninit(void)
{

}

//====================================================================
//更新処理
//====================================================================
void CScene::Update(void)
{

}

//====================================================================
//描画処理
//====================================================================
void CScene::Draw(void)
{

}

//====================================================================
//モード設定処理
//====================================================================
void CScene::SetMode(MODE mode)
{
	CScene* pScene = CManager::GetInstance()->GetScene();
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