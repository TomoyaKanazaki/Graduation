//============================================
//
//	ランキングの処理 [ranking.cpp]
//	Author:sakamoto kai
//
//============================================
#include "manager.h"
#include "ranking.h"
#include "number.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float SPACE_TIME = 50.0f;		//ランキング同士の隙間
	const D3DXVECTOR3 TIME_POS = D3DXVECTOR3(1000.0f, 200.0f, 0.0f);		//ランキングの位置
	const D3DXVECTOR3 RANKING_SIZE = D3DXVECTOR3(70.0f, 70.0f, 0.0f);		//ランキングの大きさ
	const float NUMBER_DISTANCE = 50.0f;		//文字同士の幅
	const float RANKING_DISTANCE = 85.0f;	//ランキング同士の幅
	const int NUM_DIGIT = 6;				// 桁数
}

//静的メンバ変数宣言

//====================================================================
//コンストラクタ
//====================================================================
CRanking::CRanking(int nPriority) : CObject(nPriority)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
		{
			m_apObject[nCntTime][nCntRank] = nullptr;
			m_nNumber[nCntTime][nCntRank] = 0;
		}

		m_nTime[nCntRank] = NULL;

	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCount = 0;
	m_StartTime = 0;
	m_StopTime = false;
	m_SetRanking = false;
	m_bTime = false;
	m_nRankNumber = MAX_RANK;
}

//====================================================================
//デストラクタ
//====================================================================
CRanking::~CRanking()
{

}

//====================================================================
//生成処理
//====================================================================
CRanking *CRanking::Create(char* pModelName)
{
	CRanking *pMultiBG = nullptr;

	if (pMultiBG == nullptr)
	{
		//オブジェクト2Dの生成
		pMultiBG = new CRanking();
	}

	pMultiBG->SetTextName(pModelName);

	//オブジェクトの初期化処理
	if (FAILED(pMultiBG->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pMultiBG;
}

//====================================================================
//ランキングの初期化
//====================================================================
HRESULT CRanking::Init(void)
{
	SetType(CObject::TYPE_TIME);

	SetPos(TIME_POS);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
		{
			if (m_apObject[nCntObject][nCntRank] == nullptr)
			{
				//数字の生成
				m_apObject[nCntObject][nCntRank] = CNumber::Create();
				m_apObject[nCntObject][nCntRank]->SetNumber(0);
				m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
				m_apObject[nCntObject][nCntRank]->SetSize(RANKING_SIZE);
			}
		}
	}

	// txt読み込み
	LoadRanking();

	return S_OK;
}

//====================================================================
//ランキングの終了
//====================================================================
void CRanking::Uninit(void)
{

}

//====================================================================
//ランキングの終了
//====================================================================
void CRanking::SetNULL(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
		{
			if (m_apObject[nCntObject][nCntRank] != nullptr)
			{
				m_apObject[nCntObject][nCntRank]->Uninit();
				m_apObject[nCntObject][nCntRank] = nullptr;
			}
		}
	}
}

//====================================================================
//ランキングの終了
//====================================================================
void CRanking::SetUseTime(bool Set)
{
	m_bTime = Set;

	if (m_bTime == true)
	{
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
			{
				if (m_apObject[nCntObject][nCntRank] != nullptr)
				{
					if (nCntObject < NUM_TIME - 4)
					{
						//数字の生成
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE - 4.0f), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
						m_apObject[nCntObject][nCntRank]->SetSize(RANKING_SIZE);
					}
					else if (nCntObject < NUM_TIME - 2)
					{
						//数字の生成
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
						m_apObject[nCntObject][nCntRank]->SetSize(RANKING_SIZE);
					}
					else
					{
						//数字の生成
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(
							m_pos.x + (nCntObject * NUMBER_DISTANCE + RANKING_SIZE.x * 0.2f), 
							m_pos.y + (nCntRank * RANKING_DISTANCE + RANKING_SIZE.y * 0.2f), 
							m_pos.z));
						m_apObject[nCntObject][nCntRank]->SetSize(RANKING_SIZE);
					}
				}
			}
		}
	}
}

