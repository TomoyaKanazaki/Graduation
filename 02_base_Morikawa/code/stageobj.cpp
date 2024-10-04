//=============================================================================
// 
//  �X�e�[�W�̔z�u������ [transferBeacon.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "stageobj.h"
#include "manager.h"
#include "calculation.h"
#include "model.h"
#include "input.h"

// �h���N���X
#include "obstacle.h"
#include "reverse.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL[] = 
	{
		"data\\MODEL\\obstacle\\wood00.x",
		"data\\MODEL\\obstacle\\wood01.x",
	};
	const float ENABLERANGE = 2000.0f;	// �L���͈�
}

namespace StateTime
{
	const float APPEARANCE = 0.3f;	// �o��
	const float LEAVE = 0.7f;		// �ޏ�
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CStageObj::SAMPLE_FUNC CStageObj::m_SampleFuncList[] =
{
	&CStageObj::StateNone,			// �Ȃ�
	&CStageObj::StateAppearance,	// �o��
	&CStageObj::StateLeave,			// �ޏ�
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CStageObj> CStageObj::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStageObj::CStageObj(int nPriority) : CObject(nPriority),
m_fStateTime(0.0f),		// ��ԃ^�C�}�[
m_state(State::STATE_NONE),	// ���
m_pModel(nullptr),		// ���f��
m_bWorking(false)		// �ғ�����
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStageObj::~CStageObj()
{
	
}

//==========================================================================
// �w�i�̐�������
//==========================================================================
CStageObj* CStageObj::CreateBg(const MyLib::Vector3& pos)
{
	// �������̊m��
	CStageObj* pObj = new CStageObj;

	if (pObj != nullptr)
	{
		// �����ݒ�
		pObj->m_type = TYPE_BG;
		pObj->SetPos(pos);
		pObj->SetOriginPosition(pos);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ��Q���̐�������
//==========================================================================
CStageObj* CStageObj::CreateObstacle(const MyLib::Vector3& pos, const int type)
{
	// �������̊m��
	CStageObj* pObj = CObstacle::Create((CObstacle::EType)(type), pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (pObj != nullptr)
	{
		// �����ݒ�
		pObj->m_type = TYPE_OBSTACLE;
		pObj->SetPos(pos);
		pObj->SetOriginPosition(pos);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ���]�|�C���g�̐�������
//==========================================================================
CStageObj* CStageObj::CreateReverse(const MyLib::Vector3& pos)
{
	// �������̊m��
	CStageObj* pObj = new CReverse;

	if (pObj != nullptr)
	{
		// �����ݒ�
		pObj->m_type = TYPE_REVERSE;
		pObj->SetPos(pos);
		pObj->SetOriginPosition(pos);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStageObj::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ���f������
	if (m_pModel == nullptr &&
		m_type != Type::TYPE_REVERSE)
	{
		int randNum = (rand() % 2);
		m_pModel = CModel::Create(MODEL[randNum]);
	}

	if (m_pModel != nullptr)
	{
		m_pModel->SetType(CModel::TYPE_NOT_HIERARCHY);
		m_pModel->SetPosition(GetPos());
		m_pModel->SetScale(0.0f);
	}

	// �o��
	SetState(State::STATE_NONE);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStageObj::Uninit()
{
	if (m_pModel != nullptr)
	{// ���f���̍폜
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I��
	Release();
}

//==========================================================================
// �ʒu
//==========================================================================
void CStageObj::SetPos(D3DXVECTOR3 pos)
{
	CObject::SetPos(pos);

	if (m_pModel != nullptr)
	{
		m_pModel->SetPosition(pos);
	}
}

//==========================================================================
// �폜
//==========================================================================
void CStageObj::Kill()
{
	if (m_pModel != nullptr)
	{// ���f���̍폜
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CStageObj::Update()
{
	// ��ԍX�V
	UpdateState();

	if (m_pModel != nullptr)
	{// ���f���̍X�V
		m_pModel->SetPosition(GetPos());
		m_pModel->Update();
	}

	// CInputKeyboard�^�̃|�C���^
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

#ifdef _DEBUG
	
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		SetState(State::STATE_LEAVE);
	}
#endif
}

//==========================================================================
// �͈͔���
//==========================================================================
void CStageObj::CollisionRange(const MyLib::Vector3& rPos)
{
	MyLib::Vector3 pos = GetPos();

	if (m_bWorking &&
		m_state != State::STATE_LEAVE &&
		rPos.x >= pos.x + ENABLERANGE * 0.5f)
	{// �ޏ�͈�
		SetState(State::STATE_LEAVE);
		m_bWorking = false;
		return;
	}

	if (!m_bWorking &&
		m_state == State::STATE_NONE &&
		rPos.x >= pos.x - ENABLERANGE)
	{// �o��͈�
		SetState(State::STATE_APPEARANCE);
		m_bWorking = true;
	}
	
}

//==========================================================================
// �v���C���[�Ƃ̓����蔻��
//==========================================================================
bool CStageObj::Collision(const D3DXMATRIX& rMtx, const D3DXVECTOR3& rSize, const CPlayer::PLAYERSTATE state)
{
	return false;
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CStageObj::UpdateState()
{
	// ��ԃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԕʏ���
	(this->*(m_SampleFuncList[m_state]))();
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CStageObj::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// �o��
//==========================================================================
void CStageObj::StateAppearance()
{
	MyLib::Vector3 posOrigin = GetOriginPosition();	// �����ʒu
	MyLib::Vector3 posDest = GetOriginPosition();	// �ڕW�ʒu
	posDest.y = 0.0f;

	// ���`��Ԃňړ�
	MyLib::Vector3 pos = posOrigin;
	pos.y = UtilFunc::Correction::EaseOutBack(posOrigin.y, posDest.y, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetPos(pos);

	// ���`��ԂŊg�k
	float scale = UtilFunc::Correction::EaseOutBack(0.0f, 1.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	
	if (m_pModel != nullptr)
	{
		m_pModel->SetScale(scale);
	}
}

//==========================================================================
// �ޏ�
//==========================================================================
void CStageObj::StateLeave()
{
	MyLib::Vector3 posOrigin = GetOriginPosition();	// �����ʒu
	MyLib::Vector3 posDest = GetOriginPosition();	// �ڕW�ʒu
	posDest.y = 0.0f;

	// ���`��Ԃňړ�
	MyLib::Vector3 pos = posOrigin;
	pos.y = UtilFunc::Correction::EaseInBack(0.0f, posOrigin.y, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	SetPos(pos);

	// ���`��ԂŊg�k
	float scale = UtilFunc::Correction::EaseInBack(1.0f, 0.0f, 0.0f, StateTime::APPEARANCE, m_fStateTime);
	
	if (m_pModel != nullptr)
	{
		m_pModel->SetScale(scale);
	}

	if (m_fStateTime >= StateTime::APPEARANCE)
	{// ���Ԍo��
		Kill();
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CStageObj::SetState(const State& state)
{
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_state = state;		// ���
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStageObj::Draw()
{
	if (m_pModel != nullptr)
	{// ���f���̕`��
		m_pModel->Draw();
	}
}

