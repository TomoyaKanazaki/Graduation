//============================================
//
//	聖書の処理 [bible.cpp]
//	Author:morikawa shunya
//
//============================================
#include "bible.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "MapSystem.h"
#include "effect.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const char* MODEL_PASS = "data\\MODEL\\02_item\\holybible.x"; // モデルパス
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CBible>* CBible::m_pList = nullptr; // オブジェクトリスト

//====================================================================
// コンストラクタ
//====================================================================
CBible::CBible(int nPriority) : CItem(nPriority),
m_posBase(INITVECTOR3)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
	m_fMove = 0.0f;
}

//====================================================================
// デストラクタ
//====================================================================
CBible::~CBible()
{

}

//====================================================================
// 初期化
//====================================================================
HRESULT CBible::Init()
{
	// 継承クラスの初期化
	CItem::Init(MODEL_PASS);

	SetType(CObject::TYPE_BIBLE);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CBible>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// スクロールの対象から外す
	SetMapScroll(false);

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CBible::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 継承クラスの終了
	CItem::Uninit();
}

//====================================================================
// 更新
//====================================================================
void CBible::Update(void)
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
// タイトルでの更新
//====================================================================
void CBible::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	//位置更新
	pos += move;

	SetPos(pos);

	//頂点情報の更新
	CItem::Update();
}

//====================================================================
// ゲームでの更新
//====================================================================
void CBible::GameUpdate(void)
{
	//状態管理
	StateManager();

	//親クラスの更新
	CItem::Update();
}

//====================================================================
// 描画
//====================================================================
void CBible::Draw(void)
{
	// 継承クラスの描画
	CItem::Draw();
}

//====================================================================
// 動きの制御
//====================================================================
void CBible::Move()
{
	D3DXVECTOR3 pos = GetPos();

#ifdef _DEBUG

	CEffect* pEffect = CEffect::Create();
	pEffect->SetPos(pos);
	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pEffect->SetRadius(50.0f);

#endif // _DEBUG

	//位置更新
	CItem::SetPos(pos);
	CObjectX::SetPos(pos);
}

//==========================================
//  マップ番号の設定
//==========================================
void CBible::SetGrid(const GRID& pos)
{
	// 親クラスの設定処理を呼び出す
	CItem::SetGrid(pos);

	// グリッド情報から自身の座標を算出する
	m_posBase = CMapSystem::GetInstance()->GetGritPos(pos.x, pos.y);

	// 位置を設定
	CItem::SetPos(m_posBase);
	CObjectX::SetPos(m_posBase);
}

//====================================================================
// 状態管理
//====================================================================
bool CBible::Hit(CPlayer* pPlayer)
{
	if (pPlayer->GetItemType() != CPlayer::TYPE_NONE
		&& pPlayer->GetItemType() != CPlayer::TYPE_BIBLE)
	{
		return false;
	}

	// プレイヤーのアイテムを設定
	pPlayer->SetItemType(CPlayer::TYPE_BIBLE);

	// 自身の削除
	Uninit();
	return true;
}

//====================================================================
//状態管理
//====================================================================
void CBible::StateManager(void)
{
	CItem::STATE State = GetState();

	int nStateCounter = GetStateCounter();

	switch (State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (nStateCounter > 0)
	{
		nStateCounter--;
	}
}

//==========================================
//  リストの取得
//==========================================
CListManager<CBible>* CBible::GetList(void)
{
	return m_pList;
}
