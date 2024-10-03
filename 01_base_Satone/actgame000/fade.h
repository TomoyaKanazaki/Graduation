//==============================================================
//
//�t�F�[�h����[fade.h]
//Author:����������
//
//==============================================================
#ifndef _FADE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _FADE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"
#include "manager.h"

//�t�F�[�h�N���X
class CFade : public CObject2D
{
public:
	//�t�F�[�h�̏��
	enum FADE
	{
		FADE_NONE = 0,		//�������Ă��Ȃ����
		FADE_IN,			//�t�F�[�h�C�����
		FADE_OUT,			//�t�F�[�h�A�E�g���
		FADE_MAX
	};

	CFade();	//�R���X�g���N�^
	~CFade();	//�f�X�g���N�^

	static CFade *Create();		//��������

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��
	void SetFade(CScene::MODE modeNext);		//�t�F�[�h�̐ݒ�
	void SetNormalFade(void);					//�����̃t�F�[�h�̐ݒ�
	FADE GetFade(void) { return m_fade; }		//�t�F�[�h�̎擾

private:
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	bool m_bTrans;			//��ʑJ�ڂ��邩

	FADE m_fade;										//�t�F�[�h�̏��
	CScene::MODE m_modeNext;							//���̉�ʁi���[�h�j
	D3DXCOLOR m_colorFade;								//�|���S���i�t�F�[�h�j�̐F
};

#endif
