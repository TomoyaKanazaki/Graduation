//============================================
//
//	オブジェクトゲージ2D [objGauge2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "objGauge2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//====================================================================
//コンストラクタ
//====================================================================
CObjGauge2D::CObjGauge2D(int nPriority) :CObject(nPriority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_Width = 1.0f;
	m_Height = 1.0f;
	m_AddDarw = false;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Ratio = 1.0f;
	m_type = TYPE_NONE;
	m_bAppear = false;
}

//====================================================================
//デストラクタ
//====================================================================
CObjGauge2D::~CObjGauge2D()
{

}

//====================================================================
//生成処理
//====================================================================
CObjGauge2D* CObjGauge2D::Create(int nPriority)
{
	CObjGauge2D* pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//オブジェクト2Dの生成
		pObject2D = new CObjGauge2D(nPriority);
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
HRESULT CObjGauge2D::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	SetTexture("data\\TEXTURE\\Test.jpg");

	SetType(TYPE_OBJECT2D);
	m_bAppear = true;

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

	VERTEX_2D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + m_Width * m_Ratio;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x;
	pVtx[2].pos.y = m_pos.y + m_Height;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + m_Width * m_Ratio;
	pVtx[3].pos.y = m_pos.y + m_Height;
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
void CObjGauge2D::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//終了処理
//====================================================================
void CObjGauge2D::SetNULL(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CObjGauge2D::Update(void)
{
	switch (m_type)
	{
	case CObjGauge2D::TYPE_NONE:
		SetVerTex();
		break;
	case CObjGauge2D::TYPE_LEFT:
		SetVerTex1();
		break;
	}
}

//====================================================================
//描画処理
//====================================================================
void CObjGauge2D::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (!m_bAppear)
	{
		return;
	}

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

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	assert(GetIdx() != -1);		//テクスチャの番号を入れ忘れた場合エラーを吐く

	//テクスチャの設定
	m_pDevice->SetTexture(0, pTexture->GetAddress(GetIdx()));

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
void CObjGauge2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObjGauge2D::SetVerTex(void)
{
	VERTEX_2D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos.x = m_pos.x;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + m_Width * m_Ratio;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x;
	pVtx[2].pos.y = m_pos.y + m_Height;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + m_Width * m_Ratio;
	pVtx[3].pos.y = m_pos.y + m_Height;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObjGauge2D::SetVerTex1(void)
{
	VERTEX_2D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos.x = m_pos.x - m_Width * m_Ratio;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x - m_Width * m_Ratio;
	pVtx[2].pos.y = m_pos.y + m_Height;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x;
	pVtx[3].pos.y = m_pos.y + m_Height;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャ座標の設定
//====================================================================
void CObjGauge2D::SetAnim(D3DXVECTOR2 Tex)
{
	VERTEX_2D* pVtx;	//頂点ポインタを所得

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
//テクスチャ座標の設定
//====================================================================
void CObjGauge2D::SetAnim1(D3DXVECTOR2 Tex)
{
	VERTEX_2D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(Tex.x, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(Tex.x, Tex.y);
	pVtx[3].tex = D3DXVECTOR2(0.0f, Tex.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャ座標の設定
//====================================================================
void CObjGauge2D::SetScroll(D3DXVECTOR2 Tex)
{
	VERTEX_2D* pVtx;	//頂点ポインタを所得

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
void CObjGauge2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CObjGauge2D::SetTexture(const char* name)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	m_nIdxTexture = pTexture->Regist(name);
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CObjGauge2D::SetGaugeWidth(float nCMax, float nCMeter)
{
	m_Ratio = (float)nCMeter / nCMax;
}