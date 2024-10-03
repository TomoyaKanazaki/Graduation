//==============================================================
//
//ビルボード処理[billboard.h]
//Author:佐藤根詩音
//
//==============================================================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//マクロ定義
#define PRIORITY		(3)		//優先順位

//==============================================================
//コンストラクタ
//==============================================================
CBillboard::CBillboard()
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_type = CObject::TYPE_NONE;		//種類
	m_fHeight = 0.0f;		//縦幅
	m_fWidth = 0.0f;		//横幅
}

//==============================================================
//デストラクタ
//==============================================================
CBillboard::~CBillboard()
{

}

//==============================================================
//生成処理
//==============================================================
CBillboard *CBillboard::Create(void)
{
	CBillboard *pBillboard = NULL;

	if (pBillboard == NULL)
	{//メモリが使用されてなかったら

		//ビルボードの生成
		pBillboard = new CBillboard;
	}

	//初期化処理
	pBillboard->Init();

	return pBillboard;
}

//==============================================================
//テクスチャ割り当て処理
//==============================================================
void CBillboard::BindTexture(int nIdxTexture)
{
	m_nIdxTexture = nIdxTexture;		//テクスチャ割り当て
}

//==============================================================
//ビルボードの初期化処理
//==============================================================
HRESULT CBillboard::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

//==============================================================
//ビルボードの終了処理
//==============================================================
void CBillboard::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//ビルボードの更新処理
//==============================================================
void CBillboard::Update(void)
{

}

//==============================================================
//ビルボードの描画処理
//==============================================================
void CBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();		//テクスチャの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//ワールドマトリックスを初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);		//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//プリミティブ（ビルボードの数）
		2);												//描画するプリミティブ数

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==============================================================
//ビルボードの描画設定
//==============================================================
void CBillboard::SetDraw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();		//テクスチャの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ワールドマトリックスを初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);		//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//プリミティブ（ビルボードの数）
		2);												//描画するプリミティブ数

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================
//ビルボードのサイズ設定処理
//==============================================================
void CBillboard::SetSize(float fWidth, float fHeight)
{
	m_fHeight = fHeight; 
	m_fWidth = fWidth;

	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================
//ビルボードの色設定処理
//==============================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	m_col = col;		//色

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_type)
	{
	case CObject::TYPE_EFFECT:		//エフェクト

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

		break;

	default:

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================
//アニメーション設定処理
//==============================================================
void CBillboard::SetType(int nPatternAnim)
{
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_type)
	{
	case TYPE_EXPLOSION:

		//テクスチャ座標の再設定
		pVtx[0].tex = D3DXVECTOR2(nPatternAnim * 0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nPatternAnim * 0.125f + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nPatternAnim * 0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nPatternAnim * 0.125f + 0.125f, 1.0f);

		break;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}