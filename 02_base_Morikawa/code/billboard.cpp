//=======================================
//
//�r���{�[�h[billboard.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"
#include "billboard.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"

//========================================
// �萔��`
//========================================
namespace
{
const char* BILLBOARD_TEX = "data\\TEXTURE\\effect000.jpg";	// �}�b�v�I�u�W�F�N�g�̃p�X
}

//=======================================
//�R���X�g���N�^
//=======================================
CBillboard::CBillboard(int nPriority) :
	CObject(nPriority),
	m_nIdxTexture(0),		//�e�N�X�`���ԍ�
	m_pVtxBuff(nullptr),	//���_���
	m_col(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)),	//�F
	m_fLength(0.0f),	//�Ίp���̒���
	m_fAngle(0.0f),		//�Ίp���̒���
	m_bFront(true)		// ���ʌ�����
{
}

//=======================================
//�f�X�g���N�^
//=======================================
CBillboard::~CBillboard()
{
}

//=======================================
//�r���{�[�h����
//=======================================
CBillboard *CBillboard::Create(D3DXVECTOR3 pos, float fwidth, float fheight)
{
	//pBillboard�^�̃|�C���^
	CBillboard *pBillboard = nullptr;

	if (pBillboard == nullptr)
	{//pBillboard��nullptr�̎�
		//�r���{�[�h����
		pBillboard = new CBillboard;

		//������
		pBillboard->Init();

		//�ʒu�ݒ�
		pBillboard->SetPos(pos);

		//�T�C�Y�ݒ�
		pBillboard->SetSizeVertex(fwidth, fheight);
	}

	//�|�C���^��Ԃ�
	return pBillboard;
}

//=======================================
//�r���{�[�h�̏���������
//=======================================
HRESULT CBillboard::Init(void)
{
	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);


	return S_OK;
}

//=======================================
//�r���{�[�h�̏I������
//=======================================
void CBillboard::Uninit(void)
{
	//���_�o�b�t�@�̏I������
	if (m_pVtxBuff != nullptr)
	{//m_pTexture��nullptr����Ȃ��Ƃ�
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//�I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//=======================================
//�r���{�[�h�̍X�V����
//=======================================
void CBillboard::Update(void)
{
	// ���_���W�̐ݒ�
	SetVertex();
}

//=======================================
//�r���{�[�h�̕`�揈��
//=======================================
void CBillboard::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXMATRIX mtxView;		//�r���[�}�g���N�X�擾�p

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//CTexture�^�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// �ʒu
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 Rot = GetRot();


	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);	//�t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture)); //�e�N�X�`�����g�p���ĂȂ��Ƃ���nullptr

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
//�e�N�X�`���̊��蓖��
//========================================
void CBillboard::BindTexture(int pTexture)
{
	m_nIdxTexture = pTexture;
}

//========================================
//�e�̒��_���W
//========================================
void CBillboard::SetPosVertex(float size)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-size, size, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size, size, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size, -size, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size, -size, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//�A�j���[�V�����r���{�[�h�̒��_���W
//========================================
void CBillboard::SetVertexAnim(int Num, int Cnt)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍Đݒ�
	pVtx[0].tex = D3DXVECTOR2(1.0f / Num * Cnt, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / Num * (Cnt + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / Num * Cnt, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / Num * (Cnt + 1), 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//���_���̃T�C�Y�ݒ�
//========================================
void CBillboard::SetSizeVertex(float fwidth, float fheight)
{
	m_fWidth = fwidth;
	m_fHeight = fheight;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fwidth, fheight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fwidth, fheight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fwidth, -fheight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fwidth, -fheight, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//�r���{�[�h�G�t�F�N�g�̒��_���W
//========================================
void CBillboard::SetEffectVertex(float fwidth, float fHeight)
{
	m_fWidth = fwidth;

	m_fHeight = fHeight;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍Đݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//�r���{�[�h�̒��_���W
//========================================
void CBillboard::SetVertex(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//�e�r���{�[�h�̒��_���W
//========================================
void CBillboard::SetSize(float fWidht, float fHeight)
{
	m_fWidth = fWidht;
	m_fHeight = fHeight;

	//�Ίp���̒���(���A�c)
	m_fLength = sqrtf(fWidht * fWidht + fHeight * fHeight) * 1.0f;

	//�Ίp���̌���(���A�c)
	m_fAngle = atan2f(fWidht, fHeight);
}

//========================================
//�e�r���{�[�h�̒��_���W
//========================================
float CBillboard::GetSize()
{
	return m_fWidth, m_fHeight;
}

//========================================
//�F�ݒ�
//========================================
void CBillboard::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

//========================================
//�F�擾
//========================================
D3DXCOLOR CBillboard::GetCol()
{
	return m_col;
}
