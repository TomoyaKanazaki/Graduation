//============================================
//
//	ゲーム画面 [game.cpp]
//	Author:sakamoto kai
//
//============================================
#include "game.h"
#include "fade.h"
#include "objmeshField.h"
#include "GamePlayer.h"
#include "player.h"
#include "camera.h"
#include "timer.h"
#include "Score.h"
#include "devil.h"
#include "DevilHole.h"
#include "renderer.h"
#include "slowManager.h"
#include "enemyYoungDevil.h"
#include "RailBlock.h"
#include "RollRock.h"
#include "bowabowa.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "mask.h"
#include "signal.h"
#include "pause.h"
#include "EventMovie.h"
#include "Cross.h"
#include "MapMove.h"
#include "Motion.h"

#include "sound.h"
#include "shadow.h"

namespace
{
	const int SAMPLE_NAMESPACE = 0;

	const float LETTERBOX_HEIGHT = 100.0f;		//演出時の上下の黒ポリゴンの太さ

	const CMapSystem::GRID FIELD_GRID = { 64, 64 }; // 下の床のサイズ
	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\tile_test_02.png";		// 下床のテクスチャ
	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);	// 下床の位置
	const int BIBLE_OUTGRIT = 2;	// 聖書がマップの外側から何マス内側にいるか

	const std::string SCROLL_DEVICE_MODEL_WIDTH = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice_w.txt";
	const std::string SCROLL_DEVICE_MODEL_HEIGHT = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice_h.txt";
	const std::string SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";

	const std::string SLOPE_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\01_SlopeDevice\\motion_slopedevice.txt";
	const std::string SLOPE_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";

	const D3DXCOLOR MASK_DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 通常のステンシルカラー(白)
	const D3DXCOLOR MASK_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);			// タマゴンのステンシルカラー(緑)
	const D3DXCOLOR MASK_MULTI_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// 2Pタマゴンのステンシルカラー(水色)
	const D3DXCOLOR MASK_MEDAMAN_COLOR = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);			// メダマンのステンシルカラー(ピンク)
	const D3DXCOLOR MASK_BONBON_COLOR = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);			// ボンンボンのステンシルカラー(オレンジ)
	const D3DXCOLOR MASK_YUNGDEVIL_COLOR = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);		// 子デビルのステンシルカラー(青)
	const D3DXCOLOR MASK_ITEM_COLOR = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);			// アイテムのステンシルカラー(青)

	const int PLAYER_REF = 2;		// プレイヤーのステンシル参照値
	const int ITEM_REF = 4;			// アイテムのステンシル参照値
	const int MEDAMAN_REF = 102;	// メダマンのステンシル参照値
}

//静的メンバ変数宣言
CGame* CGame::m_pGame = nullptr;

