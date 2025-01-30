//============================================
//
//	獲得スコアUI [getscoreUi.cpp]
//	Author:澗口将太郎
//
//============================================
#include "getscoreUi.h"
#include "numBillboard.h"
#include "renderer.h"
#include "manager.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int OBJ_LIFE = 50;

	const float OBJ_DEGITPOS_X =35.0f;

	const float OBJ_ADDPOS_Y = 60.0f;
}

//====================================================================
//コンストラクタ
//====================================================================
CGetscoreUi::CGetscoreUi(int nPriority) : CObject(nPriority)
{
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		m_pNum[nCnt] = nullptr;
	}
	m_nLife = OBJ_LIFE;

}

//====================================================================
//デストラクタ
//====================================================================
CGetscoreUi::~CGetscoreUi()
{

}

//====================================================================
//生成処理
//====================================================================
CGetscoreUi* CGetscoreUi::Create(int nGetScore,D3DXVECTOR3 pos,int nPriority)
{
	CGetscoreUi* pSample2D = nullptr;

	if (pSample2D == nullptr)
	{
		//オブジェクト2Dの生成
		pSample2D = new CGetscoreUi(nPriority);
	}

	int nScore = nGetScore;
	bool bDegitHead = false;

	for (int nCnt = MAX_NUM-1; nCnt >= 0; nCnt--)
	{
		int nDegit = pow(10, nCnt);
		if (nScore / nDegit >= 1|| bDegitHead==true)
		{
			bDegitHead = true;
			pSample2D->m_pNum[nCnt] =  CNumBillboard::Create();	//数字ビルボードobjの生成
			pSample2D->m_pNum[nCnt]->SetNumber(nScore / nDegit);//表示する数の設定

			pSample2D->m_pNum[nCnt]->SetPos(D3DXVECTOR3(pos.x - nCnt * OBJ_DEGITPOS_X + OBJ_DEGITPOS_X * 2, pos.y + OBJ_ADDPOS_Y, pos.z+50.0f));

			nScore = nScore - nScore / nDegit * nDegit;//現在の桁数を削除
		}
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample2D->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pSample2D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CGetscoreUi::Init(void)
{
	
	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CGetscoreUi::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		if (m_pNum[nCnt] != nullptr)
		{
			m_pNum[nCnt]->Uninit();
			m_pNum[nCnt] = nullptr;
		}
	}
}

//====================================================================
//更新処理
//====================================================================
void CGetscoreUi::Update(void)
{
	
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		if (m_pNum[nCnt] != nullptr)
		{
			D3DXVECTOR3 posold = m_pNum[nCnt]->GetPos();
			m_pNum[nCnt]->SetPos(D3DXVECTOR3(posold.x, posold.y+ 0.3f, posold.z + 0.3f));
		}
	}
	//寿命カウントダウン
	m_nLife--;
	
	if (m_nLife <= 0)
	{//寿命が切れた場合削除
		this->Uninit();
	}
}
//====================================================================
//描画処理
//====================================================================
void CGetscoreUi::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		if (m_pNum[nCnt] != nullptr)
		{
			m_pNum[nCnt]->Draw();
		}
	}
}