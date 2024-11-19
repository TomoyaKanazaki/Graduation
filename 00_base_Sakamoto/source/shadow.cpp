//============================================
//
//	�e���� [shadow.cpp]
//	Author:morikawa shunya
//
//============================================

#include "shadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CShadow>* CShadow::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//===========================================
// �R���X�g���N�^
//===========================================
CShadow::CShadow(int nPriority)
{
}

//===========================================
// �f�X�g���N�^
//===========================================
CShadow::~CShadow()
{
}

//===========================================
// ����
//===========================================
CShadow* CShadow::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight)
{
	// �C���X�^���X����
	CShadow* pShadow = new CShadow;

	// ������
	pShadow->Init();

	// �ʒu�ݒ�
	pShadow->SetPos(pos);

	// �傫��
	pShadow->SetpVtx(fWidth, fHeight);

	return pShadow;
}

//===========================================
// ������
//===========================================
HRESULT CShadow::Init()
{
	// �e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\player\\shadow000.jpg");

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ
		
		return E_FAIL;
	}

	return S_OK;
}

//===========================================
// �I��
//===========================================
void CShadow::Uninit(void)
{
	// �p���N���X�̏I��
	CObject3D::Uninit();
}

//===========================================
// �X�V
//===========================================
void CShadow::Update(void)
{
	// �p���N���X�̍X�V
	CObject3D::Update();
}

//===========================================
// �`��
//===========================================
void CShadow::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �p���N���X�̕`��
	CObject3D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================================
// ���X�g�擾
//===========================================
CListManager<CShadow>* CShadow::GetList(void)
{
	return m_pList;
}
