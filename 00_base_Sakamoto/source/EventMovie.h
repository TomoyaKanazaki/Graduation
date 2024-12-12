//============================================
//
//	演出用のムービー[EventMovie.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _EVENTMOVIE_H_
#define _EVENTMOVIE_H_

//前方宣言
class CObject2D;

//レベルアップクラス
class CEventMovie
{
public:
	CEventMovie();
	~CEventMovie();

	enum STATE
	{
		STATE_START = 0,	//ゲーム開始
		STATE_CHANGE,		//マップ遷移
		STATE_END,			//ゲーム終了
		STATE_MAX
	};

	static CEventMovie* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetEventType(STATE Event) { m_State = Event; }
private:

	void StartMovie(void);
	void StageChangeMovie(void);
	void EndMovie(void);

	void ShootUpCross(void);
	void SetPosYCross(float PosY);
	void SetPosCross(D3DXVECTOR3 Pos);

	STATE m_State;
	int m_nWave;
	int m_nCount;
	float m_fSinFloat;
};
#endif