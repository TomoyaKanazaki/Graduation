//==============================================================
//
//�z�u����[edit.h]
//Author:����������
//
//==============================================================
#ifndef _EDIT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _EDIT_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"objectX.h"

//�}�N����`
#define MAX_DEBUG_MODEL			(1040)		//���f���̐�
#define MAX_STR					(128)		//�ő啶����

//�O���錾
class CObjectX;		//�I�u�W�F�N�gX

//���f���z�u�N���X�̒�`
class CEdit : CObject
{
public:

	//�}�b�v�z�u�̍\����
	struct Edit
	{
		LPD3DXMESH pMesh;			//���b�V���i���_���j�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;		//�}�e���A���ւ̃|�C���^
		DWORD dwNumMat;				//�}�e���A���̐�
		LPDIRECT3DTEXTURE9 *pTexture;	//�e�N�X�`���ւ̃|�C���^
		D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
		D3DXVECTOR3 vtxMin;				//���f���̍ŏ��l
		D3DXVECTOR3 vtxMax;				//���f���̍ő�l
		D3DXVECTOR3 pos;		//�ʒu
		D3DXVECTOR3 rot;		//����
		int nIndex;				//���f���̔ԍ�
		char aFilename[MAX_STR];			//���f���̃t�@�C����
		char aFilecomment[MAX_STR];			//���f���̃t�@�C���R�����g

	};

	//�z�u�������f���̍\����
	struct SetEdit
	{
		D3DXVECTOR3 pos;		//�ʒu
		D3DXVECTOR3 rot;		//����
		int nIndex;				//���f���̔ԍ�

	};

	CEdit();		//�R���X�g���N�^
	~CEdit();		//�f�X�g���N�^

	HRESULT Load(void);		//�e�N�X�`���̓ǂݍ���
	void Unload(void);		//�e�N�X�`���̔j��
	static CEdit *Create(void);		//��������

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSize(void);			//���f���̑傫���ݒ�
	void LoadFile(void);		//�f�[�^�ǂݍ���

private:
	
	static int m_nNumAll;			//���f���̎��
	static CObjectX *m_apObjectX[MAX_DEBUG_MODEL];

	static LPD3DXMESH m_pMesh;			//���b�V���i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMat;		//�}�e���A���ւ̃|�C���^
	static DWORD m_dwNumMat;			//�}�e���A���̐�
	static const char *m_apFilename[MAX_DEBUG_MODEL];		//�t�@�C����

	static int m_nCnt;

	D3DXMATRIX m_mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtxMin;						//���f���̍ŏ��l
	D3DXVECTOR3 m_vtxMax;						//���f���̍ő�l

	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//����
	int m_nNumModel;			//���f���̔ԍ�
	int m_nMaxModel;			//���f���̐ݒu��

	Edit m_aEdit[MAX_DEBUG_MODEL];	//�G�f�B�b�g�̏��
	SetEdit m_aSetEdit[MAX_DEBUG_MODEL];	//�ݒu�������f���̏��
};

#endif