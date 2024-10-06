//===============================================================================
//
// スロー処理[slowManajer.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _SLOWMANAGER_H_
#define _SLOWMANAGER_H_

// インクルード宣言
#include "main.h"
#include "object.h"

// スローマネージャクラス
class CSlowManager
{
private:
	static const int MAX_SLOW_OBJ = 24;	// 最大数

public:
	enum ECamp	// 陣営
	{
		CAMP_NONE,
		CAMP_PLAYER,
		CAMP_ENEMY,
		CAMP_MAX
	};

	enum ETag
	{
		TAG_NONE = 0,	// 陣営なし
		TAG_PLAYER,		// プレイヤー陣営
		TAG_ENEMY,		// 敵陣営
		TAG_MAX
	};

public:
	CSlowManager(ECamp nCamp = CAMP_NONE, ETag tag = TAG_NONE);	// コンストラクタ
	~CSlowManager();	// デストラクタ

	static CSlowManager* Create(ECamp camp = CAMP_NONE, ETag tag = TAG_NONE);

	// プロトタイプ宣言
	HRESULT Init();

	static void ReleaseAll(void);

	// 設定
	void SetTag(ETag tag) { m_tag = tag; }	// タグ
	void SetCamp(ECamp camp) { m_camp = camp; }	// 所属陣営
	void SetValue(float fValue) { m_fValue = fValue; }
	static void SetValue(ECamp camp, float fValue);
	static void SetValue(ECamp camp, ETag tag, float fValue);
	static void SetValueAll(float fValue);	// 全体をスロー
	static void SetValueDefault(void);	// デフォルト

	// 取得
	ETag GetTag(void) const { return m_tag; }
	ECamp GetCamp(void) const { return m_camp; }
	float GetValue(void) const { return m_fValue; }
	CSlowManager** GetInfo(void) { return &m_apSlow[0][0]; }

private:
	// メンバ変数
	ETag m_tag;	// タグ
	float m_fValue;	// 数値
	float m_fCount;	// カウント用

	// 情報
	static CSlowManager* m_apSlow[CAMP_MAX][MAX_SLOW_OBJ];	// 管理用
	ECamp m_camp;	// 所属
};

#endif