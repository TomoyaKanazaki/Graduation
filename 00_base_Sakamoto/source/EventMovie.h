//============================================
//
//	���o�p�̃��[�r�[[EventMovie.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _EVENTMOVIE_H_
#define _EVENTMOVIE_H_

//�O���錾
class CObject2D;

//���x���A�b�v�N���X
class CEventMovie
{
public:
	CEventMovie();
	~CEventMovie();

	enum STATE
	{
		STATE_START = 0,
		STATE_END,
		STATE_MAX
	};

	static CEventMovie* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:

	void StartMovie(void);

	STATE m_State;
	int m_nWave;
	int m_nCount;
};
#endif