//==============================================================
//
//�ǂ̏���[wall.h]
//Author:����������
//
//==============================================================
#ifndef _WALL_H_		//���̃}�N����`������Ă��Ȃ�������
#define _WALL_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object3D.h"

//�ǃN���X�̒�`
class CWall : public CObject3D
{
public:
	CWall();		//�R���X�g���N�^
	CWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CWall();		//�f�X�g���N�^

	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//��������

	HRESULT Init(void);
	void UnInit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		//�ʒu
	int m_nType;			//�ǂ̎��
};

#endif
