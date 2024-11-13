//============================================
//
//	タイムの処理 [timer.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "object.h"

//マクロ定義
#define NUM_TIME (6)		//スコアの桁数

//前方宣言
class CObject;
class CNumber;

//多重背景クラス
class CTimer : public CObject
{
public:
	CTimer(int nPriority = 5);
	~CTimer();

	static CTimer* Create();
	static void SetTime(int Time) { m_nTime = Time; }
	static int GetTime(void) { return m_nTime; }
	static void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	static void AddTime(int Time) { m_nTime += Time; }
	static int GetTimeNumber(void);
	void FloatSetTime(int Time);
	void SetStartTime(int Time);
	void SetStopTime(bool Stop) { m_StopTime = Stop; }

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNULL(void);

private:
	static CNumber* m_apObject[NUM_TIME];
	static D3DXVECTOR3 m_pos;
	static int m_nCount;
	static int m_StartTime;
	static int m_nTime;
	static int m_nNumber[NUM_TIME];
	static bool m_StopTime;
};

#endif