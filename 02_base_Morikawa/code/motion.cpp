//========================================
//
//���[�V����[motion.cpp]
//Author�F�X��x��
//
//========================================
#include "motion.h"
#include "debugproc.h"
#include "renderer.h"
#include "manager.h"

//========================================
//�R���X�g���N�^
//========================================
CMotion::CMotion() :
	m_pParent(nullptr),	// �e���f���̃|�C���^
	m_nNumAll(0),		// ���[�V�����̑���
	m_nType(0),			// ���[�V�������
	m_nKey(0),			// ���݂̃L�[��
	m_nCounter(0),		// �t���[���J�E���^�[
	m_bFinish(true),	// �I���������ǂ���
	m_nNumModel(0),		// ���f���̑���
	m_nNumMotion(0),	// ���݂̃��[�V����
	m_nNumParts(0),		// �p�[�c��
	m_nPartsIndex(0),	// �p�[�c�ԍ�
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{//�l�N���A
	memset(&m_aInfo[0], 0, sizeof(m_aInfo));		//���[�V�������
	memset(&m_ppModel[0], 0, sizeof(m_ppModel));	//���f�����
}

//========================================
//�f�X�g���N�^
//========================================
CMotion::~CMotion()
{	
}

//========================================
//����
//========================================
CMotion *CMotion::Create(void)
{
	//CMotion�^�̃|�C���^
	CMotion *pMotion = nullptr;

	if (pMotion == nullptr)
	{//nullptr�̎�
		//�C���X�^���X����
		pMotion = new CMotion;

		//������
		pMotion->Init();
	}

	//�|�C���^��Ԃ�
	return pMotion;
}

//========================================
//������
//========================================
HRESULT CMotion::Init(void)
{
	//�e�l�̏����l�ݒ�
	memset(&m_aInfo[0], 0, sizeof(m_aInfo));		// ���[�V�������
	m_nNumAll = 0;				// ���[�V��������
	m_nType = 0;				// ���[�V�������
	m_nKey = 0;					// ���݂̃L�[�i���o�[
	m_nCounter = 0;				// ���[�V�����J�E���^�[
	m_nNumMotion = 0;			// ���݂̃��[�V����
	m_nNumParts = 0;			// �p�[�c��
	m_bFinish = true;			// �I���������ǂ���

	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{// �_�u���|�C���^�̏�����
		m_ppModel[nCntModel] = nullptr;
	}

	// ���f���̑���
	m_nNumModel = 0;	

	//������Ԃ�
	return S_OK;
}

//========================================
//�I��
//========================================
void CMotion::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{// �_�u���|�C���^�̏�����
		if (m_ppModel[nCntModel] != nullptr)
		{
			m_ppModel[nCntModel]->Uninit();
			delete m_ppModel[nCntModel];
			m_ppModel[nCntModel] = nullptr;
		}
	}
}

