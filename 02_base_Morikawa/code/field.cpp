//========================================
//
// フィールド[field.cpp]
// Author：森川駿弥
//
//========================================
#include "field.h"
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
	const float INTERVAL_TEXU = 128.0f;	// U座標の間隔
}

//========================================
//静的メンバ変数
//========================================
LPDIRECT3DTEXTURE9 CField::m_pTexture = nullptr;

//========================================
//コンストラクタ
//========================================
CField::CField(int nPriority) :
	CObject3D(nPriority)
{
}

//========================================
//デストラクタ
//========================================
CField::~CField()
{
}

//========================================
//ポリゴン生成
//========================================
CField* CField::Create(void)
{
	//CPlayer型のポインタ
	CField* pField = new CField;;

	//初期化
	pField->Init();

	//ポインタを返す
	return pField;
}

//========================================
//初期化
//========================================
HRESULT CField::Init(void)
{
	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// 継承の初期化
	CObject3D::Init();

	//テクスチャ割り当て
	BindTexture(pTexture->Regist("data\\TEXTURE\\glass.jpg"));


	return S_OK;
}

//========================================
//終了
//========================================
void CField::Uninit(void)
{
	// 継承の初期化
	CObject3D::Uninit();
}

//========================================
//更新
//========================================
void CField::Update(void)
{
	// 継承の初期化
	CObject3D::Update();
}

//========================================
// 頂点情報
//========================================
void CField::SetVertex3D()
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
	pVtx[0].pos = D3DXVECTOR3(-size.x, 0.0f, size.y);
	pVtx[1].pos = D3DXVECTOR3(size.x, 0.0f, size.y);
	pVtx[2].pos = D3DXVECTOR3(-size.x, 0.0f, -size.y);
	pVtx[3].pos = D3DXVECTOR3(size.x, 0.0f, -size.y);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	// テクスチャ座標の設定
	float ratioX = size.x / INTERVAL_TEXU;
	float ratioY = ratioX / UtilFunc::Calculation::AspectRatio(size);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(ratioX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, ratioY);
	pVtx[3].tex = D3DXVECTOR2(ratioX, ratioY);

	// 頂点バッファをアンロック
	GetVtxBuff()->Unlock();
}

//========================================
//描画
//========================================
void CField::Draw(void)
{
	//描画
	CObject3D::Draw();
}
