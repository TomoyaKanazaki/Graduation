//============================================
//
//	リザルト画面 [result.cpp]
//	Author:sakamoto kai
//
//============================================
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "ranking.h"
#include "number.h"
#include "BgObjManager.h"

using namespace Result;

//マクロ定義
namespace
{
	const char* START_OK("STARTSETSTAGE");	//スタートメッセージがあるかどうかの確認
	const char* END_SET_OK("ENDSETSTAGE");	//エンドメッセージがあるかどうかの確認
	const float SCORE_POSX(300.0f);
	const float SCORE_MOVEX(20.1f);

	const CMapSystem::GRID FIELD_GRID = { 64, 64 }; // 下の床のサイズ

	//シングルプレイ用
	const D3DXVECTOR3 SINGLE_RANKING_TXT_POS(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 230.0f, 0.0f));						//「RANKING」の位置
	const D3DXVECTOR3 SINGLE_RANKING_TXT_SIZE(D3DXVECTOR3(340.0f, 180.0f, 0.0f));									//「RANKING」の大きさ

	const D3DXVECTOR3 SINGLE_RANKING_NUMBER_POS(D3DXVECTOR3(SCREEN_WIDTH * 0.46f, SCREEN_HEIGHT * 0.445f, 0.0f));	// ランキングに含まれる数字の位置
	const D3DXVECTOR2 SINGLE_RANKING_NUMBER_SIZE(D3DXVECTOR2(340.0f, 180.0f));										// ランキングに含まれる数字の大きさ

	const D3DXVECTOR3 SINGLE_RANKING_RANK_POS(D3DXVECTOR3(-500.0f, 80.0f, 0.0f));								//「○位」の位置
	const D3DXVECTOR3 SINGLE_RANKING_RANK_SIZE(D3DXVECTOR3(256.0f, 102.4f, 0.0f));								//「○位」の大きさ

	const D3DXVECTOR3 SINGLE_SCORE_TEX_POS(D3DXVECTOR3(500.0f, 100.0f, 0.0f));									//スコアの位置
	const D3DXVECTOR3 SINGLE_SCORE_TEX_SIZE(D3DXVECTOR3(200.0f, 80.0f, 0.0f));									//スコアの大きさ
	const D3DXVECTOR3 SINGLE_SCORE_VALUE_POS(D3DXVECTOR3(SINGLE_SCORE_TEX_POS.x + 105.0f, SINGLE_SCORE_TEX_POS.y, 0.0f));		//スコアの値の位置

	//マルチプレイ用
	const D3DXVECTOR3 MULTI_RANKING_TXT_POS(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 260.0f, 0.0f));						//「RANKING」の位置
	const D3DXVECTOR3 MULTI_RANKING_TXT_SIZE(D3DXVECTOR3(230.0f, 120.0f, 0.0f));									//「RANKING」の大きさ

	const D3DXVECTOR3 MULTI_RANKING_NUMBER_POS(D3DXVECTOR3(SCREEN_WIDTH * 0.46f, SCREEN_HEIGHT * 0.465f, 0.0f));	// ランキングに含まれる数字の位置
	const D3DXVECTOR2 MULTI_RANKING_NUMBER_SIZE(D3DXVECTOR2(120.0f, 120.0f));										// ランキングに含まれる数字の大きさ

	const D3DXVECTOR3 MULTI_RANKING_RANK_POS(D3DXVECTOR3(-485.0f, 100.0f, 0.0f));								//「○位」の位置
	const D3DXVECTOR3 MULTI_RANKING_RANK_SIZE(D3DXVECTOR3(200.0f, 90.0f, 0.0f));								//「○位」の大きさ

	const D3DXVECTOR3 MULTI_SCORE_TEX_POS(D3DXVECTOR3(500.0f, 60.0f, 0.0f));									//スコアの位置
	const D3DXVECTOR3 MULTI_SCORE_TEX_SIZE(D3DXVECTOR3(200.0f, 80.0f, 0.0f));									//スコアの大きさ
	const D3DXVECTOR3 MULTI_SCORE_VALUE_POS(D3DXVECTOR3(MULTI_SCORE_TEX_POS.x + 105.0f, MULTI_SCORE_TEX_POS.y, 0.0f));		//スコアの値の位置

	const D3DXVECTOR3 MULTI_1P_TEX_POS(D3DXVECTOR3(200.0f, 160.0f, 0.0f));									//スコアの位置
	const D3DXVECTOR3 MULTI_2P_TEX_POS(D3DXVECTOR3(800.0f, 160.0f, 0.0f));									//スコアの位置
	const D3DXVECTOR3 MULTI_PLAYER_TEX_SIZE(D3DXVECTOR3(300.0f, 160.0f, 0.0f));								//スコアの大きさ
	const D3DXVECTOR3 MULTI_1P_VALUE_POS(D3DXVECTOR3(MULTI_1P_TEX_POS.x + 105.0f, MULTI_1P_TEX_POS.y, 0.0f));		//スコアの値の位置
	const D3DXVECTOR3 MULTI_2P_VALUE_POS(D3DXVECTOR3(MULTI_2P_TEX_POS.x + 105.0f, MULTI_2P_TEX_POS.y, 0.0f));		//スコアの値の位置

	//タイトルからの遷移用
	const D3DXVECTOR3 NONE_RANKING_TXT_POS(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 160.0f, 0.0f));						//「RANKING」の位置
	const D3DXVECTOR3 NONE_RANKING_TXT_SIZE(D3DXVECTOR3(230.0f, 120.0f, 0.0f));									//「RANKING」の大きさ

	const D3DXVECTOR3 NONE_RANKING_NUMBER_POS(D3DXVECTOR3(SCREEN_WIDTH * 0.46f, SCREEN_HEIGHT * 0.355f, 0.0f));	// ランキングに含まれる数字の位置
	const D3DXVECTOR2 NONE_RANKING_NUMBER_SIZE(D3DXVECTOR2(120.0f, 120.0f));										// ランキングに含まれる数字の大きさ

	const D3DXVECTOR3 NONE_RANKING_RANK_POS(D3DXVECTOR3(-485.0f, 20.0f, 0.0f));								//「○位」の位置
	const D3DXVECTOR3 NONE_RANKING_RANK_SIZE(D3DXVECTOR3(200.0f, 90.0f, 0.0f));								//「○位」の大きさ

	const float SCORE_DISTANCE(25.0f);																	//スコアと数字の距離

	const float FADE_TIME = 12.0f; // 自動で遷移するまでの時間
}

