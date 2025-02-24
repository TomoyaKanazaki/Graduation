//============================================
//
//	オブジェクト2Dのサンプル [SampleObj2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "LifeUi.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float SAMPLE_WIGHT = 70.0f;		//横幅
	const float SAMPLE_HEIGHT = 70.0f;		//縦幅
}

//====================================================================
//コンストラクタ
//====================================================================
CLifeUi::CLifeUi(int nPriority) : CObject2D(nPriority)
{
	m_pNumber = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CLifeUi::~CLifeUi()
{

}

//====================================================================
//生成処理
//====================================================================
CLifeUi* CLifeUi::Create(int nPriority)
{
	CLifeUi* pSample2D = nullptr;

	if (pSample2D == nullptr)
	{
		//オブジェクト2Dの生成
		pSample2D = new CLifeUi(nPriority);
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample2D->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSample2D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CLifeUi::Init(void)
{
	CObject2D::Init();

	//テクスチャ設定
	SetTexture("data\\TEXTURE\\player\\life.png");

	SetSize(D3DXVECTOR3(SAMPLE_WIGHT, SAMPLE_HEIGHT, 0.0f));

	m_pNumber = CNumber::Create();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CLifeUi::Uninit(void)
{
	if (m_pNumber != nullptr)
	{
		m_pNumber->Uninit();
		m_pNumber = nullptr;
	}

	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CLifeUi::Update(void)
{
	//頂点情報の更新
	CObject2D::Update();
}

//====================================================================
//描画処理
//====================================================================
void CLifeUi::Draw(void)
{
	CObject2D::Draw();
}