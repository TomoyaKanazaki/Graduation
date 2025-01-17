//============================================
//
//	吹き出し型UI [popUI.cpp]
//	Author:澗口将太郎
//
//============================================
#include "popUI.h"
#include "renderer.h"
#include "manager.h"

//==========================================
//  定数定義
//==========================================
namespace
{

	const float SAMPLE_WIGHT = 50.0f;		//横幅
	const float SAMPLE_HEIGHT = 50.0f;		//縦幅
	const char TEXTURE_PASS[] = "data\\TEXTURE\\UI\\popUI.png";

}

//====================================================================
//コンストラクタ
//====================================================================
CPopUi::CPopUi(int nPriority) : CObjectBillboard(nPriority)
{
	m_fStateCountParcent = 0.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CPopUi::~CPopUi()
{

}

//====================================================================
//生成処理
//====================================================================
CPopUi* CPopUi::Create(int nPriority)
{
	CPopUi* pSample2D = nullptr;

	if (pSample2D == nullptr)
	{
		//オブジェクト2Dの生成
		pSample2D = new CPopUi(nPriority);
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
HRESULT CPopUi::Init(void)
{
	CObjectBillboard::Init();

	//テクスチャ設定
	SetTexture(TEXTURE_PASS);

	SetWidth(SAMPLE_WIGHT);
	SetHeight(SAMPLE_HEIGHT);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CPopUi::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CPopUi::Update(void)
{

	//頂点情報の更新
	CObjectBillboard::Update();

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
void CPopUi::Draw(void)
{
	CObjectBillboard::Draw();
}