//============================================
//
//	�^�C�g����� [title.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "manager.h"

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

	// ���
	struct SInfoSlash
	{
		D3DXVECTOR3 pos;		// �ʒu���
		D3DXCOLOR color;		// �F���

		D3DXVECTOR3 angle;		// ����

		D3DXVECTOR2 sizeDef;	// �f�t�H���g�T�C�Y

		int nCnt;				// �J�E���g
		bool AllAppear;			// �S�\���̗L��

		SInfo2D aInfo2D[SLASH_2D_NUM];	// 2D�̏��

		bool bStart;
	};

public:
	CTitle();
	~CTitle();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	void Select(void);				// �I��
	void Button(void);				// �{�^��

	float m_fCountFade;

	static CObject2D* m_pLogo;				//�^�C�g�����S�̃|���S��
	static CObject2D* m_pTitleButton;		//�{�^������UI�̃|���S��
	static CObjmeshDome* m_pMeshDome;		// ���b�V���h�[��

	SInfoSlash m_pInfoSlash;
	
	bool m_bStart;
	bool m_bSlash;
	bool m_bTurn;
};
#endif