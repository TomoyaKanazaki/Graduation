//==============================================================
//
//�}�e���A���̏���[texture.cpp]
//Author:����������
//
//==============================================================
#include "material.h"
#include "manager.h"
#include "renderer.h"

//�ÓI�����o�ϐ��錾
int CMaterial::m_nNumAll = -1;										//���f���ԍ�
const char *CMaterial::m_apFilename[MAX_TEX] = 
{
	"data\\MODEL\\enemy\\00_body.x",
	"data\\MODEL\\enemy\\01_head.x",
	"data\\MODEL\\enemy\\02_hair.x",
	"data\\MODEL\\enemy\\03_LU_arm.x",
	"data\\MODEL\\enemy\\04_LD_arm.x",
	"data\\MODEL\\enemy\\05_L_hand.x",
	"data\\MODEL\\enemy\\06_RU_arm.x",
	"data\\MODEL\\enemy\\07_RD_arm.x",
	"data\\MODEL\\enemy\\08_R_arm.x",
	"data\\MODEL\\enemy\\09_waist.x",
	"data\\MODEL\\enemy\\10_LU_leg.x",
	"data\\MODEL\\enemy\\11_LD_leg.x",
	"data\\MODEL\\enemy\\12_L_shoe.x",
	"data\\MODEL\\enemy\\13_RU_leg.x",
	"data\\MODEL\\enemy\\14_RD_leg.x",
	"data\\MODEL\\enemy\\15_R_shoe.x",
};
CMaterial::Material CMaterial::m_Material[MAX_TEX] = {};			//�}�e���A���̏��

//==============================================================
//�R���X�g���N�^
//==============================================================
CMaterial::CMaterial()
{
	//�}�e���A��������
	for (int nCntModel = 0; nCntModel < MAX_TEX; nCntModel++)
	{
		m_Material[nCntModel].pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^
		m_Material[nCntModel].pMesh = NULL;			//���b�V���ւ̃|�C���^
		m_Material[nCntModel].dwNumMat = NULL;		//�}�e���A���̐�
		m_Material[nCntModel].pTexture = NULL;		//�e�N�X�`��
	}
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CMaterial::~CMaterial()
{

}

//==============================================================
//�}�e���A���ǂݍ��ݏ���
//==============================================================
HRESULT CMaterial::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	for (int nCntModel = 0; nCntModel < MAT_MAX; nCntModel++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(m_apFilename[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_Material[nCntModel].pBuffMat,
			NULL,
			&m_Material[nCntModel].dwNumMat,
			&m_Material[nCntModel].pMesh);

		D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^

		m_Material[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[m_Material[nCntModel].dwNumMat];

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)m_Material[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_Material[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`�������݂���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_Material[nCntModel].pTexture[nCntMat]);
			}
			else
			{//�e�N�X�`�������݂��Ȃ�
				m_Material[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		m_nNumAll++;		//���f���̐����Z
	}

	return S_OK;
}

//==============================================================
//�}�e���A���j������
//==============================================================
void CMaterial::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_TEX; nCntModel++)
	{
		//�e�N�X�`���̔j��
		for (int nCntMat = 0; nCntMat < (int)m_Material[nCntModel].dwNumMat; nCntMat++)
		{
			if (m_Material[nCntModel].pTexture != NULL)
			{
				delete m_Material[nCntModel].pTexture;
				m_Material[nCntModel].pTexture = NULL;
			}
		}

		//�}�e���A���̔j��
		if (m_Material[nCntModel].pBuffMat != NULL)
		{
			m_Material[nCntModel].pBuffMat->Release();
			m_Material[nCntModel].pBuffMat = NULL;
		}

		//���b�V���̔j��
		if (m_Material[nCntModel].pMesh != NULL)
		{
			m_Material[nCntModel].pMesh->Release();
			m_Material[nCntModel].pMesh = NULL;
		}
	}
}

//==============================================================
//���f���ԍ��w�菈��
//==============================================================
int CMaterial::Regist(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	for (int nCntModel = 0; nCntModel < MAX_TEX; nCntModel++)
	{
		if (m_apFilename[nCntModel] == pFilename)
		{//�t�@�C�����������Ƃ�

			return nCntModel;		//�ԍ�
		}
		else if (m_apFilename[nCntModel] != pFilename && nCntModel > m_nNumAll)
		{//�t�@�C�������Ⴄ

			m_apFilename[nCntModel] = pFilename;

			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(m_apFilename[nCntModel],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_Material[nCntModel].pBuffMat,
				NULL,
				&m_Material[nCntModel].dwNumMat,
				&m_Material[nCntModel].pMesh);

			D3DXMATERIAL *pMat;		//�}�e���A���ւ̃|�C���^

			m_Material[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[m_Material[nCntModel].dwNumMat];

			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)m_Material[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)m_Material[nCntModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//�e�N�X�`�������݂���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&m_Material[nCntModel].pTexture[nCntMat]);
				}
				else
				{//�e�N�X�`�������݂��Ȃ�
					m_Material[nCntModel].pTexture[nCntMat] = NULL;
				}
			}

			m_nNumAll++;

			return m_nNumAll;		//�}�e���A���̔ԍ���Ԃ�
		}
	}

	return m_nNumAll;		//�}�e���A���̔ԍ���Ԃ�
}

//==============================================================
//�e�N�X�`���̏��擾
//==============================================================
LPDIRECT3DTEXTURE9 CMaterial::GetTexAddress(int nIdx, int nMat)
{
	return m_Material[nIdx].pTexture[nMat];		//�w��̃e�N�X�`����Ԃ�
}

//==============================================================
//�}�e���A���̏��擾
//==============================================================
LPD3DXBUFFER CMaterial::GetMatAddress(int nIdx)
{
	return m_Material[nIdx].pBuffMat;
}

//==============================================================
//�}�e���A���̐��̏��擾
//==============================================================
DWORD CMaterial::GetNumMatAddress(int nIdx)
{
	return m_Material[nIdx].dwNumMat;
}

//==============================================================
//���b�V�����擾
//==============================================================
LPD3DXMESH CMaterial::GetMeshAddress(int nIdx)
{
	return m_Material[nIdx].pMesh;
}