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

//マクロ定義
namespace
{
	const char* START_OK("STARTSETSTAGE");	//スタートメッセージがあるかどうかの確認
	const char* END_SET_OK("ENDSETSTAGE");	//エンドメッセージがあるかどうかの確認
	const float SCORE_POSX(300.0f);
	const float SCORE_MOVEX(20.1f);

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
	const D3DXVECTOR3 MULTI_PLAYER_TEX_SIZE(D3DXVECTOR3(200.0f, 80.0f, 0.0f));								//スコアの大きさ
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
}

//静的メンバ変数宣言
CResult* CResult::m_pResult = nullptr;

//====================================================================
//コンストラクタ
//====================================================================
CResult::CResult()
{
	m_pBg = nullptr;
	m_pLifeRanking = nullptr;
	m_pScoreTex = nullptr;
	m_Appear = false;

	for (int nCnt = 0; nCnt < 6; nCnt++)
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

	m_ScoreData = CManager::GetInstance()->GetEndScore();

	//背景
	m_pBg = CObject2D::Create();
	m_pBg->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	m_pBg->SetSize(D3DXVECTOR3(1280.0f, 720.0f, 0.0f));
	m_pBg->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_SINGLE)
	{
		for (int nCntObject = 0; nCntObject < 6; nCntObject++)
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
		m_pLifeRanking = CRanking::Create("data\\TXT\\LifeRanking.txt");
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
		for (int nCntObject = 0; nCntObject < 6; nCntObject++)
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
		//m_p1PTex->SetTexture("data\\TEXTURE\\result_score.png");

		//今回のスコア
		for (int nCntObject = 0; nCntObject < 6; nCntObject++)
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
		//m_p2PTex->SetTexture("data\\TEXTURE\\result_score.png");

		//今回のスコア
		for (int nCntObject = 0; nCntObject < 6; nCntObject++)
		{
			//数字の生成
			m_ap2P[nCntObject] = CNumber::Create();
			m_ap2P[nCntObject]->SetPos(D3DXVECTOR3(MULTI_2P_VALUE_POS.x + (nCntObject * 45.0f), MULTI_2P_VALUE_POS.y, MULTI_2P_VALUE_POS.z));
			m_ap2P[nCntObject]->SetSize(D3DXVECTOR3(60.0f, 72.0f, 0.0f));
			m_ap2P[nCntObject]->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
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
		m_pLifeRanking = CRanking::Create("data\\TXT\\LifeRanking.txt");
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
		m_pLifeRanking = CRanking::Create("data\\TXT\\LifeRanking.txt");
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

	CManager::GetInstance()->GetCamera()->SetCameraPos(D3DXVECTOR3(3500.0f, 450.0f, 0.0f));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CResult::Uninit(void)
{
	CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_RESULT);

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	if (m_pResult != nullptr)
	{
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
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
		//m_pLifeRanking->SaveRanking();
	}
	if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
		//m_pLifeRanking->SaveRanking();
	}
}

//====================================================================
//描画処理
//====================================================================
void CResult::Draw(void)
{

}