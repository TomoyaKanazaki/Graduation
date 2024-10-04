//==========================================
//
//  ���U���g�������N���X(resultscore.cpp)
//  Author : ���n�Ή�
//
//==========================================
#include "resultscore.h"
#include "number.h"
#include "manager.h"
#include "input.h"
#include "time.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const int DIGIT = 5; // �����l
	const float INIT_TIME = 30.0f; // �����l
	const D3DXVECTOR2 NUMBER_SIZE = D3DXVECTOR2(50.0f, 50.0f); // �T�C�Y
	const D3DXVECTOR3 NUMBER_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 50.0f, 0.0f); // ���W
}

//==========================================
//  �R���X�g���N�^
//==========================================
CResultScore::CResultScore() :
	m_fTimer(0.0f)
{
}

//==========================================
//  �f�X�g���N�^
//==========================================
CResultScore::~CResultScore()
{
}

//==========================================
//  ����������
//==========================================
HRESULT CResultScore::Init()
{
	D3DXVECTOR3 pos = GetPos() + MyLib::Vector3(-m_fSize * 0.5f * DIGIT, 0.0f, 0.0f);

	for (int i = 0; i < DIGIT; ++i)
	{
		m_pNumber[i] = CNumber::Create();
		m_pNumber[i]->SetSize(m_fSize, m_fSize);
		pos.x += m_fSize * 0.8f;

		if (i % 2 == 0)
		{
			pos.x += m_fSize * 0.25f;
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
void CResultScore::Uninit()
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
}

//==========================================
//  �X�V����
//==========================================
void CResultScore::Update()
{
	// �v�Z
	CalcNum();
}

//==========================================
//  �`�揈��
//==========================================
void CResultScore::Draw()
{
}

//==========================================
//  ��������
//==========================================
CResultScore* CResultScore::Create(const MyLib::Vector3& pos, float time, float size)
{
	// �|�C���^���m��
	CResultScore* pTime = new CResultScore;

	// null�`�F�b�N
	if (pTime == nullptr) { assert(false); return nullptr; }

	// ����������
	pTime->SetPos(pos);
	pTime->m_fTimer = time;
	pTime->m_fSize = size;	// �T�C�Y
	pTime->Init();

	return pTime;
}

//==========================================
//  �v�Z
//==========================================
void CResultScore::CalcNum()
{

	// �^�C�}�[�𐮐��l�ɕϊ�
	int nCalc = (float)(m_fTimer * 1000.0f);

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
