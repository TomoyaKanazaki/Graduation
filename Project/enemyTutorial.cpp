//============================================
//
//	チュートリアル敵の処理 [enemyTutorial.cpp]
//	Author:酒井 南勝
//
//============================================

#include "enemyTutorial.h"

//====================================================================
//コンストラクタ
//====================================================================
CEnemyTutorial::CEnemyTutorial(int nPriority) : CEnemyAshigaru(nPriority)
{
	ZeroMemory(&m_info, sizeof(m_info));
}

//====================================================================
//デストラクタ
//====================================================================
CEnemyTutorial::~CEnemyTutorial()
{

}

//====================================================================
//生成処理
//====================================================================
CEnemyTutorial* CEnemyTutorial::Create(const char* pFilename)
{
	// 生成
	CEnemyTutorial* pInstance = new CEnemyTutorial();

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	// モデル関連の初期化
	pInstance->InitModel(pFilename);

	return pInstance;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CEnemyTutorial::Init(void)
{
	// 継承クラスの初期化処理
	CEnemyAshigaru::Init();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CEnemyTutorial::Uninit(void)
{
	// 継承クラスの終了処理
	CEnemyAshigaru::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CEnemyTutorial::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CEnemyTutorial::TitleUpdate(void)
{
	// 継承クラスのタイトル更新処理
	CEnemyAshigaru::TitleUpdate();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CEnemyTutorial::GameUpdate(void)
{
	// 継承クラスのゲーム更新処理
	CEnemyAshigaru::GameUpdate();
}

//====================================================================
//描画処理
//====================================================================
void CEnemyTutorial::Draw(void)
{
	// 継承クラスの描画処理
	CEnemyAshigaru::Draw();
}

//====================================================================
// 接触ダメージ処理
//====================================================================
void CEnemyTutorial::HitDamage(float fDamage)
{
	CEnemyAshigaru::HitDamage(fDamage);

	if (GetDeath() == true && m_info.bTaskOk == false)
	{
		m_info.bTaskOk = true;
	}
}
