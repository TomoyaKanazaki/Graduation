//==============================================================
//
//�^�C�g������[title.h]
//Author:����������
//
//==============================================================
#ifndef _TITLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TITLE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "manager.h"

#define NUM_TITLE_TEX		(2)		//�^�C�g���̃e�N�X�`���̐�

//�O���錾
class CModel;			//���f���̏��
class CWall;			//�ǂ̏��
class CField;			//���̏��
class CEdit;			//�G�f�B�b�g�̏��

//�^�C�g����ʃN���X�̒�`
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static CModel *m_pModel;		//���f���̏��
	static CWall *m_pWall;			//�ǂ̏��
	static CField *m_pField;		//���̏��
	static CEdit *m_pEdit;			//�G�f�B�b�g�̏��

	static bool bReset;				//���Z�b�g�������ǂ���
	int m_nIdxTexture[NUM_TITLE_TEX];			//�e�N�X�`���ԍ�
	int m_nCntTrans;				//�J�ڂ���܂ł̎���
};

#endif

