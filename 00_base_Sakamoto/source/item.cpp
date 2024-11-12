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
#include "effect.h"
#include "MapSystem.h"
#include "bible.h"
#include "game.h"
#include "score.h"
#include "softcream.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	// 加算するスコア
	const int ITEM_SCORE[] =
	{
		100, // 十字架
		100, // 聖書
		100, // ぼわぼわ
		400, // ソフトクリーム
		200 // 目玉焼き
	};
}

//==========================================
//  静的警告処理
//==========================================
static_assert(NUM_ARRAY(ITEM_SCORE) == CItem::TYPE_MAX, "ERROR : Type Count Missmatch");

//====================================================================
// コンストラクタ
//====================================================================
CItem::CItem(int nPriority) : CObjectX(nPriority)
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

	// 位置の設定
	pItem->SetGrid(pos);

	// タイプの設定
	pItem->SetItem(eType);

	return pItem;
}

//====================================================================
// 初期化
//====================================================================
HRESULT CItem::Init(const char* pModelName)
{
	// 継承クラスの初期化
	CObjectX::Init(pModelName);

	//マップとのマトリックスの掛け合わせをオンにする
	SetMultiMatrix(true);

	D3DXVECTOR3 pos = GetPos();
	pos.y = 50.0f;
	SetPos(pos);

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CItem::Uninit()
{
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

	if (m_bMapScroll)
	{
		// グリッドに沿った座標を取得、設定
		D3DXVECTOR3 posGrid = CMapSystem::GetInstance()->GetGritPos(m_Grid);
		pos.x = posGrid.x;
		pos.z = posGrid.z;
	}

	// 移動処理
	Move(pos);

	// プレイヤーとアイテムの判定
	CollisionPlayer();

	// 情報の更新
	SetPos(pos);
	SetRot(rot);
	m_Grid = CMapSystem::GetInstance()->CalcGrid(pos);

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
		// プレイヤーの座標(グリッド単位)を取得
		CMapSystem::GRID gridPlayer = player->GetGrid();

		// 存在座標が一致していない場合次に進む
		if (m_Grid != gridPlayer)
		{ continue; }

		// 取得に失敗した場合次に進む
		if (!Hit(player)) { continue; }

		// スコアを加算する
		CGame::GetScore()->AddScore(ITEM_SCORE[m_eType]);
	}

	return false;
}
