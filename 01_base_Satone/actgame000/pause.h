//==============================================================
//
//�|�[�Y����[pause.h]
//Author:����������
//
//==============================================================
#ifndef _PAUSE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PAUSE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�O���錾
class CObject2D;		//�I�u�W�F�N�g2D

//�|�[�Y�N���X
class CPause : public CObject
{
public:

	//�|�[�Y���j���[
	enum PAUSE
	{
		PAUSE_CONTINUE = 0,		//�Q�[���ɖ߂�
		PAUSE_RETRY,			//�Q�[������蒼��
		PAUSE_QUIT,				//�^�C�g����ʂɖ߂�
		PAUSE_MAX
	};

	CPause();	//�R���X�g���N�^
	~CPause();	//�f�X�g���N�^

	static CPause *Create(void);		//��������

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

private:
	static CObject2D *m_apObject2D[PAUSE_MAX];		//�I�u�W�F�N�g2D�̃|�C���^
	static LPDIRECT3DTEXTURE9 m_pTexture[PAUSE_MAX];	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_pos;		//�ʒu

	int m_nIdxTexture[PAUSE_MAX];		//�e�N�X�`���̔ԍ�
	PAUSE m_pause;						//�|�[�Y���j���[
	int m_nSelect;						//�I����
	bool m_bPad;						//�p�b�h�̃X�e�B�b�N�|���Ă邩
};

#endif