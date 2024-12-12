//============================================
//
//	チュートリアルチェック [tutorialCheck.cpp]
//	Author:morikawa shunya
//
//============================================

#include "tutorialCheck.h"

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
CTutorialCheck::CTutorialCheck()
{
}

//============================================
// デストラクタ
//============================================
CTutorialCheck::~CTutorialCheck()
{
}

//============================================
// 生成
//============================================
CTutorialCheck* CTutorialCheck::Create(const D3DXVECTOR3& pos)
{
	CTutorialCheck* pTutorialCheck = new CTutorialCheck();

	//オブジェクトの初期化処理
	if (FAILED(pTutorialCheck->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	// 位置・サイズ設定
	pTutorialCheck->SetPos(pos);

	return pTutorialCheck;
}

//============================================
// 初期化
//============================================
HRESULT CTutorialCheck::Init()
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
void CTutorialCheck::Uninit()
{
	CObject2D::Uninit();
}

//============================================
// 更新
//============================================
void CTutorialCheck::Update()
{
	CObject2D::Update();
}

//============================================
// 描画
//============================================
void CTutorialCheck::Draw()
{
	CObject2D::Draw();
}
