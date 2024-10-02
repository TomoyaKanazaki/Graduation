//============================================
//
//	���S��� [logo.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _logo_H_
#define _logo_H_

#include "main.h"
#include "manager.h"

//�}�N����`
#define MAX_SELECT (3)
#define MAX_TITLEUI (3)

//�O���錾
class CObject2D;

// ���S�N���X
class CLogo : public CScene
{
public:
	CLogo();
	~CLogo();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	bool UIFade(void);

	static CObject2D* m_pBg;
	static CObject2D* m_pLogo;
	static int m_nCounter;
	static bool m_bStart;
};
#endif