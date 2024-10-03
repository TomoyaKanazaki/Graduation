//============================================
//
//	リザルト画面 [result.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "manager.h"

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

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static void SetApprear(bool Set) { m_Appear = Set; }
	static bool GetApprear(void) { return m_Appear; }

private:
	
	static bool m_Appear;

	CBreak_Block3D *m_pBreakBlock3D;
	CObject2D *m_pBg;
	CRanking* m_pLifeRanking;
	CObject2D *m_ClearText;
	CObject2D *m_pScoreTex;
	CNumber* m_apLife[6];
	int m_AddScoreCount;
	int m_LifeData;
	int m_RankData;
};
#endif