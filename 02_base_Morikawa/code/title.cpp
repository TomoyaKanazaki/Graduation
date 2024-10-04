//========================================
//
//タイトル[title.cpp]
//Author：森川駿弥
//
//========================================
#include "title.h"
#include "texture.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "field.h"
#include "mapobject.h"
#include "wall.h"
#include "player.h"
#include "calculation.h"

//========================================
// 定数定義
//========================================
namespace
{
	const char* TITLE_TEX = "data\\TEXTURE\\title.png";	// タイトルテクスチャのパス
	const std::string TEAM = "data\\TEXTURE\\team.png";	// タイトルテクスチャのパス
	const std::string PRESS = "data\\TEXTURE\\pressenter.png";	// タイトルテクスチャのパス
	const MyLib::Vector3 RANDPOS = MyLib::Vector3(
		UtilFunc::Transformation::Random(0, 13) * 100.0f,
		UtilFunc::Transformation::Random(0, 7) * 100.0f,
		0.0f);
}

//=======================================
//コンストラクタ
//=======================================
CTitle::CTitle(int nPriority)
{
	m_pObj2D = nullptr;
	m_fMoveTime = 0.0f;
}

//=======================================
//デストラクタ
//=======================================
CTitle::~CTitle()
{
}

//=======================================
//生成
//=======================================
CTitle* CTitle::Create(void)
{
	//タイトルのポインタ
	CTitle* pTitle = nullptr;

	//インスタンス生成
	pTitle = new CTitle;

	//初期化
	pTitle->Init();

	//ポインタを返す
	return pTitle;
}

//=======================================
//初期化
//=======================================
HRESULT CTitle::Init(void)
{
	//テクスチャのポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// インスタンス生成
	m_pObj2D = CObject2D::Create();

	// 位置設定
	m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// サイズ設定
	m_pObj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// テクスチャ設定
	m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\title.png"));

	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド再生
	pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);






	{
		m_pTeam = CObject2D::Create();

		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(TEAM);
		m_pTeam->BindTexture(texID);

		// サイズ設定
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

		// 縦幅を元にサイズ設定
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);

		m_pTeam->SetSize(size.x, size.y);
		m_pTeam->SetSizeOrigin(size);

		// 位置
		m_pTeam->SetPos(MyLib::Vector3(640.0f, 480.0f, 0.0f));
		m_pTeam->SetOriginPosition(m_pTeam->GetPos());
		m_pTeam->SetOldPosition(MyLib::Vector3(
			UtilFunc::Transformation::Random(0, 13) * 100.0f,
			UtilFunc::Transformation::Random(0, 7) * 100.0f,
			0.0f));
	}


	{
		m_pPressEnter = CObject2D::Create();

		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(PRESS);
		m_pPressEnter->BindTexture(texID);

		// サイズ設定
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

		// 縦幅を元にサイズ設定
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);

		m_pPressEnter->SetSize(size.x, size.y);
		m_pPressEnter->SetSizeOrigin(size);

		// 位置
		m_pPressEnter->SetPos(MyLib::Vector3(640.0f, 480.0f, 0.0f));
		m_pPressEnter->SetOriginPosition(m_pPressEnter->GetPos());
		m_pTeam->SetOldPosition(MyLib::Vector3(
			UtilFunc::Transformation::Random(0, 13) * 100.0f,
			UtilFunc::Transformation::Random(0, 7) * 100.0f,
			0.0f));
	}

	//成功を返す
	return S_OK;
}

//=======================================
//終了
//=======================================
void CTitle::Uninit(void)
{
}

//=======================================
//更新
//=======================================
void CTitle::Update(void)
{
	// キーボードとパッドのポインタ
	CInputKeyboard *pInputKeyboard = pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();	
	CInputPad *pInputPad = pInputPad = CManager::GetInstance()->GetInputPad();
	
	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();


	m_fMoveTime += CManager::GetInstance()->GetDeltaTime();

	if (m_pPressEnter != nullptr)
	{
		MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseOut(m_pPressEnter->GetOriginPosition(), m_pPressEnter->GetOldPosition(), 0.0f, 2.0f, m_fMoveTime);
		m_pPressEnter->SetPos(pos);
	}

	if (m_pTeam != nullptr)
	{
		MyLib::Vector3 pos = UtilFunc::Correction::EasingEaseOut(m_pTeam->GetOriginPosition(), m_pTeam->GetOldPosition(), 0.0f, 2.0f, m_fMoveTime);
		m_pTeam->SetPos(pos);
	}

	if (m_fMoveTime >= 2.0f)
	{
		m_fMoveTime = 0.0f;
		m_pPressEnter->SetOriginPosition(m_pPressEnter->GetPos());
		m_pPressEnter->SetOldPosition(MyLib::Vector3(
			UtilFunc::Transformation::Random(0, 13) * 100.0f,
			UtilFunc::Transformation::Random(0, 7) * 100.0f,
			0.0f));

		m_pTeam->SetOriginPosition(m_pTeam->GetPos());
		m_pTeam->SetOldPosition(MyLib::Vector3(
			UtilFunc::Transformation::Random(0, 13) * 100.0f,
			UtilFunc::Transformation::Random(0, 7) * 100.0f,
			0.0f));
	}


	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || 
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true ||
		pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true)
	{
		// 画面遷移(フェード)
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TUTORIAL);
	}
}

//=======================================
//描画
//=======================================
void CTitle::Draw(void)
{
}
