//============================================
//
//	チュートリアルUI [tutorialUi.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _TUTORIALUI_H_
#define _TUTORIALUI_H_

#include "object2D.h"

class CTutorialUi : CObject2D
{
public:
	CTutorialUi(int nPriority = 6);
	~CTutorialUi();

	static CTutorialUi* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const char* pFilename);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif
