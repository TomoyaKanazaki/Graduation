//============================================
//
//	スコアの処理 [score.cpp]
//	Author:hashimoto kenta
//
//============================================
#include "score.h"
#include "number.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float SPACE_SCORE = 40.0f;		//スコア同士の隙間
	const int INIT_SCORE = 0;		//スコアの位置
	const D3DXVECTOR2 SCORE_SIZE = D3DXVECTOR2(50.0f,50.0f);
}

//静的メンバ変数宣言

//====================================================================
//コンストラクタ
//====================================================================
CScore::CScore(int nPriority) : CObject(nPriority)
{
	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		m_apObject[nCntObject] = nullptr;
		m_nNumber[nCntObject] = 0;
	}
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nScore = 0;
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

	m_pos = (INITVECTOR3);

	SetScore(INIT_SCORE);

	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			//数字の生成
			m_apObject[nCntObject] = CNumber::Create();
			m_apObject[nCntObject]->SetNumber(0);

			m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_SCORE), m_pos.y, m_pos.z));
			m_apObject[nCntObject]->SetSize(D3DXVECTOR3(SCORE_SIZE));

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
	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{
			m_apObject[nCntObject]->Uninit();
			m_apObject[nCntObject] = nullptr;
		}
	}

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

//====================================================================
//位置の設定
//====================================================================
void CScore::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	for (int nCntObject = 0; nCntObject < NUM_SCORE; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{
			//数字の位置設定
			m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_SCORE), m_pos.y, m_pos.z));
		}
	}
}