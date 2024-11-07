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

//==========================================
//  定数定義
//==========================================
namespace
{
	// 加算するスコア
	const int ITEM_SCORE[] =
	{
		10, // 十字架
		10, // 聖書
		10 // ぼわぼわ
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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 過去の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_nMapWidth = 1;			// マップの横番号
	m_nMapHeight = 0;			// マップの縦番号
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
CItem* CItem::Create(const TYPE eType, const GRID& pos)
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
	pItem->m_eType = eType;

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
	if (m_bMapScroll == true)
	{
		m_pos = CMapSystem::GetInstance()->GetGritPos(m_nMapWidth, m_nMapHeight);
		m_pos.y = 0.0f;

		CObjectX::SetPos(m_pos);

		// 継承クラスの更新
		CObjectX::Update();
	}
	else
	{
		Move();
	}

	// プレイヤーとアイテムの判定
	CollisionPlayer();

#ifdef _DEBUG

	CEffect* pEffect = CEffect::Create();
	pEffect->SetPos(m_pos);
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pEffect->SetRadius(50.0f);

#endif // _DEBUG
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
void CItem::SetGrid(const GRID& pos)
{
	SetWightNumber(pos.x);
	SetHeightNumber(pos.y);
}

//====================================================================
// プレイヤーとの当たり判定
//====================================================================
bool CItem::CollisionPlayer()
{
	// プレイヤーのリスト構造が無ければ抜ける
	if (CPlayer::GetList() == nullptr) { return false; }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // リストを取得

	// プレイヤーリストの中身を確認する
	for (CPlayer* player : list)
	{
		D3DXVECTOR3 pos = player->GetPos();
		D3DXVECTOR3 size = player->GetSize();

		// 矩形の当たり判定
		if (useful::CollisionCircle(m_pos, pos, size.x))
		{
			// 取得に成功している場合スコアを加算する
			if (Hit(player)) { CGame::GetScore()->AddScore(ITEM_SCORE[m_eType]); }
			return true;
		}
	}

	return false;
}
