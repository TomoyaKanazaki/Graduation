//============================================
//
//	シグナルの処理 [signal.cpp]
//	Author:morikawa shunya
//
//============================================
#include "signal.h"
#include "useful.h"
#include "MapSystem.h"
#include "game.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"
#include "objmeshField.h"
#include "devil.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float BASE_Y = 50.0f; // 高さ
}

//===========================================
// 静的メンバ変数宣言
//===========================================
CListManager<CSignal>* CSignal::m_pList = nullptr; // オブジェクトリスト

//====================================================================
// コンストラクタ
//====================================================================
CSignal::CSignal(int nPriority) : CObjectX(nPriority)
{
	m_nIdxXModel = 0;			// Xモデル番号
	m_nIdxTexture = -1;			// テクスチャ番号
	m_nStateCount = 0;			// 状態管理
	m_fColorA = 0.0f;			// 不透明度
	m_Scaling = 0.0f;			// 大きさ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_nStateNum = 0;
}

//====================================================================
// デストラクタ
//====================================================================
CSignal::~CSignal()
{

}

//==========================================
//  リストの取得
//==========================================
CListManager<CSignal>* CSignal::GetList(void)
{
	return m_pList;
}

//====================================================================
// 生成
//====================================================================
CSignal* CSignal::Create(const char* pModelNameSignal)
{
	// インスタンス生成
	CSignal* pSignal = new CSignal();

	// オブジェクトの初期化処理
	if (FAILED(pSignal->Init(pModelNameSignal)))
	{// 初期化処理が失敗した場合

		return nullptr;
	}

	return pSignal;
}

//====================================================================
// 初期化
//====================================================================
HRESULT CSignal::Init(const char* pModelName)
{
	
	D3DXVECTOR3 pos = GetPos();

	// 継承クラスの初期化
	CObjectX::Init(pModelName);

	SetPos(pos);

	// リストマネージャーの生成
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CSignal>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// 終了
//====================================================================
void CSignal::Uninit()
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// 継承クラスの終了
	CObjectX::Uninit();
}

//====================================================================
// 更新
//====================================================================
void CSignal::Update()
{
	// 自身の情報を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 情報の更新
	SetPos(D3DXVECTOR3(pos.x, 300.0f, pos.z));

	// 親クラスの更新処理
	CObjectX::Update();
}

//====================================================================
// 描画
//====================================================================
void CSignal::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 => (参照値 => ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 継承クラスの描画
	CObjectX::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}