//=======================================
//
//�t�F�[�h[fade.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _FADE_H_	 //���̃}�N����`����`����Ă��Ȃ�������
#define _FADE_H_	 //2�A�C���N���[�h�h�~�̃}�N�����`����
#include "main.h"
#include "object.h"
#include "object2d.h"
#include "manager.h"

//=======================================
//�t�F�[�h�N���X
//=======================================
class CFade
{
public:
	enum FADE
	{//�t�F�[�h�̏��
		FADE_NONE = 0,	//�������ĂȂ����
		FADE_IN,		//�t�F�[�h�C��
		FADE_OUT,		//�t�F�[�h�A�E�g
		FADE_MAX
	};

	CFade();	//�R���X�g���N�^
	~CFade();	//�f�X�g���N�^

	HRESULT Init(CScene::MODE modenext);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CScene::MODE modenext);
	void SetState(FADE state);
	static CFade *Create(CScene::MODE modenext);

private:
	//�����o�ϐ�
	bool m_bTrans;				// �J�ڂ��邩�ǂ���
	FADE m_State;				//�t�F�[�h�̏��
	CScene::MODE m_modeNext;	//���̉��(���[�h)
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	CObject2D *m_pObject2D;		// �I�u�W�F�N�g�̃|�C���^
};

#endif
