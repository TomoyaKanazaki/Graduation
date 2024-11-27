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
	const D3DXVECTOR3 SCORE_TEX_POS(D3DXVECTOR3(200.0f, 400.0f, 0.0f));		//評価点の位置
	const D3DXVECTOR3 SCORE_VALUE_POS(D3DXVECTOR3(SCORE_TEX_POS.x + 105.0f, SCORE_TEX_POS.y, 0.0f));		//評価点の値の位置
	const float SCORE_DISTANCE(25.0f);
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
	m_ClearText = nullptr;
	m_pScoreTex = nullptr;
	m_Appear = false;

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		m_apLife[nCnt] = nullptr;
	}
	m_AddScoreCount = 0;
	m_LifeData = 0;

	m_AddScoreCount = 0;
	m_LifeData = 0;
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

	m_LifeData = CManager::GetInstance()->GetEndScore();

	//背景
	m_pBg = CObject2D::Create();
	m_pBg->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	m_pBg->SetSize(D3DXVECTOR3(1280.0f, 720.0f, 0.0f));
	m_pBg->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	//今回のスコア
	for (int nCntObject = 0; nCntObject < 6; nCntObject++)
	{
		//数字の生成
		m_apLife[nCntObject] = CNumber::Create();
		m_apLife[nCntObject]->SetPos(D3DXVECTOR3(SCORE_VALUE_POS.x + (nCntObject * 45.0f), SCORE_VALUE_POS.y, SCORE_VALUE_POS.z));
		m_apLife[nCntObject]->SetSize(D3DXVECTOR3(60.0f, 72.0f, 0.0f));
		m_apLife[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	m_apLife[0]->SetNumber(m_LifeData % 1000000 / 100000);
	m_apLife[1]->SetNumber(m_LifeData % 100000 / 10000);
	m_apLife[2]->SetNumber(m_LifeData % 10000 / 1000);
	m_apLife[3]->SetNumber(m_LifeData % 1000 / 100);
	m_apLife[4]->SetNumber(m_LifeData % 100 / 10);
	m_apLife[5]->SetNumber(m_LifeData % 10 / 1);

	//"評価点"っていうテクスチャポリゴン
	m_pScoreTex = CObject2D::Create();
	m_pScoreTex->SetPos(SCORE_TEX_POS);
	m_pScoreTex->SetSize(D3DXVECTOR3(200.0f, 80.0f, 0.0f));
	m_pScoreTex->SetTexture("data\\TEXTURE\\result_score.png");

	//全体ランキング(「ランキング」)
	CObject2D *pRank = CObject2D::Create();
	pRank->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.80f, SCREEN_HEIGHT * 0.185f, 0.0f));
	pRank->SetSize(D3DXVECTOR3(340.0f, 180.0f, 0.0f));
	pRank->SetTexture("data\\TEXTURE\\ranking.png");

	//全体ランキング(スコア)
	m_pLifeRanking = CRanking::Create("data\\TXT\\LifeRanking.txt");
	m_pLifeRanking->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.35f, 0.0f));
	m_pLifeRanking->SetRanking(m_LifeData);

	//全体ランキング(順位)
	for (int nCntObject = 0; nCntObject < 5; nCntObject++)
	{
		CNumber *pNumRank = m_pLifeRanking->GetNumRankScore( 0, nCntObject);
		D3DXVECTOR3 RankPos = pNumRank->GetPos();
		RankPos.y += pNumRank->GetSize().y * 0.5f;

		//数字の生成
		pRank = CObject2D::Create();
		pRank->SetPos(D3DXVECTOR3(RankPos.x - 150.0f, RankPos.y + 17.0f, RankPos.z));
		pRank->SetSize(D3DXVECTOR3(256.0f, 102.4f, 0.0f));
		pRank->SetTexture("data\\TEXTURE\\RANKING_TEXT.png");
		pRank->SetAnim(D3DXVECTOR2(0.0f, nCntObject * 0.2f),
						 D3DXVECTOR2(1.0f, nCntObject * 0.2f + 0.2f));
	}

	CManager::GetInstance()->GetCamera()->SetCameraPos(D3DXVECTOR3(3500.0f, 450.0f, 0.0f));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CResult::Uninit(void)
{
	CManager::GetInstance()->GetSound()->StopSound(CSound::SOUND_LABEL_BGM_RESULT);

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
		m_pLifeRanking->SaveRanking();
	}
	if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
		//CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
		m_pLifeRanking->SaveRanking();
	}
}

//====================================================================
//描画処理
//====================================================================
void CResult::Draw(void)
{

}