//========================================
//
//�V���[�e�B���O�A�N�V����[number.h]
//Author�F�X��x��
//
//========================================
#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "main.h"
#include "object2D.h"

//========================================
//�i���o�[�N���X
//========================================
class CNumber : public CObject2D
{
public:
	CNumber();		//�R���X�g���N�^
	~CNumber();		//�f�X�g���N�^

	HRESULT Init(void);		//������
	void Uninit(void);		//�I��
	void Update(void);		//�X�V
	void Draw(void);		//�`��

	void SetNumber(const int nNum) { m_nNumber = nNum; } // ���l�̐ݒ�

	//�����o�֐�
	static CNumber *Create(void);	//�v���C���[����

private:

	// �����o�֐�
	void CalcUV(); // �e�N�X�`�����W�̌v�Z

	// �����o�ϐ�
	int m_nNumber; // ���g�̐���
};

#endif