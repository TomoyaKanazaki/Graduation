//============================================
//
//	リザルト画面 [result.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"
#include "Scene.h"

//前方宣言
class CObject2D;
class CRanking;
class CBreak_Block3D;
class CNumber;

//シーンクラス
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	static CResult* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetApprear(bool Set) { m_Appear = Set; }
	bool GetApprear(void) { return m_Appear; }

private:

	void AutoFade();			// 自動遷移処理

	// シングルトン
	static CResult* m_pResult;

	bool m_Appear;

	CObject2D* m_pBg;			// 背景
	CObject2D* m_pScoreTex;		// スコアテキスト
	CObject2D* m_p1PTex;		// [1P]テキスト
	CObject2D* m_p2PTex;		// [2P]テキスト

	CNumber* m_apScore[6];		// 今回のスコア(数字)
	CNumber* m_ap1P[6];			// 1Pの今回のスコア(数字)
	CNumber* m_ap2P[6];			// 2Pの今回のスコア(数字)
	CRanking* m_pLifeRanking;	// ランキング

	int m_AddScoreCount;
	int m_ScoreData;
	int m_1PData;
	int m_2PData;
	int m_RankData;

	float m_fTimer; // 自動遷移タイマー
};
#endif