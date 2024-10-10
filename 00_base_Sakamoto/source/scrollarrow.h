//============================================
//
//	スクロールアロー [scrollarrow.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SCROLLARROW_H_
#define _SCROLLARROW_H_

#include "main.h"
#include "object2D.h"

//オブジェクトプレイヤークラス
class CScrollArrow : public CObject2D
{
public:

	CScrollArrow(int nPriority = 6);
	~CScrollArrow();

	//プレイヤーの状態
	enum STATE
	{
		STATE_UP = 0,	//↑
		STATE_DOWN,		//↓
		STATE_LEFT,		//←
		STATE_RIGHT,	//→
		STATE_MAX,		//最大
	};

	static CScrollArrow* Create(int nPriority = 6);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理・設定処理
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

private:
	int m_nSample;							//サンプル変数
	STATE m_State;				//状態
};
#endif