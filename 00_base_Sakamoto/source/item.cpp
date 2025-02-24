//============================================
//
//	アイテムの処理 [item.cpp]
//	Author:morikawa shunya
//
//============================================
#include "item.h"
#include "cross.h"
#include "bowabowa.h"
#include "player.h"
#include "useful.h"
#include "MapSystem.h"
#include "bible.h"
#include "game.h"
#include "score.h"
#include "softcream.h"
#include "renderer.h"
#include "tutorial.h"
#include "objmeshField.h"
#include "shadow.h"
#include "devil.h"
#include "MapMove.h"
#include "GamePlayer.h"
#include "getscoreUi.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	// 加算するスコア
	const int ITEM_SCORE[] =
	{
		100, // 十字架
		1000, // 聖書
		150, // ぼわぼわ
		300, // ソフトクリーム
		300	 // 目玉焼き
	};

	const float BASE_Y = 50.0f; // 高さ

	const float SHADOW_SIZE = 25.0f;	// 丸影の大きさ
	const float SHADOW_LIMIT = 100.0f;	// 丸影の高さ上限
}

//==========================================
//  静的警告処理
//==========================================
static_assert(NUM_ARRAY(ITEM_SCORE) == CItem::TYPE_MAX, "ERROR : Type Count Missmatch");

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CItem>* CItem::m_pList = nullptr; // オブジェクトリスト

//====================================================================
// コンストラクタ
//====================================================================
CItem::CItem(int nPriority) : CObjectX(nPriority),
m_posBase(INITVECTOR3),
m_fMoveTime(0.0f),
m_pShadow(nullptr),
m_pEffect(nullptr)
{
	m_eType = TYPE_NONE;		// 種類
	m_nIdxXModel = 0;			// Xモデル番号
	m_nIdxTexture = -1;			// テクスチャ番号
	m_nStateCount = 0;			// 状態管理
	m_fColorA = 0.0f;			// 不透明度
	m_Scaling = 0.0f;			// 大きさ
	m_bCollision = false;		// 当たり判定
	m_CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 判定座標
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_Grid.x = 1;			// マップの横番号
	m_Grid.z = 0;			// マップの縦番号
	m_bMapScroll = true;
}

//====================================================================
// デストラクタ
//====================================================================
CItem::~CItem()
{

}

//==========================================
//  リストの取得
//==========================================
CListManager<CItem>* CItem::GetList(void)
{
	return m_pList;
}

//====================================================================
// 生成
//====================================================================
CItem* CItem::Create(const TYPE eType, const CMapSystem::GRID& pos)
{
	// インスタンス生成
	CItem* pItem = nullptr;

	if (pItem == nullptr)
	{
		switch (eType)
		{
		case CItem::TYPE_CROSS:		// 十字架
			pItem = new CCross;
			break;

		case CItem::TYPE_BIBLE:		// 聖書
			 pItem = new CBible;
			break;

		case CItem::TYPE_BOWABOWA:	// ぼわぼわ
			pItem = new CBowabowa;
			break;

		case CItem::TYPE_SOFTCREAM:	// ソフトクリーム
			pItem = new CSoftCream;
			break;

		case CItem::TYPE_FRIEDEGG: // 目玉焼き
			// 必ずオーバーライド先を呼び出すこと
			assert(false);
			break;

		default:
			assert(false);
			break;
		}
	}

	// 初期化処理
	pItem->Init();

	// タイプの設定
	pItem->SetItem(eType);

	// 位置の設定
	pItem->SetGrid(pos, true);

	// エフェクトを生成
	pItem->SetEffect();

	return pItem;
}

//==========================================
//  移動処理
//==========================================
void CItem::Move(D3DXVECTOR3& pos)
{
	if (m_bMapScroll)
	{
		// グリッドに沿った座標を取得、設定
		D3DXVECTOR3 posGrid = m_Grid.ToWorld();
		pos.x = posGrid.x;
		pos.z = posGrid.z;
	}
}

