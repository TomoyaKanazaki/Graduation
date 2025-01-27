//============================================
//
//	���[�V���� [motion.cpp]
//	Author:sakamoto kai
//
//============================================
#include "motion.h"
#include "model.h"
#include "manager.h"
#include <stdio.h>
#include "MapMove.h"
#include "MapSystem.h"

//===============================================
// �֐��|�C���^
//===============================================
// �Đ���ԊǗ�
CMotion::PLAYTYPE_FUNC CMotion::m_PlayTypeFunc[] =
{
	&CMotion::PlayTypeNormal,		// �ʏ�
	&CMotion::PlayTypeBlend,		// �u�����h
};

//====================================================================
//�R���X�g���N�^
//====================================================================
CMotion::CMotion() :
	// ��̃����o�ϐ�
	m_fSlowVaule(1.0f),		// �X���[�Ɋ|����{��

	// ���̃����o�ϐ�
	m_PlayInfo(SPlayInfo()),
	m_BodyData(SBodyData())
{
	// �l�̃N���A
	for (int i = 0; i < MAX_INFO; i++)
	{
		m_aInfo[i] = INFO();
	}
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMotion::~CMotion()
{

}

//====================================================================
//���[�V�����̐ݒ菈��
//====================================================================
void CMotion::Set(int nType, float BlendTime)
{
	SPlayInfo* pInfo = &m_PlayInfo;			// �Đ����̃|�C���^
	SBlendInfo* pBlend = &pInfo->blendInfo;	// �u�����h���̃|�C���^

	// �u�����h���
	pBlend->fCounter = 0.0f;
	pBlend->fTime = BlendTime;
	pBlend->fTimeMax = pBlend->fTime;
	pBlend->nOldId = pInfo->nId;
	pBlend->nOldKey = pInfo->nKey;

	// �Đ����
	pInfo->nId = nType;					//���[�V�����̎�ނ̐ݒ�
	pInfo->nKey = 0;					//���݂̃L�[�����Z�b�g
	pInfo->nKeyNext = (pInfo->nKey + 1) % m_aInfo[nType].NumKey;	// ���̃L�[�ݒ�
	pInfo->bFinish = false;				// �I���t���O�����Z�b�g
	pInfo->fCounter = 0.0f;				// �Đ��J�E���g

	// �u�����h���Ȃ��ꍇ�͒ʏ�Đ�
	if (BlendTime == 0)
	{
		m_PlayInfo.type = PLAYTYPE::PLAYTYPE_NORMAL;
		return;
	}

	// �u�����h����ꍇ�̓u�����h�Đ�
	m_PlayInfo.type = PLAYTYPE::PLAYTYPE_BLEND;
}

//====================================================================
//���[�V�����̍X�V����
//====================================================================
void CMotion::Update(void)
{
	// �Đ��I���̏ꍇ
	if (m_PlayInfo.bFinish) { return; }

	// �Đ��ݒ�
	(this->*(m_PlayTypeFunc[m_PlayInfo.type]))();
}

//====================================================================
//���f���̃Z�b�g����
//====================================================================
void CMotion::SetModel(CModel** ppModel, int nNumModel)
{
	// �K�w���f������ݒ�
	m_BodyData.ppModel = ppModel;
	m_BodyData.nNumModel = nNumModel;
}

//====================================================================
//���[�V�����̃��[�h����
//====================================================================
void CMotion::LoadData(const std::string pFilename)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(pFilename.c_str(), "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//�S�~��
		char aMessage[128] = {};		//�X�^�[�g���b�Z�[�W
		char aBool[128] = {};				//bool�ϊ��p���b�Z�[�W

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
								m_aInfo[nCntMotion].bLoop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool�^�̏�����
							}
							if (strcmp(&aMessage[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_aInfo[nCntMotion].NumKey);	//�L�[�̑�����ݒ�
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
										fscanf(pFile, "%d", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame);	//�L�[�t���[����ݒ�
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
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosX);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosY);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosZ);	//�ʒu��ݒ�
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotX);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotY);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotZ);	//�ʒu��ݒ�
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntKey++;
										if (nCntKey >= m_BodyData.nNumModel)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeySet++;
								if (nCntKeySet >= m_aInfo[nCntMotion].NumKey)
								{
									break;
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeySet = 0;
						nCntMotion++;
					}
					else if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
					{
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
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

//====================================================================
// �ʏ�Đ�
//====================================================================
void CMotion::PlayTypeNormal()
{
	SPlayInfo* pInfo = &m_PlayInfo;		// �Đ����̃|�C���^
	SBodyData* pBody = &m_BodyData;		// �K�w�f�[�^�̃|�C���^
	if (m_BodyData.ppModel == nullptr) { return; }

	// ���[�J���Ɋi�[
	int id = pInfo->nId;				// �Đ������[�V�����ԍ�
	int key = pInfo->nKey;				// �Đ����L�[�ԍ�
	int keynext = pInfo->nKeyNext;		// ���̃L�[
	float count = pInfo->fCounter;		// �J�E���g


	// �}�b�v�̓��삪���g���̏ꍇ���[�V���������������� : TODO ����Ȃ������������
	CMapMove* pMove = CMapSystem::GetInstance()->GetMove();
	if (pMove->GetScrollType() == CMapMove::SCROLL_TYPE::SCROLL_TYPE_RETRO)
	{
		count = 0.0f;
	}

	//�S���f��(�p�[�c)�̍X�V
	for (int nCntModel = 0; nCntModel < pBody->nNumModel; nCntModel++)
	{
		// ���f���擾
		CModel* pModel = pBody->ppModel[nCntModel];

		//�L�[�̏�񂩂�ʒu�E�������Z�o
		float POSX = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fPosX - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosX;
		float POSY = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fPosY - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosY;
		float POSZ = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fPosZ - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosZ;
		float ROTX = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fRotX - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotX;
		float ROTY = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fRotY - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotY;
		float ROTZ = m_aInfo[id].aKeyInfo[keynext].aKey[nCntModel].fRotZ - m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotZ;

		//�p�[�c�̈ʒu�E������ݒ�
		pModel->SetPos(D3DXVECTOR3
		(
			pModel->GetStartPos().x + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosX + POSX * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame)),
			pModel->GetStartPos().y + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosY + POSY * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame)),
			pModel->GetStartPos().z + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosZ + POSZ * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame))
		));

		pModel->SetRot(D3DXVECTOR3
		(
			pModel->GetStartRot().x + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotX + ROTX * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame)),
			pModel->GetStartRot().y + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotY + ROTY * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame)),
			pModel->GetStartRot().z + m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotZ + ROTZ * (count / ((float)m_aInfo[id].aKeyInfo[key].nFrame))
		));
	}

	// ���݂̍Đ��J�E���g�����Z
	pInfo->fCounter += CManager::GetInstance()->GetGameSpeed() * m_fSlowVaule;

	// ���̃L�[�Ɉړ�
	int frame = m_aInfo[id].aKeyInfo[key].nFrame;
	if (pInfo->fCounter >= static_cast<int>(frame))
	{
		// �J�E���g���Z�b�g
		pInfo->fCounter = 0.0f;

		// �J�E���g�����ɐi�߂�
		pInfo->nKey = (pInfo->nKey + 1) % m_aInfo[pInfo->nId].NumKey;
		pInfo->nKeyNext = (pInfo->nKey + 1) % m_aInfo[pInfo->nId].NumKey;

		// �Đ�������
		if (pInfo->nKey != 0 && pInfo->nKeyNext != 0) { return; }

		// ���[�v���Ȃ��ꍇ�Đ��I��
		if (m_aInfo[id].bLoop) { return; }

		pInfo->bFinish = true;
	}
}

