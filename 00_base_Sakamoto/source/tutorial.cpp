//============================================
//
//	チュートリアル画面 [tutorial.cpp]
//	Author:morikawa shunya
//
//============================================
#include "tutorial.h"
#include "fade.h"
#include "objmeshField.h"
#include "TutorialPlayer.h"
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
#include "objmeshField.h"
#include "Cross.h"
#include "MapMove.h"
#include "pause.h"
#include "tutorialCheck.h"
#include "bible.h"

#include "sound.h"
#include "shadow.h"

//====================================================================
// 定数定義
//====================================================================
namespace
{
	const D3DXVECTOR3 CHECK_POS[]
	{
		{ 0.0f, 0.0f, 0.0f },	 // NONEの座標
		{ 50.0f, 110.0f, 0.0f }, // 移動の座標
		{ 50.0f, 160.0f, 0.0f }, // 十字架座標
		{ 50.0f, 200.0f, 0.0f }, // 攻撃動の座標
		{ 50.0f, 250.0f, 0.0f }, // ボワボワの座標
		{ 50.0f, 295.0f, 0.0f }, // 聖書の座標
		{ 50.0f, 360.0f, 0.0f }, // デビルホールの座標
	};

	const int BIBLE_OUTGRIT = 2;			// 聖書がマップの外側から何マス内側にいるか
	const int STENCIL_REF_PLAYER = 2;		// プレイヤーのステンシルの参照値
	const int STENCIL_REF_ITEM = 4;			// アイテムのステンシルの参照値
	const int STENCIL_REF_MEDAMAN = 102;	// メダマンのステンシルの参照値
	const int WAVE_MIDDLE = 4;				// チュートリアル4段階目
	const int WAVE_MAX = 6;					// チュートリアル最大段階

	const char* BOTTOM_FIELD_TEX = "data\\TEXTURE\\Field\\outside.jpg";		// 下床のテクスチャ
	const char* SCROLL_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\00_ScrollDevice\\motion_scrolldevice.txt";
	const char* SCROLL_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";
	const char* SLOPE_DEVICE_MODEL = "data\\TXT\\MOTION\\02_staging\\01_SlopeDevice\\motion_slopedevice.txt";
	const char* SLOPE_DEVICE_ENEMY_MODEL = "data\\TXT\\MOTION\\01_enemy\\motion_medaman.txt";
	const char* TUTORIAL_GUIDE = "data\\TEXTURE\\UI\\tutorial_guid.png";	// チュートリアルガイドのテクスチャ
	const char* CHECK_MARKER_TEX = "data\\TEXTURE\\UI\\tutorial_check.png";	// チェックマーカーテクスチャ

	const CMapSystem::GRID FIELD_GRID = { 64, 64 }; // 下の床のサイズ
	const CMapSystem::GRID BIBLE_POS = { 11, 10 };	// 聖書の位置

	const D3DXVECTOR3 BOTTOM_FIELD_POS = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);	// 下床の位置
	const D3DXVECTOR3 GUIDE_POS = D3DXVECTOR3(200.0f, 225.0f, 0.0f);	// チュートリアルガイドの位置
	const D3DXVECTOR3 GUIDE_SIZE = D3DXVECTOR3(420.0f, 360.0f, 0.0f);	// チュートリアルガイドのサイズ
	const D3DXVECTOR3 MARKER_POS = D3DXVECTOR3(50.0f, 160.0f, 0.0f);	// マーカー位置
	const D3DXVECTOR3 MARKER_SIZE = D3DXVECTOR3(50.0f, 50.0f, 0.0f);	// マーカーサイズ

	const D3DXCOLOR MASK_DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 通常のステンシルカラー(白)
	const D3DXCOLOR MASK_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);			// タマゴンのステンシルカラー(緑)
	const D3DXCOLOR MASK_MULTI_PLAYER_COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// 2Pタマゴンのステンシルカラー(水色)
	const D3DXCOLOR MASK_MEDAMAN_COLOR = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);			// メダマンのステンシルカラー(ピンク)
	const D3DXCOLOR MASK_BONBON_COLOR = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);			// ボンンボンのステンシルカラー(オレンジ)
	const D3DXCOLOR MASK_YUNGDEVIL_COLOR = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);		// 子デビルのステンシルカラー(青)
	const D3DXCOLOR MASK_ITEM_COLOR = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);			// アイテムのステンシルカラー(青)
}

