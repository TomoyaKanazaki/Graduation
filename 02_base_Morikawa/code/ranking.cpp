//========================================
//
//  ���U���g [ranking.h]
//  Author�F���n�Ή�
//
//========================================
#include "ranking.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "texture.h"
#include "sound.h"
#include "timer.h"
#include "resultscore.h"
#include "calculation.h"

namespace
{
	const int NUM = 3;
	const std::string TEXTURE[] = 
		
	{
		"data\\TEXTURE\\1.png",
		"data\\TEXTURE\\2.png",
		"data\\TEXTURE\\3.png",
	};
}

//=======================================
//�R���X�g���N�^
//=======================================
CRanking::CRanking() : 
m_bNewRecord(false)		// �j���[���R�[�h
{
}

//=======================================
//�f�X�g���N�^
//=======================================
CRanking::~CRanking()
{
}

//=======================================
//����
//=======================================
CRanking* CRanking::Create(float nowScore)
{
	//�^�C�g���̃|�C���^
	CRanking* pTutorial = nullptr;

	//�C���X�^���X����
	pTutorial = new CRanking;

	if (pTutorial != nullptr)
	{
		// ����̃X�R�A
		pTutorial->m_fNowScore = nowScore;
		pTutorial->Init();
	}

	//�|�C���^��Ԃ�
	return pTutorial;
}

//=======================================
//������
//=======================================
HRESULT CRanking::Init(void)
{
	
	Load();
	Sort();
	Save();

	for (int i = 0; i < 3; i++)
	{
		m_pRankNum[i] = CObject2D::Create();

		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(TEXTURE[i]);
		m_pRankNum[i]->BindTexture(texID);

		// �T�C�Y�ݒ�
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 180.0f);
#endif
		m_pRankNum[i]->SetSize(size.x, size.y);
		m_pRankNum[i]->SetSizeOrigin(size);

		// �ʒu
		m_pRankNum[i]->SetPos(MyLib::Vector3(200.0f + i * 400.0f, 480.0f, 0.0f));
	}

	for (int i = 0; i < 3; i++)
	{
		m_pScore[i] = CResultScore::Create(MyLib::Vector3(200.0f + i * 400.0f, 550.0f, 0.0f), m_fRanking[i], 50.0f);
	}

	//������Ԃ�
	return S_OK;
}

//=======================================
// �I��
//=======================================
void CRanking::Uninit(void)
{
}

//=======================================
// �X�V
//=======================================
void CRanking::Update(void)
{
	
}

//=======================================
// �`��
//=======================================
void CRanking::Draw(void)
{
}

//=======================================
// �`��
//=======================================
void CRanking::Sort()
{
	// �~������
	for (int nCntData = 0; nCntData < NUM - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < NUM; nCntSort++)
		{
			if (m_fRanking[nCntData] < m_fRanking[nCntSort])
			{// �v�f1���v�f2���傫��������

				int nTemp = m_fRanking[nCntData];
				m_fRanking[nCntData] = m_fRanking[nCntSort];
				m_fRanking[nCntSort] = nTemp;
			}
		}
	}

	// �����L���O�ő�l��������̂�������������
	if (m_fRanking[NUM - 1] < m_fNowScore)
	{// ����ւ�

		m_fRanking[NUM - 1] = m_fNowScore;	// �ŏ��l�ƍ���̃f�[�^����ւ�
		m_bNewRecord = true;	// �t���OON
	}

	// �~������
	for (int nCntData = 0; nCntData < NUM - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < NUM; nCntSort++)
		{
			if (m_fRanking[nCntData] < m_fRanking[nCntSort])
			{// �v�f1���v�f2���傫��������

				int nTemp = m_fRanking[nCntData];
				m_fRanking[nCntData] = m_fRanking[nCntSort];
				m_fRanking[nCntSort] = nTemp;
			}
		}
	}
}

//=======================================
// �Z�[�u
//=======================================
void CRanking::Save()
{
	// �t�@�C�����J��
	std::ofstream File("data\\FILE\\ranking.bin", std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// ��������
	File.write(reinterpret_cast<char*>(m_fRanking), sizeof(m_fRanking));

	// �t�@�C�������
	File.close();
}

//=======================================
// ���[�h
//=======================================
void CRanking::Load()
{
	// �t�@�C�����J��
	std::ifstream File("data\\FILE\\ranking.bin", std::ios::binary);
	if (!File.is_open()) {
		
		m_fRanking[0] = 20.0f;	// �����L���O
		m_fRanking[1] = 15.0f;	// �����L���O
		m_fRanking[2] = 10.0f;	// �����L���O

		return;
	}

	// ��������
	File.read(reinterpret_cast<char*>(m_fRanking), sizeof(m_fRanking));

	// �t�@�C�������
	File.close();
}

