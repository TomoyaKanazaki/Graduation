//======================================================================================
//
// スロー処理[slow.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include "slowManager.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"

// 静的メンバ変数宣言
CSlowManager* CSlowManager::m_apSlow[CAMP_MAX][MAX_SLOW_OBJ] = {};

//======================================================================================
// コンストラクタ
//======================================================================================
CSlowManager::CSlowManager(ECamp camp, ETag tag)
{
	m_tag = tag;
	m_camp = camp;
	m_fValue = 1.0f;
	m_fCount = 0.0f;

	if (camp >= CAMP_MAX && camp < CAMP_NONE)
	{// 範囲外の場合、所属なし
		m_camp = CAMP_NONE;
	}

	for (int nCntObject = 0; nCntObject < MAX_SLOW_OBJ; nCntObject++)
	{
		if (m_apSlow[m_camp][nCntObject] == nullptr)
		{
			m_apSlow[m_camp][nCntObject] = this;	//自分自身を代入

			break;
		}
	}
}

//======================================================================================
// デストラクタ
//======================================================================================
CSlowManager::~CSlowManager()
{

}

//======================================================================================
// 生成
//======================================================================================
CSlowManager* CSlowManager::Create(ECamp camp, ETag tag)
{
	CSlowManager* m_pSlow = nullptr;

	if (m_pSlow == nullptr)
	{
		m_pSlow = new CSlowManager(camp, tag);

		m_pSlow->Init();

		return m_pSlow;
	}

	return m_pSlow;
}

//======================================================================================
// 初期化処理
//======================================================================================
HRESULT CSlowManager::Init()
{

	return S_OK;
}

//======================================================================================
// 全破棄
//======================================================================================
void CSlowManager::ReleaseAll(void)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_SLOW_OBJ; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			delete m_apSlow[nCntCamp][i];
			m_apSlow[nCntCamp][i] = nullptr;
		}
	}
}

//======================================================================================
// 全体をスローにする
//======================================================================================
void CSlowManager::SetValueAll(float fValue)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_SLOW_OBJ; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			// 値を設定(全体)
			m_apSlow[nCntCamp][i]->SetValue(fValue);
		}
	}
}

//======================================================================================
// 全体を通常へ戻す
//======================================================================================
void CSlowManager::SetValueDefault(void)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_SLOW_OBJ; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			// 値をデフォルト(全体)
			m_apSlow[nCntCamp][i]->SetValue(1.0f);
		}
	}
}

//======================================================================================
// 陣営別の数値設定
//======================================================================================
void CSlowManager::SetValue(ECamp camp, float fValue)
{
	for (int i = 0; i < MAX_SLOW_OBJ; i++)
	{
		if (m_apSlow[camp][i] == nullptr)
		{
			continue;
		}

		if (m_apSlow[camp][i]->GetCamp() == camp)
		{
			// 値を設定(陣営のタグ)
			m_apSlow[camp][i]->SetValue(fValue);
		}
	}
}

//======================================================================================
// 陣営,タグ別の数値設定
//======================================================================================
void CSlowManager::SetValue(ECamp camp, ETag tag, float fValue)
{
	for (int i = 0; i < MAX_SLOW_OBJ; i++)
	{
		if (m_apSlow[camp][i] == nullptr)
		{
			continue;
		}

		if (m_apSlow[camp][i]->GetTag() == tag)
		{
			// 値を設定(陣営のタグ)
			m_apSlow[camp][i]->SetValue(fValue);
		}
	}
}
