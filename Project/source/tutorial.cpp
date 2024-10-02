//============================================
//
//	チュートリアル画面 [tutorial.cpp]
//	Author:酒井 南勝
//
//============================================

#include "tutorial.h"

#include "manager.h"
#include "renderer.h"
#include "fade.h"

#include "input.h"
#include "sound.h"

#include "debugproc.h"
#include "Edit.h"

#include "Pause.h"
#include "camera.h"

#include "player.h"

#include "objmeshField.h"

#include "slowManager.h"

#include "MapModel.h"
#include "CubeBlock.h"
#include "enemyAshigaru.h"
#include "enemyBowman.h"
#include "enemyTutorial.h"
#include "objmeshDome.h"

#include "object2D.h"
#include "object3D.h"
#include "objGauge2D.h"

namespace
{
	// マップテキストパス
	const char* MAP_BLOCK_PASS("data\\TXT\\STAGE\\Block_Tutorial_000.txt");
	const char* MAP_XMODEL_PASS("data\\TXT\\STAGE\\XModel_Tutorial_000.txt");
	const char* MAP_ENEMY_PASS("data\\TXT\\STAGE\\Enemy_Tutorial_000.txt");
	const char* MAP_STAIR_PASS("data\\TXT\\STAGE\\Stair_Tutorial_000.txt");
	const char* MAP_GIMMICK_PASS("data\\TXT\\STAGE\\Gimmick_Tutorial_000.txt");
	const char* MAP_PLAYER_PASS("data\\TXT\\STAGE\\Player_Tutorial_000.txt");

	const float SKIP_PRESS_FRAME = 60.0f;	// 遷移長押しフレーム数
}

//静的メンバ変数宣言
CEdit* CTutorial::m_pEdit = nullptr;
CPause* CTutorial::m_pPause = nullptr;
CPlayer* CTutorial::m_pPlayer = nullptr;
CAim* CTutorial::m_pAim = nullptr;

CCubeBlock* CTutorial::m_pCubeBlock = nullptr;

bool CTutorial::m_bGameEnd = false;
bool CTutorial::m_bEvent = false;
bool CTutorial::m_bEventEnd = false;
bool CTutorial::m_Wireframe = false;
bool CTutorial::m_Slow = false;
int CTutorial::m_nEnemyNum = 0;
int CTutorial::m_nTutorialWave = 0;
int CTutorial::m_nEventCount = 0;
float CTutorial::m_EventHeight = 0.0f;
float CTutorial::m_NameColorA = 0.0f;
float CTutorial::m_BGColorA = 1.0f;
D3DXVECTOR3 CTutorial::m_EventPos = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
D3DXVECTOR3 CTutorial::m_BGRot = INITVECTOR3;
CObject2D* CTutorial::m_pSkipUi = nullptr;
CObject3D* CTutorial::m_pRiver = nullptr;
CObjGauge2D* CTutorial::m_pSkipGage = nullptr;
float CTutorial::m_fCntSkip = 0.0f;
bool CTutorial::m_abIsTask[CTutorial::TASK_MAX] = {};
CTutorial::InfoEnemy CTutorial::m_infoEnemy = {};

//====================================================================
//コンストラクタ
//====================================================================
CTutorial::CTutorial()
{
	m_nEnemyNum = 0;
	m_bGameEnd = false;
	m_bEvent = false;
	m_bEventEnd = false;
	m_EventHeight = 0.0f;
	m_nEventCount = 0;
	m_BGColorA = 1.0f;
	m_nTutorialWave = 0;
	CManager::GetInstance()->GetCamera()->SetBib(false);
	ZeroMemory(&m_abIsTask, sizeof(m_abIsTask));

#ifdef _DEBUG
	CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FPS);
	//CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_CONTROL);
#else
	CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FPS);
#endif // DEBUG
}