//==========================================
//  静的警告処理
//==========================================
static_assert(NUM_ARRAY(CHECK_POS) == CTutorial::TYPE_MAX, "ERROR : Type Count Missmatch");

//====================================================================
// 静的メンバ変数宣言
//====================================================================
CTutorial* CTutorial::m_pTutorial = nullptr;

//====================================================================
// コンストラクタ
//====================================================================
CTutorial::CTutorial():
m_bTutorialEnd(false),		// ゲーム終了のフラグ
m_bDevilHoleFinish(false),	// デビルホールが4方向埋まったかどうか
m_BGColorA(1.0f),			// イベント背景の透明度
m_nTutorialWave(0),			// チュートリアルの段階
m_nNumBowabowa(0),			// ボワボワの総数
m_nNumCross(0),				// 十字架の総数
m_nNumEnemy(0),				// 敵の総数
m_pPause(nullptr),			// ポーズのポインタ
m_pDevil(nullptr),			// デビルのポインタ
m_bTutorialClear(false),	// ゲームクリアのフラグ
m_Wireframe(false),			// ワイヤーフレーム切り替え
m_Slow(false),				// スロー演出フラグ
m_pTutorialGuide(nullptr),	// チュートリアルガイドのポインタ
InitPlayerPos(D3DXVECTOR3()),	// プレイヤーの初期位置
m_nNumBible(0)				// 聖書の総数
{
	for (int nCnt = 0; nCnt < NUM_CAMERA; ++nCnt)
	{// カメラ分回す
		CManager::GetInstance()->GetCamera(nCnt)->SetBib(false);
		CManager::GetInstance()->GetCamera(nCnt)->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
	}
}

//====================================================================
// デストラクタ
//====================================================================
CTutorial::~CTutorial()
{
}

