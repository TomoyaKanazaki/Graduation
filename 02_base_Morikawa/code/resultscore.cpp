//==========================================
//
//  リザルトすこあクラス(resultscore.cpp)
//  Author : 相馬靜雅
//
//==========================================
#include "resultscore.h"
#include "number.h"
#include "manager.h"
#include "input.h"
#include "time.h"

//==========================================
//  定数定義
//==========================================
namespace
{
	const int DIGIT = 5; // 初期値
	const float INIT_TIME = 30.0f; // 初期値
	const D3DXVECTOR2 NUMBER_SIZE = D3DXVECTOR2(50.0f, 50.0f); // サイズ
	const D3DXVECTOR3 NUMBER_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 50.0f, 0.0f); // 座標
}

//==========================================
//  コンストラクタ
//==========================================
CResultScore::CResultScore() :
	m_fTimer(0.0f)
{
}

//==========================================
//  デストラクタ
//==========================================
CResultScore::~CResultScore()
{
}

//==========================================
//  初期化処理
//==========================================
HRESULT CResultScore::Init()
{
	D3DXVECTOR3 pos = GetPos() + MyLib::Vector3(-m_fSize * 0.5f * DIGIT, 0.0f, 0.0f);

	for (int i = 0; i < DIGIT; ++i)
	{
		m_pNumber[i] = CNumber::Create();
		m_pNumber[i]->SetSize(m_fSize, m_fSize);
		pos.x += m_fSize * 0.8f;

		if (i % 2 == 0)
		{
			pos.x += m_fSize * 0.25f;
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
void CResultScore::Uninit()
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
}

//==========================================
//  更新処理
//==========================================
void CResultScore::Update()
{
	// 計算
	CalcNum();
}

//==========================================
//  描画処理
//==========================================
void CResultScore::Draw()
{
}

//==========================================
//  生成処理
//==========================================
CResultScore* CResultScore::Create(const MyLib::Vector3& pos, float time, float size)
{
	// ポインタを確保
	CResultScore* pTime = new CResultScore;

	// nullチェック
	if (pTime == nullptr) { assert(false); return nullptr; }

	// 初期化処理
	pTime->SetPos(pos);
	pTime->m_fTimer = time;
	pTime->m_fSize = size;	// サイズ
	pTime->Init();

	return pTime;
}

//==========================================
//  計算
//==========================================
void CResultScore::CalcNum()
{

	// タイマーを整数値に変換
	int nCalc = (float)(m_fTimer * 1000.0f);

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
