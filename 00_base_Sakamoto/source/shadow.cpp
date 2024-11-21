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

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 DEFAULT_ROT = { D3DX_PI * 0.5f, 0.0f, 0.0f }; // �p�x�̕␳�l
	const float LIMIT_HEIGHT = 2000.0f; // �e��`�悷�����̍���
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CShadow>* CShadow::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//===========================================
// �R���X�g���N�^
//===========================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority),
m_fHeight(0.0f),
m_sizeBase(INITVECTOR3),
m_fLimit(0.0f)
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
CShadow* CShadow::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight, const float fLimit)
{
	// �C���X�^���X����
	CShadow* pShadow = new CShadow;

	// ������
	pShadow->Init();

	// �ʒu�ݒ�
	pShadow->SetPos(pos);

	// �傫��
	pShadow->SetpVtx(fWidth, fHeight);
	pShadow->m_sizeBase.x = fWidth;
	pShadow->m_sizeBase.z = fHeight;

	// �p�x��ݒ�
	pShadow->SetRot(DEFAULT_ROT);

	// ���������ݒ�
	if (fLimit == -1.0f)
	{
		pShadow->m_fLimit = LIMIT_HEIGHT;
	}
	else
	{
		pShadow->m_fLimit = fLimit;
	}

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
	if (m_pList != nullptr)
	{
		// ���X�g���玩�g�̃I�u�W�F�N�g���폜
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // �I�u�W�F�N�g������Ȃ��ꍇ

			// ���X�g�}�l�[�W���[�̔j��
			m_pList->Release(m_pList);
		}
	}

	// �p���N���X�̏I��
	CObject3D::Uninit();
}

//===========================================
// �X�V
//===========================================
void CShadow::Update(void)
{
	// �T�C�Y��ύX����
	float fScale = m_fHeight / m_fLimit;
	D3DXVECTOR3 size = m_sizeBase + m_sizeBase * 2.0f * fScale;
	SetpVtx(size.x, size.z);

	// �����x��ύX����
	SetColorA(1.0f - fScale);

	// �e�N���X�̍X�V����
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

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CShadow>* CShadow::GetList(void)
{
	return m_pList;
}