//====================================================================
//コンストラクタ
//====================================================================
CGame::CGame()
{
	m_bGameEnd = false;
	m_pEventMovie = nullptr;
	m_bEvent = false;
	m_bEventEnd = false;
	m_bDevilHoleFinish = false;
	m_BGColorA = 1.0f;
	m_nTutorialWave = 0;
	m_nNumBowabowa = 0;

	for (int nCnt = 0; nCnt < NUM_CAMERA; nCnt++)
	{
		CManager::GetInstance()->GetCamera(nCnt)->SetBib(false);
		CManager::GetInstance()->GetCamera(nCnt)->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pBGCharacter[nCnt] = nullptr;
	}
	m_nBGCount = 0;

	m_pPause = nullptr;
	m_pTime = nullptr;
	m_pMeshDomeUp = nullptr;
	m_pMapField = nullptr;
	m_pCubeBlock = nullptr;
	m_pDevil = nullptr;
	m_pPlayerMask = nullptr;
	m_pEnemyMask = nullptr;
	m_pItemMask = nullptr;

	m_bGameClear = false;
	m_Wireframe = false;
	m_Slow = false;
	m_bDevilHoleFinish = false;

	m_nTutorialWave = 0;
	m_nNumBowabowa = 0;

	m_BGColorA = 1.0f;

	m_BGRot = INITVECTOR3;

	LetterBox[0] = nullptr;
	LetterBox[1] = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CGame::~CGame()
{
}

//====================================================================
//インスタンス取得
//====================================================================
CGame* CGame::GetInstance(void)
{
	if (m_pGame == nullptr)
	{
		m_pGame = new CGame;
	}
	return m_pGame;
}


//====================================================================
//初期化処理
//====================================================================
HRESULT CGame::Init(void)
{
	////BGMの再生
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_STAGE1);

	if (m_pPause == nullptr)
	{
		m_pPause = CPause::Create();
	}

	// プレイヤー・アイテム・メダマンのステンシルカラーの設定
	CMask::Create(PLAYER_REF, MASK_PLAYER_COLOR);
	CMask::Create(ITEM_REF, MASK_ITEM_COLOR);
	CMask::Create(MEDAMAN_REF, MASK_MEDAMAN_COLOR);

	//クリアフラグのデフォルトをオンにしておく
	m_bGameClear = true;

	//ステージ情報を0にする
	CManager::GetInstance()->SetStage(0);

	//タイムの起動
	CGame::GetTime()->SetStopTime(false);

	// タイムの生成
	m_pTime = CTimer::Create();
	m_pTime->SetStartTime(timeGetTime());
	m_pTime->SetTime(0);
	m_pTime->SetStopTime(true);	//タイムの進行を止める

	// マップの生成
	CMapSystem::GetInstance();
	CMapSystem::Load("data\\TXT\\STAGE\\map01.csv");

	//デビルの生成
	m_pDevil = CDevil::Create();

	// イベントの開始
	//m_bEvent = true;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		LetterBox[nCnt] = CObject2D::Create();
		LetterBox[nCnt]->SetPos(D3DXVECTOR3(640.0f, nCnt * 720.0f, 0.0f));
		LetterBox[nCnt]->SetSize(D3DXVECTOR3(1280.0f, LETTERBOX_HEIGHT, 0.0f));
		LetterBox[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		LetterBox[nCnt]->SetTexture("data\\TEXTURE\\Test.jpg");
	}

	// 下床の生成
	auto grid = FIELD_GRID;
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);
	m_bGameEnd = false;

	// 背景モデル設定処理
	SetBgObjTest();

	if (m_pEventMovie == nullptr)
	{
		m_pEventMovie = CEventMovie::Create();
	}

	// ソフトクリームの生成
	CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());


	// プレイヤーを生成する
	for (int i = 0; i < CManager::GetInstance()->GetGameMode(); ++i)
	{
		m_pPlayer.push_back(CGamePlayer::Create(i));
	}

	//背景オブジェクトの生成

	//右山
	CObjectX* pRMountain = CObjectX::Create("data\\MODEL\\RightMountain.x");
	pRMountain->SetPos(D3DXVECTOR3(2000.0f, -1000.0f, 2000.0f));

	//左山
	CObjectX* pLMountain = CObjectX::Create("data\\MODEL\\LeftMountain.x");
	pLMountain->SetPos(D3DXVECTOR3(-1900.0f, -1000.0f, 1500.0f));

	//マグマ
	CObject3D* pMaguma = CObject3D::Create();
	pMaguma->SetPos(D3DXVECTOR3(0.0f, -950.0f, 0.0f));
	pMaguma->SetSize(D3DXVECTOR3(3000.0f, 0.0f, 6500.0f));
	pMaguma->SetScrollSpeed(D3DXVECTOR2(0.0f, -0.0001f));
	pMaguma->SetTexture("data\\TEXTURE\\MAGUMA.png");

	//メダマン
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pBGCharacter[nCnt] == nullptr)
		{
			m_pBGCharacter[nCnt] = CObjectCharacter::Create(false);
			m_pBGCharacter[nCnt]->SetTxtCharacter(SCROLL_DEVICE_ENEMY_MODEL, 0);

			m_pBGCharacter[nCnt]->SetPos(D3DXVECTOR3(
				-1975.0f + sinf(D3DX_PI * (0.5f * nCnt)) * 400.0f,
				-200.0f,
				1500.0f + cosf(D3DX_PI * (0.5f * nCnt)) * 250.0f));

			m_pBGCharacter[nCnt]->SetRot(D3DXVECTOR3(
				0.0f,
				sinf(D3DX_PI * (0.5f * (nCnt - 1))),
				0.0f));

			m_pBGCharacter[nCnt]->GetMotion()->Set(1, 0);
		}
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CGame::Uninit(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	if (m_pEventMovie != nullptr)
	{
		m_pEventMovie->Uninit();
		m_pEventMovie = nullptr;
	}

	// プレイヤーの解放
	while(1)
	{
		if (m_pPlayer.size() <= 0) { m_pPlayer.clear(); break; }
		m_pPlayer.back()->SetDeathFlag(true);
		m_pPlayer.pop_back();
	}

	CManager::GetInstance()->GetSound()->Stop();

	// スロー情報の全削除
	CSlowManager::ReleaseAll();

	// マップシステムの終了
	CMapSystem::GetInstance()->Uninit();

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	CScene::Uninit();

	if (m_pGame != nullptr)
	{
		delete m_pGame;
		m_pGame = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CGame::Update(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	DebugProc::Print(DebugProc::POINT_LEFT, "ゲームスピード : %f\n", CManager::GetInstance()->GetGameSpeed());

	CMapSystem::GetInstance()->Update();

	m_nBGCount++;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pBGCharacter[nCnt] != nullptr)
		{
			m_pBGCharacter[nCnt]->SetPos(D3DXVECTOR3(
				-1975.0f + sinf(D3DX_PI * (0.5f * (nCnt + (float)m_nBGCount * 0.005f))) * 400.0f,
				-200.0f,
				1500.0f + cosf(D3DX_PI * (0.5f * (nCnt + (float)m_nBGCount * 0.005f))) * 250.0f));

			m_pBGCharacter[nCnt]->SetRot(D3DXVECTOR3(
				0.0f,
				sinf(D3DX_PI * (0.5f * ((nCnt - 1) + (float)m_nBGCount * 0.005f))),
				0.0f));
		}
	}

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

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed -= 0.90f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed += 0.90f;

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

#endif

	if (m_bEvent == true)
	{
		if (m_pEventMovie != nullptr)
		{
			m_pEventMovie->Update();
		}
	}

	if (CManager::GetInstance()->GetPause() == true)
	{
		m_pTime->SetStopTime(true);		//タイムの進行を止める
	}
	else
	{
		m_pTime->SetStopTime(false);	//タイムの進行を進める
	}

	if (CManager::GetInstance()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//ステージクリア条件の設定
		switch (CManager::GetInstance()->GetStage())
		{
		case 0:
			 // ボワボワのリスト構造が無ければ抜ける
			if (CBowabowa::GetList() == nullptr)
			{
				m_bGameEnd = true;
			}
			break;

		case 1:
			if (m_bDevilHoleFinish == true)
			{
				m_bGameEnd = true;
			}
			break;
		}

		//ステージクリア時の処理
		if (m_bGameEnd == true)
		{
			if (m_bGameClear == true)
			{
				StageClear(CManager::GetInstance()->GetStage());
			}
			else
			{
				CFade::SetFade(CScene::MODE_RESULT);
				m_pTime->SetStopTime(true);

				int EndScore = 0;

				for (unsigned int nCnt = 0; nCnt < m_pPlayer.size(); nCnt++)
				{
					if (m_pPlayer.at(nCnt) != nullptr)
					{
						EndScore += m_pPlayer.at(nCnt)->GetScore()->GetScore();
					}
				}

				CManager::GetInstance()->SetEndScore(EndScore);

				if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
				{
					CManager::GetInstance()->SetEnd1PScore(m_pPlayer.at(0)->GetScore()->GetScore());
					CManager::GetInstance()->SetEnd2PScore(m_pPlayer.at(1)->GetScore()->GetScore());
				}
			}
		}

		//レターボックスの更新
		UpdateLetterBox();

		//ポーズの更新処理
		if (m_pPause != nullptr)
	
		{
			m_pPause->Update();
		}

#ifdef _DEBUG

		if (pInputKeyboard->GetTrigger(DIK_F3))
		{
			StageClear(0);
		}

		if (pInputKeyboard->GetTrigger(DIK_F4))
		{
			StageClear(1);
		}

		if (pInputKeyboard->GetTrigger(DIK_F5))
		{
			CFade::SetFade(CScene::MODE_TUTORIAL);
		}

#endif // _DEBUG
	}
}

