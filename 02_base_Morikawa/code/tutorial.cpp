//========================================
//
//チュートリアル[tutorial.h]
//Author：森川駿弥
//
//========================================
#include "tutorial.h"
#include "manager.h"
#include "player.h"
#include "field.h"
#include "fade.h"
#include "texture.h"
#include "wall.h"
#include "mapobject.h"
#include "sound.h"

//=======================================
//コンストラクタ
//=======================================
CTutorial::CTutorial()
{
}

//=======================================
//デストラクタ
//=======================================
CTutorial::~CTutorial()
{
}

//=======================================
//生成
//=======================================
CTutorial* CTutorial::Create(void)
{
	//タイトルのポインタ
	CTutorial* pTutorial = nullptr;

	//インスタンス生成
	pTutorial = new CTutorial;

	//ポインタを返す
	return pTutorial;
}

//=======================================
//初期化
//=======================================
HRESULT CTutorial::Init(void)
{
	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// インスタンス生成
	m_pObj2D = CObject2D::Create();

	// 位置設定
	m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// サイズ設定
	m_pObj2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// テクスチャ設定
	m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\tutorial.png"));

	m_bEnd = false;

	//成功を返す
	return S_OK;
}

//=======================================
//終了
//=======================================
void CTutorial::Uninit(void)
{
	// サウンド情報取得
	CSound* pSound = CManager::GetInstance()->GetSound();

	// サウンド停止
	pSound->Stop(CSound::SOUND_LABEL_BGM_TITLE);
}

//=======================================
//更新
//=======================================
void CTutorial::Update(void)
{
	//CInputKeyboard情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//CInputPad情報取得
	CInputPad* pInputPad = CManager::GetInstance()->GetInputPad();

	//テクスチャのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	if (!m_bEnd &&
		(pInputKeyboard->GetTrigger(DIK_D) ||
		pInputPad->GetTrigger(CInputPad::BUTTON_A, 0)))
	{
		// テクスチャ設定
		m_pObj2D->BindTexture(pTexture->Regist("data\\texture\\tutorial2.png"));
		m_bEnd = true;
		return;
	}

	
	if (m_bEnd && 
		(pInputKeyboard->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_A, 0)))
	{
		// 画面遷移(フェード)
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_GAME);
	}
}

//=======================================
//描画
//=======================================
void CTutorial::Draw(void)
{
}
