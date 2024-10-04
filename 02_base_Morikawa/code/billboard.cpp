//=======================================
//
//ビルボード[billboard.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"
#include "billboard.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"

//========================================
// 定数定義
//========================================
namespace
{
const char* BILLBOARD_TEX = "data\\TEXTURE\\effect000.jpg";	// マップオブジェクトのパス
}

//=======================================
//コンストラクタ
//=======================================
CBillboard::CBillboard(int nPriority) :
	CObject(nPriority),
	m_nIdxTexture(0),		//テクスチャ番号
	m_pVtxBuff(nullptr),	//頂点情報
	m_col(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)),	//色
	m_fLength(0.0f),	//対角線の長さ
	m_fAngle(0.0f),		//対角線の長さ
	m_bFront(true)		// 正面向ける
{
}

//=======================================
//デストラクタ
//=======================================
CBillboard::~CBillboard()
{
}

//=======================================
//ビルボード生成
//=======================================
CBillboard *CBillboard::Create(D3DXVECTOR3 pos, float fwidth, float fheight)
{
	//pBillboard型のポインタ
	CBillboard *pBillboard = nullptr;

	if (pBillboard == nullptr)
	{//pBillboardがnullptrの時
		//ビルボード生成
		pBillboard = new CBillboard;

		//初期化
		pBillboard->Init();

		//位置設定
		pBillboard->SetPos(pos);

		//サイズ設定
		pBillboard->SetSizeVertex(fwidth, fheight);
	}

	//ポインタを返す
	return pBillboard;
}

//=======================================
//ビルボードの初期化処理
//=======================================
HRESULT CBillboard::Init(void)
{
	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);


	return S_OK;
}

//=======================================
//ビルボードの終了処理
//=======================================
void CBillboard::Uninit(void)
{
	//頂点バッファの終了処理
	if (m_pVtxBuff != nullptr)
	{//m_pTextureがnullptrじゃないとき
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//オブジェクト(自分自身)の破棄
	Release();
}

//=======================================
//ビルボードの更新処理
//=======================================
void CBillboard::Update(void)
{
	// 頂点座標の設定
	SetVertex();
}

//=======================================
//ビルボードの描画処理
//=======================================
void CBillboard::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXMATRIX mtxView;		//ビューマトリクス取得用

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//CTexture型のポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// 位置
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 Rot = GetRot();


	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);	//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture)); //テクスチャを使用してないときはnullptr

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
//テクスチャの割り当て
//========================================
void CBillboard::BindTexture(int pTexture)
{
	m_nIdxTexture = pTexture;
}

//========================================
//弾の頂点座標
//========================================
void CBillboard::SetPosVertex(float size)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size, size, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size, size, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size, -size, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size, -size, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//アニメーションビルボードの頂点座標
//========================================
void CBillboard::SetVertexAnim(int Num, int Cnt)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の再設定
	pVtx[0].tex = D3DXVECTOR2(1.0f / Num * Cnt, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / Num * (Cnt + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / Num * Cnt, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / Num * (Cnt + 1), 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//頂点情報のサイズ設定
//========================================
void CBillboard::SetSizeVertex(float fwidth, float fheight)
{
	m_fWidth = fwidth;
	m_fHeight = fheight;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fwidth, fheight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fwidth, fheight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fwidth, -fheight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fwidth, -fheight, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//ビルボードエフェクトの頂点座標
//========================================
void CBillboard::SetEffectVertex(float fwidth, float fHeight)
{
	m_fWidth = fwidth;

	m_fHeight = fHeight;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の再設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//ビルボードの頂点座標
//========================================
void CBillboard::SetVertex(void)
{
	//頂点情報へのポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//========================================
//弾ビルボードの頂点座標
//========================================
void CBillboard::SetSize(float fWidht, float fHeight)
{
	m_fWidth = fWidht;
	m_fHeight = fHeight;

	//対角線の長さ(横、縦)
	m_fLength = sqrtf(fWidht * fWidht + fHeight * fHeight) * 1.0f;

	//対角線の向き(横、縦)
	m_fAngle = atan2f(fWidht, fHeight);
}

//========================================
//弾ビルボードの頂点座標
//========================================
float CBillboard::GetSize()
{
	return m_fWidth, m_fHeight;
}

//========================================
//色設定
//========================================
void CBillboard::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

//========================================
//色取得
//========================================
D3DXCOLOR CBillboard::GetCol()
{
	return m_col;
}
