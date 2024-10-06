//============================================
//
//	�|�[�Y���� [Pause.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//�}�N����`
#define MAX_PAUSE (3)
#define MAX_FG (4)

//�O���錾
class CObject2D;

//���x���A�b�v�N���X
class CPause
{
public:
	CPause();
	~CPause();

	static CPause *Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetAppear(bool Set) { m_Appear = Set; m_PauseSelect = 0; }
	bool SetAppear(void) { return m_Appear; }

private:
	int m_PauseSelect;
	bool m_Appear;
	static bool m_bColor;	// �F�̕ύX�����邩�ǂ���
	static CObject2D *m_pPauseUI[MAX_PAUSE];
	static CObject2D *m_pPauseFG[MAX_FG];
	static CObject2D *m_pPauseMark;

	float m_MoveRot;
	D3DXVECTOR3 m_MarkRot;
	D3DXVECTOR3 m_InitPos[3];
};
#endif