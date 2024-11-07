//============================================
//
//	オブジェクトＸモデルのサンプル [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "cross.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int CROSS_DELETTIME = 600;	// 十字架消滅時間
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 200.0f);		//当たり判定
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//当たり判定
	const char* MODEL_PASS = "data\\MODEL\\zyuzika.x"; // モデルパス
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CCross>* CCross::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CCross::CCross(int nPriority) : CItem(nPriority)
{
	SetSize(SAMPLE_SIZE);
	SetPos(INITVECTOR3);
}

//====================================================================
//デストラクタ
//====================================================================
CCross::~CCross()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CCross::Init()
{
	// 親クラスの初期化
	if (FAILED(CItem::Init(MODEL_PASS))) { assert(false); return E_FAIL; }

	// オブジェクトの種類を設定
	SetType(CObject::TYPE_CROSS);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CCross>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCross::Uninit(void)
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 親クラスの終了処理
	CItem::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CCross::Update(void)
{
	// 親クラスの更新処理
	CItem::Update();
}

//====================================================================
//描画処理
//====================================================================
void CCross::Draw(void)
{
	// 親クラスの描画処理
	CItem::Draw();
}

//====================================================================
//　ヒット処理
//====================================================================
bool CCross::Hit(CPlayer* pPlayer)
{
	if (pPlayer->GetItemType() != CPlayer::TYPE_NONE
		&& pPlayer->GetItemType() != CPlayer::TYPE_CROSS)
	{
		return false;
	}

	// プレイヤーのアイテムを設定
	pPlayer->SetItemType(CPlayer::TYPE_CROSS);
	return true;
}

//==========================================
//  リストの取得
//==========================================
CListManager<CCross>* CCross::GetList(void)
{
	return m_pList;
}
