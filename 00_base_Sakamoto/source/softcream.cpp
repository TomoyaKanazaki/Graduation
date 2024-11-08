//============================================
//
//  ソフトクリームの処理 [softcream.cpp]
//	Author:morikawa shunya
//
//============================================
#include "softcream.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "MapSystem.h"
#include "effect.h"
#include "game.h"
#include "objmeshField.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const char* MODEL_PASS = "data\\MODEL\\02_item\\softcream.x"; // モデルパス
	const float MOVE_SCALE_X = 1050.0f; // 移動幅
	const float MOVE_SCALE_Z = 650.0f; // 移動幅

	// TODO : 本仕様の時に必ず消す
	float fMove = 0;
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CSoftCream>* CSoftCream::m_pList = nullptr; // オブジェクトリスト

//====================================================================
// コンストラクタ
//====================================================================
CSoftCream::CSoftCream(int nPriority) : CItem(nPriority)
{
	SetSize(COLLISION_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
// デストラクタ
//====================================================================
CSoftCream::~CSoftCream()
{

}

//====================================================================
// 初期化
//====================================================================
HRESULT CSoftCream::Init()
{
	// 親クラスの初期化
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// オブジェクトの種類を設定
	SetType(CObject::TYPE_SOFTCREAM);

	// スクロールの対象から外す
	SetMapScroll(false);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CSoftCream>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CSoftCream::Uninit(void)
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
void CSoftCream::Update(void)
{
	DebugProc::Print(DebugProc::POINT_CENTER, "ソフトクリーム座標 : %f, %f, %f\n", GetPos().x, GetPos().y, GetPos().z);

	//親クラスの更新
	CItem::Update();
}

//====================================================================
// 描画
//====================================================================
void CSoftCream::Draw(void)
{
	// 継承クラスの描画
	CItem::Draw();
}

//====================================================================
// 動きの制御
//====================================================================
void CSoftCream::Move(D3DXVECTOR3& pos)
{
	// 移動量加算
	pos.x += MOVE_SCALE_X * DeltaTime::Get() * sinf(fMove);
	pos.z += MOVE_SCALE_Z * DeltaTime::Get() * cosf(fMove);

	// TODO : 本仕様の時に必ず消す
	fMove += DeltaTime::Get();
}

//==========================================
//  マップ番号の設定
//==========================================
void CSoftCream::SetGrid(const CMapSystem::GRID& pos)
{
	// 親クラスの設定処理を呼び出す
	CItem::SetGrid(pos);

	// グリッド情報から自身の座標を算出する
	D3DXVECTOR3 posThis = CMapSystem::GetInstance()->GetGritPos(pos.x, pos.z);

	// 位置を設定
	CItem::SetPos(posThis);
	CObjectX::SetPos(posThis);
}

//====================================================================
// 状態管理
//====================================================================
bool CSoftCream::Hit(CPlayer* pPlayer)
{
	if (pPlayer->GetItemType() != CPlayer::TYPE_NONE
		&& pPlayer->GetItemType() != CPlayer::TYPE_BIBLE)
	{
		return false;
	}

	// 自身の削除
	Uninit();
	return true;
}

//==========================================
// リストの取得
//==========================================
CListManager<CSoftCream>* CSoftCream::GetList(void)
{
	return m_pList;
}
