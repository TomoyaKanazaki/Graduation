//============================================
//
//	�e���� [shadow.cpp]
//	Author:morikawa shunya
//
//============================================
#include "footprint.h"
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
	const D3DXVECTOR3 DEFAULT_SIZE = { 80.0f, 0.0f, 80.0f };
	const char* TEXTURE_PASS = "data\\TEXTURE\\foot_print.png";
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CFootPrint>* CFootPrint::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//===========================================
// �R���X�g���N�^
//===========================================
CFootPrint::CFootPrint(int nPriority) : CObject3D(nPriority)
{
}

//===========================================
// �f�X�g���N�^
//===========================================
CFootPrint::~CFootPrint()
{
}

//===========================================
// ������
//===========================================
HRESULT CFootPrint::Init()
{
	// �p���N���X�̏�����
	if (FAILED(CObject3D::Init()))
	{
		assert(false);
		return E_FAIL;
	}

	// �s�����x��1�Őݒ肷��
	SetAlpha(1.0f);

	//�}�b�v�Ƃ̃}�g���b�N�X�̊|�����킹���I���ɂ���
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// �e�N�X�`���ݒ�
	SetTexture(TEXTURE_PASS);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CFootPrint>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//===========================================
// �I��
//===========================================
void CFootPrint::Uninit(void)
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
void CFootPrint::Update(void)
{
	// �s�����x���擾
	float fAlpha = GetAlpha();

	// ���S�ɓ��߂���Ă����ꍇ�֐��𔲂���
	if (fAlpha <= 0.0f)
	{
		Uninit();
		return;
	}

	// �s�����x��������
	SetAlpha(fAlpha - DeltaTime::Get());

	// �e�N���X�̍X�V����
	CObject3D::Update();
}

//===========================================
// �`��
//===========================================
void CFootPrint::Draw(void)
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
// ����
//===========================================
CFootPrint* CFootPrint::Create(CMapSystem::GRID& grid, const D3DXVECTOR3& rot)
{
	// �C���X�^���X����
	CFootPrint* pFootPrint = new CFootPrint;

	// ������
	if (FAILED(pFootPrint->Init()))
	{
		assert(false);
		return nullptr;
	}

	// �ʒu�ݒ�
	pFootPrint->SetPos(grid.ToWorld());

	// �����̐ݒ�
	pFootPrint->SetRot(rot);

	// �T�C�Y�̐ݒ�
	pFootPrint->SetSize(DEFAULT_SIZE);

	return pFootPrint;
}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CFootPrint>* CFootPrint::GetList(void)
{
	return m_pList;
}

//==========================================
//  ���W�̐ݒ�
//==========================================
void CFootPrint::SetPos(const D3DXVECTOR3& pos)
{
	// �ϐ����ꎞ�ۑ�
	D3DXVECTOR3 temp = pos;

	// y���W�������グ��
	temp.y += 15.0f;

	// ���W��ݒ�
	CObject3D::SetPos(temp);
}
