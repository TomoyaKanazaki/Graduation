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

#include "sound.h"
#include "shadow.h"

namespace
{
	const int SAMPLE_NAMESPACE = 0;

	const CMapSystem::GRID FIELD_GRID = { 64, 64 }; // 下の床のサイズ
	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\outside.jpg";		// 下床のテクスチャ
	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1500.0f, 0.0f);	// 下床の位置
	const int BIBLE_OUTGRIT = 3;	// 聖書がマップの外側から何マス内側にいるか

	const char* SCROLL_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice.txt";
	const char* SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";

	const char* SLOPE_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\01_SlopeDevice\\motion_slopedevice.txt";
	const char* SLOPE_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";

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
	CManager::GetInstance()->GetCamera()->SetBib(false);
	CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);

	m_pPause = nullptr;
	m_pTime = nullptr;
	m_pMeshDomeUp = nullptr;
	m_pMapField = nullptr;
	m_pCubeBlock = nullptr;
	m_pDevil = nullptr;
	m_pMask = nullptr;
	m_pEnemyMask = nullptr;

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

	if (m_pMask == nullptr)
	{// 2Dマスクの生成
		m_pMask = CMask::Create(2, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	if (m_pEnemyMask == nullptr)
	{
		m_pEnemyMask = CMask::Create(102, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	}

	//クリアフラグのデフォルトをオンにしておく
	m_bGameClear = true;

	//タイムの起動
	CGame::GetTime()->SetStopTime(false);

	// タイムの生成
	m_pTime = CTimer::Create();
	m_pTime->SetStartTime(timeGetTime());
	m_pTime->SetTime(0);

	//デビルの生成
	m_pDevil = CDevil::Create();
	m_pDevil->SetPos(D3DXVECTOR3(0.0f, 100.0f, 500.0f));

	// マップの生成
	CMapSystem::GetInstance()->Init();
	CMapSystem::Load("data\\TXT\\STAGE\\map06.csv");

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		LetterBox[nCnt] = CObject2D::Create();
		LetterBox[nCnt]->SetPos(D3DXVECTOR3(640.0f, nCnt * 720.0f, 0.0f));
		LetterBox[nCnt]->SetSize(D3DXVECTOR3(1280.0f, 0.0f, 0.0f));
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

	//ステージの読み込み
	switch (CManager::GetInstance()->GetStage())
	{
	case 0:

		//m_bEvent = true;

		if (m_pEventMovie == nullptr)
		{
			m_pEventMovie = CEventMovie::Create();
		}

		// ソフトクリームの生成
		CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());

		break;

	case 1:

		// 聖書生成
		CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(BIBLE_OUTGRIT - 1, BIBLE_OUTGRIT - 1));
		CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(NUM_WIGHT - BIBLE_OUTGRIT, BIBLE_OUTGRIT - 1));
		CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(BIBLE_OUTGRIT - 1, NUM_HEIGHT - BIBLE_OUTGRIT));
		CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(NUM_WIGHT - BIBLE_OUTGRIT, NUM_HEIGHT - BIBLE_OUTGRIT));

		break;
	}


	// プレイヤーを生成する
	for (int i = 0; i < CManager::GetInstance()->GetGameMode(); ++i)
	{
		m_pPlayer.push_back(CGamePlayer::Create(i));
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

	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{
		m_pTime->SetStartTime(0);
		m_pTime->SetTime(0);
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
//ステージクリア処理
//====================================================================
void CGame::StageClear(int Stage)
{
	if (Stage == 1)
	{
		CManager::GetInstance()->SetStage(0);

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
	else
	{
		CManager::GetInstance()->SetStage(Stage + 1);

		CFade::SetFade(CScene::MODE_GAME);
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

			if (Height.y < 200.0f)
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
void CGame::LoadStageRailBlock(const char* pFilename)
{
	//ファイルを開く
	FILE* pFile = fopen(pFilename, "r");

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
void CGame::LoadStageMapModel(const char* pFilename)
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
#if 1 // 酒井のデバッグ用（モデル読み込みクラスできるまで停止中）

	// マップ移動装置
	{
		CScrollDevice* pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(1400.0f, -300.0f, -400.0f));

		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(-1400.0f, -300.0f, -400.0f));
	}

	// ジャッキ
	{
		CSlopeDevice* pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(1500.0f, BOTTOM_FIELD_POS.y, 1500.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-1500.0f, BOTTOM_FIELD_POS.y, 1500.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_RIGHT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(1500.0f, BOTTOM_FIELD_POS.y, -300.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-1500.0f, BOTTOM_FIELD_POS.y, -300.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_RIGHT);
	}
#endif

}