//========================================
//�X�V
//========================================
void CMotion::Update(void)
{
	//�ړI�̈ʒu
	D3DXVECTOR3 diffPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ړI�̌���
	D3DXVECTOR3 diffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���̒l
	int nNextKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{//�p�[�c�̍ő啪��
		//���f���̃|�C���^
		CModel *pModel = m_ppModel[nCntParts];

		//�ʒu�擾
		D3DXVECTOR3 pos = pModel->GetPosition();

		//�ʒu�擾
		D3DXVECTOR3 rot = pModel->GetRot();

		//�ʒu���Z�o
		diffPos = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntParts].pos - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].pos;

		//�������Z�o
		diffRot = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntParts].rot - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].rot;

		//�ʒu
		pos = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].pos
				+ diffPos
				* ((float)m_nCounter
				/ (float)m_aInfo[m_nType].aKeyInfo[nNextKey].nFrame);

		//����
		rot = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].rot
				+ diffRot
				* ((float)m_nCounter 
				/ (float)m_aInfo[m_nType].aKeyInfo[nNextKey].nFrame);

		//�ʒu���f
		pModel->SetPosition(pos + pModel->GetPos());

		//�������f
		pModel->SetRot(rot + pModel->GetRotate());
	}

	if (m_nCounter >= m_aInfo[m_nType].aKeyInfo[nNextKey].nFrame)
	{// �J�E���^�[���Đ��t���[���𒴂�����
		if (m_aInfo[m_nType].bLoop == true)
		{// ���[�V���������[�v�̎�
			// ���[�V�����J�E���^�[���Z�b�g
			m_nCounter = 0;

			// ���݂̃L�[���Z
			m_nKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;
		}
		else
		{// ���[�v���Ȃ���
			if (m_nKey < m_aInfo[m_nType].nNumKey)
			{// �L�[�̑��������݂̃L�[�𒴂�����
				// ���[�V�����J�E���^�[���Z�b�g
				m_nCounter = 0;

				// �L�[���Z
				m_nKey++;
			}
			else
			{// �����ĂȂ��Ƃ�
				// ���[�V�������I������
				m_bFinish = true;
			}
		}
	}
	else
	{// �J�E���^�[���Đ��t���[���𒴂��ĂȂ�������
		// ���݂̃L�[��1�i�߂�
		m_nCounter++;
	}
}

//========================================
//�`��
//========================================
void CMotion::Draw(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{//���f���̕`��(�S�p�[�c)
 		if (m_ppModel[nCntParts] != nullptr)
		{//m_apModel��nullptr�̎�
			m_ppModel[nCntParts]->Draw();
		}
	}
}

//========================================
//���[�V������ނ̐ݒ�
//========================================
void CMotion::SetInfo(int nType)
{
	m_nType = nType;
}

//========================================
//�ݒ�
//========================================
void CMotion::Set(int nType)
{
	if (!m_aInfo[m_nType].bLoop && !m_bFinish)
	{
		return;
	}

	if (nType != m_nType)
	{// ���݂̃��[�V�����ƈ������
		m_nType = nType;

		// �L�[���Z�b�g
		m_nKey = 0;

		m_nCounter = 0;

		// �I���󋵂�false�ɂ���
		m_bFinish = false;

		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{// ���f���̑�������
			//�ʒu
			m_ppModel[nCnt]->SetPosition(m_aInfo[nType].aKeyInfo[m_nKey].aKey[nCnt].pos);

			//����
			m_ppModel[nCnt]->SetRot(m_aInfo[nType].aKeyInfo[m_nKey].aKey[nCnt].rot);
		}
	}
}

//========================================
//���f���ݒ�
//========================================
void CMotion::SetModel(CModel *ppModel, int nNumModel)
{
	//�|�C���^
	m_ppModel[nNumModel] = ppModel;

	//�p�[�c�ԍ�
	m_nNumModel++;
}

