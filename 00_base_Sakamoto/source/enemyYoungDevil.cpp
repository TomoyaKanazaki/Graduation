//============================================
//
//	子デビルの処理 [enemyMedaman.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemyYoungDevil.h"
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
	const float MOVE_SPEED = 1.0f;
	const float ASHIGARU_SEARCHDISTANCE = 700.0f;	//	足軽の索敵距離
}

//====================================================================
//コンストラクタ
//====================================================================
CEnemyYoungDevil::CEnemyYoungDevil(int nPriority) : CEnemy(nPriority)
{
	m_AtkAction = ACTION_SWAIT;
}

//====================================================================
//デストラクタ
//====================================================================
CEnemyYoungDevil::~CEnemyYoungDevil()
{

}

//====================================================================
//生成処理
//====================================================================
CEnemyYoungDevil* CEnemyYoungDevil::Create(const char* pFilename)
{
	// 生成
	CEnemyYoungDevil* pInstance = new CEnemyYoungDevil();

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
HRESULT CEnemyYoungDevil::Init(void)
{
	// 継承クラスの初期化処理
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_MEDAMAN);	//敵の種類設定

	return S_OK;
}

//====================================================================
//自分が保持するオブジェクトの生成
//====================================================================
void CEnemyYoungDevil::MyObjCreate(void)
{

}

//====================================================================
//終了処理
//====================================================================
void CEnemyYoungDevil::Uninit(void)
{
	// 継承クラスの終了処理
	CEnemy::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CEnemyYoungDevil::Update(void)
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
void CEnemyYoungDevil::TitleUpdate(void)
{
	// 継承クラスのタイトル更新処理
	CEnemy::TitleUpdate();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CEnemyYoungDevil::GameUpdate(void)
{
	// 継承クラスのゲーム更新処理
	CEnemy::GameUpdate();

	//モーションの管理
	ActionState();
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CEnemyYoungDevil::ActionState(void)
{
	// 情報取得
	D3DXVECTOR3 move = GetMove();	// 移動量

		//移動モーション
	if (move.x > 0.1f || move.x < -0.1f || move.z > 0.1f || move.z < -0.1f)
	{
		if (m_Action != ACTION_BMOVE)
		{
			m_Action = ACTION_BMOVE;
			GetMotion()->Set(ACTION_BMOVE, 5);
		}
	}
	//ニュートラルモーション
	else
	{
		if (m_Action != ACTION_SWAIT)
		{
			m_Action = ACTION_SWAIT;
			GetMotion()->Set(ACTION_SWAIT, 5);
		}
	}
}

//====================================================================
//探索状態管理
//====================================================================
void CEnemyYoungDevil::StateManager(void)
{

}

//====================================================================
//ダメージ判定処理
//====================================================================
void CEnemyYoungDevil::SetDamage(float Damage)
{

}

//====================================================================
//描画処理
//====================================================================
void CEnemyYoungDevil::Draw(void)
{
	// 継承クラスの描画処理
	CEnemy::Draw();
}

//====================================================================
// 接触ダメージ処理
//====================================================================
void CEnemyYoungDevil::HitDamage(float fDamage)
{
	CEnemy::HitDamage(fDamage);

	if (GetLife() > 0 && m_Action != ACTION_ATTACK)
	{
		m_nStateCount = 1000;
	}
}