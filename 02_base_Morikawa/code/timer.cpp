//==========================================
//
//  �^�C�}�[�N���X(timer.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "timer.h"
#include "number.h"
#include "manager.h"
#include "input.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float INIT_TIME = 60.0f; // �����l
	const D3DXVECTOR3 NUMBER_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.72f, 50.0f, 0.0f); // ���W
}

//==========================================
//  �ÓI�����o�ϐ��錾
//==========================================
float CTimer::m_fTimer = 0.0f;

//==========================================
//  �R���X�g���N�^
//==========================================
CTimer::CTimer()
{
}

//==========================================
//  �f�X�g���N�^
//==========================================
CTimer::~CTimer()
{
}

//==========================================
//  ����������
//==========================================
HRESULT CTimer::Init()
{
	// �����l��ݒ�
	m_fTimer = INIT_TIME;

	// �����𐶐�

	D3DXVECTOR3 pos = NUMBER_POS;

	for (int i = 0; i < DIGIT; ++i)
	{
		m_pNumber[i] = CNumber::Create();
		m_pNumber[i]->SetSize(60.0f, 60.0f);
		pos.x += 55.0f;

		if (i % 2 == 0)
		{
			pos.x += 15.0f;
		}

		m_pNumber[i]->SetPos(pos);
		m_pNumber[i]->SetPosVertex();
	}

	// �v�Z
	CalcNum();

	return S_OK;
}

//==========================================
//  �I������
//==========================================
void CTimer::Uninit()
{
	// �����̏I��
	for (int i = 0; i < DIGIT; ++i)
	{
		if (m_pNumber[i] != nullptr)
		{
			m_pNumber[i]->Uninit();
			m_pNumber[i] = nullptr;
		}
	}

	this->Release();
}

//==========================================
//  �X�V����
//==========================================
void CTimer::Update()
{
	// �^�C�}�[������
	m_fTimer -= CManager::GetInstance()->GetDeltaTime();

	// �l��␳
	if (m_fTimer < 0.0f)
	{
		m_fTimer = 0.0f;
	}

	// �v�Z
	CalcNum();
}

//==========================================
//  �`�揈��
//==========================================
void CTimer::Draw()
{
}

//==========================================
//  �^�C���A�b�v���Ă锻��
//==========================================
bool CTimer::GetTimeZero()
{
	return m_fTimer <= 0.0f;
}

//==========================================
//  ��������
//==========================================
CTimer* CTimer::Create()
{
	// �|�C���^���m��
	CTimer* pTime = new CTimer;

	// null�`�F�b�N
	if (pTime == nullptr) { assert(false); return nullptr; }

	// ����������
	pTime->Init();

	return pTime;
}

//==========================================
//  �������Ԃ̎擾
//==========================================
float CTimer::GetInitTime()
{
	return INIT_TIME;
}

//==========================================
//  �v�Z
//==========================================
void CTimer::CalcNum()
{
	// �^�C�}�[�𐮐��l�ɕϊ�
	int nCalc = (float)(m_fTimer * 1000.0f);

	DebugProc::Print(DebugProc::POINT_RIGHT, "\n���� : %d\n", nCalc);

	// �������X�V
	for (int i = DIGIT - 1; i >= 0; --i)
	{
		if (m_pNumber[i] != nullptr)
		{
			// ���ɍ��킹�����l���Z�o
			int nNum = nCalc % 10;
			nCalc /= 10;

			// ���l��ݒ�
			m_pNumber[i]->SetNumber(nNum);
		}
	}
}
