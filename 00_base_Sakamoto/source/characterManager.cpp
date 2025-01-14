//============================================
//
//	�L�����N�^�[�̊Ǘ� [characterManager.cpp]
//	Author:sakai minato
//
//============================================

#include "characterManager.h"

#include "renderer.h"
#include "manager.h"

#include "objectcharacter.h"
#include "model.h"
#include "motion.h"

// �萔��`
namespace
{
	const int MAX_MODEL_PARTS = 32;	// ���f���p�[�c���̍ő吔
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CCharacterManager::CCharacterManager()
{
	ZeroMemory(&m_aCharacterInfo[0], sizeof(m_aCharacterInfo));
	m_nNumAll = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CCharacterManager::~CCharacterManager()
{

}

//====================================================================
//�ǂݍ���
//====================================================================
HRESULT CCharacterManager::Load(void)
{
	return S_OK;
}

//====================================================================
//�j��
//====================================================================
void CCharacterManager::Unload(void)
{
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		// ���擾
		int nMaxParts = m_aCharacterInfo[nCnt].ModelManager.nNumModel;		// �p�[�c��
		int nMaxMotion = m_aCharacterInfo[nCnt].MotionManager.nNumMotion;	// ���[�V������
		
		// ���[�V�����p�[�c�̊J������
		for (int nCntMotion = 0; nCntMotion < nMaxMotion; nCntMotion++)
		{
			// ���擾
			int nMaxKey = m_aCharacterInfo[nCnt].MotionManager.aMotionInfo[nCntMotion].nNumKey;	// �L�[��

			for (int nCntKey = 0; nCntKey < nMaxKey; nCntKey++)
			{
				// �L�[���̊J������
				delete[] m_aCharacterInfo[nCnt].MotionManager.aMotionInfo[nCntMotion].aKeyManager[nCntKey].apKey;
				m_aCharacterInfo[nCnt].MotionManager.aMotionInfo[nCntMotion].aKeyManager[nCntKey].apKey = nullptr;
			}
		}

		// ���f���p�[�c���̊J������
		delete[] m_aCharacterInfo[nCnt].ModelManager.apModelParts;
		m_aCharacterInfo[nCnt].ModelManager.apModelParts = nullptr;
	}
}

//====================================================================
// �L�����N�^�[�̔ԍ��w��
//====================================================================
int CCharacterManager::Regist(CObjectCharacter* pObjCharacter, const std::string pFilename)
{
	// �L�����N�^�[�t�@�C����������
	for (int nCntCharacter = 0; nCntCharacter < m_nNumAll; nCntCharacter++)
	{
		if (strcmp(&m_aCharacterInfo[nCntCharacter].acFileName[0], pFilename.c_str()) == 0)
		{
			// ���f���E���[�V������������
			SetModelData(pObjCharacter, nCntCharacter);
			SetMotionData(pObjCharacter, nCntCharacter);

			return nCntCharacter;
		}
	}

	// �ǂݍ��݊�������
	if (m_nNumAll < MAX_CHARACTER)
	{
		int nCharacterNum = m_nNumAll;

		// ���f���ǂݍ���
		if (LoadModel(pFilename, nCharacterNum))
		{
			// ���f����������
			SetModelData(pObjCharacter, nCharacterNum);
		}
		else
		{
			// ���s
			assert(("���f���ǂݍ��ݎ��s", false));
		}

		// ���f�������擾
		int nNumModel = pObjCharacter->GetNumModel();

		// ���[�V�����ǂݍ���
		if (LoadMotion(pFilename, nNumModel, nCharacterNum))
		{
			// ���[�V������������
			SetMotionData(pObjCharacter, nCharacterNum);
		}
		else
		{
			// ���s
			assert(("���[�V�����ǂݍ��ݎ��s", false));
		}

		// �t�@�C��������
		strcpy(&m_aCharacterInfo[m_nNumAll].acFileName[0], pFilename.c_str());

		m_nNumAll++;

		return nCharacterNum;
	}
	else
	{
		assert(("�L�����N�^�[�Ǘ��̍ő吔�I�[�o�[", false));
	}

	return -1;
}

//====================================================================
// ���f����������
//====================================================================
void CCharacterManager::SetModelData(CObjectCharacter* pObjCharacter,int nNumCharacter)
{	
	// ���f���g�p�����擾�E�ݒ�
	int nNumModel = m_aCharacterInfo[nNumCharacter].ModelManager.nNumModel;
	pObjCharacter->SetNumModel(nNumModel);

	CModel* apModel[MAX_MODEL_PARTS] = {};

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// �p�[�c�����擾
		SModelParts ModelParts = m_aCharacterInfo[nNumCharacter].ModelManager.apModelParts[nCnt];

		// ���f���̐�������
		if (FAILED(apModel[nCnt] = CModel::Create(&ModelParts.acModelFileName[0]))) { assert(false); }
		apModel[nCnt]->SetColorType(CModel::COLORTYPE_FALSE);

		// �e�q�t���ݒ�
		if (ModelParts.nParent == -1)
		{
			apModel[nCnt]->SetParent(nullptr);
		}
		else
		{
			apModel[nCnt]->SetParent(apModel[ModelParts.nParent]);
		}

		// �ʒu�ƌ�����ݒ�
		apModel[nCnt]->SetStartPos(ModelParts.pos);
		apModel[nCnt]->SetStartRot(ModelParts.rot);

		// ���f�����L�����N�^�[�N���X�ɑ��
		pObjCharacter->SetModel(apModel[nCnt], nCnt);
	}
}

