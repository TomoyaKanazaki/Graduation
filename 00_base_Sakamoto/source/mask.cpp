//=======================================
//
//�X�e���V���o�b�t�@�ɕK�v��2D�}�X�N[mask.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "mask.h"
#include "manager.h"
#include "renderer.h"

//========================================
// �R���X�g���N�^
//========================================
CMask::CMask(int nPriority) : CObject2D(nPriority)
{
}

//========================================
// �f�X�g���N�^
//========================================
CMask::~CMask()
{
}

//========================================
// ����
//========================================
CMask* CMask::Create(int nPriority)
{
	CMask* pMask = new CMask(nPriority);

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pMask->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pMask;
}

//========================================
// ������
//========================================
HRESULT CMask::Init()
{
	// �p���N���X�̏�����
	CObject2D::Init();

	// ���A�����̐ݒ�
	SetWidth(SCREEN_WIDTH_F);
	SetHeight(SCREEN_HEIGHT_F);

	// �ʒu�ݒ�
	SetPos(SCREEN_CENTER_F);

	return S_OK;
}

//========================================
// �I��
//========================================
void CMask::Uninit()
{
	// �p���N���X�̏I��
	CObject2D::Uninit();
}

//========================================
// �X�V
//========================================
void CMask::Update()
{
	// �p���N���X�̍X�V
	CObject2D::Update();
}

//========================================
// �`��
//========================================
void CMask::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//�X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�̐ݒ� => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 2);

	//�X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//�X�e���V���o�b�t�@�̔�r���@ <= (�Q�ƒl <= �X�e���V���o�b�t�@�̎Q�ƒl)�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);

	//�X�e���V���e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �p���N���X�̕`��
	CObject2D::Draw();

	//�X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}