//静的メンバ変数宣言
CResult* CResult::m_pResult = nullptr;

//====================================================================
//コンストラクタ
//====================================================================
CResult::CResult() :
	m_fTimer(0.0f)
{
	m_pBg = nullptr;
	m_pLifeRanking = nullptr;
	m_pScoreTex = nullptr;
	m_Appear = false;

	m_p1PTex = nullptr;
	m_p2PTex = nullptr;

	for (int nCnt = 0; nCnt < NUM_DIGIT; nCnt++)
	{
		m_apScore[nCnt] = nullptr;
		m_ap1P[nCnt] = nullptr;
		m_ap2P[nCnt] = nullptr;
	}
	m_AddScoreCount = 0;
	m_ScoreData = 0;
	m_1PData = 0;
	m_2PData = 0;

	m_AddScoreCount = 0;
	m_ScoreData = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CResult::~CResult()
{

}

//====================================================================
//インスタンス取得
//====================================================================
CResult* CResult::GetInstance(void)
{
	if (m_pResult == nullptr)
	{
		m_pResult = new CResult;
	}
	return m_pResult;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CResult::Init(void)
{
	////BGMの再生
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_RESULT);

	// スコア取得
	m_ScoreData = CManager::GetInstance()->GetEndScore();

	if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_SINGLE)
	{
		for (int nCntObject = 0; nCntObject < NUM_DIGIT; nCntObject++)
		{
			//数字の生成
			m_apScore[nCntObject] = CNumber::Create();
			m_apScore[nCntObject]->SetPos(D3DXVECTOR3(SINGLE_SCORE_VALUE_POS.x + (nCntObject * 45.0f), SINGLE_SCORE_VALUE_POS.y, SINGLE_SCORE_VALUE_POS.z));
			m_apScore[nCntObject]->SetSize(D3DXVECTOR3(60.0f, 72.0f, 0.0f));
			m_apScore[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}

		m_apScore[0]->SetNumber(m_ScoreData % 1000000 / 100000);
		m_apScore[1]->SetNumber(m_ScoreData % 100000 / 10000);
		m_apScore[2]->SetNumber(m_ScoreData % 10000 / 1000);
		m_apScore[3]->SetNumber(m_ScoreData % 1000 / 100);
		m_apScore[4]->SetNumber(m_ScoreData % 100 / 10);
		m_apScore[5]->SetNumber(m_ScoreData % 10 / 1);

		//"SCORE"っていうテクスチャポリゴン
		m_pScoreTex = CObject2D::Create();
		m_pScoreTex->SetPos(SINGLE_SCORE_TEX_POS);
		m_pScoreTex->SetSize(SINGLE_SCORE_TEX_SIZE);
		m_pScoreTex->SetTexture("data\\TEXTURE\\result_score.png");

		//全体ランキング(「ランキング」)
		CObject2D* m_pRank = CObject2D::Create();
		m_pRank->SetPos(SINGLE_RANKING_TXT_POS);
		m_pRank->SetSize(SINGLE_RANKING_TXT_SIZE);
		m_pRank->SetTexture("data\\TEXTURE\\ranking.png");

		//全体ランキング(スコア)
		m_pLifeRanking = CRanking::Create("data\\TXT\\ranking.txt");
		m_pLifeRanking->SetPos(SINGLE_RANKING_NUMBER_POS);
		m_pLifeRanking->SetRanking(m_ScoreData);

		//全体ランキング(順位)
		for (int nCntObject = 0; nCntObject < 5; nCntObject++)
		{
			CNumber* pNumRank = m_pLifeRanking->GetNumRankScore(0, nCntObject);
			D3DXVECTOR3 RankPos = D3DXVECTOR3(pNumRank->GetPos().x, pNumRank->GetPos().y, pNumRank->GetPos().z);
			RankPos.y += pNumRank->GetSize().y * 0.5f;

			//数字の生成
			CObject2D* pNumber = CObject2D::Create();
			pNumber->SetPos(D3DXVECTOR3(RankPos.x + SINGLE_RANKING_RANK_POS.x, RankPos.y + SINGLE_RANKING_RANK_POS.y, RankPos.z));
			pNumber->SetSize(SINGLE_RANKING_RANK_SIZE);
			pNumber->SetTexture("data\\TEXTURE\\RANKING_TEXT.png");
			pNumber->SetAnim(D3DXVECTOR2(0.0f, nCntObject * 0.2f),
				D3DXVECTOR2(1.0f, nCntObject * 0.2f + 0.2f));
		}
	}
	else if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
	{
		m_1PData = CManager::GetInstance()->GetEnd1PScore();
		m_2PData = CManager::GetInstance()->GetEnd2PScore();


		//"SCORE"っていうテクスチャポリゴン
		m_pScoreTex = CObject2D::Create();
		m_pScoreTex->SetPos(MULTI_SCORE_TEX_POS);
		m_pScoreTex->SetSize(MULTI_SCORE_TEX_SIZE);
		m_pScoreTex->SetTexture("data\\TEXTURE\\result_score.png");

		//今回のスコア
		for (int nCntObject = 0; nCntObject < NUM_DIGIT; nCntObject++)
		{
			//数字の生成
			m_apScore[nCntObject] = CNumber::Create();
			m_apScore[nCntObject]->SetPos(D3DXVECTOR3(MULTI_SCORE_VALUE_POS.x + (nCntObject * 45.0f), MULTI_SCORE_VALUE_POS.y, MULTI_SCORE_VALUE_POS.z));
			m_apScore[nCntObject]->SetSize(D3DXVECTOR3(60.0f, 72.0f, 0.0f));
			m_apScore[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}

		m_apScore[0]->SetNumber(m_ScoreData % 1000000 / 100000);
		m_apScore[1]->SetNumber(m_ScoreData % 100000 / 10000);
		m_apScore[2]->SetNumber(m_ScoreData % 10000 / 1000);
		m_apScore[3]->SetNumber(m_ScoreData % 1000 / 100);
		m_apScore[4]->SetNumber(m_ScoreData % 100 / 10);
		m_apScore[5]->SetNumber(m_ScoreData % 10 / 1);

		//"1P"っていうテクスチャポリゴン
		m_p1PTex = CObject2D::Create();
		m_p1PTex->SetPos(MULTI_1P_TEX_POS);
		m_p1PTex->SetSize(MULTI_PLAYER_TEX_SIZE);
		m_p1PTex->SetTexture("data\\TEXTURE\\UI\\1p.png");

		//今回のスコア
		for (int nCntObject = 0; nCntObject < NUM_DIGIT; nCntObject++)
		{
			//数字の生成
			m_ap1P[nCntObject] = CNumber::Create();
			m_ap1P[nCntObject]->SetPos(D3DXVECTOR3(MULTI_1P_VALUE_POS.x + (nCntObject * 45.0f), MULTI_1P_VALUE_POS.y, MULTI_1P_VALUE_POS.z));
			m_ap1P[nCntObject]->SetSize(D3DXVECTOR3(60.0f, 72.0f, 0.0f));
			m_ap1P[nCntObject]->SetColor(D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));
		}

		m_ap1P[0]->SetNumber(m_1PData % 1000000 / 100000);
		m_ap1P[1]->SetNumber(m_1PData % 100000 / 10000);
		m_ap1P[2]->SetNumber(m_1PData % 10000 / 1000);
		m_ap1P[3]->SetNumber(m_1PData % 1000 / 100);
		m_ap1P[4]->SetNumber(m_1PData % 100 / 10);
		m_ap1P[5]->SetNumber(m_1PData % 10 / 1);

		//"2P"っていうテクスチャポリゴン
		m_p2PTex = CObject2D::Create();
		m_p2PTex->SetPos(MULTI_2P_TEX_POS);
		m_p2PTex->SetSize(MULTI_PLAYER_TEX_SIZE);
		m_p2PTex->SetTexture("data\\TEXTURE\\UI\\2p.png");

		//今回のスコア
		for (int nCntObject = 0; nCntObject < NUM_DIGIT; nCntObject++)
		{
			//数字の生成
			m_ap2P[nCntObject] = CNumber::Create();
			m_ap2P[nCntObject]->SetPos(D3DXVECTOR3(MULTI_2P_VALUE_POS.x + (nCntObject * 45.0f), MULTI_2P_VALUE_POS.y, MULTI_2P_VALUE_POS.z));
			m_ap2P[nCntObject]->SetSize(D3DXVECTOR3(60.0f, 72.0f, 0.0f));
			m_ap2P[nCntObject]->SetColor(D3DXCOLOR(0.3f, 0.7f, 1.0f, 1.0f));
		}

		m_ap2P[0]->SetNumber(m_2PData % 1000000 / 100000);
		m_ap2P[1]->SetNumber(m_2PData % 100000 / 10000);
		m_ap2P[2]->SetNumber(m_2PData % 10000 / 1000);
		m_ap2P[3]->SetNumber(m_2PData % 1000 / 100);
		m_ap2P[4]->SetNumber(m_2PData % 100 / 10);
		m_ap2P[5]->SetNumber(m_2PData % 10 / 1);


		//全体ランキング(「ランキング」)
		CObject2D* m_pRank = CObject2D::Create();
		m_pRank->SetPos(MULTI_RANKING_TXT_POS);
		m_pRank->SetSize(MULTI_RANKING_TXT_SIZE);
		m_pRank->SetTexture("data\\TEXTURE\\ranking.png");

		//全体ランキング(スコア)
		m_pLifeRanking = CRanking::Create("data\\TXT\\ranking.txt");
		m_pLifeRanking->SetPos(MULTI_RANKING_NUMBER_POS);
		m_pLifeRanking->SetRanking(m_ScoreData);

		//全体ランキング(順位)
		for (int nCntObject = 0; nCntObject < 5; nCntObject++)
		{
			CNumber* pNumRank = m_pLifeRanking->GetNumRankScore(0, nCntObject);
			D3DXVECTOR3 RankPos = D3DXVECTOR3(pNumRank->GetPos().x, pNumRank->GetPos().y, pNumRank->GetPos().z);
			RankPos.y += pNumRank->GetSize().y * 0.5f;

			//数字の生成
			CObject2D* pNumber = CObject2D::Create();
			pNumber->SetPos(D3DXVECTOR3(RankPos.x + MULTI_RANKING_RANK_POS.x, RankPos.y + MULTI_RANKING_RANK_POS.y, RankPos.z));
			pNumber->SetSize(MULTI_RANKING_RANK_SIZE);
			pNumber->SetTexture("data\\TEXTURE\\RANKING_TEXT.png");
			pNumber->SetAnim(D3DXVECTOR2(0.0f, nCntObject * 0.2f),
				D3DXVECTOR2(1.0f, nCntObject * 0.2f + 0.2f));
		}
	}
	else
	{
		//全体ランキング(「ランキング」)
		CObject2D* m_pRank = CObject2D::Create();
		m_pRank->SetPos(NONE_RANKING_TXT_POS);
		m_pRank->SetSize(NONE_RANKING_TXT_SIZE);
		m_pRank->SetTexture("data\\TEXTURE\\ranking.png");

		//全体ランキング(スコア)
		m_pLifeRanking = CRanking::Create("data\\TXT\\ranking.txt");
		m_pLifeRanking->SetPos(NONE_RANKING_NUMBER_POS);
		m_pLifeRanking->SetRanking(m_ScoreData);

		//全体ランキング(順位)
		for (int nCntObject = 0; nCntObject < 5; nCntObject++)
		{
			CNumber* pNumRank = m_pLifeRanking->GetNumRankScore(0, nCntObject);
			D3DXVECTOR3 RankPos = D3DXVECTOR3(pNumRank->GetPos().x, pNumRank->GetPos().y, pNumRank->GetPos().z);
			RankPos.y += pNumRank->GetSize().y * 0.5f;

			//数字の生成
			CObject2D* pNumber = CObject2D::Create();
			pNumber->SetPos(D3DXVECTOR3(RankPos.x + NONE_RANKING_RANK_POS.x, RankPos.y + NONE_RANKING_RANK_POS.y, RankPos.z));
			pNumber->SetSize(NONE_RANKING_RANK_SIZE);
			pNumber->SetTexture("data\\TEXTURE\\RANKING_TEXT.png");
			pNumber->SetAnim(D3DXVECTOR2(0.0f, nCntObject * 0.2f),
				D3DXVECTOR2(1.0f, nCntObject * 0.2f + 0.2f));
		}
	}

	CManager::GetInstance()->GetCamera(0)->SetCameraPos(D3DXVECTOR3(3500.0f, 450.0f, 0.0f));
	CManager::GetInstance()->SetEndScore(0);		// スコア初期化

	// タイマーの初期化
	m_fTimer = 0.0f;

	// 背景オブジェクトのゲーム設置処理
	auto grid = FIELD_GRID;
	CBgObjManager::GetInstance()->SetGame(grid);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CResult::Uninit(void)
{
	CManager::GetInstance()->GetSound()->Stop();

	// 背景オブジェクトの終了処理
	CBgObjManager::GetInstance()->Uninit();

	if (m_pScoreTex != nullptr)
	{
		m_pScoreTex->Uninit();
		m_pScoreTex = nullptr;
	}

	if (m_pLifeRanking != nullptr)
	{
		m_pLifeRanking->Uninit();
		m_pLifeRanking = nullptr;
	}

	if (m_p1PTex != nullptr)
	{
		m_p1PTex->Uninit();
		m_p1PTex = nullptr;
	}

	if (m_p2PTex != nullptr)
	{
		m_p2PTex->Uninit();
		m_p2PTex = nullptr;
	}

	for (int i = 0; i < NUM_DIGIT; i++)
	{
		if (m_apScore[i] != nullptr)
		{
			m_apScore[i]->Uninit();
			m_apScore[i] = nullptr;
		}

		if (m_ap1P[i] != nullptr)
		{
			m_ap1P[i]->Uninit();
			m_ap1P[i] = nullptr;
		}

		if (m_ap2P[i] != nullptr)
		{
			m_ap2P[i]->Uninit();
			m_ap2P[i] = nullptr;
		}
	}

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	if (m_pResult != nullptr)
	{
		delete m_pResult;
		m_pResult = nullptr;
	}
}

//====================================================================
//更新処理
//====================================================================
void CResult::Update(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
	}
	if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
	}
	if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 1) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
	}

	// 背景モデルの更新処理
	CBgObjManager::GetInstance()->Update();

	// 自動遷移
	AutoFade();
}

//====================================================================
//描画処理
//====================================================================
void CResult::Draw(void)
{

}

//==========================================
//  自動遷移処理
//==========================================
void CResult::AutoFade()
{
	// 自動遷移タイマーを加算
	m_fTimer += DeltaTime::Get();

	// 規定時間を超えた場合ランキングに遷移する
	if (m_fTimer >= FADE_TIME)
	{
		CFade::SetFade(CScene::MODE_TITLE);
	}
}
