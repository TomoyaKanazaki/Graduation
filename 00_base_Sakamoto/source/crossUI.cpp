//============================================
//
//	オブジェクト2Dのサンプル [SampleObj2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "crossUI.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//==========================================
//  定数定義
//==========================================
namespace
{

	const float SAMPLE_WIGHT = 125.0f;		//横幅
	const float SAMPLE_HEIGHT = 125.0f;		//縦幅
	const char TEXTURE_PASS[] = "data\\TEXTURE\\UI\\cross_UI.png";

}

//====================================================================
//コンストラクタ
//====================================================================
CCrossUi::CCrossUi(int nPriority) : CObject2D(nPriority)
{
	m_fStateCountParcent = 0.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CCrossUi::~CCrossUi()
{

}

//====================================================================
//生成処理
//====================================================================
CCrossUi* CCrossUi::Create(int nPriority)
{
	CCrossUi* pSample2D = nullptr;

	if (pSample2D == nullptr)
	{
		//オブジェクト2Dの生成
		pSample2D = new CCrossUi(nPriority);
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
HRESULT CCrossUi::Init(void)
{
	CObject2D::Init();

	//テクスチャ設定
	SetTexture(TEXTURE_PASS);

	SetSize(D3DXVECTOR3(SAMPLE_WIGHT, SAMPLE_HEIGHT, 0.0f));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCrossUi::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CCrossUi::Update(void)
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
void CCrossUi::Draw(void)
{
	CObject2D::Draw();
}

//====================================================================
//UIのデフォルトのサイズ取得
//====================================================================
D3DXVECTOR2 CCrossUi::GetSizeDefault(void)
{
	return D3DXVECTOR2(SAMPLE_WIGHT,SAMPLE_HEIGHT);
}