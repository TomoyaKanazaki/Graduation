//============================================
//
//	タイムの処理 [timer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "timer.h"
#include "number.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float SPACE_TIME = 40.0f;		//タイム同士の隙間
	const D3DXVECTOR3 TIME_POS = D3DXVECTOR3(540.0f, 40.0f, 0.0f);		//タイムの位置
}

//静的メンバ変数宣言
CNumber* CTimer::m_apObject[NUM_TIME] = {};
D3DXVECTOR3 CTimer::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CTimer::m_nTime = 0;
int CTimer::m_nCount = 0;
int CTimer::m_StartTime = 0;
int CTimer::m_nNumber[NUM_TIME] = { 0 };
bool CTimer::m_StopTime = false;

//====================================================================
//コンストラクタ
//====================================================================
CTimer::CTimer(int nPriority) : CObject(nPriority)
{

}

//====================================================================
//デストラクタ
//====================================================================
CTimer::~CTimer()
{

}

//====================================================================
//生成処理
//====================================================================
CTimer* CTimer::Create()
{
	CTimer* pTimer = nullptr;

	if (pTimer == nullptr)
	{
		//オブジェクト2Dの生成
		pTimer = new CTimer();
	}

	//オブジェクトの初期化処理
	if (FAILED(pTimer->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pTimer;
}

//====================================================================
//タイムの初期化
//====================================================================
HRESULT CTimer::Init(void)
{
	SetType(CObject::TYPE_TIME);

	SetPos(TIME_POS);

	SetTime(0);

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			//数字の生成
			m_apObject[nCntObject] = CNumber::Create();
			m_apObject[nCntObject]->SetNumber(0);

			if (nCntObject < 2)
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_TIME) - 7.0f, m_pos.y, m_pos.z));
				m_apObject[nCntObject]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
			}
			else if (nCntObject < 4)
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_TIME), m_pos.y, m_pos.z));
				m_apObject[nCntObject]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
			}
			else
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * (SPACE_TIME - 5.0f)) + 20.0f, m_pos.y + 5.0f, m_pos.z));
				m_apObject[nCntObject]->SetSize(D3DXVECTOR3(40.0f, 40.0f, 0.0f));
			}

			m_apObject[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
	return S_OK;
}

//====================================================================
//タイムの終了
//====================================================================
void CTimer::Uninit(void)
{
	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		m_apObject[nCntObject]->Uninit();
		m_apObject[nCntObject] = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//所持ポインタの終了
//====================================================================
void CTimer::SetNULL(void)
{
	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		m_apObject[nCntObject] = nullptr;
	}
}

//====================================================================
//タイムの更新
//====================================================================
void CTimer::Update(void)
{
	int nTime = m_nTime / 1000;

	//分
	m_nNumber[0] = (nTime / 60) % 100 / 10;
	m_nNumber[1] = (nTime / 60) % 10 / 1;
	nTime %= 60;

	//秒
	m_nNumber[2] = (nTime) % 100 / 10;
	m_nNumber[3] = (nTime) % 10 / 1;

	//ミリ秒
	m_nNumber[4] = (m_nTime) % 1000 / 100;
	m_nNumber[5] = (m_nTime) % 100 / 10;

	//m_nNumber[0] = m_nTime % 10000000 / 1000000;
	//m_nNumber[1] = m_nTime % 1000000 / 100000;
	//m_nNumber[2] = m_nTime % 100000 / 10000;
	//m_nNumber[3] = m_nTime % 10000 / 1000;
	//m_nNumber[4] = m_nTime % 1000 / 100;
	//m_nNumber[5] = m_nTime % 100 / 10;

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		m_apObject[nCntObject]->SetNumber(m_nNumber[nCntObject]);
	}
}

//====================================================================
//タイムの描画
//====================================================================
void CTimer::Draw(void)
{

}

//====================================================================
//タイムの描画
//====================================================================
void CTimer::FloatSetTime(int Time)
{
	if (m_StopTime == false)
	{
		m_nTime = Time - m_StartTime;
	}
}

//====================================================================
//タイムの描画
//====================================================================
void CTimer::SetStartTime(int Time)
{
	m_StartTime = Time;

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{
			m_apObject[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//====================================================================
//タイムの情報取得
//====================================================================
int CTimer::GetTimeNumber(void)
{
	int nTime = m_nTime / 1000;
	int ReturnTime = 0;

	//分
	ReturnTime += ((nTime / 60) % 100 / 10) * 100000;
	ReturnTime += ((nTime / 60) % 10 / 1) * 10000;
	nTime %= 60;

	//秒
	ReturnTime += ((nTime) % 100 / 10) * 1000;
	ReturnTime += ((nTime) % 10 / 1) * 100;

	//ミリ秒
	ReturnTime += ((m_nTime) % 1000 / 100) * 10;
	ReturnTime += ((m_nTime) % 100 / 10);

	return ReturnTime;
}