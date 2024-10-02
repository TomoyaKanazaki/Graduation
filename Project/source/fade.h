//========================================================================================
//
// �t�F�[�h����[fade.h]
// Author: ��{�@�ėB
//
//========================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "manager.h"

class CObject2D;

//�t�F�[�h�̃N���X�錾
class CFade
{
public:
	//�t�F�[�h�̏��
	typedef enum
	{
		FADE_NONE = 0,	//�������Ă��Ȃ����
		FADE_IN,		//�t�F�[�h�C�����
		FADE_OUT,		//�t�F�[�h�C�����
		FADE_MAX
	}FADE;

	CFade();
	~CFade();

	void Init(CScene::MODE modeNext);
	void Uninit();
	void Update();
	void Draw(void);
	static void SetFade(CScene::MODE modeNext);
	static void SetFade();
	static FADE GetFade(void) { return m_fade; }
	static void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }
	static D3DXVECTOR3 GetPos(void) { return m_Pos; }
	void Fade_false(void);
	bool Fade_OK(void);

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade = NULL;		//���_�o�b�t�@�ւ̃|�C���^
	static FADE m_fade;
	static CScene::MODE m_modeNext;
	static D3DXCOLOR m_colorFade;
	static CObject2D* m_FadeFG[4];
	static D3DXVECTOR3 m_Pos;
	static float m_FadeFloat;
	bool m_Fade_OK;
	static bool m_bSetMode;
};

#endif
