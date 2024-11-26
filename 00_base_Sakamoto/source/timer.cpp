//============================================
//
//	�^�C���̏��� [timer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "timer.h"
#include "number.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float SPACE_TIME = 40.0f;		//�^�C�����m�̌���
	const D3DXVECTOR3 TIME_POS = D3DXVECTOR3(540.0f, 40.0f, 0.0f);		//�^�C���̈ʒu
}

//�ÓI�����o�ϐ��錾
CNumber* CTimer::m_apObject[NUM_TIME] = {};
D3DXVECTOR3 CTimer::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CTimer::m_nTime = 0;
int CTimer::m_nCount = 0;
int CTimer::m_StartTime = 0;
int CTimer::m_nNumber[NUM_TIME] = { 0 };
bool CTimer::m_StopTime = false;

//====================================================================
//�R���X�g���N�^
//====================================================================
CTimer::CTimer(int nPriority) : CObject(nPriority)
{

}

//====================================================================
//�f�X�g���N�^
//====================================================================
CTimer::~CTimer()
{

}

//====================================================================
//��������
//====================================================================
CTimer* CTimer::Create()
{
	CTimer* pTimer = nullptr;

	if (pTimer == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pTimer = new CTimer();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pTimer->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pTimer;
}

//====================================================================
//�^�C���̏�����
//====================================================================
HRESULT CTimer::Init(void)
{
	SetType(CObject::TYPE_TIME);

	SetPos(TIME_POS);

	SetTime(0);

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			//�����̐���
			m_apObject[nCntObject] = CNumber::Create();
			m_apObject[nCntObject]->SetNumber(0);

			if (nCntObject < 2)
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_TIME) - 7.0f, m_pos.y, m_pos.z));
				m_apObject[nCntObject]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
			}
			else if (nCntObject < 4)
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_TIME), m_pos.y, m_pos.z));
				m_apObject[nCntObject]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
			}
			else
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * (SPACE_TIME - 5.0f)) + 20.0f, m_pos.y + 5.0f, m_pos.z));
				m_apObject[nCntObject]->SetSize(D3DXVECTOR3(40.0f, 40.0f, 0.0f));
			}

			m_apObject[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
	return S_OK;
}

//====================================================================
//�^�C���̏I��
//====================================================================
void CTimer::Uninit(void)
{
	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		m_apObject[nCntObject]->Uninit();
		m_apObject[nCntObject] = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//�����|�C���^�̏I��
//====================================================================
void CTimer::SetNULL(void)
{
	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		m_apObject[nCntObject] = nullptr;
	}
}

//====================================================================
//�^�C���̍X�V
//====================================================================
void CTimer::Update(void)
{
	int nTime = m_nTime / 1000;

	//��
	m_nNumber[0] = (nTime / 60) % 100 / 10;
	m_nNumber[1] = (nTime / 60) % 10 / 1;
	nTime %= 60;

	//�b
	m_nNumber[2] = (nTime) % 100 / 10;
	m_nNumber[3] = (nTime) % 10 / 1;

	//�~���b
	m_nNumber[4] = (m_nTime) % 1000 / 100;
	m_nNumber[5] = (m_nTime) % 100 / 10;

	//m_nNumber[0] = m_nTime % 10000000 / 1000000;
	//m_nNumber[1] = m_nTime % 1000000 / 100000;
	//m_nNumber[2] = m_nTime % 100000 / 10000;
	//m_nNumber[3] = m_nTime % 10000 / 1000;
	//m_nNumber[4] = m_nTime % 1000 / 100;
	//m_nNumber[5] = m_nTime % 100 / 10;

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		m_apObject[nCntObject]->SetNumber(m_nNumber[nCntObject]);
	}
}

//====================================================================
//�^�C���̕`��
//====================================================================
void CTimer::Draw(void)
{

}

//====================================================================
//�^�C���̕`��
//====================================================================
void CTimer::FloatSetTime(int Time)
{
	if (m_StopTime == false)
	{
		m_nTime = Time - m_StartTime;
	}
}

//====================================================================
//�^�C���̕`��
//====================================================================
void CTimer::SetStartTime(int Time)
{
	m_StartTime = Time;

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{
			m_apObject[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//====================================================================
//�^�C���̏��擾
//====================================================================
int CTimer::GetTimeNumber(void)
{
	int nTime = m_nTime / 1000;
	int ReturnTime = 0;

	//��
	ReturnTime += ((nTime / 60) % 100 / 10) * 100000;
	ReturnTime += ((nTime / 60) % 10 / 1) * 10000;
	nTime %= 60;

	//�b
	ReturnTime += ((nTime) % 100 / 10) * 1000;
	ReturnTime += ((nTime) % 10 / 1) * 100;

	//�~���b
	ReturnTime += ((m_nTime) % 1000 / 100) * 10;
	ReturnTime += ((m_nTime) % 100 / 10);

	return ReturnTime;
}