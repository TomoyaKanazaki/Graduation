//========================================
//
// �E�H�[��[wall.h]
// Author�F�X��x��
//
//========================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "object3D.h"
#include "object2D.h"

//========================================
// �E�H�[���N���X
//========================================
class CWall : public CObject3D
{
public:
	CWall(int nPriority = 0);		//�R���X�g���N�^
	~CWall();	//�f�X�g���N�^

	//�����o�֐�
	static CWall* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetVertex3D(void) override;

private:
	//�����o�ϐ�
	float m_aTexU;			//�e�N�X�`����U�l

	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���̃|�C���^
};

#endif