//====================================================================
// 初期化
//====================================================================
HRESULT CItem::Init(const char* pModelName)
{
	D3DXVECTOR3 pos = GetPos();

	// 継承クラスの初期化
	CObjectX::Init(pModelName);

	//マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	if (m_pShadow == nullptr)
	{// シャドウ生成
		m_pShadow = CShadow::Create(pos, D3DXVECTOR3(SHADOW_SIZE, 0.0f, SHADOW_SIZE), SHADOW_LIMIT);
	}

	pos.y = BASE_Y;
	SetPos(pos);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CItem>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CItem::Uninit()
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

	// 影の終了
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// エフェクトを消去
	if (m_pEffect != nullptr)
	{
		m_pEffect->SetDeath();
	}

	// 継承クラスの終了
	CObjectX::Uninit();
}

//====================================================================
// 更新
//====================================================================
void CItem::Update()
{
	// 自身の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 移動処理
	Move(pos);

	rot = GetRot();

	// 経過時間を取得
	m_fMoveTime += DeltaTime::Get();

	// プレイヤーとアイテムの判定
	CollisionPlayer();

	if (m_pShadow != nullptr)
	{// シャドウの位置設定
		m_pShadow->SetPos(D3DXVECTOR3(pos.x, 1.0f, pos.z));
		m_pShadow->SetBaseHeight(pos.y);
	}

	// エフェクトを動かす
	if (m_pEffect != nullptr)
	{
		D3DXMATRIX mat = *GetUseMultiMatrix();
		D3DXVECTOR3 ef = useful::CalcMatrix(pos, rot, *GetUseMultiMatrix());
		m_pEffect->SetPosition(ef);
	}

	// 情報の更新
	SetPos(pos);
	SetRot(rot);

	// 親クラスの更新処理
	CObjectX::Update();
}

//====================================================================
// 描画
//====================================================================
void CItem::Draw()
{

	// 継承クラスの描画
	CObjectX::Draw();
}

//==========================================
//  マップ番号の設定
//==========================================
void CItem::SetGrid(const CMapSystem::GRID& pos, bool bBase)
{
	// グリッド情報を保存する
	m_Grid = pos;

	// グリッド情報から自身の座標を算出する
	D3DXVECTOR3 calc = CMapSystem::GetInstance()->GetGritPos(pos);
	calc.y = BASE_Y;
	if (bBase) { m_posBase = calc; }

	// 位置を設定
	CItem::SetPos(calc);
	CObjectX::SetPos(calc);
}

//====================================================================
// プレイヤーとの当たり判定
//====================================================================
bool CItem::CollisionPlayer()
{
	// プレイヤーのリスト構造が無ければ抜ける
	if (CPlayer::GetList() == nullptr) { return false; }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // リストを取得

	// 自身の座標を取得する
	D3DXVECTOR3 posThis = GetPos();

	// プレイヤーリストの中身を確認する
	for (CPlayer* player : list)
	{
		// 自身が死んでいた場合関数を抜ける
		if (GetDeathFlag()) { return true; }

		// 死んでる場合は取得できない
		if(player->GetState() == CPlayer::STATE_EGG || player->GetState() == CPlayer::STATE_DEATH)
		{ continue; }

		// プレイヤーの座標(グリッド単位)を取得
		CMapSystem::GRID gridPlayer = player->GetGrid();

		// 存在座標が一致していない場合次に進む
		if (m_Grid != gridPlayer)
		{ continue; }

		// 取得に失敗した場合次に進む
		if (!Hit(player)) { continue; }

		// スコアが存在しない場合次に進む
		if (player->GetScore() == nullptr) { continue; }

		// スコアを加算する
		player->GetScore()->AddScore(ITEM_SCORE[m_eType]);
		//獲得スコア表示UIの生成
		CGetscoreUi::Create(ITEM_SCORE[m_eType], player->GetPos(), 6);
	}

	return false;
}
