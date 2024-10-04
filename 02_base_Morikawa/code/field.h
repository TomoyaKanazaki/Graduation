//========================================
//
// �t�B�[���h[field.h]
// Author�F�X��x��
//
//========================================
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
#include "object3D.h"

//========================================
//�|���S���N���X
//========================================
class CField : public CObject3D
{
public:
	CField(int nPriority = 0);		//�R���X�g���N�^
	~CField();	//�f�X�g���N�^

	//�����o�֐�
	static CField* Create(void);	//�|���S������

	HRESULT Init(void);		//������
	void Uninit(void);		//�I��
	void Update(void);		//�X�V
	void Draw(void);		//�`��

	virtual void SetVertex3D() override;
private:
	//�����o�ϐ�
	static 	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
};

#endif
