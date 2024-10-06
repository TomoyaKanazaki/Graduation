//============================================
//
//	スコアの処理 [score.h]
//	Author:hashimoto kenta
//
//============================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "object.h"

//マクロ定義
#define NUM_SCORE (6)		//スコアの桁数

//前方宣言
class CObject;
class CNumber;

//スコアクラス
class CScore : public CObject
{
public:
	CScore(int nPriority = 5);
	~CScore();

	static CScore* Create();
	static void SetScore(int nScore) { m_nScore = nScore; }
	static int GetScore(void) { return m_nScore; }
	static void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	static void AddScore(int nScore) { m_nScore += nScore; }

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNULL(void);

private:
	static CNumber* m_apObject[NUM_SCORE];
	static D3DXVECTOR3 m_pos;
	static int m_nCount;
	static int m_nScore;
	static int m_nNumber[NUM_SCORE];
};

#endif