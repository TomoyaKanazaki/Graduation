//============================================
//
//	���f���̏��� [model.cpp]
//	Author:sakamoto kai
//
//============================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//====================================================================
//�R���X�g���N�^
//====================================================================
CModel::CModel()
{
	m_pMesh = NULL;								//���b�V��(���_���)�ւ̃|�C���^
	m_pBuffMat = NULL;							//�}�e���A���ւ̃|�C���^
	m_dwNumMat = 0;								//�}�e���A���̐�
	m_pTexture = nullptr;

	m_Startpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_Startrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_Scaling = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�g�嗦
	m_pParent = NULL;							//�e���f���ւ̃|�C���^

	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ColorType = COLORTYPE_FALSE;
	m_bDisp = true;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CModel::~CModel()
{

}

//====================================================================
//��������
//====================================================================
CModel *CModel::Create(const char *pModelName)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{
		//���f���̐���
		pModel = new CModel();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pModel->Init(pModelName)))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pModel;
}

//====================================================================
//����������
//====================================================================
HRESULT CModel::Init(const char *pModelName)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pModelName,
		D3DXMESH_SYSTEMMEM,
		m_pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	strcpy(&m_acModelName[0], pModelName);

	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	//�e�N�X�`���̃��������m��
	if (m_pTexture == NULL)
	{
		//���������m�ۂ���
		m_pTexture = new LPDIRECT3DTEXTURE9[m_dwNumMat];
	}

	//�}�e���A�����ɑ΂���|�C���^������
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(m_pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTexture[nCntMat]);
		}
		else
		{
			m_pTexture[nCntMat] = NULL;
		}
	}

	//�ʒu�̐ݒ�

	//�����̐ݒ�


	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CModel::Uninit(void)
{
	//�o�b�t�@�̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//���b�V���̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (m_pTexture[nCntMat] != NULL)
		{
			m_pTexture[nCntMat]->Release();
		}
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		//���������m�ۂ���
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CModel::Update(void)
{

}

//====================================================================
//�`�揈��
//====================================================================
void CModel::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&m_mtxWorld, m_Scaling.x, m_Scaling.y, m_Scaling.z);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX mtxParent;

	if (m_pParent != NULL)
	{//�e�����݂���
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//�e�����݂��Ȃ�
		//����(�ŐV)�̃}�g���b�N�X���擾����[=�v���C���[�̃}�g���b�N�X]
		m_pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�Z�o�����}�g���N�X���������킹��
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	m_WorldPos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

	//���݂̃}�e���A��������
	m_pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^����������
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		D3DCOLORVALUE color;

		switch (m_ColorType)
		{
		case CModel::COLORTYPE_FALSE:

			//�}�e���A���̐ݒ�
			m_pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			m_pDevice->SetTexture(0, m_pTexture[nCntMat]);

			break;
			
		case CModel::COLORTYPE_TRUE_ALL:

			color = pMat[nCntMat].MatD3D.Diffuse;
			pMat[nCntMat].MatD3D.Diffuse = m_Color;

			//�}�e���A���̐ݒ�
			m_pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D.Diffuse = color;

			break;

		case CModel::COLORTYPE_TRUE_A:

			color = pMat[nCntMat].MatD3D.Diffuse;
			pMat[nCntMat].MatD3D.Diffuse.a = m_Color.a;

			//�}�e���A���̐ݒ�
			m_pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D.Diffuse = color;

			//�e�N�X�`���̐ݒ�
			m_pDevice->SetTexture(0, m_pTexture[nCntMat]);

			break;
		}

		if (m_bDisp == true)
		{
			//���f��(�p�[�c)�̕`��
			m_pMesh->DrawSubset(nCntMat);
		}
	}
	//�ۑ����Ă����}�e���A����߂�
	m_pDevice->SetMaterial(&matDef);
}