//====================================================================
// �u�����h�Đ�
//====================================================================
void CMotion::PlayTypeBlend()
{
	SPlayInfo* pInfo = &m_PlayInfo;		// �Đ����̃|�C���^
	SBodyData* pBody = &m_BodyData;		// �K�w�f�[�^�̃|�C���^
	SBlendInfo* pBlend = &pInfo->blendInfo;	// �u�����h���̃|�C���^

	if (m_BodyData.ppModel == nullptr) { return; }

	// ���[�J���Ɋi�[
	int id = pInfo->nId;				// �Đ������[�V�����ԍ�
	int key = pInfo->nKey;				// �Đ����L�[�ԍ�
	int blendid = pBlend->nOldId;		// �u�����h�p���[�V�����ԍ�
	int blendkey = pBlend->nOldKey;		// �u�����h�p�L�[�ԍ�

	for (int nCntModel = 0; nCntModel < pBody->nNumModel; nCntModel++)
	{
		CModel* pModel = pBody->ppModel[nCntModel];
		//�L�[�̏�񂩂�ʒu�E�������Z�o
		float POSX = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosX - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosX;
		float POSY = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosY - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosY;
		float POSZ = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fPosZ - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosZ;
		float ROTX = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotX - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotX;
		float ROTY = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotY - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotY;
		float ROTZ = m_aInfo[id].aKeyInfo[key].aKey[nCntModel].fRotZ - m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotZ;

		//�p�[�c�̈ʒu�E������ݒ�
		pModel->SetPos(D3DXVECTOR3
		(

			pModel->GetStartPos().x + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosX + POSX * (pBlend->fCounter / pBlend->fTimeMax),
			pModel->GetStartPos().y + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosY + POSY * (pBlend->fCounter / pBlend->fTimeMax),
			pModel->GetStartPos().z + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fPosZ + POSZ * (pBlend->fCounter / pBlend->fTimeMax)
		));

		pModel->SetRot(D3DXVECTOR3
		(
			pModel->GetStartRot().x + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotX + ROTX * (pBlend->fCounter / pBlend->fTimeMax),
			pModel->GetStartRot().y + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotY + ROTY * (pBlend->fCounter / pBlend->fTimeMax),
			pModel->GetStartRot().z + m_aInfo[blendid].aKeyInfo[blendkey].aKey[nCntModel].fRotZ + ROTZ * (pBlend->fCounter / pBlend->fTimeMax)
		));
	}

	// ���݂̃J�E���g���Z�A�^�C�}�[���Z
	pBlend->fCounter += CManager::GetInstance()->GetGameSpeed() * m_fSlowVaule;
	pBlend->fTime -= CManager::GetInstance()->GetGameSpeed() * m_fSlowVaule;

	// �u�����h�I����f�t�H���g�ɖ߂�
	if (pBlend->fTime <= 0.0f)
	{
		m_PlayInfo.type = PLAYTYPE::PLAYTYPE_NORMAL;
	}
}

//====================================================================
// ���[�v�擾
//====================================================================
bool CMotion::GetInfoLoop(int nMotion)
{
	// �͈͊O
	if (nMotion < 0 || nMotion >= MAX_INFO)
	{
		assert(false);
	}

	return m_aInfo[nMotion].bLoop;
}

//====================================================================
// �L�[�����擾
//====================================================================
int CMotion::GetInfoNumKey(int nMotion)
{
	// �͈͊O
	if (nMotion < 0 || nMotion >= MAX_INFO)
	{
		assert(false);
	}

	return m_aInfo[nMotion].NumKey;
}

//====================================================================
// �w��L�[�̃t���[�����擾
//====================================================================
int CMotion::GetInfoKeyFrame(int nMotion, int nKey)
{
	// �͈͊O
	if (nMotion < 0 || nMotion >= MAX_INFO)
	{
		assert(false);
	};

	// �͈͊O
	if (nKey < 0 || nKey >= MAX_KEY)
	{
		assert(false);
	};

	return m_aInfo[nMotion].aKeyInfo[nKey].nFrame;
}
