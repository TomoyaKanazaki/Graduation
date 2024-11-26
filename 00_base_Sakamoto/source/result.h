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

	// シングルトン
	static CResult* m_pResult;

	bool m_Appear;

	CObject2D* m_pBg;			// 背景
	CObject2D* m_pScoreTex;		// 

	CRanking* m_pLifeRanking;	// 
	CNumber* m_apLife[6];		// 

	int m_AddScoreCount;
	int m_LifeData;
	int m_RankData;
};
#endif