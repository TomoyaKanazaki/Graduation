//========================================
//
// �t�B�[���h[sky.h]
// Author�F���n�Ή�
//
//========================================
#ifndef _SKY_H_
#define _SKY_H_
#include "main.h"
#include "object3D.h"

//========================================
//�|���S���N���X
//========================================
class CSky : public CObject3D
{
public:
	CSky(int nPriority = 0);		//�R���X�g���N�^
	~CSky();	//�f�X�g���N�^

	//�����o�֐�
	static CSky* Create(void);	//�|���S������

	HRESULT Init(void);		//������
	void Uninit(void);		//�I��
	void Update(void);		//�X�V
	void Draw(void);		//�`��

	virtual void SetVertex3D() override;
private:
	
	float m_fTexU;
	//�����o�ϐ�
	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
};

#endif