//====================================================================
// インスタンス取得
//====================================================================
CTutorial* CTutorial::GetInstance(void)
{
	if (m_pTutorial == nullptr)
	{// インスタンス生成
		m_pTutorial = new CTutorial;
	}

	return m_pTutorial;
}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CTutorial::Init(void)
{
	//BGMの再生
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_STAGE1);

	if (m_pPause == nullptr)
	{// ポーズの生成
		m_pPause = CPause::Create();
	}

	// プレイヤー・アイテム・メダマンのステンシルカラーの設定
	CMask::Create(2, MASK_PLAYER_COLOR);
	CMask::Create(4, MASK_ITEM_COLOR);
	CMask::Create(102, MASK_MEDAMAN_COLOR);

	if (m_pTutorialGuide == nullptr)
	{// チュートリアルガイドの生成
		m_pTutorialGuide = CObject2D::Create();
	}
	if (m_pTutorialGuide != nullptr)
	{// テクスチャ・位置・サイズ設定
		m_pTutorialGuide->SetTexture(TUTORIAL_GUIDE);
		m_pTutorialGuide->SetPos(GUIDE_POS);
		m_pTutorialGuide->SetSize(GUIDE_SIZE);
	}

	//クリアフラグのデフォルトをオンにしておく
	m_bTutorialClear = true;

	//タイムの起動
	CTutorial::GetTime()->SetStopTime(false);

	// マップの生成
	CMapSystem::GetInstance()->Init();
	CMapSystem::Load("data\\TXT\\STAGE\\map01.csv");

	//デビルの生成
	m_pDevil = CDevil::Create();

	// 十字架の総数保存
	m_nNumCross = CCross::GetList()->GetList().size();

	// ボワボワの総数保存
	m_nNumBowabowa = CBowabowa::GetList()->GetList().size();

	// 敵の総数保存
	m_nNumEnemy = CEnemy::GetList()->GetList().size();

	for (int i = 0; i < TYPE_MAX; ++i)
	{
		m_bCheck[i] = false;
	}
	
	// 下床の生成
	auto grid = FIELD_GRID;
	CObjmeshField* pBottonField = CObjmeshField::Create(grid);
	pBottonField->SetTexture(BOTTOM_FIELD_TEX);
	pBottonField->SetPos(BOTTOM_FIELD_POS);

	// 背景モデル設定処理
	SetBgObjTest();

	// プレイヤーを生成する
	for (int i = 0; i < CManager::GetInstance()->GetGameMode(); ++i)
	{
		m_pPlayer.push_back(CTutorialPlayer::Create(i));
		m_gridPlayer.push_back(m_pPlayer.at(i)->GetGrid());
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CTutorial::Uninit(void)
{
	if (m_pPause != nullptr)
	{// ポーズの終了
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	// プレイヤーの解放
	while (1)
	{
		if (m_pPlayer.size() <= 0) { m_pPlayer.clear(); break; }
		m_pPlayer.back()->SetDeathFlag(true);
		m_pPlayer.pop_back();
	}

	// サウンド停止
	CManager::GetInstance()->GetSound()->Stop();

	// スロー情報の全削除
	CSlowManager::ReleaseAll();

	// マップシステムの終了
	CMapSystem::GetInstance()->Uninit();

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	// シーンの終了
	CScene::Uninit();

	if (m_pTutorial != nullptr)
	{
		delete m_pTutorial;
		m_pTutorial = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CTutorial::Update(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	DebugProc::Print(DebugProc::POINT_LEFT, "ゲームスピード : %f\n", CManager::GetInstance()->GetGameSpeed());
	DebugProc::Print(DebugProc::POINT_CENTER, "チュートリアル段階 : %d\n", m_nTutorialWave);

	// マップシステムの更新
	CMapSystem::GetInstance()->Update();
	
	// プレイヤーリストを取得
	if (CPlayer::GetList() == nullptr) { assert(false); }
	std::list<CPlayer*> list = CPlayer::GetList()->GetList();    // リストを取得

	int nNumPlayer = 0;

	for (CPlayer* player : list)
	{
		if (m_gridPlayer.at(nNumPlayer) != player->GetGrid()
			&& m_bCheck[TYPE_MOVE] == false)
		{// 座標が一致しなかったら
			CTutorialCheck::Create(CHECK_POS[TYPE_MOVE]);
			m_bCheck[TYPE_MOVE] = true;

			// チュートリアル段階を進める
			m_nTutorialWave += 1;
		}

		if (player->GetItemType() == CPlayer::TYPE_CROSS
			&& m_bCheck[TYPE_CROSS] == false)
		{// 十字架取得
			CTutorialCheck::Create(CHECK_POS[TYPE_CROSS]);
			m_bCheck[TYPE_CROSS] = true;

			// チュートリアル段階を進める
			m_nTutorialWave += 1;
		}

		if (player->GetItemType() == CPlayer::TYPE_BIBLE
			&& m_bCheck[TYPE_BIBLE] == false)
		{// 聖書取得
			CTutorialCheck::Create(CHECK_POS[TYPE_BIBLE]);
			
			m_bCheck[TYPE_BIBLE] = true;

			// チュートリアル段階を進める
			m_nTutorialWave += 1;
		}
	}

	if (m_nNumEnemy != CEnemy::GetList()->GetList().size()
		&& m_bCheck[TYPE_ATTACK] == false)
	{// 敵の総数減少
		CTutorialCheck::Create(CHECK_POS[TYPE_ATTACK]);
		m_bCheck[TYPE_ATTACK] = true;

		// チュートリアル段階を進める
		m_nTutorialWave += 1;
	}

	if (m_nNumBowabowa != CBowabowa::GetList()->GetList().size()
		&& m_bCheck[TYPE_BOWABOWA] == false)
	{// ボワボワの総数減少
		CTutorialCheck::Create(CHECK_POS[TYPE_BOWABOWA]);
		m_bCheck[TYPE_BOWABOWA] = true;

		// チュートリアル段階を進める
		m_nTutorialWave += 1;
	}

	if (m_nTutorialWave == WAVE_MIDDLE)
	{// チュートリアルが4段階目まで進んだら
		if (CBible::GetList() == nullptr)
		{// 1つだけ聖書の生成
			CItem::Create(CItem::TYPE_BIBLE, CMapSystem::GRID(BIBLE_POS));
		}
	}

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{// チュートリアル段階を4にする
		m_nTutorialWave = WAVE_MIDDLE;
	}

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

	if (CManager::GetInstance()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//ステージクリア条件の設定
		switch (CManager::GetInstance()->GetStage())
		{
		case 0:
			// ボワボワのリスト構造が無ければ抜ける
			if (CBowabowa::GetList() == nullptr)
			{
				m_bTutorialEnd = true;
			}
			break;

		case 1:
			if (m_bDevilHoleFinish == true)
			{
				m_bTutorialEnd = true;
			}
			break;
		}

		//ステージクリア時の処理
		if (m_bTutorialEnd == true)
		{
			if (m_bTutorialEnd == true
				&& pInputKeyboard->GetTrigger(DIK_RETURN)
				&& m_nTutorialWave == WAVE_MAX)
			{// 好きなタイミングでゲームに遷移
				CFade::SetFade(CScene::MODE_GAME);
			}
			else
			{
				CFade::SetFade(CScene::MODE_RESULT);

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

		//ポーズの更新処理
		if (m_pPause != nullptr)

		{
			m_pPause->Update();
		}

#ifdef _DEBUG

		if (pInputKeyboard->GetTrigger(DIK_F6) == true)
		{// チュートリアル最初から
			CFade::SetFade(CScene::MODE_TUTORIAL);
		}

#endif // _DEBUG
	}
}

//====================================================================
//描画処理
//====================================================================
void CTutorial::Draw(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause->Draw();
	}
}

//====================================================================
//ステージ進行処理
//====================================================================
void CTutorial::NextStage(void)
{
	// マップの生成
	CMapSystem::GetInstance()->GetMove()->Init();
	CObjmeshField::GetListTop()->SetRot(INITVECTOR3);

	//十字架の削除
	DeleteCross();

	// ソフトクリームの生成
	CItem::Create(CItem::TYPE_SOFTCREAM, CMapSystem::GetInstance()->GetCenter());

	//聖書の生成
	CreateBible();

	//ステージ情報を進める
	CManager::GetInstance()->SetStage(1);

	m_bTutorialEnd = false;
}

//====================================================================
//十字架の削除
//====================================================================
void CTutorial::DeleteCross(void)
{
	// 十字架のリスト構造が無ければ抜ける
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // リストを取得

	// 十字架リストの中身を確認する
	for (CCross* pCross : list)
	{
		pCross->Uninit();
	}
}

//====================================================================
//聖書の生成
//====================================================================
void CTutorial::CreateBible(void)
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
//マップオブジェクトの削除
//====================================================================
void CTutorial::DeleteMap(void)
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
void CTutorial::LoadStageRailBlock(const char* pFilename)
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
void CTutorial::LoadStageMapModel(const char* pFilename)
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
void CTutorial::SetBgObjTest(void)
{

	// マップ移動装置
	{
		CScrollDevice* pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(1300.0f, 0.0f, 0.0f));

		pScrollDevice = CScrollDevice::Create(SCROLL_DEVICE_MODEL, SCROLL_DEVICE_ENEMY_MODEL);
		pScrollDevice->SetPos(D3DXVECTOR3(-1300.0f, 0.0f, 0.0f));
	}

#if 0 // 酒井のデバッグ用（テスト中でめり込むため一時停止）

	// ジャッキ
	{
		CSlopeDevice* pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(900.0f, BOTTOM_FIELD_POS.y, 500.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-900.0f, BOTTOM_FIELD_POS.y, 500.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_TOP_RIGHT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetPos(D3DXVECTOR3(900.0f, BOTTOM_FIELD_POS.y, -500.0f));
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_LEFT);

		pSlopeDevice = CSlopeDevice::Create(SLOPE_DEVICE_MODEL, SLOPE_DEVICE_ENEMY_MODEL);
		pSlopeDevice->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		pSlopeDevice->SetPos(D3DXVECTOR3(-900.0f, BOTTOM_FIELD_POS.y, -500.0f));
		pSlopeDevice->SetLocateWorldType(CSlopeDevice::LOCATE_WORLD_TYPE_BOTTOM_RIGHT);
	}
#endif

}
