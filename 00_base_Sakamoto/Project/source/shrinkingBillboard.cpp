//============================================
//
//	数字の処理 [numberBillboard.cpp]
//	Author:sakamoto kai
//
//============================================
#include "shrinkingBillboard.h"
#include "objectBillboard.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"

//=============================
// 定数定義
//=============================
namespace
{
	const float DEF_RADIUS_MIN = 10.0f;
	const float DEF_RADIUS_MAX = 100.0f;
}

//====================================================================
//コンストラクタ
//====================================================================
CShrinkingBillboard::CShrinkingBillboard(int nPriority) : CObject(nPriority)
{
	m_nShrinkFrameAll = 0;
	m_nShrinkNow = 0;
	m_fShrinkRadius = 0;
	m_fHoldTime	= 0;
}

//====================================================================
//デストラクタ
//====================================================================
CShrinkingBillboard::~CShrinkingBillboard()
{

}

//====================================================================
//生成処理
//====================================================================
CShrinkingBillboard* CShrinkingBillboard::Create(void)
{
	CShrinkingBillboard* pNumber = NULL;

	if (pNumber == NULL)
	{
		//オブジェクト2Dの生成
		pNumber = new CShrinkingBillboard();
	}

	//オブジェクトの初期化処理
	if (FAILED(pNumber->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pNumber;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CShrinkingBillboard::Init(void)
{
	m_pMin = CObjectBillboard::Create();
	m_pMax = CObjectBillboard::Create();

	m_pMin->SetTexture("data\\TEXTURE\\circle.png");
	m_pMax->SetTexture("data\\TEXTURE\\circle.png");

	m_pMin->SetWidth(DEF_RADIUS_MIN);
	m_pMin->SetHeight(DEF_RADIUS_MIN);
	m_pMax->SetWidth(DEF_RADIUS_MAX);
	m_pMax->SetHeight(DEF_RADIUS_MAX);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CShrinkingBillboard::Uninit(void)
{
	if (m_pMin != nullptr)
	{
		m_pMin->Uninit();
		m_pMin = nullptr;
	}

	if (m_pMax != nullptr)
	{
		m_pMax->Uninit();
		m_pMax = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CShrinkingBillboard::Update(void)
{
	if (m_nShrinkFrameAll >= m_nShrinkNow)
	{
		float fRadius = m_pMax->GetWidth();
		float fRadiusMin = m_pMin->GetWidth();
		fRadius -= m_fShrinkRadius;

		if (fRadius < fRadiusMin)
		{//基準より小さくならない
			fRadius = fRadiusMin;
		}

		m_pMax->SetWidth(fRadius);
		m_pMax->SetHeight(fRadius);
	}
	else
	{
		Uninit();
		return;
	}
	
	m_fHoldTime += CManager::GetInstance()->GetGameSpeed();

	if (m_fHoldTime >= 1.0f)
	{
		m_nShrinkNow++;
		m_fHoldTime -= 1.0f;
	}
}

//====================================================================
// 描画
//====================================================================
void CShrinkingBillboard::Draw()
{

}

//====================================================================
// 半径設定
//====================================================================
void CShrinkingBillboard::SetRadius(float fRadMin, float fRadMax)
{
	m_pMin->SetWidth(fRadMin);
	m_pMin->SetHeight(fRadMin);
	m_pMax->SetWidth(fRadMax);
	m_pMax->SetHeight(fRadMax);

	m_fShrinkRadius = (float)(fRadMax - fRadMin) / m_nShrinkFrameAll;
}

//====================================================================
// 総フレーム設定
//====================================================================
void CShrinkingBillboard::SetFrameAll(int nFrame)
{
	m_nShrinkFrameAll = nFrame;

	float fRadMin = m_pMin->GetWidth();
	float fRadMax = m_pMax->GetWidth();

	m_fShrinkRadius = (float)(fRadMax - fRadMin) / m_nShrinkFrameAll;
}

//====================================================================
// 位置設定
//====================================================================
void CShrinkingBillboard::SetPos(D3DXVECTOR3 pos)
{
	m_pMin->SetPos(pos);
	m_pMax->SetPos(pos);
}
