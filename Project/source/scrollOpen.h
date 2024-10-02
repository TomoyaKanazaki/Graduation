//============================================
//
//	巻物処理 [makimono.h]
//	Author : Takeru Ogasawara
//
//============================================

#ifndef _MAKIMONO_H_
#define _MAKIMONO_H_

// インクルード宣言
#include "object.h"

// 前方宣言
class CObject2D;
class CObjGauge2D;

// 巻物クラス
class CScrollOpen : public CObject
{
public:

	enum MODE
	{
		MODE_NONE = 0,
		MODE_OPEN,
		MODE_STOP,
		MODE_CLOSE,
		MODE_FADE,
		MODE_END,
		MODE_MAX
	};

public:
	CScrollOpen();
	~CScrollOpen();

	static CScrollOpen* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetMode(MODE mode) { m_mode = mode; }
	MODE GetMode() { return m_mode; }

private:

	void InitScroll();
	void InitPaper();
	void Open();
	void Close();
	void Fade();
	void movePaper(D3DXVECTOR3 pos);

	CObject2D* m_pScroll;
	CObjGauge2D* m_pPaper;
	
	MODE m_mode;

	bool m_bSound;
};


#endif