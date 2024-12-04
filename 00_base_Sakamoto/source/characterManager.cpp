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
// �L�����N�^�[�̔ԍ��w��
//====================================================================
int CCharacterManager::Regist(CObjectCharacter* pObjCharacter, const char* pFilename)
{
	// �L�����N�^�[�t�@�C����������
	for (int nCntCharacter = 0; nCntCharacter < m_nNumAll; nCntCharacter++)
	{
		if (strcmp(&m_aCharacterInfo[nCntCharacter].acFileName[0], pFilename) == 0)
		{
			// ���f���E���[�V������������
			SetModelData(pObjCharacter, m_nNumAll);
			SetMotionData(pObjCharacter, m_nNumAll);

			return nCntCharacter;
		}
	}

	// �ǂݍ��݊�������
	if (m_nNumAll < MAX_CHARACTER)
	{
		m_nNumAll++;

		// ���f���ǂݍ���
		if (LoadModel(pFilename, m_nNumAll))
		{
			// ���f����������
			SetModelData(pObjCharacter,m_nNumAll);
		}
		else
		{
			// ���s
			assert(("���f���ǂݍ��ݎ��s", false));
		}

		// ���[�V�����ǂݍ���
		if (LoadMotion(pFilename, m_nNumAll))
		{
			// ���[�V������������
			SetMotionData(pObjCharacter, m_nNumAll);
		}
		else
		{
			// ���s
			assert(("���[�V�����ǂݍ��ݎ��s", false));
		}
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
	// ���f���Ǘ����擾
	ModelManager ModelManager = m_aCharacterInfo[nNumCharacter].ModelManager;
	
	// ���f���g�p�����擾�E�ݒ�
	int nNumModel = ModelManager.nNumModel;
	pObjCharacter->SetNumModel(nNumModel);

	CModel* apModel[MAX_MODEL_PARTS] = {};

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// �p�[�c�����擾
		ModelParts ModelParts = ModelManager.aModelParts[nCnt];

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
		apModel[nCnt]->SetPos(ModelParts.pos);
		apModel[nCnt]->SetRot(ModelParts.rot);

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
	MotionManager MotionManager = m_aCharacterInfo[nNumCharacter].MotionManager;

	// ���[�V�����g�p�����擾�E�ݒ�
	int nNumMotion = MotionManager.nNumMotion;

	// ���f���g�p���̎擾
	int nNumModel = pObjCharacter->GetNumModel();

	CMotion* pMotion = new CMotion;

	if (pMotion != nullptr)
	{
		// �擪���f�����擾
		CModel* pModel = pObjCharacter->GetModel(0);

		// ���[�V�����̃��f���R�Â�
		pMotion->SetModel(&pModel, nNumModel);
	}

	for (int nCntMotion = 0; nCntMotion < nNumMotion; nCntMotion++)
	{
		// �p�[�c�����擾
		MotionInfo MotionInfo = MotionManager.aMotionInfo[nCntMotion];

		// ���[�v�E�ő�L�[����ݒ�
		pMotion->SetInfoLoop(MotionInfo.bLoop, nCntMotion);
		pMotion->SetInfoNumKey(MotionInfo.nNumKey, nCntMotion);

		for (int nCntKey = 0; nCntKey < MotionInfo.nNumKey; nCntKey++)
		{
			// �L�[���Ƃ̃t���[������ݒ�
			pMotion->SetInfoKeyFrame(MotionInfo.aKeyInfo[nCntKey].nFrame, nCntMotion, nCntKey);

			for (int nCntParts = 0; nCntParts < nNumModel; nCntParts++)
			{
				// �p�[�c���Ƃ̈ʒu�E������ݒ�
				pMotion->SetInfoPartsPosX(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].pos, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsPosY(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].pos, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsPosZ(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].pos, nCntMotion, nCntKey, nCntParts);

				pMotion->SetInfoPartsRotX(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].rot, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsRotY(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].rot, nCntMotion, nCntKey, nCntParts);
				pMotion->SetInfoPartsRotZ(MotionInfo.aKeyInfo[nCntKey].aKey[nCntParts].rot, nCntMotion, nCntKey, nCntParts);
			}
		}
	}
}

//====================================================================
// ���f�����[�h����
//====================================================================
bool CCharacterManager::LoadModel(const char* pFileName, int nNumCharacter)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		int nCntModel = 0;

		int nNumModel = 0;
		ModelParts ModelParts[MAX_MODEL_PARTS] = {};

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

				for (int nCnt = 0; nCnt < nCntModel; nCnt++)
				{
					// ���f���Ǘ��Ƀp�[�c�����
					m_aCharacterInfo[nNumCharacter].ModelManager.aModelParts[nCnt] = ModelParts[nCntModel];
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
bool CCharacterManager::LoadMotion(const char* pFileName, int nNumCharacter)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		int nCntMotion = 0;
		int nCntKeyInfo = 0;
		int nCntKey = 0;

		MotionInfo MotionInfo[MAX_MOTION] = {};

		char aString[128] = {};			//�S�~��
		char aMessage[128] = {};		//�X�^�[�g���b�Z�[�W
		char aBool[128] = {};			//bool�ϊ��p���b�Z�[�W

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
										fscanf(pFile, "%d", &MotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame);	//�L�[�t���[����ݒ�
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
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].pos.x);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].pos.y);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].pos.z);	//�ʒu��ݒ�
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].rot.x);	//������ݒ�
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].rot.y);	//������ݒ�
												fscanf(pFile, "%f", &MotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntKey].rot.z);	//������ݒ�
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntKey++;

										if (nCntKey >= nCntMotion)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeyInfo++;

								if (nCntKeyInfo >= MotionInfo[nCntMotion].nNumKey)
								{
									break;
								}
							}
						}
					}

					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeyInfo = 0;
						nCntMotion++;
					}
					else if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
					{
						// �G���[���p�i���������j
						int nNumMotion = 0;

						// ���f���Ǘ��Ƀ��[�V���������
						m_aCharacterInfo[nNumCharacter].MotionManager.nNumMotion = nNumMotion;

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