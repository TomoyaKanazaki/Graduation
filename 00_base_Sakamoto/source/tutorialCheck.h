//============================================
//
//	チュートリアルチェック [tutorialCheck.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _TUTORIALCHECK_H_
#define _TUTORIALCHECK_H_

#include "object2D.h"

class CTutorialCheck : CObject2D
{
public:
	CTutorialCheck(int nPriority = 6);
	~CTutorialCheck();

	static CTutorialCheck* Create(const D3DXVECTOR3& pos);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif
