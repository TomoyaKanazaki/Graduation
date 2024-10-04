//========================================
//
//�V���[�e�B���O�A�N�V����[objectX.h]
//Author�F�X��x��
//
//========================================
#include "objectX.h"
#include "renderer.h"
#include "manager.h"

//========================================
//�R���X�g���N�^
//========================================
CObjectX::CObjectX(int nPriority) :
		CObject(nPriority),
		m_pTexture(nullptr),						//�e�N�X�`���ւ̃|�C���^
		m_pMesh(nullptr),							//���b�V��(���_���)�ւ̃|�C���^
		m_pBuffMat(nullptr),						//�}�e���A���ւ̃|�C���^
		m_dwNumMat(0),								//�}�e���A���̐�
		m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		//�ʒu
		m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		//����
		m_vtxMin(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	//���f���̍ŏ��l
		m_vtxMax(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//���f���̍ő�l											
{
}

//========================================
//�f�X�g���N�^
//========================================
CObjectX::~CObjectX()
{

}

//========================================
//������
//========================================
HRESULT CObjectX::Init(void)
{
	//���[�J���ϐ��錾
	D3DXMATERIAL *pMat;
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃|�C���^
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���f���̍ŏ��l��������
	m_vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	//���f���̍ő�l��������
	m_vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntTex = 0; nCntTex < (int)m_dwNumMat; nCntTex++)
	{//�}�e���A���̐�����
		if (pMat[nCntTex].pTextureFilename != nullptr)
		{//�e�N�X�`���t�@�C���������݂���
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntTex].pTextureFilename, &m_pTexture);
		}
	}

	//���_���̎擾
	nNumVtx = m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//���ׂĂ̒��_���r���ă��f���ŏ��l�E�ő�l�𔲂��o��
		if (m_vtxMax.x < vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (m_vtxMin.x > vtx.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (m_vtxMax.y < vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (m_vtxMin.y > vtx.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (m_vtxMax.z < vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}
		if (m_vtxMin.z > vtx.z)
		{
			m_vtxMin.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	//���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	//������Ԃ�
	return S_OK;
}

//========================================
//�I��
//========================================
void CObjectX::Uninit(void)
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
	//�e�N�X�`���̔j��
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	//�I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//========================================
//�X�V
//========================================
void CObjectX::Update(void)
{
}

//========================================
//�`��
//========================================
void CObjectX::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�ʒu�擾
	D3DXVECTOR3 pos = GetPosition();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{//�}�e���A���̐�����
		//�}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture); //�e�N�X�`�����g�p���ĂȂ��Ƃ���nullptr

		//���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================================
//�I�u�W�F�N�g�̊O���t�@�C��
//===========================================
void Load(void)
{
}

//========================================
//3D�I�u�W�F�N�g�̐���
//========================================
void CObjectX::SetVertex(void)
{

}

//========================================
//3D�I�u�W�F�N�g�̐���
//========================================
void CObjectX::SetSize(float fWidht, float fHeight)
{

}

//========================================
//3D�I�u�W�F�N�g�̐���
//========================================
void CObjectX::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
