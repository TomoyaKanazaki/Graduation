//==============================================================
//
//�I�u�W�F�N�g3D����[object3D.h]
//Author:����������
//
//==============================================================
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//�}�N����`
#define PRIORITY		(3)		//�D�揇��

//==============================================================
//�R���X�g���N�^
//==============================================================
CObject3D::CObject3D()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_type = CObject::TYPE_NONE;		//���
	m_fHeight = 0.0f;		//�c��
	m_fWidth = 0.0f;		//����
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CObject3D::~CObject3D()
{

}

//==============================================================
//��������
//==============================================================
CObject3D *CObject3D::Create(void)
{
	CObject3D *pObject3D = NULL;

	if (pObject3D == NULL)
	{//���������g�p����ĂȂ�������

		//�I�u�W�F�N�g2D�̐���
		pObject3D = new CObject3D;
	}

	//����������
	pObject3D->Init();

	return pObject3D;
}

//==============================================================
//�e�N�X�`�����蓖�ď���
//==============================================================
void CObject3D::BindTexture(int nIdxTexture)
{
	m_nIdxTexture = nIdxTexture;		//�e�N�X�`�����蓖��
}

//==============================================================
//�I�u�W�F�N�g3D�̏���������
//==============================================================
HRESULT CObject3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f, m_fHeight);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fHeight);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeight);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeight);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================
//�I�u�W�F�N�g3D�̏I������
//==============================================================
void CObject3D::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�I�u�W�F�N�g3D�̍X�V����
//==============================================================
void CObject3D::Update(void)
{

}

//==============================================================
//�I�u�W�F�N�g3D�̕`�揈��
//==============================================================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();		//�e�N�X�`���̎擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X��������
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
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//�I�u�W�F�N�g3D�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,												//�v���~�e�B�u�i�I�u�W�F�N�g3D�̐��j
		2);												//�`�悷��v���~�e�B�u��
}

//==============================================================
//�I�u�W�F�N�g3D�̃T�C�Y�ݒ菈��
//==============================================================
void CObject3D::SetSize(float fWidth, float fHeight)
{
	m_fHeight = fHeight; 
	m_fWidth = fWidth;

	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	switch (m_type)
	{
	case TYPE_FIELD:		//��

		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f, m_fHeight);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fHeight);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeight);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeight);

		break;

	default:		//����ȊO

		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		break;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================
//�I�u�W�F�N�g�̐F�ݒ�
//==============================================================
void CObject3D::SetColor(D3DXCOLOR col)
{
	VERTEX_3D* pVtx;		//���_���ւ̃|�C���^

	m_col = col;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}