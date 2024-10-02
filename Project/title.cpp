//============================================
//
//	タイトル画面 [title.cpp]
//	Author:sakamoto kai
//
//============================================
#include "title.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object2D.h"
#include "texture.h"
#include "particle.h"
#include "sound.h"
#include "objmeshDome.h"
#include "player.h"
#include "CubeBlock.h"
#include "2DUI_edit.h"
#include "objmeshDome.h"
#include "objmeshField.h"
#include "camera.h"
//#include "cursorUI.h"
#include "useful.h"
#include "MapModel.h"
#include "light.h"
#include "sound.h"

// 定数定義
namespace
{
	
	const char* MAP_BLOCK_PASS("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	const char* MAP_XMODEL_PASS("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	const char* MAP_GIMMICK_PASS("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");
	
	const float FADE_WAIT_TIME = 120.0f;	// UIが消えるまでの時間
	const float FADE_SPEED = 0.01f;	// UIが消える速さ

	const float SLASH_SPEED = 3.5f;	// 斬れた後の落ちる速度
	const float SLASH_POS_LIMIT = 40.0f;	// UIが斬れる上限位置

	const D3DXVECTOR2 BUTTON_SIZE = { 300.0f, 60.0f };	// ボタンの大きさ

	// 斬撃に関する
	const D3DXVECTOR3 SLASH_POS = { 610.0f, 310.0f, 0.0f };	// 位置
	const float SLASH_LENGTH = 350.0f;		// 長さ
	const float SLASH_ANGLE = 2.93f;		// 角度
	const D3DXVECTOR2 SLASH_SIZE_DEFAULT = D3DXVECTOR2(SLASH_LENGTH * 0.5f, SLASH_LENGTH * 0.25f);	// 大きさのデフォルト
	const D3DXVECTOR2 SLASH_SIZE_DEST_2D = D3DXVECTOR2(0.0f, 30.0f);	// 大きさの減少量
	const int EVENT_SPWAN_SLASH_DIV = 3;	// 次のスラッシュコマまでの出現フレーム数
	const int EVENT_DEST_START = 1;		// 大きさの減少開始のフレーム
	const int EVENT_END = 60;				// 終了のフレーム

	const float DOME_ROT_SPEED = 0.001f;	// メッシュドームの回転速度
}

//静的メンバ変数宣言
CObject2D* CTitle::m_pLogo = nullptr;
CObject2D *CTitle::m_pTitleButton = nullptr;
CObjmeshDome* CTitle::m_pMeshDome = nullptr;

//====================================================================
//コンストラクタ
//====================================================================
CTitle::CTitle()
{
	ZeroMemory(&m_pInfoSlash, sizeof(m_pInfoSlash));
	m_fCountFade = 0.0f;
	m_bStart = false;
	m_bTurn = false;
	m_bSlash = false;
}

//====================================================================
//デストラクタ
//====================================================================
CTitle::~CTitle()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CTitle::Init(void)
{
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// スカイドーム
	m_pMeshDome = CObjmeshDome::Create();
	if (m_pMeshDome)
	{
		m_pMeshDome->SetTexture("data\\TEXTURE\\sky.jpg");
	}

	CObjmeshField* pMeshField = CObjmeshField::Create(21, 21);
	pMeshField->SetPos(INITVECTOR3);
	pMeshField->SetTexture("data\\TEXTURE\\field00.jpg");

	// タイトル文字
	m_pLogo = CObject2D::Create();
	m_pLogo->SetPos(D3DXVECTOR3(640.0f, 241.0f, 0.0f));
	m_pLogo->SetWidth(1000.0f);
	m_pLogo->SetHeight(400.0f);
	if (pTexture)
	{
		m_pLogo->SetIdx(pTexture->Regist("data\\TEXTURE\\title\\logo001.png"));
	}

	// ボタン
	m_pTitleButton = CObject2D::Create();
	m_pTitleButton->SetPos(D3DXVECTOR3(640.0f, 670.0f, 0.0f));
	m_pTitleButton->SetWidth(BUTTON_SIZE.x);
	m_pTitleButton->SetHeight(BUTTON_SIZE.y);
	//m_pTitleButton->SetColorA(0.2f);
	if (pTexture)
	{
		m_pTitleButton->SetIdx(pTexture->Regist("data\\TEXTURE\\title\\titleButton.png"));
	}

	// カーソルの生成
	//m_pCorsorUI = CCursorUI::Create();

	// フェードカウント設定
	m_fCountFade = FADE_WAIT_TIME;
	
	// ライトの初期化
	CManager::GetInstance()->GetLight()->Init();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CTitle::Uninit(void)
{
	//全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//====================================================================
//更新処理
//====================================================================
void CTitle::Update(void)
{	
	// 選択処理
	Select();
}

//====================================================================
//描画処理
//====================================================================
void CTitle::Draw(void)
{

}

//====================================================================
//選択処理
//====================================================================
void CTitle::Select(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_MNK);	// 入力タイプ：キーマウ
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);

		// ゲーム遷移
		CFade::SetFade(CScene::MODE_GAME);
	}
	else if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CManager::GetInstance()->SetTypeInput(CManager::GetInstance()->TYPE_JOYPAD);	// 入力タイプ：パッド
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);

		// ゲーム遷移
		CFade::SetFade(CScene::MODE_GAME);
	}
}