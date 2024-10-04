//=============================================================================
// 
//  �X�e�[�W�}�l�[�W���[���� [stagemanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "stagemanager.h"
#include "stageobj.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TXT = "data\\TXT\\Stage.txt";
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStageManager::CStageManager()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStageManager::~CStageManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CStageManager* CStageManager::Create()
{
	// �������̊m��
	CStageManager* pObj = new CStageManager;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStageManager::Init()
{
	// �X�e�[�W���[�h����
	StageLoad();

	for (int i = 0 ; i < 800; i++)
	{
		float distance = UtilFunc::Transformation::Random(-50, 50) * 10.0f;
		CStageObj::CreateBg(MyLib::Vector3(MyLib::Vector3(-2000.0f + i * 200.0f, 0.0f, 1200.0f + distance)));
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStageManager::Uninit()
{
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CStageManager::Update()
{
	
}

//==========================================================================
// �X�e�[�W���[�h
//==========================================================================
HRESULT CStageManager::StageLoad()
{
	// �ϐ���錾
	int nEnd;							// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	std::string str;					// ������
	int nType = 0;						// ���炩�̃^�C�v
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	std::ifstream file(TXT);  // �ǂݍ��ރt�@�C���̃p�X���w��

	// �t�@�C�����J���Ȃ������ꍇ�A���s��Ԃ�
	if (!file.is_open()) { return E_FAIL; }

	while (str != "END_TXT")
	{  // 1�s���ǂݍ���

		file >> str;

		if (str == "#")
		{ // # �̏ꍇ

			// 1�s�ǂݍ���
			std::getline(file, str);
		}

		if (str == "SET_BG")
		{ // �������w�i�������Ă����ꍇ

			while (file >> str)
			{  // 1�s���ǂݍ���

				if (str == "END_SET_BG") { break; }
				if (str == "POS")
				{ // �ʒu��ǂݍ��񂾏ꍇ

					file >> str;
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
			}

			CStageObj::CreateBg(MyLib::Vector3(pos));
		}

		if (str == "SET_OBSTACLE")
		{ // ��������Q���������Ă����ꍇ

			while (file >> str)
			{  // 1�s���ǂݍ���

				if (str == "END_SET_OBSTACLE") { break; }
				if (str == "TYPE")
				{ // ��ނ�ǂݍ��񂾏ꍇ

					file >> str;
					file >> nType;
				}
				else if (str == "POS")
				{ // �ʒu��ǂݍ��񂾏ꍇ

					file >> str;
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
			}

			CStageObj::CreateObstacle(MyLib::Vector3(pos), nType);
		}

		if (str == "SET_REVERSE")
		{ // ���������]�|�C���g�������Ă����ꍇ

			while (file >> str)
			{  // 1�s���ǂݍ���

				if (str == "END_SET_REVERSE") { break; }
				if (str == "POS")
				{ // �ʒu��ǂݍ��񂾏ꍇ

					file >> str;
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
			}

			CStageObj::CreateReverse(MyLib::Vector3(pos));
		}
	}

	// ������Ԃ�
	return S_OK;
}
