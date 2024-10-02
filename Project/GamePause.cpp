//============================================
//
//	ゲーム用ポーズ処理 [GamePause.cpp]
//	Author:sakamoto kai
//
//============================================
#include "GamePause.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"

//マクロ定義
#define PAUSE_POS (D3DXVECTOR3(640.0f, 100.0f, 0.0f))
#define PAUSE_WIGHT (600.0f)
#define PAUSE_HEIGHT (200.0f)
#define UI_POS (D3DXVECTOR3(640.0f, 300.0f, 0.0f))
#define UI_WIGHT (600.0f)
#define UI_HEIGHT (150.0f)
#define UI_HSIDE (175.0f)

//静的メンバ変数宣言

CObject2D* CGamePause::m_pPauseFG[MAX_GAMEFG] = {};

bool CGamePause::m_bColor = false;

//====================================================================
//コンストラクタ
//====================================================================
CGamePause::CGamePause()
{
	m_PauseSelect = 0;
	m_Appear = false;
}

//====================================================================
//デストラクタ
//====================================================================
CGamePause::~CGamePause()
{

}

//====================================================================
//生成処理
//====================================================================
CGamePause* CGamePause::Create()
{
	CGamePause* pPause = NULL;

	if (pPause == NULL)
	{
		//敵の生成
		pPause = new CGamePause();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPause->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pPause;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CGamePause::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_GAMEFG; nCnt++)
	{
		m_pPauseFG[nCnt] = CObject2D::Create(4);
		m_pPauseFG[nCnt]->SetType(CObject::TYPE_TUTORIALUI);

		switch (nCnt)
		{
		case 0:
			m_pPauseFG[nCnt]->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
			m_pPauseFG[nCnt]->SetWidth(1280.0f);
			m_pPauseFG[nCnt]->SetHeight(720.0f);
			m_pPauseFG[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pPauseFG[nCnt]->SetTexture("data\\TEXTURE\\HGS\\pause_1.png");
			break;

		case 1:
			m_pPauseFG[nCnt]->SetPos(PAUSE_POS);
			m_pPauseFG[nCnt]->SetWidth(PAUSE_WIGHT);
			m_pPauseFG[nCnt]->SetHeight(PAUSE_HEIGHT);
			m_pPauseFG[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_pPauseFG[nCnt]->SetTexture("data\\TEXTURE\\pause\\paper00.png");
			break;

		default:
			break;
		}
	}

	pNoise = CObjectAnim2D::Create(SCREEN_CENTER, 5, 21,5*21, true, 60, 5);
	pNoise->SetHeight(SCREEN_HEIGHT);
	pNoise->SetWidth(SCREEN_WIDTH);
	pNoise->SetTexture("data\\TEXTURE\\HGS\\noise.jpg");
	pNoise->SetAddDraw(true);
	m_MoveRot = 0.04f;
	m_MarkRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CGamePause::Uninit(void)
{

}

//====================================================================
//更新処理
//====================================================================
void CGamePause::Update(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();


	if (m_Appear == true)
	{
		pNoise->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		for (int nCnt = 0; nCnt < MAX_GAMEFG; nCnt++)
		{
			m_pPauseFG[nCnt]->SetAppear(true);
		}
	
		if (!m_bColor)
		{
			m_pPauseFG[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			//m_pPauseFG[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		m_bColor = true;
	}
	else
	{
		pNoise->SetCurrent(0);
		pNoise->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		for (int nCnt = 0; nCnt < MAX_GAMEFG; nCnt++)
		{
			m_pPauseFG[nCnt]->SetAppear(false);
		}
		
		if (m_bColor)
		{
			m_pPauseFG[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			//m_pPauseFG[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		m_bColor = false;
	}
}

//====================================================================
//描画処理
//====================================================================
void CGamePause::Draw(void)
{

}