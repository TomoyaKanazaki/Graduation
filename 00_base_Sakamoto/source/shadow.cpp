//============================================
//
//	影処理 [shadow.cpp]
//	Author:morikawa shunya
//
//============================================

#include "shadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "objmeshField.h"

//===========================================
// コンストラクタ
//===========================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{
}

//===========================================
// デストラクタ
//===========================================
CShadow::~CShadow()
{
}

//===========================================
// 生成
//===========================================
CShadow* CShadow::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight)
{
	// インスタンス生成
	CShadow* pShadow = new CShadow;

	// 初期化
	pShadow->Init();

	// 位置設定
	pShadow->SetPos(pos);

	// 大きさ
	pShadow->SetpVtx(fWidth, fHeight);

	return pShadow;
}

//===========================================
// 初期化
//===========================================
HRESULT CShadow::Init()
{
	// 継承クラスの初期化
	CObject3D::Init();

	// マップとのマトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

	// テクスチャ設定
	SetTexture("data\\TEXTURE\\player\\shadow000.jpg");

	return S_OK;
}

//===========================================
// 終了
//===========================================
void CShadow::Uninit(void)
{
	// 継承クラスの終了
	CObject3D::Uninit();
}

//===========================================
// 更新
//===========================================
void CShadow::Update(void)
{
}

//===========================================
// 描画
//===========================================
void CShadow::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 継承クラスの描画
	CObject3D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
