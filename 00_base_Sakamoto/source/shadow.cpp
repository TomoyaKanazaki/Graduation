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
#include "Scene.h"

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CShadow>* CShadow::m_pList = nullptr; // �I�u�W�F�N�g���X�g

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
	SetUseMultiMatrix(CGame::GetInstance()->GetMapField()->GetMatrix());

	// �e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\player\\shadow000.jpg");

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CShadow>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//===========================================
// �I��
//===========================================
void CShadow::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

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

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CShadow>* CShadow::GetList(void)
{
	return m_pList;
}
