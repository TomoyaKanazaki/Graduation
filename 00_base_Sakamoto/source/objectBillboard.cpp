//============================================
//
//	オブジェクトビルボード [objectBillboard.cpp]
//	Author:sakamoto kai
//
//============================================
#include "objectBillboard.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "objmeshField.h"
#include "MapSystem.h"

#define POLYDON_SIZE (10.0f)

//====================================================================
//コンストラクタ
//====================================================================
CObjectBillboard::CObjectBillboard(int nPriority) :CObject(nPriority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_Width = POLYDON_SIZE;
	m_Height = POLYDON_SIZE;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Digit = 0;
	m_AddDarw = false;
}

//====================================================================
//デストラクタ
//====================================================================
CObjectBillboard::~CObjectBillboard()
{

}

//====================================================================
//生成処理
//====================================================================
CObjectBillboard *CObjectBillboard::Create(void)
{
	CObjectBillboard *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		//オブジェクト3Dの生成
		pObject3D = new CObjectBillboard();
	}

	//オブジェクトの初期化処理
	if (FAILED(pObject3D->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pObject3D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CObjectBillboard::Init(void)
{
	//マトリックスの掛け合わせをオンにする
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = D3DXVECTOR3(-m_Width * 0.5f, +m_Height * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_Width * 0.5f, +m_Height * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_Width * 0.5f, -m_Height * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_Width * 0.5f, -m_Height * 0.5f, 0.0f);

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

//====================================================================
//終了処理
//====================================================================
void CObjectBillboard::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//終了処理
//====================================================================
void CObjectBillboard::SetNULL(void)
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
void CObjectBillboard::Update(void)
{
	SetVerTex();
}

//====================================================================
//描画処理
//====================================================================
void CObjectBillboard::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス所得用

	if (m_AddDarw == true)
	{
		//aブレンディングを加算合成に設定
		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//Zテストを無効にする
	m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ライティングをオフにする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを所得
	m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面を向ける
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);	//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_UseMultiMatrix != nullptr)
	{
		//算出したマトリクスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			m_UseMultiMatrix);
	}

	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	assert(GetIdx() != -1);		//テクスチャの番号を入れ忘れた場合エラーを吐く

	//テクスチャの設定
	m_pDevice->SetTexture(0, pTexture->GetAddress(GetIdx()));

	//ポリゴンの描画
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,										//プリミティブ(ポリゴン)数
		2);

	//ライティングをオンにする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//アルファテストを無効にする
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを有効にする
	m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

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
void CObjectBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObjectBillboard::SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = Pos0;
	pVtx[1].pos = Pos1;
	pVtx[2].pos = Pos2;
	pVtx[3].pos = Pos3;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点座標の設定
//====================================================================
void CObjectBillboard::SetVerTex(void)
{
	VERTEX_3D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = D3DXVECTOR3(-m_Width * 0.5f + (m_Digit * m_Width * 0.75f), +m_Height * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_Width * 0.5f + (m_Digit * m_Width * 0.75f), +m_Height * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_Width * 0.5f + (m_Digit * m_Width * 0.75f), -m_Height * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_Width * 0.5f + (m_Digit * m_Width * 0.75f), -m_Height * 0.5f, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャ座標の設定
//====================================================================
void CObjectBillboard::SetAnim(D3DXVECTOR2 Tex)
{
	VERTEX_3D*pVtx;	//頂点ポインタを所得

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
void CObjectBillboard::SetAnim(D3DXVECTOR2 TexMin, D3DXVECTOR2 TexMax)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

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
void CObjectBillboard::SetAnimTex(const int nNum)
{
	// ポインタを宣言
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * nNum, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nNum+0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nNum,1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nNum+0.1f,1.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}
//====================================================================
//頂点カラーの設定
//====================================================================
void CObjectBillboard::SetColor(D3DXCOLOR col)
{
	m_Color = col;

	VERTEX_3D*pVtx;	//頂点ポインタを所得

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
//頂点カラーの設定
//====================================================================
void CObjectBillboard::SetTexture(const char *name)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	m_nIdxTexture = pTexture->Regist(name);
}