//====================================================================
//ランキングの更新
//====================================================================
void CRanking::Update(void)
{
	if (m_bTime == true)
	{
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
			{
				if (m_apObject[nCntObject][nCntRank] != nullptr)
				{
					if (nCntObject < NUM_TIME - 4)
					{
						//数字の生成
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE - 4.0f), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
					}
					else if (nCntObject < NUM_TIME - 2)
					{
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
					}
					else
					{
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(
							m_pos.x + (nCntObject * NUMBER_DISTANCE + RANKING_SIZE.x * 0.2f),
							m_pos.y + (nCntRank * RANKING_DISTANCE + RANKING_SIZE.y * 0.2f),
							m_pos.z));
					}
				}
			}
		}
	}
	else
	{
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
			{
				if (m_apObject[nCntObject] != nullptr)
				{
					m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));

					if (m_nRankNumber == nCntRank)
					{
						m_apObject[nCntObject][nCntRank]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					}
				}
			}
		}
	}

	int nDigit = 0;	

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int i = 0; i < NUM_DIGIT; i++)
		{
			nDigit = (int)pow(10, (NUM_DIGIT - i));		//桁数

			m_nNumber[i][nCntRank] = m_nTime[nCntRank] % nDigit / (nDigit * 0.1f);
		}

		for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
		{
			m_apObject[nCntObject][nCntRank]->SetNumber(m_nNumber[nCntObject][nCntRank]);
			//m_apObject[nCntObject][nCntRank]->SetNumber(nCntObject * 2 + nCntRank * 2);
		}
	}
}

//====================================================================
//ランキングの描画
//====================================================================
void CRanking::Draw(void)
{

}

//====================================================================
//ランキングの描画
//====================================================================
void CRanking::FloatSetTime(int Time)
{
	if (m_StopTime == false)
	{
		m_nTime[0] = Time - m_StartTime;
	}
}

//====================================================================
//ランキングの設定処理
//====================================================================
void CRanking::SetRanking(int nRanKing)
{
	int nCount1;
	int nCount2;
	int nData;

	//昇順
	//if (nRanKing < m_nTime[MAX_RANK - 1])
	//{
	//	m_nTime[MAX_RANK - 1] = nRanKing;

	//	for (nCount1 = 0; nCount1 < MAX_RANK; nCount1++)
	//	{
	//		for (nCount2 = nCount1; nCount2 < MAX_RANK; nCount2++)
	//		{
	//			if (m_nTime[nCount1] > m_nTime[nCount2])
	//			{
	//				nData = m_nTime[nCount1];
	//				m_nTime[nCount1] = m_nTime[nCount2];
	//				m_nTime[nCount2] = nData;
	//			}
	//		}
	//	}
	//}

	//降順
	if (nRanKing > m_nTime[MAX_RANK - 1])
	{
		m_nTime[MAX_RANK - 1] = nRanKing;

		for (nCount1 = 0; nCount1 < MAX_RANK; nCount1++)
		{
			for (nCount2 = nCount1; nCount2 < MAX_RANK; nCount2++)
			{
				if (m_nTime[nCount1] < m_nTime[nCount2])
				{
					nData = m_nTime[nCount1];
					m_nTime[nCount1] = m_nTime[nCount2];
					m_nTime[nCount2] = nData;
				}
			}
		}

		for (nCount1 = 0; nCount1 < MAX_RANK; nCount1++)
		{
			if (m_nTime[nCount1] == nRanKing && m_nTime[nCount1] != m_nTime[nCount1 - 1])
			{
				m_nRankNumber = nCount1;
			}
		}
	}

	// ランキングの保存
	SaveRanking();
}

//====================================================================
//保存処理
//====================================================================
void CRanking::SaveRanking(void)
{
	FILE *pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(m_cTextName, "w");
	
	if (pFile != nullptr)
	{//ファイルが開けた場合

		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fprintf(pFile, "%d\n", m_nTime[nCnt]);
		}

		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
//マップを全て埋めて生成
//====================================================================
void CRanking::LoadRanking(void)
{
	FILE *pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(m_cTextName, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fscanf(pFile, "%d", &m_nTime[nCnt]);
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}