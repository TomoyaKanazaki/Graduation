//=============================================================================
//
//�X�R�A���� [score.h]
//Author:����������
//
//=============================================================================
#ifndef _SCORE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SCORE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//�}�N����`
#define NUM_DIGIT		(8)			//�X�R�A�̌���

class CNumber;		//����

//�X�R�A�N���X
class CScore : public CObject
{
public:
	CScore();	//�R���X�g���N�^
	~CScore();	//�f�X�g���N�^

	static HRESULT Load(void);		//�e�N�X�`���̓ǂݍ���
	static void Unload(void);		//�e�N�X�`���̔j��
	static CScore *Create(void);	//��������

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);		//�O������e�N�X�`���w��

	static void Set(int nNum);					//�X�R�A�ݒ�
	int Get(void) { return m_nNum; }			//�X�R�A�擾
	static void Add(int nValue);				//�X�R�A�����Đݒ�

private:
	int Calculation(void);		//�X�R�A�v�Z����

	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
	int m_nIdxTex;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;			//�X�R�A�̈ʒu
	static int m_nNum;			//�X�R�A�̒l
	static int m_aTexU[NUM_DIGIT];

	static CNumber *m_apNumber[NUM_DIGIT];
};

#endif