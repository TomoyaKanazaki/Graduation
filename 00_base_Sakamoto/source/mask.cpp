//=======================================
//
// ステンシルバッファに必要な2Dマスク[mask.cpp]
// Author : MORIKAWA SHUNYA
//
//=======================================

#include "mask.h"
#include "manager.h"
#include "renderer.h"

//========================================
// コンストラクタ
//========================================
CMask::CMask(int nPriority) : CObject2D(nPriority)
{
}

//========================================
// デストラクタ
//========================================
CMask::~CMask()
{
}

//========================================
// 生成
//========================================
CMask* CMask::Create(int nRefdx, D3DXCOLOR col)
{
	CMask* pMask = new CMask();

	//オブジェクトの初期化処理
	if (FAILED(pMask->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	// 参照値
	pMask->SetRefidx(nRefdx);

	// カラー設定
	pMask->SetColor(col);

	return pMask;
}

//========================================
// 初期化
//========================================
HRESULT CMask::Init()
{
	// 継承クラスの初期化
	CObject2D::Init();

	// 幅、高さの設定
	SetSize(SCREEN_SIZE);

	// 位置設定
	SetPos(SCREEN_CENTER_F);

	return S_OK;
}

//========================================
// 終了
//========================================
void CMask::Uninit()
{
	// 継承クラスの終了
	CObject2D::Uninit();
}

//========================================
// 更新
//========================================
void CMask::Update()
{
	// 継承クラスの更新
	CObject2D::Update();
}

//========================================
// 描画
//========================================
void CMask::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値の設定 => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, m_nRefidx);

	//ステンシルバッファの値に対してのマスク設定 => 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//ステンシルバッファの比較方法 <= (参照値 <= ステンシルバッファの参照値)なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Zテスト・ステンシルテスト失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテスト失敗・ステンシルテスト成功

	// 継承クラスの描画
	CObject2D::Draw();

	//ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}
