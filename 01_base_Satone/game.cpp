//==============================================================
//
//ゲーム処理[game.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "game.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "edit.h"
#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "item.h"
#include "pause.h"
#include "fade.h"
#include "score.h"
#include "sound.h"
#include "UI_death.h"
#include "UI_item.h"
#include "wall.h"
#include "tutorial_texture.h"
#include "test.h"

//静的メンバ変数宣言
CEdit *CGame::m_pEdit = NULL;				//エディットの情報
CPlayer *CGame::m_pPlayer = NULL;			//プレイヤーの情報
bool CGame::m_bReset = true;				//リセットしたかどうか
bool CGame::m_bPause = false;				//ポーズ画面か
CPause *CGame::m_pPause = NULL;				//ポーズ画面の情報
CScore *CGame::m_pScore = NULL;				//スコアの情報
bool CGame::m_bEnemySpawnFirst = false;			//スタートしたか
bool CGame::m_bEnemySpawnSecond = false;		//敵が出現したか
CDeathUI *CGame::m_pDeathUI = NULL;			//死亡UIの情報
CItemUI *CGame::m_pItemUI = NULL;			//アイテムUIの情報
CGame::GAMEMODE CGame::m_gameMode = GAMEMODE_START;	//ゲームモード

//==============================================================
//コンストラクタ
//==============================================================
CGame::CGame()
{
	m_nCntEnemy = 0;				//敵出現カウント

	m_bEnemySpawnFirst = false;			//敵出現したか
	m_bEnemySpawnSecond = false;
}

//==============================================================
//デストラクタ
//==============================================================
CGame::~CGame()
{

}

//==============================================================
//初期化処理
//==============================================================
HRESULT CGame::Init(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CSound *pSound = CManager::GetInstance()->GetSound();

	m_bReset = true;		//リセットしてる状態にする
	m_bPause = false;		//ポーズしてない

	//カメラの初期化処理
	pCamera->Init();

	//壁の生成
	//for (int nCntWallWidth = 0; nCntWallWidth < 10; nCntWallWidth++)
	//{//横
	//	for (int nCntWallHeight = 0; nCntWallHeight < 15; nCntWallHeight++)
	//	{//縦

	//		CWall::Create(D3DXVECTOR3(-7000.0f + (nCntWallWidth * 1000.0f), 2000.0f + (nCntWallHeight * -1000.0f), 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//	}
	//}

	// テストプレイヤー
	CTest::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	//プレイヤーの生成
	//m_pPlayer = m_pPlayer->Create(D3DXVECTOR3(-5400.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f));

	//ポーズ画面の生成
	m_pPause = CPause::Create();

	//BGM再生
	pSound->Play(pSound->SOUND_LABEL_BGM002);

	CObject::SetType(CObject::TYPE_SCENE);

	return S_OK;
}

//==============================================================
//終了処理
//==============================================================
void CGame::Uninit(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//BGMの停止
	pSound->Stop();

	//プレイヤーの破棄
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
	}

	//エディターの破棄
	if (m_pEdit != NULL)
	{
		m_pEdit->Uninit();
		m_pEdit = NULL;
	}

	//ポーズの破棄
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//更新処理
//==============================================================
void CGame::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//パッドの情報取得
	CFade *pFade = CManager::GetInstance()->GetFade();			//フェードの情報取得

	if (pInputKeyboard->GetTrigger(DIK_P) == true || 
		pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_START, 0) == true)
	{//Pキーを押したら

		m_bPause = m_bPause ? false : true;		//ポーズ切り替え
	}

	if (m_bPause == true)
	{//ポーズしてるとき

		if (pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_B, 0) == true)
		{
			m_bPause = false;
		}
	}
	else if(m_bPause == false)
	{//ポーズしてないとき

		//敵出現させる
		//CGame::SetEnemy();
	}
}

//==============================================================
//描画処理
//==============================================================
void CGame::Draw(void)
{

}

//==============================================================
//ポーズ画面の設定処理
//==============================================================
void CGame::SetEnablePause(bool bPouse)
{
	m_bPause = bPouse;
}

//==============================================================
//敵出現処理
//==============================================================
void CGame::SetEnemy(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();
	int nNumSavePoint = m_pPlayer->GetNumSavePoint();		//何番目のセーブポイントか取得

	if (nNumSavePoint == CPlayer::POINT_ENEMYBRIDGE && m_bEnemySpawnFirst == false)
	{//敵が出現する場所に来たら

		pSound->Play(pSound->SOUND_LABEL_SE_APPEAR);

		//敵の生成
		CEnemy::Create(D3DXVECTOR3(-5400.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		m_bEnemySpawnFirst = true;		//敵出現した状態にする
	}
	else if (nNumSavePoint == CPlayer::POINT_BIGINEND && m_bEnemySpawnSecond == false)
	{//敵が出現する場所に来たら

		pSound->Play(pSound->SOUND_LABEL_SE_APPEAR);

		//敵の生成
		CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		m_bEnemySpawnSecond = true;		//敵出現した状態にする
	}

}