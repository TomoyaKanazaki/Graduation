//========================================
//
// �t�B�[���h[sky.cpp]
// Author�F�X��x��
//
//========================================
#include "sky.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float WIDTH = 200000.0f;	// ����
	const float INTERVAL_TEXU = 2000.0f;	// U���W�̊Ԋu
}

//========================================
//�ÓI�����o�ϐ�
//========================================
LPDIRECT3DTEXTURE9 CSky::m_pTexture = nullptr;

//========================================
//�R���X�g���N�^
//========================================
CSky::CSky(int nPriority) :
	CObject3D(nPriority),
	m_fTexU(0.0f)
{
}

//========================================
//�f�X�g���N�^
//========================================
CSky::~CSky()
{
}

//========================================
//�|���S������
//========================================
CSky* CSky::Create(void)
{
	//CPlayer�^�̃|�C���^
	CSky* pField = new CSky;;

	//������
	pField->Init();

	//�|�C���^��Ԃ�
	return pField;
}

//========================================
//������
//========================================
HRESULT CSky::Init(void)
{
	//�e�N�X�`���̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// �p���̏�����
	CObject3D::Init();

	//�e�N�X�`�����蓖��
	BindTexture(pTexture->Regist("data\\TEXTURE\\sky_02.jpg"));


	return S_OK;
}

//========================================
//�I��
//========================================
void CSky::Uninit(void)
{
	// �p���̏�����
	CObject3D::Uninit();
}

//========================================
//�X�V
//========================================
void CSky::Update(void)
{
	m_fTexU += 0.001f;

	// �p���̏�����
	CObject3D::Update();
}

//========================================
// ���_���
//========================================
void CSky::SetVertex3D()
{
	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	int idxTexture = GetIdxTexture();
	if (idxTexture == 0) return;
	D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(idxTexture), WIDTH);

	// ���_���W�̐ݒ�
	MyLib::Vector3 pos(0.0f, -size.y * 0.5f, 5000.0f);

	pVtx[0].pos = pos + D3DXVECTOR3(-size.x, size.y,	0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(size.x, size.y, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(size.x, -size.y, 0.0f);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	// �e�N�X�`�����W�̐ݒ�
	float ratioX = size.x / INTERVAL_TEXU;
	float ratioY = ratioX / UtilFunc::Calculation::AspectRatio(size);
	pVtx[0].tex = D3DXVECTOR2(m_fTexU + 0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + ratioX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU + 0.0f, ratioY);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + ratioX, ratioY);

	// ���_�o�b�t�@���A�����b�N
	GetVtxBuff()->Unlock();
}

//========================================
//�`��
//========================================
void CSky::Draw(void)
{
	//�`��
	CObject3D::Draw();
}
