//============================================
//
//	��ʂ𕢂�2D [screenCover.h]
//	Author : Takeru Ogasawara
//
//============================================
#ifndef _SCREENCOVER2D_H_
#define _SCREENCOVER2D_H_

#include "main.h"
#include "object2D.h"

//�I�u�W�F�N�g2D�N���X
class CScreenCover2D : public CObject2D
{
private:

	//�t�F�[�h�̏��
	enum FADE
	{
		FADE_NONE = 0,	//�������Ă��Ȃ����
		FADE_IN,		//�t�F�[�h�C�����
		FADE_OUT,		//�t�F�[�h�C�����
		FADE_MAX
	};

public:
	CScreenCover2D(int nPriority);
	~CScreenCover2D();

	static CScreenCover2D* Create(int nPriority = 7);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void Fade(float fSpeed, int nCount);
	void SetConst(int nCount);

private:
	FADE m_fade;
	int m_nCount;
};

#endif