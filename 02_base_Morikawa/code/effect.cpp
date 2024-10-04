//=======================================
//
//エフェクト処理[effect.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "effect.h"      //作成したeffect.hをインクルードする
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE[CEffect::TYPE::TYPE_MAX] =
	{
		"data\\TEXTURE\\effect\\effect000.jpg",		// 通常
		"data\\TEXTURE\\effect\\effect_point01.tga",	// 点
		"data\\TEXTURE\\effect\\smoke_07.png",			// 煙
		"data\\TEXTURE\\effect\\smoke_05.tga",			// 黒煙
		"data\\TEXTURE\\effect\\effect000.png",			// 黒
		"data\\TEXTURE\\effect\\effect001.png",			// 十字
		"data\\TEXTURE\\effect\\Star01.png",			// 十字
		"data\\TEXTURE\\effect\\thunder_02.tga",		// 雷
		"",												// NULL
	};
}
int CEffect::m_TexIdx[TYPE::TYPE_MAX] = {};	// テクスチャインデックス
bool CEffect::m_bTexLoad = false;			// テクスチャの読み込み判定

//===========================================
//コンストラクタ
//===========================================
CEffect::CEffect(int nPriority) : CBillboard(nPriority)
{
	m_aEffect.nLife = 0;		// 寿命(表示時間)
	m_aEffect.nFirstLife = 0;	// 初期寿命
	m_aEffect.balpha = false;	// 加算合成
	m_fAddSizeValue = 0.0f;		// サイズ加算量
}

//===========================================
//デストラクタ
//===========================================
CEffect::~CEffect()
{
}

//===========================================
//生成
//===========================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool balpha, TYPE type)
{
	// テクスチャ読み込み
	if (!m_bTexLoad) TexLoad();

	//CEffect型のポインタ
	CEffect *pEffect = nullptr;

	if (pEffect == nullptr)
	{//nullptrの時
		//インスタンス生成
		pEffect = new CEffect;

		pEffect->SetPos(pos);
		pEffect->SetMove(move);
		pEffect->SetCol(col);
		pEffect->SetSize(fRadius, fRadius);
		pEffect->m_aEffect.nLife = nLife;
		pEffect->m_aEffect.balpha = balpha;

		pEffect->m_aEffect.fFirstAlpha = col.a;

		//初期化
		pEffect->Init();

		//テクスチャ割り当て
		pEffect->BindTexture(m_TexIdx[type]);
	}

	//ポインタを返す
	return pEffect;
}

//===========================================
//初期化処理
//===========================================
HRESULT CEffect::Init(void)
{
	//初期化
	CBillboard::Init();

	// 初期寿命
	m_aEffect.nFirstLife = m_aEffect.nLife;

	//成功を返す
	return S_OK;
}

//===========================================
//テクスチャ読み込み
//===========================================
void CEffect::TexLoad()
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_TexIdx[i] = pTexture->Regist(TEXTURE[i]);
	}

	m_bTexLoad = true;
}

//===========================================
//終了処理
//===========================================
void CEffect::Uninit(void)
{
	//終了
	CBillboard::Uninit();
}

//===========================================
//更新処理
//===========================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXCOLOR col = GetCol();

	//更新
	CBillboard::Update();

	//α値(消える速度)
	col.a = m_aEffect.fFirstAlpha * ((float)m_aEffect.nLife / (float)m_aEffect.nFirstLife);

	// 色設定
	SetCol(col);

	// 位置更新
	pos += move;

	// 位置設定
	SetPos(pos);

	// リサイズ
	Resize();

	//体力減算
	m_aEffect.nLife--;

	if (m_aEffect.nLife < 0)
	{
		//破棄する
		Uninit();
	}
}

//===========================================
// リサイズ
//===========================================
void CEffect::Resize()
{
	float radius = GetSize();
	radius += m_fAddSizeValue;
	SetSize(radius, radius);
}

//===========================================
//描画処理
//===========================================
void CEffect::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//CRenderer型のポインタ
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_aEffect.balpha == true)
	{// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//描画
	CBillboard::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
