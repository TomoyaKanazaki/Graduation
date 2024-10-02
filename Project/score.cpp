//============================================
//
//	スコアの処理 [score.cpp]
//	Author:hashimoto kenta
//
//============================================
#include "score.h"
#include "number.h"

//マクロ定義
#define SPACE_SCORE (60.0f)		//スコア同士の隙間
#define SCORE_POS (D3DXVECTOR3(900.0f, 60.0f, 0.0f))		//スコアの位置
#define INIT_SCORE 0		//スコアの位置

//静的メンバ変数宣言
CNumber* CScore::m_apObject[NUM_SCORE] = {};
D3DXVECTOR3 CScore::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CScore::m_nScore = 0;
int CScore::m_nNumber[NUM_SCORE] = { 0 };

//====================================================================
//コンストラクタ
//====================================================================
CScore::CScore(int nPriority) : CObject(nPriority)
{

}

//====================================================================
//デストラクタ
//====================================================================
CScore::~CScore()
{

}

//====================================================================
//生成処理
//====================================================================
CScore* CScore::Create()
{
	CScore* pScore = nullptr;

	if (pScore == nullptr)
	{
		//オブジェクト2Dの生成
		pScore = new CScore();
	}

	//オブジェクトの初期化処理
	if (FAILED(pScore->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pScore;
}

//====================================================================
//スコアの初期化
//====================================================================
HRESULT CScore::Init(void)
{
	SetType(CObject::TYPE_TIME);

	SetPos(SCORE_POS);

	SetScore(INIT_SCORE);

	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			//数字の生成
			m_apObject[nCntObject] = CNumber::Create();
			m_apObject[nCntObject]->SetNumber(0);

			m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_SCORE), m_pos.y, m_pos.z));
			m_apObject[nCntObject]->SetWidth(65.0f);
			m_apObject[nCntObject]->SetHeight(85.0f);

			m_apObject[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	return S_OK;
}

//====================================================================
//スコアの終了
//====================================================================
void CScore::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//所持ポインタの終了
//====================================================================
void CScore::SetNULL(void)
{
	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		m_apObject[nCntObject] = nullptr;
	}
}

//====================================================================
//スコアの更新
//====================================================================
void CScore::Update(void)
{
	m_nNumber[0] = m_nScore % 1000000 / 100000;
	m_nNumber[1] = m_nScore % 100000 / 10000;
	m_nNumber[2] = m_nScore % 10000 / 1000;
	m_nNumber[3] = m_nScore % 1000 / 100;
	m_nNumber[4] = m_nScore % 100 / 10;
	m_nNumber[5] = m_nScore % 10;

	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		m_apObject[nCntObject]->SetNumber(m_nNumber[nCntObject]);
	}
}

//====================================================================
//スコアの描画
//====================================================================
void CScore::Draw(void)
{
	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		m_apObject[nCntObject]->Draw();
	}
}