//====================================================================
// ���[�V������������
//====================================================================
void CCharacterManager::SetMotionData(CObjectCharacter* pObjCharacter, int nNumCharacter)
{
	// ���[�V�����Ǘ����擾
	SMotionManager MotionManager = m_aCharacterInfo[nNumCharacter].MotionManager;

	// ���[�V�����g�p�����擾�E�ݒ�
	int nNumMotion = MotionManager.nNumMotion;

	// ���f���g�p���̎擾
	int nNumModel = pObjCharacter->GetNumModel();

	CMotion* pMotion = new CMotion;

	if (pMotion != nullptr)
	{
		// �擪���f�����擾
		CModel** pModel = pObjCharacter->GetModel();

		// ���[�V�����̃��f���R�Â�
		pMotion->SetModel(pModel, nNumModel);
	}

	for (int nCntMotion = 0; nCntMotion < nNumMotion; nCntMotion++)
	{
		// �p�[�c�����擾
		SMotionInfo MotionInfo = MotionManager.aMotionInfo[nCntMotion];

		// ���[�v�E�ő�L�[����ݒ�
		pMotion->SetInfoLoop(MotionInfo.bLoop, nCntMotion);
		pMotion->SetInfoNumKey(MotionInfo.nNumKey, nCntMotion);

		for (int nCntKey = 0; nCntKey < MotionInfo.nNumKey; nCntKey++)
		{
			// �L�[�Ǘ������擾
			SKeyManager KeyManager = MotionInfo.aKeyManager[nCntKey];

			// �L�[���Ƃ̃t���[������ݒ�
			pMotion->SetInfoKeyFrame(KeyManager.nFrame, nCntMotion, nCntKey);

			for (int nCntParts = 0; nCntParts < nNumModel; nCntParts++)
			{
				// �L�[�����擾
				SKey Key = KeyManager.apKey[nCntParts];

				// �p�[�c���Ƃ̈ʒu�E������ݒ�
				pMotion->SetInfoPartsPosX(Key.pos.x, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsPosY(Key.pos.y, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsPosZ(Key.pos.z, nCntMotion, nCntKey, nCntParts);

				pMotion->SetInfoPartsRotX(Key.rot.x, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsRotY(Key.rot.y, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsRotZ(Key.rot.z, nCntMotion, nCntKey, nCntParts);
			}
		}
	}

	// ���f�����L�����N�^�[�N���X�ɑ��
	pObjCharacter->SetMotion(pMotion);
}

//====================================================================
// ���f�����[�h����
//====================================================================
bool CCharacterManager::LoadModel(const std::string pFilename,int nNumCharacter)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pFilename.c_str(), "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		int nCntModel = 0;

		int nNumModel = 0;
		SModelParts ModelParts[MAX_MODEL_PARTS] = {};

		char aString[128] = {};				//�S�~��
		char aMessage[128] = {};			//�X�^�[�g�ƃG���h�̃��b�Z�[�W

		// �ǂݍ��݊J�n-----------------------------------------------------
		while (1)
		{//�uSCRIPT�v��T��
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// ���f�����ǂݍ���-----------------------------------------------------
				while (1)
				{//�uNUM_MODEL�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &nNumModel);		//���f�����̐ݒ�
						break;
					}
				}

				//���f���t�@�C���̓ǂݍ���
				while (1)
				{//�uMODEL_FILENAME�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelParts[nCntModel].acModelFileName[0]);		//�ǂݍ��ރ��f���̃p�X���擾

						nCntModel++;
					}
					if (nCntModel >= nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// �L�����N�^�[���ǂݍ���-----------------------------------------------------
				while (1)
				{//�uPARTSSET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParts[nCntModel].nIndex);		//�C���f�b�N�X��ݒ�
								nCntModel = ModelParts[nCntModel].nIndex;
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParts[nCntModel].nParent);	//�e���f���̃C���f�b�N�X��ݒ�
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelParts[nCntModel].pos.x);	// �ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelParts[nCntModel].pos.y);	// �ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelParts[nCntModel].pos.z);	// �ʒu(�I�t�Z�b�g)�̏����ݒ�
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelParts[nCntModel].rot.x);	// �����i�I�t�Z�b�g�j�̏����ݒ�
								fscanf(pFile, "%f", &ModelParts[nCntModel].rot.y);	// �����i�I�t�Z�b�g�j�̏����ݒ�
								fscanf(pFile, "%f", &ModelParts[nCntModel].rot.z);	// �����i�I�t�Z�b�g�j�̏����ݒ�
							}

							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}

						nCntModel++;

						if (nCntModel >= nNumModel)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				// ���f���Ǘ��ɍő僂�f�������
				m_aCharacterInfo[nNumCharacter].ModelManager.nNumModel = nNumModel;

				// ���I�m��
				m_aCharacterInfo[nNumCharacter].ModelManager.apModelParts = new SModelParts[nNumModel];

				for (int nCnt = 0; nCnt < nCntModel; nCnt++)
				{
					// ���f���Ǘ��Ƀp�[�c�����
					m_aCharacterInfo[nNumCharacter].ModelManager.apModelParts[nCnt] = ModelParts[nCnt];
				}

				break;
			}
		}

		fclose(pFile);

		return true;
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");

		return false;
	}
}