//====================================================================
//描画処理
//====================================================================
void CGame::Draw(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause->Draw();
	}
}

//====================================================================
//ステージの初期化処理
//====================================================================
void CGame::ResetStage(void)
{
	//イベントフラグを立てる
	m_bEvent = true;

	//現在のスクロール状態を保存する
	CMapSystem::GetInstance()->GetMove()->SetScroolSetState(CMapSystem::GetInstance()->GetMove()->GetState());

	//保存したスクロール状態を確認する用の変数
	//CMapMove::MOVE m_Scrool = CMapSystem::GetInstance()->GetMove()->GetScroolSetState();

	// マップの初期化
	CMapSystem::GetInstance()->GetMove()->Init();
	CMapSystem::GetInstance()->GetMove()->SetStateCount(200);
	CObjmeshField::GetListTop()->SetRot(INITVECTOR3);

	// 傾き装置のリスト構造が無ければ抜ける
	if (CSlopeDevice::GetList() == nullptr) { return; }
	std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // リストを取得

	// 傾き装置のリストの中身を確認する
	for (CSlopeDevice* pSlopeDevice : list)
	{
		// 方向の傾き装置を上昇状態に変更
		pSlopeDevice->ReSet();
	}

	if (m_pEventMovie != nullptr)
	{
		m_pEventMovie->SetEventType(CEventMovie::STATE_CHANGE);
	}

	m_bGameEnd = false;
}

