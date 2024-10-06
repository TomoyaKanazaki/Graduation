#pragma once
//==============================================================
//
//�v���C���[�̃e�X�g����[tutorial_texture.h]
//Author:����������
//
//==============================================================

#include "object3D.h"

class CTutorialUI;

//�`���[�g���A��UI�N���X�̒�`
class CTestLine : public CObject3D
{
public:

	//�`���[�g���A���̃X�e�b�v
	enum STEP
	{
		STEP_MOVE = 0,		//�ړ�
		STEP_JUMP,			//�W�����v
		STEP_MAX
	};

	CTestLine();		//�R���X�g���N�^
	CTestLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CTestLine();		//�f�X�g���N�^

	static CTestLine* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//��������

	HRESULT Init(void);
	void UnInit(void);
	void Update(void);
	void Draw(void);

	void SetLinePos(const D3DXVECTOR3& pos) { m_pos = pos; CObject3D::SetPosition(m_pos); }

private:

	static LPDIRECT3DTEXTURE9 m_pTexture;
	static const char* m_apFilename[STEP_MAX];		//�e�N�X�`���̖��O
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_posOld;	//�O��̈ʒu

};
