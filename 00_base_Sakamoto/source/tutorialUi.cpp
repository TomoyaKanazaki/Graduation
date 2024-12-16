//============================================
//
//	チュートリアルUI [tutorialUi.cpp]
//	Author:morikawa shunya
//
//============================================

#include "tutorialUi.h"

//============================================
// 定数定義
//============================================
namespace
{
}

//============================================
// コンストラクタ
//============================================
CTutorialUi::CTutorialUi(int nPriority) : CObject2D(nPriority)
{
}

//============================================
// デストラクタ
//============================================
CTutorialUi::~CTutorialUi()
{
}

//============================================
// 生成
//============================================
CTutorialUi* CTutorialUi::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, std::string pFilename)
{
	CTutorialUi* pTutorialUi = new CTutorialUi();

	//オブジェクトの初期化処理
	if (FAILED(pTutorialUi->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	// 位置・サイズ・テクスチャ設定
	pTutorialUi->SetPos(pos);
	pTutorialUi->SetSize(size);
	pTutorialUi->SetTexture(pFilename);

	return pTutorialUi;
}

//============================================
// 初期化
//============================================
HRESULT CTutorialUi::Init()
{
	//オブジェクトの初期化処理
	if (FAILED(CObject2D::Init()))
	{//初期化処理が失敗した場合
		return E_FAIL;
	}

	return S_OK;
}

//============================================
// 終了
//============================================
void CTutorialUi::Uninit()
{
	CObject2D::Uninit();
}

//============================================
// 更新
//============================================
void CTutorialUi::Update()
{
	CObject2D::Update();
}

//============================================
// 描画
//============================================
void CTutorialUi::Draw()
{
	CObject2D::Draw();
}
