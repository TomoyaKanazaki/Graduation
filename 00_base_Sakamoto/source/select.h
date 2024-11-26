//============================================
//
//	�I����� [select.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "Scene.h"

//�}�N����`
#define NUM_SCROLLTYPE (2)
#define NUM_STAGE (3)
#define MAX_TITLEUI (3)
#define NUM_SCROLL (2)

// �O���錾
class CObject2D;
class CObjmeshDome;
class CPlayer;
class CCubeBlock;
class CCursorUI;

// �^�C�g���N���X
class CSelect : public CScene
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
	CSelect();
	~CSelect();

	static CSelect* GetInstance();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	void StageSelect(void);				// �I��
	void StageButton(void);				// �{�^��
	void ScrollSelect(void);			// �I��
	void ScrollButton(void);			// �{�^��

	// �V���O���g��
	static CSelect* m_pSelect;

	float m_fCountFade;

	CObject2D* m_pStageSelect[NUM_STAGE];		//�X�e�[�W�Z���N�g
	CObject2D* m_pScrollSelect[NUM_SCROLLTYPE];	//�X�N���[���Z���N�g
	CObject2D* m_pTitleButton;					//�{�^������UI�̃|���S��
	CObject2D* m_pTexScroll[NUM_SCROLL];		// �e�N�X�`���X�N���[��
	CObjmeshDome* m_pMeshDome;					// ���b�V���h�[��

	int m_nSelect;
	int m_nStep;
	int m_nSetStage;
	bool m_bStart;
	bool m_bSlash;
	bool m_bTurn;
};
#endif