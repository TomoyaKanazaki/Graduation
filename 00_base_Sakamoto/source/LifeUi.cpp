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
	SetWidth(SAMPLE_WIGHT);
	SetHeight(SAMPLE_HEIGHT);
	m_nSample = 0;
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

	m_pNumber = CNumber::Create();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CLifeUi::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CLifeUi::Update(void)
{
	//頂点情報の更新
	CObject2D::Update();

	//----頂点情報を変更したい場合は以下のコードを使用すること-----
	//VERTEX_2D* pVtx;	//頂点ポインタを所得

	////頂点バッファをロックし、両店情報へのポインタを所得
	//GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	////位置
	//pVtx[0].pos;
	//pVtx[1].pos;
	//pVtx[2].pos;
	//pVtx[3].pos;

	////色
	//pVtx[0].col;
	//pVtx[1].col;
	//pVtx[2].col;
	//pVtx[3].col;

	////テクスチャ
	//pVtx[0].tex;
	//pVtx[1].tex;
	//pVtx[2].tex;
	//pVtx[3].tex;

	////頂点バッファをアンロックする
	//GetVtxBuff()->Unlock();
}

//====================================================================
//描画処理
//====================================================================
void CLifeUi::Draw(void)
{
	CObject2D::Draw();
}