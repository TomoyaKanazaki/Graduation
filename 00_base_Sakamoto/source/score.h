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
class CNumber;

//スコアクラス
class CScore : public CObject
{
public:
	CScore(int nPriority = 5);
	~CScore();

	static CScore* Create();
	void SetScore(int nScore) { m_nScore = nScore; }
	int GetScore(void) { return m_nScore; }
	void SetPos(D3DXVECTOR3 pos);
	void AddScore(int nScore) { m_nScore += nScore; }

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNULL(void);

private:
	CNumber* m_apObject[NUM_SCORE];
	D3DXVECTOR3 m_pos;
	int m_nCount;
	int m_nScore;
	int m_nNumber[NUM_SCORE];
};

#endif