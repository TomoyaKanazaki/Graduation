//============================================
//
//	�^�C�g����� [title.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "Scene.h"

//�}�N����`
#define MAX_SELECT (3)
#define MAX_TITLEUI (3)

// �O���錾
class CObject2D;
class CObjmeshDome;
class CPlayer;
class CCubeBlock;
class CCursorUI;

// �^�C�g���N���X
class CTitle : public CScene
{
private:
	static const int SLASH_2D_NUM = 4;

	// 2D�̏��
	struct SInfo2D
	{
		D3DXVECTOR3 pos;	// �ʒu���
		D3DXVECTOR2 size;	// �傫�����
		D3DXCOLOR color;	// �F���

		CObject2D* pObj2d;	// 2D�I�u�W�F�N�g
	};

public:
	CTitle();
	~CTitle();

	static CTitle* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	void Select(void);				// �I��
	void Button(void);				// �{�^��

	// �V���O���g��
	static CTitle* m_pTitle;

	float m_fCountFade;

	CObject2D* m_pLogo;						//�^�C�g�����S�̃|���S��
	CObject2D* m_pModeSelect[MAX_SELECT];	//���[�h�Z���N�g
	CObject2D* m_pTitleButton;				//�{�^������UI�̃|���S��
	CObjmeshDome* m_pMeshDome;				// ���b�V���h�[��
	
	int m_nSelect;
	bool m_bStart;
	bool m_bSlash;
	bool m_bTurn;
};
#endif