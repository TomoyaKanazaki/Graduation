//=======================================
//
//�|���S������[idxmeshfield.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "idxmeshfield.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//========================================
// �萔��`
//========================================
namespace
{
const char* IDXMESHFIELD_TEX = "data\\TEXTURE\\block001.jpg";	// �G�t�F�N�g�̍ő吔
}

//=======================================
//�ÓI�����o�ϐ�
//=======================================
LPDIRECT3DTEXTURE9 CIdxMeshField::m_pTexture = NULL;			//�e�N�X�`���ւ̃|�C���^

//=======================================
//�R���X�g���N�^
//=======================================
CIdxMeshField::CIdxMeshField() :
	m_pVtx(),								//���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuffMeshField(nullptr),			//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	//�ʒu
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	//����
	m_mtxWorld()							//���[���h�}�g���b�N�X
{
}

//=======================================
//�f�X�g���N�^
//=======================================
CIdxMeshField::~CIdxMeshField()
{

}

//=======================================
//�ǂݍ���
//=======================================
HRESULT CIdxMeshField::Load(void)
{
	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		IDXMESHFIELD_TEX,
		&m_pTexture)))
	{// ���s��Ԃ�
		return E_FAIL;
	}

	//������Ԃ�
	return S_OK;
}

//=======================================
//�j��
//=======================================
void CIdxMeshField::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=======================================
//����
//=======================================
CIdxMeshField *CIdxMeshField::Create(void)
{
	//�C���f�b�N�X���b�V���̃|�C���^
	CIdxMeshField *pIdxMeshField = nullptr;

	//�e�N�X�`���̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	if (pIdxMeshField == nullptr)
	{//nullptr�̎�
		//�C���X�^���X����
		pIdxMeshField = new CIdxMeshField;

		//������
		pIdxMeshField->Init();

		//�v���C���[�e�N�X�`���̊��蓖��
		pIdxMeshField->BindTexture(pTexture->Regist(IDXMESHFIELD_TEX));
	}

	return pIdxMeshField;
}

//=======================================
// ����������
//=======================================
HRESULT CIdxMeshField::Init(void)
{
	m_pos = D3DXVECTOR3(-1000.0f, 0.0f, 1000.0f);

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CObject3D::MeshVertex();

	return S_OK;
}
 
//=======================================
// �I������
//=======================================
void CIdxMeshField::Uninit(void)
{
	CObject3D::Uninit();

	//���_�o�b�t�@�̔j��
	if (m_pIdxBuffMeshField != NULL)
	{
		m_pIdxBuffMeshField->Release();
		m_pIdxBuffMeshField = NULL;
	}
}

//=======================================
// �X�V����
//=======================================
void CIdxMeshField::Update(void)
{
}

//=======================================
// �`�揈��
//=======================================
void CIdxMeshField::Draw(void)
{
	//CRenderer�^�̃|�C���^
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtx, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL

	//�|���S���̕`��								 
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,		// �p�ӂ������_�̐�
		0,
		12);	// �|���S���̐�

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL); //�e�N�X�`�����g�p���ĂȂ��Ƃ���NULL
}
