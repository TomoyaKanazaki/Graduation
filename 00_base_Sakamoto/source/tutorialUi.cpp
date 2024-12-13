//============================================
//
//	チュートリアルUI [tutorialUi.h]
//	Author:morikawa shunya
//
//============================================

#include "tutorialUi.h"

//============================================
// 定数定義
//============================================
namespace
{
	const char* CHECK_MARKER_TEX = "data\\TEXTURE\\UI\\tutorial_check.png";	// チェックマーカーテクスチャ
	const D3DXVECTOR3 MARKER_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);	// マーカーサイズ
}

//============================================
// コンストラクタ
//============================================
CTutorialUi::CTutorialUi()
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
CTutorialUi* CTutorialUi::Create(const D3DXVECTOR3& pos)
{
	CTutorialUi* pTutorialUi = new CTutorialUi();

	//オブジェクトの初期化処理
	if (FAILED(pTutorialUi->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	// 位置・サイズ設定
	pTutorialUi->SetPos(pos);

	return pTutorialUi;
}

//============================================
// 初期化
//============================================
HRESULT CTutorialUi::Init()
{
	// サイズ設定
	SetSize(MARKER_SIZE);

	// テクスチャ設定
	SetTexture(CHECK_MARKER_TEX);

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
