//==============================================================
//
//�}�e���A���̏���[texture.h]
//Author:����������
//
//==============================================================
#ifndef _MATERIAL_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MATERIAL_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_TEX		(128)		//�}�e���A���̍ő吔

//�}�e���A���N���X�̒�`
class CMaterial
{
public:
	//�}�e���A���̎��
	enum MAT
	{
		PARTS_BODY = 0,		//[0]��
		PARTS_HEAD,			//[1]��
		PARTS_HAIR,			//[2]��
		PARTS_LU_ARM,		//[3]���r��
		PARTS_LD_ARM,		//[4]���r��
		PARTS_L_HAND,		//[5]����
		PARTS_RU_ARM,		//[6]�E�r��
		PARTS_RD_ARM,		//[7]�E�r��
		PARTS_R_HAND,		//[8]�E��
		PARTS_WAIST,		//[9]��
		PARTS_LU_LEG,		//[10]��������
		PARTS_LD_LEG,		//[11]���ӂ���͂�
		PARTS_L_SHOE,		//[12]���C
		PARTS_RU_LEG,		//[13]�E������
		PARTS_RD_LEG,		//[14]�E�ӂ���͂�
		PARTS_R_SHOE,		//[15]�E�C
		MAT_MAX
	};

	//���f���̍\���̂̒�`
	struct Material
	{
		LPD3DXMESH pMesh;					//���b�V���i���_���j�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;				//�}�e���A���ւ̃|�C���^
		DWORD dwNumMat;						//�}�e���A���̐�
		LPDIRECT3DTEXTURE9 *pTexture;
	};

	CMaterial();		//�R���X�g���N�^
	~CMaterial();	//�f�X�g���N�^

	HRESULT Load(void);		//�e�N�X�`���ǂݍ��ݏ���
	void Unload(void);		//�e�N�X�`���j������

	int Regist(const char *pFilename);							//�e�N�X�`���̔ԍ��w��
	LPDIRECT3DTEXTURE9 GetTexAddress(int nIdx, int nMat);		//�e�N�X�`���̏��擾
	LPD3DXBUFFER GetMatAddress(int nIdx);		//�}�e���A���̏��擾
	DWORD GetNumMatAddress(int nIdx);			//�}�e���A���̐��̏��擾
	LPD3DXMESH GetMeshAddress(int nIdx);		//���b�V���̏��擾

private:
	static int m_nNumAll;		//�e�N�X�`���̐�
	static const char *m_apFilename[MAX_TEX];		//���f���̃e�L�X�g�t�@�C����
	static Material m_Material[MAX_TEX];			//�}�e���A���̏��
};

#endif
