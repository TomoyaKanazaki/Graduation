//==============================================================
//
//�A�C�e������[item.h]
//Author:����������
//
//==============================================================
#ifndef _ITEMMODEL_H_		//���̃}�N����`������Ă��Ȃ�������
#define _ITEMMODEL_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"objectX.h"

//���f���z�u�N���X�̒�`
class CItem : CObjectX
{
public:

	//�A�C�e���̎��
	enum TYPEITEM
	{
		TYPEITEM_NONE = 0,
		TYPEITEM_RHOMBUS,		//�Ђ��`
		TYPEITEM_STAR,			//��
		TYPE_MAX
	};

	CItem();		//�R���X�g���N�^
	CItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPEITEM type);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CItem();		//�f�X�g���N�^

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPEITEM type);		//��������

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(void);

	int GetItemType(void) { return m_type; }

private:

	static int m_nNumAll;			//���f���̎��

	static LPD3DXMESH m_pMesh;			//���b�V���i���_���j�ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMat;		//�}�e���A���ւ̃|�C���^
	static DWORD m_dwNumMat;			//�}�e���A���̐�
	static const char *m_pFilename[TYPE_MAX];		//�t�@�C����

	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//����
	int m_nMaxModel;			//���f���̐ݒu��
	int m_nIdx;					//���f���̔ԍ�
	int m_nRespawnCounter;		//���X�|�[���J�E���^�[

	int m_type;			//�A�C�e���̎��

	bool m_bRespawn;	//���X�|�[�����邩
};

#endif