//========================================
//�ǂݍ���
//========================================
void CMotion::Load(std::string pfile)
{
	//�ϐ��錾
	char garbage[640];		// �s�v�����i�[�p
	char FileName[64];		// �e���f���̃t�@�C����
	float move = 0.0f;		// �ړ���
	int nIdx = 0;			// ���Ԗڂ̃p�[�c��
	int nParent = 0;		// �e�q�֌W
	int nNum = 0;			// �ǂݍ��ޔԍ�
	int nMotion = 0;		// ���Ԗڂ̃��[�V������
	int nCntKey = 0;		// ���ڂ̃L�[��
	int nLoop = 0;			// ���[�v���邩���Ȃ���
	int KeyCount = 0;		// ���[�V�����̃L�[��

	//FILE�̃|�C���^
	FILE *pFile = nullptr;

	//�t�@�C�����J��
	pFile = fopen(pfile.c_str(), "r");

	while (1)
	{//END_SCRIPT������܂ŌJ��Ԃ�
		//�s�v�ȕ������̂Ă�
		fscanf(pFile, "%s", &garbage[0]);

		if (strcmp(&garbage[0], "END_SCRIPT") == 0)
		{//������END_SCRIPT�̂Ƃ�
			break;
		}

		//========================================
		//���f���t�@�C���̓ǂݍ���
		//========================================
		if (strcmp(&garbage[0], "MODEL_FILENAME") == 0)
		{//������MODEL_FILENAME�̂Ƃ�
			//=�̂Ă�
			fscanf(pFile, "%s", &garbage[0]);

			//�t�@�C���̖��O�擾
			fscanf(pFile, "%s", &FileName[0]);

			//���f���̐���
			m_ppModel[nNum] = CModel::Create(&FileName[0]);

			//���f���ݒ�
			SetModel(m_ppModel[nNum], nNum);

			//���f���̔ԍ������Z����
			nNum++;
		}

		//========================================
		//���f���ݒ�
		//========================================
		if (strcmp(&garbage[0], "CHARACTERSET") == 0)
		{//������CHARACTERSET�̂Ƃ�
			while (1)
			{//������END_CHARACTERSET�ɂȂ�܂ŌJ��Ԃ�
				fscanf(pFile, "%s", &garbage[0]);

				if (strcmp(&garbage[0], "END_CHARACTERSET") == 0)
				{//������END_CHARACTERSET�̎�
					break;
				}

				if (strcmp(&garbage[0], "NUM_PARTS") == 0)
				{//������NUM_PARTS�̎�
					//=�̂Ă�
					fscanf(pFile, "%s", &garbage[0]);

					//�p�[�c��������
					fscanf(pFile, "%d", &m_nNumParts);
				}

				if (strcmp(&garbage[0], "PARTSSET") == 0)
				{//������PARTSSET�̎�
					while (1)
					{//END_PARTSSET������܂ŉ�
						fscanf(pFile, "%s", &garbage[0]);

						if (strcmp(&garbage[0], "END_PARTSSET") == 0)
						{//������END_PARTSSET�̎�
							//�p�[�c�������Z
							m_nPartsIndex++;

							break;
						}

						if (strcmp(&garbage[0], "INDEX") == 0)
						{//������INDEX�̎�
							//=�̂Ă�
							fscanf(pFile, "%s", &garbage[0]);

							//���Ԗڂ̃p�[�c���̐ݒ�
							fscanf(pFile, "%d", &nIdx);

							//���f���̔ԍ��ݒ�
							m_ppModel[m_nPartsIndex]->SetIndex(m_ppModel[nIdx]);
						}

						if (strcmp(&garbage[0], "PARENT") == 0)
						{//������PARENT�̎�
							//=�̂Ă�
							fscanf(pFile, "%s", &garbage[0]);

							//�e�q�֌W�ݒ�
							fscanf(pFile, "%d", &nParent);

							if (nParent == -1)
							{//�e�p�[�c�������ꍇ
								m_ppModel[m_nPartsIndex]->SetParent(nullptr);
							}
							else
							{//�e�p�[�c������ꍇ
								m_ppModel[m_nPartsIndex]->SetParent(m_ppModel[nParent]);
							}
						}

						if (strcmp(&garbage[0], "POS") == 0)
						{//������PARENT�̎�
							//=�̂Ă�
							fscanf(pFile, "%s", &garbage[0]);

							fscanf(pFile, "%f", &m_pos.x);
							fscanf(pFile, "%f", &m_pos.y);
							fscanf(pFile, "%f", &m_pos.z);

							//�ʒu��ݒ�
							m_ppModel[m_nPartsIndex]->SetPos(m_pos);
						}

						if (strcmp(&garbage[0], "ROT") == 0)
						{//������PARENT�̎�
							//=�̂Ă�
							fscanf(pFile, "%s", &garbage[0]);

							fscanf(pFile, "%f", &m_rot.x);
							fscanf(pFile, "%f", &m_rot.y);
							fscanf(pFile, "%f", &m_rot.z);

							//������ݒ�
							m_ppModel[m_nPartsIndex]->SetRotate(m_rot);
						}
					}
				}
			}
		}

		//========================================
		//���[�V�����ݒ�
		//========================================
		if (strcmp(&garbage[0], "MOTIONSET") == 0)
		{//������MOTIONSET�̎�
			while (1)
			{//END_PARTSSET������܂ŉ�
				fscanf(pFile, "%s", &garbage[0]);

				if (strcmp(&garbage[0], "END_MOTIONSET") == 0)
				{//������END_PARTSSET�̎�
					// ���[�V�����̑��������Z
					m_nNumAll++;

					// ���݂̃L�[���Z�b�g
					m_nNowKey = 0;

					break;
				}

				if (strcmp(&garbage[0], "LOOP") == 0)
				{//������INDEX�̎�
					//=�̂Ă�
					fscanf(pFile, "%s", &garbage[0]);

					//�e�q�֌W�ݒ�
					fscanf(pFile, "%d", &nLoop);

					// ���[�v���邩���Ȃ���
					if (nLoop == 0)
					{
						m_aInfo[m_nNumAll].bLoop = false;
					}
					if (nLoop == 1)
					{
						m_aInfo[m_nNumAll].bLoop = true;
					}
				}

				if (strcmp(&garbage[0], "NUM_KEY") == 0)
				{//������NUM_KEY�̎�
					//=�̂Ă�
					fscanf(pFile, "%s", &garbage[0]);

					//�L�[�̔ԍ�
					fscanf(pFile, "%d", &m_aInfo[m_nNumAll].nNumKey);
				}

				if (strcmp(&garbage[0], "KEYSET") == 0)
				{//������PARENT�̎�
					while (1)
					{//END_KEYSET������܂ŌJ��Ԃ�
						//=�̂Ă�
						fscanf(pFile, "%s", &garbage[0]);

						if (strcmp(&garbage[0], "END_KEYSET") == 0)
						{//������END_PARTSSET�̎�
							//���Z����
							m_nNowKey++;

							// �J�E���g���Z�b�g
							KeyCount = 0;

							break;
						}

						if (strcmp(&garbage[0], "FRAME") == 0)
						{//������FRAME�̎�
							//=�̂Ă�
							fscanf(pFile, "%s", &garbage[0]);

							//�t���[����
							fscanf(pFile, "%d", &m_aInfo[m_nNumAll].aKeyInfo[m_nNowKey].nFrame);
						}

						if (strcmp(&garbage[0], "KEY") == 0)
						{//������KEY�̎�
							while (1)
							{//END_KEY������܂ŌJ��Ԃ�
								//=�̂Ă�
								fscanf(pFile, "%s", &garbage[0]);

								if (strcmp(&garbage[0], "END_KEY") == 0)
								{//������END_KEY�̎�

									// �J�E���g���Z����
									KeyCount++;

									break;
								}

								if (strcmp(&garbage[0], "POS") == 0)
								{//������POS�̎�
									//=�̂Ă�
									fscanf(pFile, "%s", &garbage[0]);

									fscanf(pFile, "%f", &m_pos.x);
									fscanf(pFile, "%f", &m_pos.y);
									fscanf(pFile, "%f", &m_pos.z);

									KEY* pKey = &m_aInfo[m_nNumAll].aKeyInfo[m_nNowKey].aKey[KeyCount];

									//�ʒu��ݒ�
									pKey->pos = m_pos;
								}

								if (strcmp(&garbage[0], "ROT") == 0)
								{//������ROT�̎�
									//=�̂Ă�
									fscanf(pFile, "%s", &garbage[0]);

									fscanf(pFile, "%f", &m_rot.x);
									fscanf(pFile, "%f", &m_rot.y);
									fscanf(pFile, "%f", &m_rot.z);

									KEY* pKey = &m_aInfo[m_nNumAll].aKeyInfo[m_nNowKey].aKey[KeyCount];

									//������ݒ�
									pKey->rot = m_rot;
								}
							}
						}
					}
				}
			}
		}
	}
	//�t�@�C�������
	fclose(pFile);
}
