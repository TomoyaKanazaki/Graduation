//============================================
//
//	�V�O�i���̏��� [signal.cpp]
//	Author:morikawa shunya
//
//============================================
#include "signal.h"
#include "useful.h"
#include "MapSystem.h"
#include "game.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"
#include "objmeshField.h"
#include "devil.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float BASE_Y = 50.0f; // ����
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CSignal>* CSignal::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CSignal::CSignal(int nPriority) : CObjectX(nPriority)
{
	m_nIdxXModel = 0;			// X���f���ԍ�
	m_nIdxTexture = -1;			// �e�N�X�`���ԍ�
	m_nStateCount = 0;			// ��ԊǗ�
	m_fColorA = 0.0f;			// �s�����x
	m_Scaling = 0.0f;			// �傫��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_nStateNum = 0;
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CSignal::~CSignal()
{

}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CSignal>* CSignal::GetList(void)
{
	return m_pList;
}

//====================================================================
// ����
//====================================================================
CSignal* CSignal::Create(const char* pModelNameSignal)
{
	// �C���X�^���X����
	CSignal* pSignal = new CSignal();

	// �I�u�W�F�N�g�̏���������
	if (FAILED(pSignal->Init(pModelNameSignal)))
	{// ���������������s�����ꍇ

		return nullptr;
	}

	return pSignal;
}

//====================================================================
// ������
//====================================================================
HRESULT CSignal::Init(const char* pModelName)
{
	
	D3DXVECTOR3 pos = GetPos();

	// �p���N���X�̏�����
	CObjectX::Init(pModelName);

	SetPos(pos);

	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CSignal>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CSignal::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//====================================================================
// �X�V
//====================================================================
void CSignal::Update()
{
	// ���g�̏����擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// ���̍X�V
	SetPos(D3DXVECTOR3(pos.x, 300.0f, pos.z));

	// �e�N���X�̍X�V����
	CObjectX::Update();
}

//====================================================================
// �`��
//====================================================================
void CSignal::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//�X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�̐ݒ� => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	//�X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//�X�e���V���o�b�t�@�̔�r���@ => (�Q�ƒl => �X�e���V���o�b�t�@�̎Q�ƒl)�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//�X�e���V���e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �p���N���X�̕`��
	CObjectX::Draw();

	//�X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}