//====================================================================
//デストラクタ
//====================================================================
CTutorial::~CTutorial()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CTutorial::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TUTORIAL);

	//ポーズの生成
	if (m_pPause == nullptr)
	{
		m_pPause = CPause::Create();
	}

	m_bGameEnd = false;
	CManager::GetInstance()->GetInstance()->SetStop(false);

	// スキップゲージUI
	SkipUICreate();

	// 床の生成
	for (int nCnt = 0; nCnt < 28; nCnt++)
	{
		if (nCnt == 18 || nCnt == 19 || nCnt == 20 || nCnt == 21)
		{
			CObjmeshField* MeshField1 = CObjmeshField::Create(2, 12);
			MeshField1->SetPos(D3DXVECTOR3(-6000.0f + (500.0f * nCnt), 0.0f, 4000.0f));
			MeshField1->SetTexture("data\\TEXTURE\\field00.jpg");

			CObjmeshField* MeshField2 = CObjmeshField::Create(2, 12);
			MeshField2->SetPos(D3DXVECTOR3(-6000.0f + (500.0f * nCnt), 0.0f, -4000.0f));
			MeshField2->SetTexture("data\\TEXTURE\\field00.jpg");
		}
		else
		{
			CObjmeshField* MeshField = CObjmeshField::Create(2, 32);
			MeshField->SetPos(D3DXVECTOR3(-6000.0f + (500.0f * nCnt), 0.0f, 0.0f));
			MeshField->SetTexture("data\\TEXTURE\\field00.jpg");
		}
	}

	// スカイドーム
	CObjmeshDome* pSkyDome = CObjmeshDome::Create();
	pSkyDome->SetTexture("data\\TEXTURE\\sky.jpg");

	m_pRiver = CObject3D::Create();
	m_pRiver->SetPos(D3DXVECTOR3(4000.0f, -100.0f, 0.0f));
	m_pRiver->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	m_pRiver->SetWidth(2500.0f);
	m_pRiver->SetHeight(2500.0f);
	m_pRiver->SetScrollSpeed(D3DXVECTOR2(0.0f, 0.001f));
	m_pRiver->SetTexture("data\\TEXTURE\\river.png");

	if (m_pPlayer == nullptr)
	{
		// プレイヤーの生成
		m_pPlayer = CPlayer::Create();
		m_pPlayer->SetPos(D3DXVECTOR3(-500.0f, 0.0f, 0.0f));
	}

#if _DEBUG
	if (m_pEdit == nullptr)
	{
		//m_pEdit = CEdit::Create();
	}

#endif
	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CTutorial::Uninit(void)
{
	m_pAim = nullptr;
	m_pPlayer = nullptr;

	m_pSkipGage = nullptr;
	m_pSkipUi = nullptr;
	m_pSkipGage = nullptr;
	m_pCubeBlock = nullptr;

	ZeroMemory(&m_infoEnemy, sizeof(m_infoEnemy));

	// スロー情報の全削除
	CSlowManager::ReleaseAll();

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	if (m_pPause != nullptr)
	{
		delete m_pPause;
		m_pPause = nullptr;
	}

#if _DEBUG
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		delete m_pEdit;
		m_pEdit = nullptr;
	}
#endif

}