//====================================================================
//ステージ進行処理
//====================================================================
void CGame::NextStage(void)
{
	//十字架の削除
	DeleteCross();

	////マップの削除
	//CMapSystem::MapDelete();

	//// マップの生成
	//CMapSystem::Load("data\\TXT\\STAGE\\map06.csv");

	// ソフトクリームの生成
	CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());

	//聖書の生成
	CreateBible();

	//ステージ情報を進める
	CManager::GetInstance()->SetStage(1);
}

//====================================================================
//十字架の削除
//====================================================================
void CGame::DeleteCross(void)
{
	// デビルホールのリスト構造が無ければ抜ける
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // リストを取得

	// デビルホールリストの中身を確認する
	for (CCross* pCross : list)
	{
		pCross->Uninit();
	}
}

//====================================================================
//聖書の生成
//====================================================================
void CGame::CreateBible(void)
{
	//グリッド最大・最小位置取得
	CMapSystem::GRID GMax = CMapSystem::GetInstance()->GetMove()->GetMaxGrid();
	CMapSystem::GRID GMin = CMapSystem::GetInstance()->GetMove()->GetMinGrid();

	// 聖書生成
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMin.x + BIBLE_OUTGRIT, GMin.z + BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMax.x - BIBLE_OUTGRIT, GMin.z + BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMin.x + BIBLE_OUTGRIT, GMax.z - BIBLE_OUTGRIT));
	CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(GMax.x - BIBLE_OUTGRIT, GMax.z - BIBLE_OUTGRIT));
}

//====================================================================
//ステージクリア処理
//====================================================================
void CGame::StageClear(int Stage)
{
	if (Stage == 0)
	{
		ResetStage();
	}
	else
	{
		CFade::SetFade(CScene::MODE_RESULT);
		m_pTime->SetStopTime(true);

		int EndScore = 0;

		for (unsigned int nCnt = 0; nCnt < m_pPlayer.size(); nCnt++)
		{
			if (m_pPlayer.at(nCnt) != nullptr)
			{
				EndScore += m_pPlayer.at(nCnt)->GetScore()->GetScore();
			}
		}

		CManager::GetInstance()->SetEndScore(EndScore);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			CManager::GetInstance()->SetEnd1PScore(m_pPlayer.at(0)->GetScore()->GetScore());
			CManager::GetInstance()->SetEnd2PScore(m_pPlayer.at(1)->GetScore()->GetScore());
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CGame::UpdateLetterBox(void)
{
	if (m_bEvent)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			D3DXVECTOR3 Height = LetterBox[nCnt]->GetSize();

			if (Height.y < LETTERBOX_HEIGHT)
			{
				Height.y += 2.0f;
			}

			LetterBox[nCnt]->SetSize(Height);
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			D3DXVECTOR3 Height = LetterBox[nCnt]->GetSize();

			if (Height.y > 0.0f)
			{
				Height.y -= 2.0f;
			}

			LetterBox[nCnt]->SetSize(Height);
		}
	}
}

