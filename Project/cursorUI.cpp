//============================================
//
//	�}�E�XUI [mouseUI.cpp]
//	Author : Takeru Ogasawara
//
//============================================

#include "cursorUI.h"
#include "manager.h"
#include "input.h"
#include "useful.h"
#include "debugproc.h"

// �萔��`
namespace
{
	const char* TEX_PATH = "data\\TEXTURE\\mouseCursor2.png";
	const D3DXVECTOR2 SIZE_UI = { 50.0f, 50.0f };	// �傫��
	const D3DXVECTOR2 COLL_POS = { SIZE_UI.x * 0.5f, SIZE_UI.y * 0.5f };	// ����̈ʒu(���㔻��)
	const float SPEED = 5.5f;	// �J�[�\�����x
}

//====================================================================
// �R���X�g���N�^
//====================================================================
CCursorUI::CCursorUI(int nPriority) : CObject2D(nPriority)
{
	m_move = { 0.0f, 0.0f, 0.0f }; 
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CCursorUI::~CCursorUI()
{

}

//====================================================================
// ��������
//====================================================================
CCursorUI* CCursorUI::Create(D3DXVECTOR2 pos)
{
	CCursorUI* pMouseUI = nullptr;

	if (!pMouseUI)
	{
		pMouseUI = new CCursorUI;
	}

	// �I�u�W�F�N�g�̏���������
	if (FAILED(pMouseUI->Init()))
	{// ���������������s�����ꍇ
		return nullptr;
	}

	pMouseUI->SetPos({ pos.x, pos.y, 0.0f });

	return pMouseUI;
}

//====================================================================
// ����������
//====================================================================
HRESULT CCursorUI::Init(void)
{
	// �e�N���X�̏���������
	CObject2D::Init();

	SetTexture(TEX_PATH);
	SetWidth(SIZE_UI.x);
	SetHeight(SIZE_UI.y);

	return S_OK;
}

//====================================================================
// �I������
//====================================================================
void CCursorUI::Uninit(void)
{
	// �e�N���X�̏I������
	CObject2D::Uninit();
}

//====================================================================
// �X�V����
//====================================================================
void CCursorUI::Update(void)
{
	// �ړ�
	move();

	// �e�N���X�̍X�V����
	CObject2D::Update();
}

//====================================================================
// �`�揈��
//====================================================================
void CCursorUI::Draw(void)
{
	// �e�N���X�̕`�揈��
	CObject2D::Draw();
}

//====================================================================
// �ړ�
//====================================================================
void CCursorUI::move()
{
	// ���̎擾
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (!pMouse)
	{
		return;
	}

	// ���̎擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ��ʂ̉��Z
	m_move = pMouse->GetMouseMove();

	// �ʒu�ɉ��Z
	pos += m_move * SPEED;

	// �E�B���h�E��ʂ̓����蔻��
	useful::CollisionWindow(&pos);

	// �ʒu�̐ݒ�
	SetPos(pos);

	CManager::GetInstance()->GetDebugProc()->Print("\n�J�[�\��UI�ʒu %f %f\n", pos.x, pos.y);
}

//====================================================================
// �����蔻�� : �ʒu�A�傫�� [�ǂ�������肷��2D������]
//====================================================================
bool CCursorUI::Collsion(D3DXVECTOR3 pos2D, D3DXVECTOR3 size2D)
{
	// ���̎擾
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (!pMouse)
	{
		return false;
	}

	// ���̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posCollision = {};

	// ����ʒu
	posCollision = { pos.x - COLL_POS.x, pos.y - COLL_POS.y, 0.0f };

	if (useful::CollisionRectangle2D(posCollision, pos2D, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size2D, useful::COLLISION_XY))
	{
		return true;
	}

	return false;
}
