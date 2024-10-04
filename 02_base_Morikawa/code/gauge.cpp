//========================================
//
// ゲージ[gauge.h]
// Author：森川駿弥
//
//========================================
#include "gauge.h"
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//========================================
// 定数定義
//========================================
namespace
{
const float LENGTH = 640.0f;		// ゲージの長さ
const char* GAUGETEX[] =
	{// 読み込むテクスチャ
		"",
		"data\\texture\\gauge.png",
		"data\\texture\\frame.png"
	};
}

//========================================
// コンストラクタ
//========================================
CGauge::CGauge(int nPriority) : CObject(nPriority)
{
	m_nMaxLife = 0;			// 体力の最大値
	m_nLife = 0;			// 現在の体力
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_aTexU = 0.0f;			// テクスチャのU値
	memset(m_p2D, 0, sizeof(m_p2D));	// オブジェクト2Dポインタ
}

//========================================
// デストラクタ
//========================================
CGauge::~CGauge()
{
}

//========================================
// 生成
//========================================
CGauge* CGauge::Create(int nMaxLife)
{
	CGauge* pGauge = new CGauge;

	// 体力の最大値
	pGauge->m_nMaxLife = nMaxLife;

	pGauge->Init();

	return pGauge;
}

//========================================
// 初期化
//========================================
HRESULT CGauge::Init()
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// 体力の初期値
	m_nLife = m_nMaxLife;

	// テクスチャ座標の初期化(U値)
	m_aTexU = 0.0f;

	for (int i = 0; i < CGauge::TYPE_MAX; i++)
	{
		if (m_p2D[i] == nullptr)
		{// 種類分回す
			m_p2D[i] = CObject2D::Create();

			m_p2D[i]->BindTexture(pTexture->Regist(GAUGETEX[i]));
		}
	}

	// 頂点情報の設定
	SetVertex();

	// 頂点カラーの設定
	m_p2D[CGauge::TYPE_BASE]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//========================================
// 終了
//========================================
void CGauge::Uninit()
{
	// 自身の終了
	Release();
}

//========================================
// 更新
//========================================
void CGauge::Update()
{
	for (int i = 0; i < CGauge::TYPE_MAX; i++)
	{
		// 2Dの更新
		if (m_p2D[i] != nullptr)
		{
			m_p2D[i]->Update();
		}
	}

	// 頂点情報
	SetVertex();

	// テクスチャ座標の更新(U値)
	m_aTexU += 0.002f;
}

//========================================
// 描画
//========================================
void CGauge::Draw()
{
	for (int i = 0; i < CGauge::TYPE_MAX; i++)
	{
		if (m_p2D[i] != nullptr)
		{// 2Dの描画
			m_p2D[i]->Draw();
		}
	}
}

//========================================
// サイズ設定
//========================================
void CGauge::SetSize(float fWidht, float fHeight)
{
	// 対角線の長さ(横、縦)
	m_fLength = sqrtf(fWidht * fWidht + fHeight * fHeight) * 0.5f;

	// 対角線の向き(横、縦)
	m_fAngle = atan2f(fWidht, fHeight);

	// 頂点情報
	SetVertex();
}

//========================================
// 頂点情報の設定
//========================================
void CGauge::SetVertex()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 体力の比率
	float ratio = (float)m_nLife / (float)m_nMaxLife;

	for (int i = 0; i < CGauge::TYPE_MAX; i++)
	{
		// 2Dの頂点情報設定
		m_p2D[i]->SetVertex();

		D3DXCOLOR col = m_p2D[i]->GetCol();

		// 頂点情報の取得
		LPDIRECT3DVERTEXBUFFER9 Vertex = m_p2D[i]->GetVtxBuff();

		// 頂点情報へのポインタ
		VERTEX_2D* pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		Vertex->Lock(0, 0, (void**)&pVtx, 0);

		switch (i)
		{
		case TYPE_GAUGE:
			// 頂点座標の設定
			pVtx[0].pos = pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3(LENGTH * ratio, 0.0f, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(0.0f, m_fLength, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3(LENGTH * ratio, m_fLength, 0.0f);
			break;

		default:
			// 頂点座標の設定
			pVtx[0].pos = pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3(LENGTH, 0.0f, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(0.0f, m_fLength, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3(LENGTH, m_fLength, 0.0f);
			break;
		}

		// 位置・向き設定
		SetPos(pos);
		SetRot(rot);
	}

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 Vertex = m_p2D[CGauge::TYPE_GAUGE]->GetVtxBuff();

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	Vertex->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_aTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_aTexU + ratio, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_aTexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_aTexU + ratio, 1.0f);

	// 頂点バッファをアンロック
	Vertex->Unlock();
}
