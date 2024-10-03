//==============================================================
//
//���f���̊K�w�\������[modelHierarchy.h]
//Author:����������
//
//==============================================================
#ifndef _MODELHIERARCHY_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MODELHIERARCHY_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "objectX.h"

//���f���̊K�w�\���N���X�̒�`
class CModelHier
{
public:
	CModelHier();		//�R���X�g���N�^
	CModelHier(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CModelHier();		//�f�X�g���N�^

	HRESULT Load(char *pFileName);		//�e�N�X�`���̓ǂݍ���
	static CModelHier *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pFileName);		//��������

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParent(CModelHier *pModelHier) { m_pParent = pModelHier; }		//�e���f���̐ݒ�
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }	//���[���h�}�g���b�N�X�擾
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }					//�ʒu�ݒ�
	void SetDefaultPosition(D3DXVECTOR3 pos) { m_posDefault = pos; }	//�����ʒu�ݒ�
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }					//�����ݒ�
	void SetDefaultRotation(D3DXVECTOR3 rot) { m_rotDefault = rot; }	//���������ݒ�
	void SetColor(D3DXCOLOR col);										//�F�ݒ�
	void SetDisp(void) { m_bDisp = m_bDisp ? false : true; }			//�\���ݒ�
	void SetState(CObjectX::STATE state) { m_state = state; }			//�G�̏�Ԑݒ�

	D3DXVECTOR3 GetPosition(void) { return m_pos; }						//�ʒu�擾
	D3DXVECTOR3 GetDefaultPosition(void) { return m_posDefault; }		//�����ʒu�擾
	D3DXVECTOR3 GetRotation(void) { return m_rot; }						//�����擾
	D3DXVECTOR3 GetDefaultRotation(void) { return m_rotDefault; }		//���������擾
	D3DXVECTOR3 GetSizeMax(void) { return m_vtxMax; }			//���f���̍ő�̑傫���擾
	D3DXVECTOR3 GetSizeMin(void) { return m_vtxMin; }			//���f���̍ŏ��̑傫���擾
	CObjectX::STATE GetState(void) { return m_state; }			//�G�̏�Ԏ擾

private:
	void SetModel(void);		//���f���̐ݒ菈��

	LPD3DXMESH m_pMesh;					//���b�V���i���_���j�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;			//�}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;					//�}�e���A���̐�

	D3DXVECTOR3 m_vtxMin;				//���f���̍ŏ��l
	D3DXVECTOR3 m_vtxMax;				//���f���̍ő�l

	int m_nIdxModel;		//���f���̔ԍ�
	int m_nAppear;			//�_�ŃJ�E���g

	bool m_bDisp;			//�\�����邩���Ȃ���

	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_rot;				//����
	D3DXVECTOR3 m_posDefault;		//�����ʒu
	D3DXVECTOR3 m_rotDefault;		//��������
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X

	CObjectX::STATE m_state;		//���
	D3DXMATERIAL m_matColor;		//�}�e���A���f�[�^�ւ̃|�C���^

	CModelHier *m_pParent;		//�e���f���ւ̃|�C���^
};

#endif
