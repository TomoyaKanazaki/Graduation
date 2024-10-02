//============================================
//
//	ロゴ画面 [logo.cpp]
//	Author : Takeru Ogasawara
//
//============================================

#include "logo.h"
#include "object2D.h"

// 定数定義
namespace
{
	const int WAIT = 120;	// ロゴの待機時間
	const float DECLEASE = 0.01f;	// ロゴの減少速度
}

// 静的メンバ変数宣言
CObject2D* CLogo::m_pLogo = nullptr;
CObject2D* CLogo::m_pBg = nullptr;
int CLogo::m_nCounter = 0;
bool CLogo::m_bStart = false;

//====================================================================
// コンストラクタ
//====================================================================
CLogo::CLogo()
{
	m_pLogo = nullptr;
}

//====================================================================
// デストラクタ
//====================================================================
CLogo::~CLogo()
{

}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CLogo::Init(void)
{
	//// 背景の生成
	//m_pBg = CObject2D::Create();

	//if (m_pBg)
	//{
	//	m_pBg->SetPos({ 640.0f, 360.0f, 0.0f });
	//	m_pBg->SetWidth(1280.0f);
	//	m_pBg->SetHeight(720.0f);

	//}

	// ロゴの生成
	m_pLogo = CObject2D::Create();

	if (m_pLogo)
	{
		m_pLogo->SetTexture("data\\TEXTURE\\logo\\logo.jpg");
		m_pLogo->SetPos({ 640.0f, 360.0f, 0.0f });
		m_pLogo->SetWidth(500.0f);
		m_pLogo->SetHeight(500.0f);
	}

	m_nCounter = WAIT;

	return S_OK;
}

//====================================================================
// 終了処理
//====================================================================
void CLogo::Uninit(void)
{

}

//====================================================================
// 更新処理
//====================================================================
void CLogo::Update(void)
{
	if (m_nCounter < 0)
	{
		m_bStart = true;
	}

	if (!m_bStart)
	{
		m_nCounter -= (int)CManager::GetInstance()->GetGameSpeed();

		return;
	}

	if (UIFade())
	{
		CScene::SetMode(CScene::MODE_TITLE);
	}
}

//====================================================================
// 描画処理
//====================================================================
void CLogo::Draw(void)
{

}

//====================================================================
// UIフェード
//====================================================================
bool CLogo::UIFade(void)
{
	float fColorA = 0.0f;

	// ロゴの色(α値)の取得
	fColorA = m_pLogo->GetColor().a;

	// 色(α値)の減少
	fColorA -= DECLEASE;

	// 色の設定
	m_pLogo->SetColorA(fColorA);

	if (fColorA < 0.0f)
	{
		fColorA = 0.0f;

		// 色(α値)が0になったことを返す
		return true;
	}

	return false;
}
