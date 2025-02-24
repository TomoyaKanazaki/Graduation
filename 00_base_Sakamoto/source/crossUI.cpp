//============================================
//
//	十字架アイテムのUI [crossUI.cpp]
//	Author:澗口将太郎
//
//============================================
#include "crossUI.h"
#include "renderer.h"
#include "manager.h"

//==========================================
//  定数定義
//==========================================
namespace
{

	const float SAMPLE_WIGHT = 125.0f;		//横幅
	const float SAMPLE_HEIGHT = 125.0f;		//縦幅
	const char TEXPASS_CROSS[] = "data\\TEXTURE\\UI\\cross_UI.png";
	const char TEXPASS_KEY[] = "data\\TEXTURE\\UI\\Key_UI.png";

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
	SetTexture(TEXPASS_CROSS);

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
//====================================================================
//表示内容変更処理
//====================================================================
void CCrossUi::SetUIType(UI_TYPE type)
{
	switch (type)
	{
	case CCrossUi::TYPE_CROSS:
		SetTexture(TEXPASS_CROSS);
		break;

	case CCrossUi::TYPE_KEY:
		SetTexture(TEXPASS_KEY);
		break;

	default:
		break;
	}

}