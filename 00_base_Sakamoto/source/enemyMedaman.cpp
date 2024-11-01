//============================================
//
//	メダマンの処理 [enemyMedaman.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemyMedaman.h"
#include "LevelModelEffect.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"
#include "effect.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float MOTION_MOVE = 0.1f;		// 移動状態モーション変更への移動量

	const int MOTION_BLEND_NEUTRAL = 5;	// 待機状態のモーションブレンド時間
	const int MOTION_BLEND_MOVE = 5;	// 移動状態のモーションブレンド時間
}

//====================================================================
//コンストラクタ
//====================================================================
CEnemyMedaman::CEnemyMedaman(int nPriority) : CEnemy(nPriority)
{
	m_AtkAction = ACTION_NEUTRAL;
}

//====================================================================
//デストラクタ
//====================================================================
CEnemyMedaman::~CEnemyMedaman()
{

}

//====================================================================
//生成処理
//====================================================================
CEnemyMedaman* CEnemyMedaman::Create(const char* pFilename)
{
	// 生成
	CEnemyMedaman* pInstance = new CEnemyMedaman();

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	// モデル関連の初期化
	pInstance->InitModel(pFilename);

	return pInstance;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CEnemyMedaman::Init(void)
{
	// 継承クラスの初期化処理
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_MEDAMAN);	//敵の種類設定

	return S_OK;
}

//====================================================================
//自分が保持するオブジェクトの生成
//====================================================================
void CEnemyMedaman::MyObjCreate(void)
{

}

//====================================================================
//終了処理
//====================================================================
void CEnemyMedaman::Uninit(void)
{
	// 継承クラスの終了処理
	CEnemy::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CEnemyMedaman::Update(void)
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
void CEnemyMedaman::TitleUpdate(void)
{
	// 継承クラスのタイトル更新処理
	CEnemy::TitleUpdate();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CEnemyMedaman::GameUpdate(void)
{
	// 継承クラスのゲーム更新処理
	CEnemy::GameUpdate();

	//モーションの管理
	ActionState();
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CEnemyMedaman::ActionState(void)
{
	// 情報取得
	D3DXVECTOR3 move = GetMove();	// 移動量

		//移動モーション
	if (move.x > MOTION_MOVE || move.x < -MOTION_MOVE || move.z > MOTION_MOVE || move.z < -MOTION_MOVE)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			GetMotion()->Set(ACTION_MOVE, MOTION_BLEND_NEUTRAL);
		}
	}
	//ニュートラルモーション
	else
	{
		if (m_Action != ACTION_NEUTRAL)
		{
			m_Action = ACTION_NEUTRAL;
			GetMotion()->Set(ACTION_NEUTRAL, MOTION_BLEND_MOVE);
		}
	}
}

//====================================================================
//探索状態管理
//====================================================================
void CEnemyMedaman::StateManager(void)
{

}

//====================================================================
//ダメージ判定処理
//====================================================================
void CEnemyMedaman::SetDamage(float Damage)
{

}

//====================================================================
//描画処理
//====================================================================
void CEnemyMedaman::Draw(void)
{
	// 継承クラスの描画処理
	CEnemy::Draw();
}

//====================================================================
// 接触ダメージ処理
//====================================================================
void CEnemyMedaman::HitDamage(float fDamage)
{
	CEnemy::HitDamage(fDamage);

	if (GetLife() > 0 && m_Action != ACTION_TEST)
	{
		m_nStateCount = 1000;
	}
}