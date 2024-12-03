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
	// 移動情報を取得
	D3DXVECTOR3 base = GetBase();
	float time = GetMoveTime();

	// 移動量加算
	float x = MOVE_SCALE_X * sinf(time);
	float z = MOVE_SCALE_Z * cosf(time);

	// 基準位置に移動量を加算する
	pos.x = base.x + x;
	pos.z = base.z + z;

	DebugProc::Print(DebugProc::POINT_CENTER, "ソフトクリーム : %f, %f\n", pos.x, pos.z);
}

//====================================================================
// 状態管理
//====================================================================
bool CSoftCream::Hit(CPlayer* pPlayer)
{
	// エフェクトを生成
		// エフェクトを生成する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_GET_SOFTCREAM, false, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot);

	// 自身の削除
	Uninit();
	return true;
}

//==========================================
//  エフェクトを生成
//==========================================
void CSoftCream::SetEffect()
{
	// 自身の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	pos.y = 0.0f;
	D3DXVECTOR3 rot = GetRot();

	Effect(MyEffekseer::EffectCreate(CMyEffekseer::TYPE_SOFTCREAM, true, useful::CalcMatrix(pos, rot, *GetUseMultiMatrix()), rot, D3DXVECTOR3(20.0f, 20.0f, 20.0f)));
}

//==========================================
// リストの取得
//==========================================
CListManager<CSoftCream>* CSoftCream::GetList(void)
{
	return m_pList;
}
