//============================================
//
//	�Q�[���p�|�[�Y���� [GamePause.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _GAMEPAUSE_H_
#define _GAMEPAUSE_H_

#include "main.h"
#include "objectAnim2d.h"
//�}�N����`
#define MAX_GAMEPAUSE (1)
#define MAX_GAMEFG (1)

//�O���錾
class CObject2D;

//���x���A�b�v�N���X
class CGamePause
{
public:
	CGamePause();
	~CGamePause();

	static CGamePause* Create();

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
	
	static CObject2D* m_pPauseFG[MAX_GAMEFG];

	CObjectAnim2D* pNoise;
	float m_MoveRot;
	D3DXVECTOR3 m_MarkRot;
	D3DXVECTOR3 m_InitPos[3];
};
#endif