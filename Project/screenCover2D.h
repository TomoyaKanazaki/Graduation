//============================================
//
//	画面を覆う2D [screenCover.h]
//	Author : Takeru Ogasawara
//
//============================================
#ifndef _SCREENCOVER2D_H_
#define _SCREENCOVER2D_H_

#include "main.h"
#include "object2D.h"

//オブジェクト2Dクラス
class CScreenCover2D : public CObject2D
{
private:

	//フェードの状態
	enum FADE
	{
		FADE_NONE = 0,	//何もしていない状態
		FADE_IN,		//フェードイン状態
		FADE_OUT,		//フェードイン状態
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