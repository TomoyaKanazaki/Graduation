//==========================================
//
//  タイマークラス(timer.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "timer.h"
#include "number.h"
#include "manager.h"
#include "input.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const float INIT_TIME = 60.0f; // 初期値
	const D3DXVECTOR3 NUMBER_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.72f, 50.0f, 0.0f); // 座標
}

//==========================================
//  静的メンバ変数宣言
//==========================================
float CTimer::m_fTimer = 0.0f;

//==========================================
//  コンストラクタ
//==========================================
CTimer::CTimer()
{
}

//==========================================
//  デストラクタ
//==========================================
CTimer::~CTimer()
{
}

//==========================================
//  初期化処理
//==========================================
HRESULT CTimer::Init()
{
	// 初期値を設定
	m_fTimer = INIT_TIME;

	// 数字を生成

	D3DXVECTOR3 pos = NUMBER_POS;

	for (int i = 0; i < DIGIT; ++i)
	{
		m_pNumber[i] = CNumber::Create();
		m_pNumber[i]->SetSize(60.0f, 60.0f);
		pos.x += 55.0f;

		if (i % 2 == 0)
		{
			pos.x += 15.0f;
		}

		m_pNumber[i]->SetPos(pos);
		m_pNumber[i]->SetPosVertex();
	}

	// 計算
	CalcNum();

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void CTimer::Uninit()
{
	// 数字の終了
	for (int i = 0; i < DIGIT; ++i)
	{
		if (m_pNumber[i] != nullptr)
		{
			m_pNumber[i]->Uninit();
			m_pNumber[i] = nullptr;
		}
	}

	this->Release();
}

//==========================================
//  更新処理
//==========================================
void CTimer::Update()
{
	// タイマーを減少
	m_fTimer -= CManager::GetInstance()->GetDeltaTime();

	// 値を補正
	if (m_fTimer < 0.0f)
	{
		m_fTimer = 0.0f;
	}

	// 計算
	CalcNum();
}

//==========================================
//  描画処理
//==========================================
void CTimer::Draw()
{
}

//==========================================
//  タイムアップしてる判定
//==========================================
bool CTimer::GetTimeZero()
{
	return m_fTimer <= 0.0f;
}

//==========================================
//  生成処理
//==========================================
CTimer* CTimer::Create()
{
	// ポインタを確保
	CTimer* pTime = new CTimer;

	// nullチェック
	if (pTime == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	pTime->Init();

	return pTime;
}

//==========================================
//  制限時間の取得
//==========================================
float CTimer::GetInitTime()
{
	return INIT_TIME;
}

//==========================================
//  計算
//==========================================
void CTimer::CalcNum()
{
	// タイマーを整数値に変換
	int nCalc = (float)(m_fTimer * 1000.0f);

	DebugProc::Print(DebugProc::POINT_RIGHT, "\n時間 : %d\n", nCalc);

	// 数字を更新
	for (int i = DIGIT - 1; i >= 0; --i)
	{
		if (m_pNumber[i] != nullptr)
		{
			// 桁に合わせた数値を算出
			int nNum = nCalc % 10;
			nCalc /= 10;

			// 数値を設定
			m_pNumber[i]->SetNumber(nNum);
		}
	}
}
