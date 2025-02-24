//============================================
//
//	オブジェクト2D [object2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//====================================================================
//コンストラクタ
//====================================================================
CObject2D::CObject2D(int nPriority) :CObject(nPriority),
	m_nIdxTexture(-1)
{
	m_pTexture = nullptr;
	m_pTargetTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_AddDarw = false;
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MultiTarget = false;
}

//====================================================================
//デストラクタ
//====================================================================
CObject2D::~CObject2D()
{

}

//====================================================================
//生成処理
//====================================================================
CObject2D *CObject2D::Create(int nPriority)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//オブジェクト2Dの生成
		pObject2D = new CObject2D(nPriority);
	}

	//オブジェクトの初期化処理
	if (FAILED(pObject2D->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObject2D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CObject2D::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	SetType(TYPE_OBJECT2D);

	//頂点バッファの生成
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	VERTEX_2D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	float nLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;
	float fAngle = atan2f(m_size.x, m_size.y);

	pVtx[0].pos.x = m_pos.x + sinf(-D3DX_PI + fAngle + m_rot.z) * nLength;
	pVtx[0].pos.y = m_pos.y + cosf(-D3DX_PI + fAngle + m_rot.z) * nLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - fAngle + m_rot.z) * nLength;
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - fAngle + m_rot.z) * nLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(-fAngle + m_rot.z) * nLength;
	pVtx[2].pos.y = m_pos.y + cosf(-fAngle + m_rot.z) * nLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(+fAngle + m_rot.z) * nLength;
	pVtx[3].pos.y = m_pos.y + cosf(+fAngle + m_rot.z) * nLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CObject2D::Uninit(void)
{
	SetNULL();
	SetDeathFlag(true);
}

//====================================================================
//終了処理
//====================================================================
void CObject2D::SetNULL(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pTargetTexture != nullptr)
	{
		m_pTargetTexture->Release();
		delete m_pTargetTexture;
		m_pTargetTexture = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CObject2D::Update(void)
{
	SetVerTex();
}

//====================================================================
//描画処理
//====================================================================
void CObject2D::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_AddDarw == true)
	{
		//aブレンディングを加算合成に設定
		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//頂点バッファをデータストリームに設定
	m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX_2D);

	////テクスチャの設定
	//pDevice->SetTexture(0, m_pTexture);

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//assert(GetIdx() != -1);		//テクスチャの番号を入れ忘れた場合エラーを吐く

	if (m_MultiTarget == true)
	{
		//テクスチャの設定
		m_pDevice->SetTexture(0, m_pTargetTexture);
	}
	else
	{
		//テクスチャの設定
		if (GetIdx() != -1)
		{
			m_pDevice->SetTexture(0, pTexture->GetAddress(GetIdx()));
		}
		else
		{
			m_pDevice->SetTexture(0, nullptr);
		}
	}

	//ポリゴンの描画
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,						//描画の開始位置
		2);						//プリミティブ(ポリゴン)数

	if (m_AddDarw == true)
	{
		//aブレンディングを通常に設定
		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//====================================================================
//テクスチャの割り当て
//====================================================================
void CObject2D ::BindTexture (LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObject2D::SetVerTex(void)
{
	VERTEX_2D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	float nLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;
	float fAngle = atan2f(m_size.x, m_size.y);

	pVtx[0].pos.x = m_pos.x + sinf(-D3DX_PI + fAngle + m_rot.z) * nLength;
	pVtx[0].pos.y = m_pos.y + cosf(-D3DX_PI + fAngle + m_rot.z) * nLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - fAngle + m_rot.z) * nLength;
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - fAngle + m_rot.z) * nLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(-fAngle + m_rot.z) * nLength;
	pVtx[2].pos.y = m_pos.y + cosf(-fAngle + m_rot.z) * nLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(+fAngle + m_rot.z) * nLength;
	pVtx[3].pos.y = m_pos.y + cosf(+fAngle + m_rot.z) * nLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャ座標の設定
//====================================================================
void CObject2D::SetAnim(D3DXVECTOR2 Tex)
{
	VERTEX_2D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Tex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, Tex.y);
	pVtx[3].tex = D3DXVECTOR2(Tex.x, Tex.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャ座標の設定（酒井）
//====================================================================
void CObject2D::SetAnim(D3DXVECTOR2 TexMin, D3DXVECTOR2 TexMax)
{
	VERTEX_2D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(TexMin.x, TexMin.y);
	pVtx[1].tex = D3DXVECTOR2(TexMax.x, TexMin.y);
	pVtx[2].tex = D3DXVECTOR2(TexMin.x, TexMax.y);
	pVtx[3].tex = D3DXVECTOR2(TexMax.x, TexMax.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//============================================================
//	アニメーションのテクスチャ座標の設定処理(丹野竜之介)
//============================================================
void CObject2D::SetAnimTex(const int nPattern, const int nWidthPtrn, const int nHeightPtrn)
{
	// 変数を宣言
	float fWidthRate = 1.0f / nWidthPtrn;	// 横の分割数の割合
	float fHeightRate = 1.0f / nHeightPtrn;	// 縦の分割数の割合
	int nWidthCurrent = nPattern % nWidthPtrn;					// 現在の横のパターン
	int nHeightCurrent = (nPattern / nWidthPtrn) % nHeightPtrn;	// 現在の縦のパターン

	// ポインタを宣言
	VERTEX_2D* pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(fWidthRate * nWidthCurrent, fHeightRate * nHeightCurrent);
		pVtx[1].tex = D3DXVECTOR2(fWidthRate * (nWidthCurrent + 1), fHeightRate * nHeightCurrent);
		pVtx[2].tex = D3DXVECTOR2(fWidthRate * nWidthCurrent, fHeightRate * (nHeightCurrent + 1));
		pVtx[3].tex = D3DXVECTOR2(fWidthRate * (nWidthCurrent + 1), fHeightRate * (nHeightCurrent + 1));

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}
//====================================================================
//テクスチャ座標の設定
//====================================================================
void CObject2D::SetScroll(D3DXVECTOR2 Tex)
{
	VERTEX_2D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(Tex.x, Tex.y);
	pVtx[1].tex = D3DXVECTOR2(Tex.x + 1.0f, Tex.y);
	pVtx[2].tex = D3DXVECTOR2(Tex.x, Tex.y + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(Tex.x + 1.0f, Tex.y + 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CObject2D::SetColor(D3DXCOLOR col)
{
	if (m_pVtxBuff == nullptr) { return; }

	m_Color = col;

	VERTEX_2D *pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラー（アルファ値）の設定
//====================================================================
void CObject2D::SetAlpha(float A)
{
	m_Color.a = A;

	VERTEX_2D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャの設定
//====================================================================
void CObject2D::SetTexture(const std::string name)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	m_nIdxTexture = pTexture->Regist(name);
}
