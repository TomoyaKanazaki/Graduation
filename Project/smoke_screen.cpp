//============================================
//
//	2D煙幕処理 [smoke_screen.h]
//	Author:Kai Takada
//
//============================================
#include "smoke_screen.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//定数定義
namespace
{
	const float SMALLER(2.0f);
	const char* TEX_PASS("data\\TEXTURE\\screenEffect\\smoke_screen.png");
	const D3DXCOLOR DEF_COL(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
}

//====================================================================
//コンストラクタ
//====================================================================
CSmokeScreen::CSmokeScreen(int nPriority) : CObject2D(nPriority)
{
	m_nLife = 50;
	m_nLifeMax = m_nLife;
}

//====================================================================
//デストラクタ
//====================================================================
CSmokeScreen::~CSmokeScreen()
{

}

//====================================================================
//生成処理
//====================================================================
CSmokeScreen* CSmokeScreen::Create()
{
	CSmokeScreen* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//オブジェクト2Dの生成
		pPlayer = new CSmokeScreen();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPlayer->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pPlayer;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CSmokeScreen::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();;
	SetIdx(pTexture->Regist(TEX_PASS));

	SetType(CObject::TYPE_EFFECT);

	CObject2D::Init();

	//サイズ
	SetWidth(SCREEN_WIDTH);
	SetHeight(SCREEN_HEIGHT);
	SetPos(D3DXVECTOR3( SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	//頂点カラーの設定
	SetColor(DEF_COL);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CSmokeScreen::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CSmokeScreen::Update(void)
{
	//サイズの調整

	m_nLife--;

	//不透明度の設定
	float fRad = (float)m_nLife / (float)m_nLifeMax;

	SetColorA(fRad * DEF_COL.a);

	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}

	//頂点情報の更新
	CObject2D::Update();
}

//====================================================================
//描画処理
//====================================================================
void CSmokeScreen::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//aブレンディングを加算合成に設定
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//aブレンディングを通常に設定
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}