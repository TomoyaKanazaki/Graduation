//==============================================================
//
//�v���C���[�̃e�X�g����[tutorial_texture.h]
//Author:����������
//
//==============================================================
#ifndef _TEST_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TEST_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object3D.h"

class CTutorialUI;
class CTestLine;

//�`���[�g���A��UI�N���X�̒�`
class CTest : public CObject3D
{
public:

	//�`���[�g���A���̃X�e�b�v
	enum STEP
	{
		STEP_MOVE = 0,		//�ړ�
		STEP_JUMP,			//�W�����v
		STEP_MAX
	};

	CTest();		//�R���X�g���N�^
	CTest(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CTest();		//�f�X�g���N�^

	static CTest *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//��������

	HRESULT Init(void);
	void UnInit(void);
	void Update(void);
	void Draw(void);

private:

	void MoveKeyboard(void);	//�v���C���[�L�[�{�[�h�̈ړ�����
	void ScreenLimit(void);		// ��ʊO����
	void CircleColllllll(void);

	static LPDIRECT3DTEXTURE9 m_pTexture;
	static const char *m_apFilename[STEP_MAX];		//�e�N�X�`���̖��O
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_posOld;	//�O��̈ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_distance;		//�ړ���

	int m_nType;			//�`���[�g���A���̎��

	float m_fDistance;
	float m_fScale;			// �����̔{��

	CTutorialUI* m_pTest2;
	CTestLine* m_pTestLine;
	CObject3D* m_pCircle;
};

#endif
