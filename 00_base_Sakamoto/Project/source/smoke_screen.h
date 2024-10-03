//============================================
//
//	2D�������� [smoke_screen.h]
//	Author:Kai Takada
//
//============================================
#ifndef _SMOKE_SCREEN_H_
#define _SMOKE_SCREEN_H_

#include "main.h"
#include "object2D.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CSmokeScreen : public CObject2D
{
public:

	CSmokeScreen(int nPriority = 4);
	~CSmokeScreen();

	static CSmokeScreen* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetLife(float Life) { m_nLife = Life; m_nLifeMax = Life; }
	float GetLife(void) { return m_nLife; }

private:
	float m_nLifeMax;							//�����̍ő�l
	float m_nLife;							//����
};
#endif