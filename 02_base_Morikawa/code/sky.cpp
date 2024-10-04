//========================================
//
// フィールド[sky.cpp]
// Author：森川駿弥
//
//========================================
#include "sky.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float WIDTH = 200000.0f;	// 横幅
	const float INTERVAL_TEXU = 2000.0f;	// U座標の間隔
}

//========================================
//静的メンバ変数
//========================================
LPDIRECT3DTEXTURE9 CSky::m_pTexture = nullptr;

//========================================
//コンストラクタ
//========================================
CSky::CSky(int nPriority) :
	CObject3D(nPriority),
	m_fTexU(0.0f)
{
}

//========================================
//デストラクタ
//========================================
CSky::~CSky()
{
}

//========================================
//ポリゴン生成
//========================================
CSky* CSky::Create(void)
{
	//CPlayer型のポインタ
	CSky* pField = new CSky;;

	//初期化
	pField->Init();

	//ポインタを返す
	return pField;
}

//========================================
//初期化
//========================================
HRESULT CSky::Init(void)
{
	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// 継承の初期化
	CObject3D::Init();

	//テクスチャ割り当て
	BindTexture(pTexture->Regist("data\\TEXTURE\\sky_02.jpg"));


	return S_OK;
}

//========================================
//終了
//========================================
void CSky::Uninit(void)
{
	// 継承の初期化
	CObject3D::Uninit();
}

//========================================
//更新
//========================================
void CSky::Update(void)
{
	m_fTexU += 0.001f;

	// 継承の初期化
	CObject3D::Update();
}

//========================================
// 頂点情報
//========================================
void CSky::SetVertex3D()
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	int idxTexture = GetIdxTexture();
	if (idxTexture == 0) return;
	D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(idxTexture), WIDTH);

	// 頂点座標の設定
	MyLib::Vector3 pos(0.0f, -size.y * 0.5f, 5000.0f);

	pVtx[0].pos = pos + D3DXVECTOR3(-size.x, size.y,	0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(size.x, size.y, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(size.x, -size.y, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	// テクスチャ座標の設定
	float ratioX = size.x / INTERVAL_TEXU;
	float ratioY = ratioX / UtilFunc::Calculation::AspectRatio(size);
	pVtx[0].tex = D3DXVECTOR2(m_fTexU + 0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + ratioX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU + 0.0f, ratioY);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + ratioX, ratioY);

	// 頂点バッファをアンロック
	GetVtxBuff()->Unlock();
}

//========================================
//描画
//========================================
void CSky::Draw(void)
{
	//描画
	CObject3D::Draw();
}
