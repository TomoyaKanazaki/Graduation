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
#include "game.h"
#include "objmeshField.h"

//===========================================
// �R���X�g���N�^
//===========================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
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
	// �p���N���X�̏�����
	CObject3D::Init();

	// �}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

	// �e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\player\\shadow000.jpg");

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
