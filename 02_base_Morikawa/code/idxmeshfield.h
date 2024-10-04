//=======================================
//
//���b�V���t�B�[���h����[meshfield.h]
//Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#ifndef _IDXMESHFIELD_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _IDXMESHFIELD_H_	 //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "object3D.h"

//=======================================
//���b�V���t�B�[���h�N���X
//=======================================
class CIdxMeshField : public CObject3D
{
public:
	CIdxMeshField();	//�R���X�g���N�^
	~CIdxMeshField();	//�f�X�g���N�^

	//�����o�֐�
	static HRESULT Load(void);
	static void Unload(void);
	static CIdxMeshField *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;					//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshField;		 //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//����
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
};

#endif