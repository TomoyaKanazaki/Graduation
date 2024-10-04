//=============================================================================
// 
//  �X�e�[�W�G�f�B�^�[���� [stageEdittor.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "stageEdittor.h"
#include "stageobj.h"
#include "obstacle.h"

#include "manager.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\box.x";
	const float ENABLERANGE = 1000.0f;	// �L���͈�
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStageEdittor::CStageEdittor() : CObject(),
m_pStageObj(nullptr),
m_nType(0)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStageEdittor::~CStageEdittor()
{

}

//==========================================================================
// ���]�|�C���g�̐�������
//==========================================================================
CStageEdittor* CStageEdittor::Create()
{
	// �������̊m��
	CStageEdittor* pObj = new CStageEdittor;

	pObj->Init();

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStageEdittor::Init()
{
	m_pStageObj = CStageObj::CreateBg(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStageEdittor::Uninit()
{
	if (m_pStageObj == nullptr)
	{
		m_pStageObj->Uninit();
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CStageEdittor::Update()
{
	// CInputKeyboard�^�̃|�C���^
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_9))
	{
		// ��ޕς�
		SetType();
	}

	// ����
	Control();

	DebugProc::Print(DebugProc::EPoint::POINT_CENTER, "��ށF%d\n�ʒu�F%f %f %f\n", m_nType, m_pStageObj->GetPos().x, m_pStageObj->GetPos().y, m_pStageObj->GetPos().z);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStageEdittor::Draw()
{
	if (m_pStageObj != nullptr)
	{// ���f���̕`��
		m_pStageObj->Draw();
	}
}

//==========================================================================
// ��ސݒ�
//==========================================================================
void CStageEdittor::SetType()
{
	D3DXVECTOR3 pos = m_pStageObj->GetPos();

	// ��ނ�ς���
	m_pStageObj->Uninit();
	m_nType = (m_nType + 1) % CStageObj::TYPE_MAX;

	switch (m_nType)
	{
	case CStageObj::TYPE_BG:

		m_pStageObj = CStageObj::CreateBg(pos);

		break;

	case CStageObj::TYPE_OBSTACLE:

		m_pStageObj = CStageObj::CreateObstacle(pos, CObstacle::TYPE_RIVER);

		break;

	case CStageObj::TYPE_REVERSE:

		m_pStageObj = CStageObj::CreateReverse(pos);

		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// ��ސݒ�
//==========================================================================
void CStageEdittor::Control()
{
	// CInputKeyboard�^�̃|�C���^
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXVECTOR3 pos = m_pStageObj->GetPos();
	D3DXVECTOR3 posOrigin = m_pStageObj->GetOriginPosition();

	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		pos.x += 3.0f;
		posOrigin.x += 3.0f;
	}
	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		pos.x -= 3.0f;
		posOrigin.x -= 3.0f;
	}
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		pos.y += 3.0f;
		posOrigin.y += 3.0f;
	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		pos.y -= 3.0f;
		posOrigin.y -= 3.0f;
	}

	m_pStageObj->SetPos(pos);
	m_pStageObj->SetOriginPosition(posOrigin);
}