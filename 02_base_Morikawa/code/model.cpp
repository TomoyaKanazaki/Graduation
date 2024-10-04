//========================================
//
//�@���f��[model.cpp]
//�@Author�F�X��x��
//
//========================================
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XLoad.h"
#include "calculation.h"

//========================================
//�R���X�g���N�^
//========================================
CModel::CModel() : 
	m_pMesh(nullptr),			//���b�V��(���_���)�ւ̃|�C���^
	m_pBuffMat(nullptr),		//�}�e���A���ւ̃|�C���^
	m_dwNumMat(NULL),			//�}�e���A���̐�
	m_pos(0.0f, 0.0f, 0.0f),	//�ʒu
	m_rot(0.0f, 0.0f, 0.0f),	//����
	m_scale(1.0f),				// �X�P�[��
	m_pParent(nullptr)			//�e���f���ւ̃|�C���^
{
	m_nIdxTexture.clear();			//���L�e�N�X�`��
}

//========================================
//�f�X�g���N�^
//========================================
CModel::~CModel()
{

}

//========================================
//���f������
//========================================
CModel *CModel::Create(const char *pFilename)
{
	//CModel�^�̃|�C���^
	CModel *pModel = nullptr;

	if (pModel == nullptr)
	{
		//�v���C���[����
		pModel = new CModel;

		//������
		pModel->Init(pFilename);
	}

	//�|�C���^��Ԃ�
	return pModel;
}

//========================================
//������
//========================================
HRESULT CModel::Init(const char *pFilename)
{
	//���[�J���ϐ��錾
	D3DXMATERIAL *pMat;

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�ʒu�̐ݒ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�����̐ݒ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �K�w
	m_nType = CModel::TYPE_HIERARCHY;


	// X�t�@�C���̃f�[�^�擾
	CXLoad* pXLoad = CXLoad::GetInstance();

	// X�t�@�C���̃��[�h
	m_nIdxXFile = pXLoad->XLoad(pFilename);

	// X�t�@�C���̃f�[�^���蓖��
	BindXData(m_nIdxXFile);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile* pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �S���_�`�F�b�N
	UtilFunc::Calculation::CalModelVtx(GetRot().y, &pXData->vtxMax, &pXData->vtxMin, pXData->pMesh, pXData->pVtxBuff);

	// �e�N�X�`�����蓖��
	BindTexture();







	//������Ԃ�
	return S_OK;
}

//==========================================================================
// X�t�@�C���̃f�[�^���蓖��
//==========================================================================
void CModel::BindXData(int nIdxXFile)
{
	// ��񊄂蓖��
	m_nIdxXFile = nIdxXFile;

	// �e�N�X�`�����蓖��
	BindTexture();
}

//==========================================================================
// �e�N�X�`�����蓖��
//==========================================================================
void CModel::BindTexture()
{
	m_nIdxTexture.clear();

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile* pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	for (int i = 0; i < (int)pXData->dwNumMat; i++)
	{
		m_nIdxTexture.push_back(pXData->nIdxTexture[i]);
	}
}

//========================================
//�I��
//========================================
void CModel::Uninit(void)
{
	//���b�V���̔j��
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	//�}�e���A���̔j��
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	//for (int i = 0; i < (int)m_nIdxTexture.size(); i++)
	//{
	//	//�e�N�X�`���̔j��
	//	if (m_nIdxTexture[i] != nullptr)
	//	{
	//		m_nIdxTexture[i]->Release();
	//		m_nIdxTexture[i] = nullptr;
	//	}
	//}

	// �S�N���A
	m_nIdxTexture.clear();
}

//========================================
//�X�V
//========================================
void CModel::Update(void)
{

}

//========================================
//�`��
//========================================
void CModel::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxParent;			//�e���f���̃}�g���b�N�X
	MyLib::Matrix mtxScale;	// �v�Z�p�}�g���b�N�X�錾

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �������K����ON�ɂ���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f����
	mtxScale.Scaling(m_scale);
	D3DXMATRIX calMtxScale = mtxScale.ConvertD3DXMATRIX();
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &calMtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{//�e�����݂���
		//�e���f���̃}�g���b�N�X���擾����
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//�e�����݂��Ȃ�
		if (m_nType == TYPE_HIERARCHY)
		{// �K�w���f�����K�w����Ȃ���
			//����(�ŐV)�̃}�g���b�N�X���擾����{ = �v���C���[�̃}�g���b�N�X}
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}
		else
		{
			D3DXMatrixIdentity(&mtxParent);
		}
	}

	//�e�̃}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// X�t�@�C���̃f�[�^�擾
	CXLoad::SXFile* pXData = CXLoad::GetInstance()->GetMyObject(m_nIdxXFile);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pXData->pBuffMat->GetBufferPointer();

	// �e�N�X�`���擾
	CTexture* pTexture = CTexture::GetInstance();

	// ���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)pXData->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture[nCntMat])); //�e�N�X�`�����g�p���ĂȂ��Ƃ���nullptr

		// �p�[�c�̕`��
		pXData->pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �������K�����f�t�H���g�ɂ���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//========================================
//�e���f��
//========================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//========================================
//���[���h�}�g���b�N�X�̎擾
//========================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}

//========================================
//���_���
//========================================
void CModel::SetVertex(void)
{

}

//========================================
//�T�C�Y����
//========================================
void CModel::SetSize(float fWidht, float fHeight)
{

}

//========================================
//�ʒu�ݒ�
//========================================
void CModel::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//========================================
//�ʒu�擾
//========================================
D3DXVECTOR3 CModel::GetPosition(void)
{
	return m_pos;
}

//========================================
//�����ݒ�
//========================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//========================================
//�����擾
//========================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_rot;
}

//========================================
//���f���̔ԍ��ݒ�
//========================================
void CModel::SetIndex(CModel *pModel)
{
	m_pParent = pModel;
}

//========================================
//���f���̈ʒu�ݒ�
//========================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_setPos = pos;
}

//========================================
//���f���̈ʒu�擾
//========================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_setPos;
}

//========================================
//���f���̌���
//========================================
void CModel::SetRotate(D3DXVECTOR3 rot)
{
	m_setRot = rot;
}

//========================================
//���f���̌����擾
//========================================
D3DXVECTOR3 CModel::GetRotate(void)
{
	return m_setRot;
}
