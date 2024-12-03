//============================================
//
//	�L�����N�^�[�̊Ǘ� [characterManager.cpp]
//	Author:sakai minato
//
//============================================

#include "characterManager.h"
#include "renderer.h"
#include "manager.h"
#include "Xmodel.h"

//�ÓI�����o�ϐ��錾
int CCharacterManager::m_NumAll = 0;

char CCharacterManager::c_apTexturename[MAX_CHARACTER][128] = {};

//====================================================================
//�R���X�g���N�^
//====================================================================
CCharacterManager::CCharacterManager()
{
	for (int nCntTex = 0; nCntTex < MAX_CHARACTER; nCntTex++)
	{
		m_apTexture[nCntTex] = nullptr;
	}
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CCharacterManager::~CCharacterManager()
{

}

//====================================================================
//�ǂݍ���
//====================================================================
HRESULT CCharacterManager::Load(void)
{
	////�f�o�C�X�̎擾
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	//{
	//	//�e�N�X�`���̓ǂݍ���
	//	if (FAILED(D3DXCreateTextureFromFile(pDevice,
	//		c_apTexturename[nCntTex],
	//		&m_apTexture[nCntTex])))
	//	{
	//		if (c_apTexturename[nCntTex] == nullptr)
	//		{
	//			m_NumAll = nCntTex;
	//			return S_OK;
	//		}
	//		else
	//		{
	//			return E_FAIL;
	//		}
	//	}
	//	else
	//	{
	//		int a = 0;
	//	}
	//}

	return S_OK;
}

//====================================================================
//�j��
//====================================================================
void CCharacterManager::Unload(void)
{
	for (int nCntTex = 0; nCntTex < m_NumAll; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != nullptr)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = nullptr;
		}
	}
}

//====================================================================
//�e�N�X�`���̔ԍ��w��
//====================================================================
int CCharacterManager::Regist(const char* pFilename)
{
	for (int nCntTex = 0; nCntTex < m_NumAll; nCntTex++)
	{
		if (strcmp(&c_apTexturename[nCntTex][0], pFilename) == 0)
		{
			return nCntTex;
		}
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	if (SUCCEEDED(D3DXCreateTextureFromFile(m_pDevice,
		pFilename,
		&m_apTexture[m_NumAll])))
	{
		if (strcmp(&c_apTexturename[m_NumAll][0], "") == 0)
		{
			strcpy(&c_apTexturename[m_NumAll][0], pFilename);
			m_NumAll++;
			return m_NumAll - 1;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

//====================================================================
//�e�N�X�`���̔ԍ��w��
//====================================================================
void CCharacterManager::XModelRegist(int* nTexIdx, int nXModelIdx)
{
	int nCheck = -1;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATERIAL* pMat;	//�}�e���A���ւ̃|�C���^

	//X���f���̎擾
	CXModel::XModel* pXmodel = CManager::GetInstance()->GetXModel()->GetXModel(nXModelIdx);

	//�}�e���A�����ɑ΂���|�C���^������
	pMat = (D3DXMATERIAL*)pXmodel->m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pXmodel->m_dwNumMat; nCntMat++)
	{
		nCheck = -1;
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{
			for (int nCntTex = 0; nCntTex < m_NumAll; nCntTex++)
			{
				if (strcmp(&c_apTexturename[nCntTex][0], pMat[nCntMat].pTextureFilename) == 0)
				{
					nTexIdx[nCntMat] = nCntTex;
					nCheck = nCntTex;
				}
			}

			if (nTexIdx[nCntMat] != nCheck)
			{
				//�e�N�X�`���̓ǂݍ���
				if (SUCCEEDED(D3DXCreateTextureFromFile(m_pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_apTexture[m_NumAll])))
				{
					strcpy(&c_apTexturename[m_NumAll][0], pMat[nCntMat].pTextureFilename);
					m_NumAll++;
					nTexIdx[nCntMat] = m_NumAll - 1;
				}
			}
		}
		else
		{
			m_apTexture[m_NumAll] = nullptr;
			nTexIdx[nCntMat] = -1;
		}
	}
}

//====================================================================
//�w��̃e�N�X�`����Ԃ�
//====================================================================
LPDIRECT3DTEXTURE9 CCharacterManager::GetAddress(int nIdx)
{
	return m_apTexture[nIdx];
}