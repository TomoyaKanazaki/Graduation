//============================================
//
//	メダマンの処理 [enemyMedaman.cpp]
//	Author: sakamoto kai
//
//============================================
#include "enemyMedaman.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "objectcharacter.h"
#include "game.h"
#include "tutorial.h"
#include "object3D.h"
#include "debugproc.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "useful.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float MOTION_MOVE = 0.1f;		// 移動状態モーション変更への移動量

	const int MOTION_BLEND_NEUTRAL = 5;	// 待機状態のモーションブレンド時間
	const int MOTION_BLEND_MOVE = 5;	// 移動状態のモーションブレンド時間

	const int MODEL_REF = 101;	// ステンシル参照値

	const char* MODEL_PASS = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt"; // モデルパス
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CEnemyMedaman>* CEnemyMedaman::m_pList = nullptr; // オブジェクトリスト

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
//初期化処理
//====================================================================
HRESULT CEnemyMedaman::Init(void)
{
	// 継承クラスの初期化処理
	CEnemy::Init();

	SetEnemyType(CEnemy::ENEMY_MEDAMAN);	//敵の種類設定

	// 見た目の設定
	InitModel(MODEL_PASS);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CEnemyMedaman>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CEnemyMedaman::Uninit(void)
{
	if (m_pList != nullptr)
	{
		// リストから自身のオブジェクトを削除
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // オブジェクトが一つもない場合

			// リストマネージャーの破棄
			m_pList->Release(m_pList);
		}
	}
	// 継承クラスの終了処理
	CEnemy::Uninit();

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CEnemyMedaman::Update(void)
{
	//モーションの管理
	ActionState();

	// 親更新
	CEnemy::Update();
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CEnemyMedaman::ActionState(void)
{
	// モーション取得
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
	{
		return;
	}

	// 情報取得
	D3DXVECTOR3 move = GetMove();	// 移動量

	//移動モーション
	if (move.x > MOTION_MOVE || move.x < -MOTION_MOVE || move.z > MOTION_MOVE || move.z < -MOTION_MOVE)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			pMotion->Set(ACTION_MOVE, MOTION_BLEND_MOVE);
		}
	}
	// ダメージモーション
	else if (GetHitState() == CEnemy::HIT_STATE_DAMAGE)
	{
		if (m_Action != ACTION_DEATH)
		{
			m_Action = ACTION_DEATH;
			pMotion->Set(ACTION_DEATH, MOTION_BLEND_MOVE);
		}
	}
	//ニュートラルモーション
	else
	{
		if (m_Action != ACTION_NEUTRAL)
		{
			m_Action = ACTION_NEUTRAL;
			pMotion->Set(ACTION_NEUTRAL, MOTION_BLEND_NEUTRAL);
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
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 5);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 継承クラスの描画処理
	CEnemy::Draw();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 7);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 継承クラスの描画処理
	CEnemy::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
// 接触ダメージ処理
//====================================================================
bool CEnemyMedaman::Hit(void)
{
	SetHitState(CEnemy::HIT_STATE_DAMAGE);
	SetHitStateCount(100);

	return true;
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CEnemyMedaman>* CEnemyMedaman::GetList(void)
{
	return m_pList;
}