//====================================================================
//更新処理
//====================================================================
void CTutorial::Update(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{
		m_Wireframe = (m_Wireframe == true) ? false : true;

		if (m_Wireframe == true)
		{
			//レンダーステートの設定
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			//レンダーステートの設定
			m_pDevice->SetRenderState(D3DRS_FILLMODE, 0);
		}
	}

	if (CManager::GetInstance()->GetEdit() == true)
	{
		CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_CONTROL);
		m_pEdit->Update();
	}

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed -= 0.91f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed += 0.91f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (CManager::GetInstance()->GetGameSpeed() <= 1.0f)
	{
		m_Slow = true;
	}
	else
	{
		m_Slow = false;
	}

	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("ゲームスピード : %f\n", CManager::GetInstance()->GetGameSpeed());

	// チュートリアルスキップ
	if (CManager::GetInstance()->GetInputKeyboard()->GetPress(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetPress(CInputJoypad::BUTTON_A, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetPress(CInputJoypad::BUTTON_B, 0) == true)
	{
		if (m_fCntSkip >= SKIP_PRESS_FRAME)
		{
			// ゲーム遷移
			CFade::SetFade(CScene::MODE_GAME);
		}
		else
		{
			m_fCntSkip += 1.0f;
		}
	}
	else
	{
		m_fCntSkip = 0.0f;
	}

#else
	// チュートリアルスキップ
	if (CManager::GetInstance()->GetInputKeyboard()->GetPress(DIK_RETURN) == true)
	{
		// ゲーム遷移
		CFade::SetFade(CScene::MODE_GAME);
	}

	// チュートリアルスキップ
	if (CManager::GetInstance()->GetInputJoyPad()->GetPress(CInputJoypad::BUTTON_A, 0) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetPress(CInputJoypad::BUTTON_B, 0) == true)
	{
		// ゲーム遷移
		CFade::SetFade(CScene::MODE_GAME);
	}
#endif
	// スキップゲージの位置管理
	if (m_pSkipGage != nullptr)
	{
		m_pSkipGage->SetGaugeWidth(SKIP_PRESS_FRAME, m_fCntSkip);
	}

	// ポーズの更新処理
	m_pPause->Update();
}

//====================================================================
//描画処理
//====================================================================
void CTutorial::Draw(void)
{

}

//====================================================================
//UIの表示状態設定
//====================================================================
void CTutorial::DispUI(bool Set)
{

}

//====================================================================
//ステージのブロック配置
//====================================================================
void CTutorial::SetStageBlock(void)
{
	m_pCubeBlock = CCubeBlock::Create();
	m_pCubeBlock->SetPos(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	m_pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
	m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//====================================================================
// ブロックの読み込み配置
//====================================================================
void CTutorial::LoadStageBlock(const char* pFilename)
{
	//ファイルを開く
	FILE* pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		char Getoff[32] = {};
		char boolLife[32] = {};
		char aString[128] = {};			//ゴミ箱
		char aStartMessage[32] = {};	//スタートメッセージ
		char aSetMessage[32] = {};		//セットメッセージ
		char aEndMessage[32] = {};		//終了メッセージ

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETBLOCK") == 0)
				{
					D3DXVECTOR3 pos;
					D3DXVECTOR3 Size;

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%f", &pos.x);
					fscanf(pFile, "%f", &pos.y);
					fscanf(pFile, "%f", &pos.z);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%f", &Size.x);
					fscanf(pFile, "%f", &Size.y);
					fscanf(pFile, "%f", &Size.z);

					CCubeBlock* pBlock = CCubeBlock::Create();
					pBlock->SetPos(pos);
					pBlock->SetSize(Size);
					pBlock->SetAppear(false);

					fscanf(pFile, "%s", &aEndMessage[0]);
					if (strcmp(&aEndMessage[0], "ENDSETBLOCK") != 0)
					{
						break;
					}
				}
				else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
				{
					break;
				}
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
// モデルの読み込み配置
//====================================================================
void CTutorial::LoadStageMapModel(const char* pFilename)
{
	//ファイルを開く
	FILE* pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		char Getoff[32] = {};
		char boolLife[32] = {};
		char aString[128] = {};			//ゴミ箱
		char aStartMessage[32] = {};	//スタートメッセージ
		char aSetMessage[32] = {};		//セットメッセージ
		char aEndMessage[32] = {};		//終了メッセージ
		char aBool[8] = {};				//bool型メッセージ
		bool Loop = false;

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETXMODEL") == 0)
				{
					char aModelName[64] = {};		//モデルのパス名
					D3DXVECTOR3 pos;
					D3DXVECTOR3 rot;
					int nEditIndex = 0;

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%s", &aModelName[0]);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &nEditIndex);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%f", &pos.x);
					fscanf(pFile, "%f", &pos.y);
					fscanf(pFile, "%f", &pos.z);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%f", &rot.x);
					fscanf(pFile, "%f", &rot.y);
					fscanf(pFile, "%f", &rot.z);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%s", &aBool[0]);	//ループするかどうかを設定
					Loop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool型の書き方

					CMapModel* pModel = CMapModel::Create(&aModelName[0]);
					pModel->SetPos(pos);
					pModel->SetRot(rot);
					pModel->SetEditIdx(nEditIndex);

					if (rot.y < 1.57f)
					{

					}
					else if (rot.y < 3.14f)
					{
						pModel->SwapSize();
					}
					else if (rot.y < 4.71f)
					{

					}
					else
					{
						pModel->SwapSize();
					}

					pModel->SetCollision(Loop);

					fscanf(pFile, "%s", &aEndMessage[0]);
					if (strcmp(&aEndMessage[0], "ENDSETXMODEL") != 0)
					{
						break;
					}
				}
				else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
				{
					break;
				}
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
//スキップUI生成
//====================================================================
void CTutorial::SkipUICreate(void)
{
	// 説明UI
	if (m_pSkipUi == nullptr)
	{
		m_pSkipUi = CObject2D::Create();
		m_pSkipUi->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.875f, 50.0f, 0.0f));
		m_pSkipUi->SetWidth(200.0f);
		m_pSkipUi->SetHeight(100.0f);
		m_pSkipUi->SetTexture("data\\TEXTURE\\Skip_Tutorial.png");
	}

	// ゲージ
	if (m_pSkipGage == nullptr)
	{
		m_pSkipGage = CObjGauge2D::Create();
		m_pSkipGage->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, 100.0f, 0.0f));
		m_pSkipGage->SetWidth(200.0f);
		m_pSkipGage->SetHeight(20.0f);
		m_pSkipGage->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_pSkipGage->SetGaugeWidth(SKIP_PRESS_FRAME, m_fCntSkip);
	}
}