//====================================================================
// ���[�V�����̃��[�h����
//====================================================================
bool CCharacterManager::LoadMotion(const std::string pFileName, int nNumModel, int nNumCharacter)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pFileName.c_str(), "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		int nCntMotion = 0;
		int nCntKeyManager = 0;
		int nCntParts = 0;
		int nMaxParts = m_aCharacterInfo[nNumCharacter].ModelManager.nNumModel;

		SMotionInfo MotionInfo[MAX_MOTION] = {};

		char aString[128] = {};			//�S�~��
		char aMessage[128] = {};		//�X�^�[�g���b�Z�[�W
		char aBool[128] = {};			//bool�ϊ��p���b�Z�[�W

		// ���f���̃p�[�c���̗L������
		if (nMaxParts == 0)
		{
			return false;
		}

		// �ǂݍ��݊J�n-----------------------------------------------------
		while (1)
		{//�uSCRIPT�v��T��
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// ���[�V�������ǂݍ���-----------------------------------------------------
				while (1)
				{//�uMOTIONSET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MOTIONSET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "LOOP") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%s", &aBool[0]);	//���[�v���邩�ǂ�����ݒ�

								MotionInfo[nCntMotion].bLoop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool�^�̏�����
							}
							if (strcmp(&aMessage[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &MotionInfo[nCntMotion].nNumKey);	//�L�[�̑�����ݒ�
								break;
							}
						}

						// �L�[���ǂݍ���-----------------------------------------------------
						while (1)
						{//�uKEYSET�v��T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "KEYSET") == 0)
							{
								while (1)
								{//�uFRAME�v��T��
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].nFrame);	//�L�[�t���[����ݒ�

										// �L�[�v�f�̃p�[�c�����I�m��
										MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey = new SKey[nMaxParts];

										break;
									}
								}

								while (1)
								{//�uKEY�v��T��
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "KEY") == 0)
									{
										while (1)
										{//�e��ϐ���T��
											fscanf(pFile, "%s", &aMessage[0]);
											if (strcmp(&aMessage[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].pos.x);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].pos.y);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].pos.z);	//�ʒu��ݒ�
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].rot.x);	//������ݒ�
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].rot.y);	//������ݒ�
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyManager[nCntKeyManager].apKey[nCntParts].rot.z);	//������ݒ�
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntParts++;

										if (nCntParts >= nNumModel)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntParts = 0;
								nCntKeyManager++;

								if (nCntKeyManager >= MotionInfo[nCntMotion].nNumKey)
								{
									break;
								}
							}
						}
					}

					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeyManager = 0;
						nCntMotion++;
					}
					else if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
					{
						// ���f���Ǘ��Ƀ��[�V���������
						m_aCharacterInfo[nNumCharacter].MotionManager.nNumMotion = nCntMotion;

						for (int nCnt = 0; nCnt < nCntMotion; nCnt++)
						{
							// ���[�V�����Ǘ��Ƀ��[�V���������
							m_aCharacterInfo[nNumCharacter].MotionManager.aMotionInfo[nCnt] = MotionInfo[nCnt];
						}

						break;
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);

		return true;
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");

		return false;
	}
}