//====================================================================
//マップオブジェクトの削除
//====================================================================
void CGame::DeleteMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::OBJECT_TYPE type = pObj->GetType();			//種類を取得

			if (type == CObject::TYPE_CUBEBLOCK ||
				type == CObject::TYPE_ENEMY3D ||
				type == CObject::TYPE_MAPMODEL)
			{//種類がマップ関連の時
				pObj->Uninit();
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
// レールブロックの読み込み配置
//====================================================================
void CGame::LoadStageRailBlock(const std::string pFilename)
{
	//ファイルを開く
	FILE* pFile = fopen(pFilename.c_str(), "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		char aString[128] = {};			//ゴミ箱
		char aStartMessage[32] = {};	//スタートメッセージ
		char aSetMessage[32] = {};		//セットメッセージ
		char aEndMessage[32] = {};		//終了メッセージ

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			CMapSystem* pMapSystem = CMapSystem::GetInstance();
			D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();

			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETRAILBLOCK") == 0)
				{
					int WightNumber, HeightNumber, nMax, RailMove[64];

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &WightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &HeightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &nMax);

					fscanf(pFile, "%s", &aString[0]);

					for (int nCnt = 0; nCnt < nMax; nCnt++)
					{
						fscanf(pFile, "%d", &RailMove[nCnt]);
					}

					CMapSystem::GetInstance()->SetGritBool(WightNumber, HeightNumber, true);
					/*CRailBlock* pBlock = CRailBlock::Create(WightNumber, HeightNumber, false, nMax, &RailMove[0]);
					pBlock->SetPos(D3DXVECTOR3(pBlock->GetPos().x, 50.0f, pBlock->GetPos().z));
					pBlock->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));*/

					fscanf(pFile, "%s", &aEndMessage[0]);
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
void CGame::LoadStageMapModel(const std::string pFilename)
{
	////ファイルを開く
	//FILE* pFile = fopen(pFilename, "r");

	//if (pFile != nullptr)
	//{//ファイルが開けた場合

	//	char Getoff[32] = {};
	//	char boolLife[32] = {};
	//	char aString[128] = {};			//ゴミ箱
	//	char aStartMessage[32] = {};	//スタートメッセージ
	//	char aSetMessage[32] = {};		//セットメッセージ
	//	char aEndMessage[32] = {};		//終了メッセージ
	//	char aBool[8] = {};				//bool型メッセージ
	//	bool Loop = false;

	//	fscanf(pFile, "%s", &aStartMessage[0]);
	//	if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
	//	{
	//		while (1)
	//		{
	//			fscanf(pFile, "%s", &aSetMessage[0]);
	//			if (strcmp(&aSetMessage[0], "STARTSETXMODEL") == 0)
	//			{
	//				char aModelName[64] = {};		//モデルのパス名
	//				D3DXVECTOR3 pos;
	//				D3DXVECTOR3 rot;
	//				int nEditIndex = 0;

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%s", &aModelName[0]);

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%d", &nEditIndex);

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%f", &pos.x);
	//				fscanf(pFile, "%f", &pos.y);
	//				fscanf(pFile, "%f", &pos.z);

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%f", &rot.x);
	//				fscanf(pFile, "%f", &rot.y);
	//				fscanf(pFile, "%f", &rot.z);

	//				fscanf(pFile, "%s", &aString[0]);
	//				fscanf(pFile, "%s", &aBool[0]);	//ループするかどうかを設定
	//				Loop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool型の書き方

	//				CMapModel* pModel = CMapModel::Create(&aModelName[0]);
	//				pModel->SetPos(pos);
	//				pModel->SetRot(rot);
	//				pModel->SetEditIdx(nEditIndex);

	//				if (rot.y < 1.57f)
	//				{

	//				}
	//				else if (rot.y < 3.14f)
	//				{
	//					pModel->SwapSize();
	//				}
	//				else if (rot.y < 4.71f)
	//				{

	//				}
	//				else
	//				{
	//					pModel->SwapSize();
	//				}

	//				pModel->SetCollision(Loop);

	//				fscanf(pFile, "%s", &aEndMessage[0]);
	//				if (strcmp(&aEndMessage[0], "ENDSETXMODEL") != 0)
	//				{
	//					break;
	//				}
	//			}
	//			else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//	fclose(pFile);
	//}
	//else
	//{//ファイルが開けなかった場合
	//	printf("***ファイルを開けませんでした***\n");
	//}
}

//====================================================================
// テスト用背景オブジェクト設定処理
//====================================================================
void CGame::SetBgObjTest(void)
{
	// マップ移動装置
	{
		// 左右
		CScrollDevice* pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_WIDTH, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(1075.0f, 75.0f, 0.0f));

		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_WIDTH, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(-1075.0f, 75.0f, 0.0f));

		// 上下
		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_HEIGHT, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(0.0f, 75.0f, 700.0f));
		pScrollDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL_HEIGHT, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(0.0f, 75.0f, -700.0f));
		pScrollDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	}

	// ジャッキ
	{
		CSlopeDevice* pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(800.0f, BOTTOM_FIELD_POS.y, 450.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-800.0f, BOTTOM_FIELD_POS.y, 450.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_RIGHT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(800.0f, BOTTOM_FIELD_POS.y, -450.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-800.0f, BOTTOM_FIELD_POS.y, -450.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_RIGHT);
	}
}
