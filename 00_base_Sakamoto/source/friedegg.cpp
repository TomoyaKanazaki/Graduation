//============================================
//
//	目玉焼きクラス(friedegg.cpp)
//	Author : Tomoya Kanazaki
//
//============================================
#include "friedegg.h"
#include "XModel.h"
#include "player.h"
#include "manager.h"
#include "sound.h"
#include "move.h"
#include "MapMove.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float DELETE_TIME = 15.0f;	// 消滅時間
	const char* MODEL_PASS = "data\\MODEL\\02_Item\\friedegg.x"; // モデルパス
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CFriedEgg>* CFriedEgg::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CFriedEgg::CFriedEgg(int nPriority) : CItem(nPriority),
m_fDeleteTimer(0.0f),
m_eCreateType(CEnemy::ENEMY_NONE),
m_pMoveState(nullptr)
{
}

//====================================================================
//デストラクタ
//====================================================================
CFriedEgg::~CFriedEgg()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CFriedEgg::Init()
{
	// 親クラスの初期化
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// オブジェクトの種類を設定
	SetType(CObject::TYPE_FRIEDEGG);

	// タイプの設定
	SetItem(CItem::TYPE_FRIEDEGG);

	// 移動状態設定
	if (m_pMoveState == nullptr)
	{ // 移動状態設定
		m_pMoveState = new CStateRandom();		// 停止状態
		m_pMoveState->SetRotState(CMoveState::ROTSTATE_MAX);		// 移動向きの状態を設定
	}

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFriedEgg>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CFriedEgg::Uninit(void)
{
	// 移動状態の破棄
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();		// 破棄
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

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

	// 親クラスの終了処理
	CItem::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CFriedEgg::Update(void)
{
	// 消滅タイマーを加算する
	m_fDeleteTimer += DeltaTime::Get();

	// 消滅条件を満たした場合
	if (m_fDeleteTimer >= DELETE_TIME)
	{
		// 敵を生成する
		CEnemy::Create(m_eCreateType, GetGrid());

		// 自身を終了し関数を抜ける
		Uninit();
		return;
	}

	// 親クラスの更新処理
	CItem::Update();
}

//====================================================================
//描画処理
//====================================================================
void CFriedEgg::Draw(void)
{
	// 親クラスの描画処理
	CItem::Draw();
}

//====================================================================
//　ヒット処理
//====================================================================
bool CFriedEgg::Hit(CPlayer* pPlayer)
{
	// 描画フラグがfalseの場合falseを返す
	if (!GetDisp()) { return false; }

	// 描画をオフ
	SetDisp(false);

	// 効果音の再生
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_EAT);

	// エフェクトを生成する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_EAT, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// エフェクトを変更する
	ChangeEffect();

	return true;
}

//==========================================
//  リストの取得
//==========================================
CListManager<CFriedEgg>* CFriedEgg::GetList(void)
{
	return m_pList;
}

//==========================================
//  生成処理
//==========================================
CFriedEgg* CFriedEgg::Create(const CEnemy::ENEMY_TYPE eType, const CMapSystem::GRID& pos)
{
	// インスタンス生成
	CFriedEgg* pItem = new CFriedEgg;

	// 初期化処理
	if (FAILED(pItem->Init())) { assert(false); return nullptr; }

	// 位置の設定
	pItem->SetGrid(pos);

	// タイプの設定
	pItem->SetEnemy(eType);

	// エフェクトを生成
	pItem->SetEffect();

	return pItem;
}

//==========================================
//  移動処理
//==========================================
void CFriedEgg::Move(D3DXVECTOR3& pos)
{
	// 移動処理
	m_pMoveState->Move(this, pos, INITVECTOR3);

	// スクロールに合わせて移動する
	CMapSystem::GetInstance()->GetMove()->FollowScroll(pos);

	// 高さを調整する
	pos.y = 50.0f;

	// 存在グリッドを設定する
	SetGrid(CMapSystem::GetInstance()->CalcGrid(pos));
}

//==========================================
//  エフェクトを生成
//==========================================
void CFriedEgg::SetEffect()
{
	// 自身の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_STEAM, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot));
}

//==========================================
//  エフェクトを変更
//==========================================
void CFriedEgg::ChangeEffect()
{
	// 現在のエフェクトを終了
	GetEffect()->SetDeath();

	// 自身の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	pos.y = 0.0f;
	D3DXVECTOR3 rot = GetRot();

	// 新しいエフェクトを生成
	switch (m_eCreateType)
	{
	case CEnemy::ENEMY_MEDAMAN:
		Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_REVIVE_MEDAMAN, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot));
		break;
	case CEnemy::ENEMY_BONBON:
		Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_REVIVE_BONBON, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot));
		break;
	case CEnemy::ENEMY_LITTLEDEVIL:
		Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_REVIVE_SMALLDEVIL, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot));
		break;
	default:
		assert(false);
		break;
	}
}

//==========================================
// 移動状態変更処理
//==========================================
void CFriedEgg::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
	m_pMoveState->Init();
}