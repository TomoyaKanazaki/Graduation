//========================================
//
//�V���[�e�B���O�A�N�V����[title.h]
//Author�F�X��x��
//
//========================================
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"
#include "manager.h"
#include "object2D.h"
#include "renderer.h"

//========================================
//�^�C�g���N���X
//========================================
class CTitle : public CScene
{
public:
	CTitle(int nPriority = 3);	//�R���X�g���N�^
	~CTitle();	//�f�X�g���N�^

	//�����o�֐�
	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CObject2D* m_pObj2D;
	CObject2D* m_pTeam = nullptr;
	CObject2D* m_pPressEnter = nullptr;
	float m_fMoveTime;
};

#endif
