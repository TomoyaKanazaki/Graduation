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
		STATE_START = 0,	//�Q�[���J�n
		STATE_CHANGE,		//�}�b�v�J��
		STATE_END,			//�Q�[���I��
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