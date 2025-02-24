//============================================
//
//	エフェクト処理 [effect.cpp]
//	Author:sakamoto kai
//
//============================================
#include "effect.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float SMALLER = 2.0f;
}

//==========================================
// 静的メンバ変数宣言
//==========================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = nullptr;
CListManager<CEffect>* CEffect::m_pList = nullptr; // オブジェクトリスト

//====================================================================
//コンストラクタ
//====================================================================
CEffect::CEffect(int nPriority) : CObjectBillboard(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 10.0f;
	m_nLife = 10;
	m_nLifeMax = m_nLife;
	m_fDel = SMALLER;
	m_cTexName = "";
	m_nIdxTexture  = -1;
}

//====================================================================
//デストラクタ
//====================================================================
CEffect::~CEffect()
{

}

//====================================================================
//生成処理
//====================================================================
CEffect *CEffect::Create()
{
	CEffect *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//オブジェクト2Dの生成
		pPlayer = new CEffect();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPlayer->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CEffect::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();;
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Effect000.jpg");
	m_cTexName = "data\\TEXTURE\\Effect000.jpg";

	CObjectBillboard::Init();

	//頂点カラーの設定
	SetColor(m_col);

	if (m_pList == nullptr)
	{// リストマネージャー生成
		m_pList = CListManager<CEffect>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CEffect::Uninit(void)
{
	if (m_pList != nullptr)
	{
		// リストから自身のオブジェクトを削除
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // オブジェクトが一つもない場合

			// リストマネージャーの破棄
			m_pList->Release(m_pList);
		}
	}

	CObjectBillboard::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CEffect::Update(void)
{
	//サイズの調整
	m_fRadius -= m_fDel;
	SetWidth(m_fRadius);
	SetHeight(m_fRadius);

	m_nLife--;

	//位置更新
	SetPos(GetPos() += m_move);

	SetColorA((float)m_nLife / (float)m_nLifeMax);

	if (m_nLife <= 0 || m_fRadius <= 0.0f)
	{
		Uninit();
		return;
	}

	//頂点情報の更新
	CObjectBillboard::Update();

	CTexture *pTexture = CManager::GetInstance()->GetTexture();;
	m_nIdxTexture = pTexture->Regist(m_cTexName);
}

//====================================================================
//描画処理
//====================================================================
void CEffect::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//aブレンディングを加算合成に設定
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObjectBillboard::Draw();

	//aブレンディングを通常に設定
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//====================================================================
//リスト取得
//====================================================================
CListManager<CEffect>* CEffect::GetList(void)
{
	return m_pList;
}
