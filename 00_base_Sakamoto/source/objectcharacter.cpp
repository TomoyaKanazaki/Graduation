//============================================
//
//	�L�����N�^�[�̏��� [character.cpp]
//	Author:sakai minato
//
//============================================

#include "objectcharacter.h"

#include "characterManager.h"

#include "manager.h"
#include "renderer.h"

#include "model.h"
#include "motion.h"

#include "game.h"

#include "objmeshField.h"

#include "shadow.h"
#include "mask.h"

#include "move.h"

//====================================================================
// �萔��`
//====================================================================
namespace
{
	const float SHADOW_SIZE = 100.0f;			// �ۉe�̑傫��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CObjectCharacter::CObjectCharacter(int nPriority) : CObject(nPriority),
m_pShadow(nullptr),
m_bUseShadow(true),
m_State(STATE_WAIT),
m_OldState(STATE_WAIT)
{
	for (int nCnt = 0; nCnt < MODEL_NUM; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
		m_aModelName[nCnt] = {};
	}

	m_pMotion = nullptr;
	m_nNumModel = 0;

	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;
	m_move = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_size = INITVECTOR3;
	m_Objmove = INITVECTOR3;

	m_UseMultiMatrix = nullptr;

	m_bUseStencil = false;
	m_bUseShadowMtx = false;

	m_pMoveState = nullptr;

	m_Grid.x = 0;
	m_Grid.z = 0;

	// �i�s����
	m_Progress.bOKD = true;
	m_Progress.bOKL = true;
	m_Progress.bOKR = true;
	m_Progress.bOKU = true;

	m_bGritCenter = true;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CObjectCharacter::~CObjectCharacter()
{

}

//====================================================================
// ��������
//====================================================================
CObjectCharacter* CObjectCharacter::Create(bool bShadow)
{
	// �I�u�W�F�N�g�̐�������
	CObjectCharacter* pInstance = new CObjectCharacter();

	// �V���h�E�̗L��
	pInstance->SetShadow(bShadow);

	// �I�u�W�F�N�g�̏���������
	if (FAILED(pInstance->Init()))
	{// ���������������s�����ꍇ
		return nullptr;
	}

	return pInstance;
}

//====================================================================
// ����������
//====================================================================
HRESULT CObjectCharacter::Init(void)
{
	if (m_pShadow == nullptr && m_bUseShadow)
	{// �e����
		m_pShadow = CShadow::Create(m_pos, D3DXVECTOR3(SHADOW_SIZE, 0.0f, SHADOW_SIZE));
	}

	if (m_pMoveState == nullptr)
	{ // �ړ���Ԑݒ�
		m_pMoveState = new CStateStop();		// ��~���
	}

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CObjectCharacter::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != nullptr)
		{
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = nullptr;
		}
	}

	// �e�̏I��
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	//���[�V�����̏I������
	if (m_pMotion != nullptr)
	{
		//���[�V�����̔j��
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// �ړ���Ԃ̔j��
	if (m_pMoveState != nullptr)
	{
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CObjectCharacter::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	if (m_pMotion != nullptr)
	{
		//���[�V�����̍X�V
		m_pMotion->Update();
	}

	if (m_pShadow != nullptr)
	{// �V���h�E�̍X�V
		m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, 1.0f, m_pos.z));
		m_pShadow->SetBaseHeight(pos.y);
	}

	// ��Ԃ̕ۑ�
	m_OldState = m_State;
}

//====================================================================
//�`�揈��
//====================================================================
void CObjectCharacter::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bUseShadowMtx == true)
	{
		D3DXMATRIX mtxShadow;	// �V���h�E�}�g���b�N�X
		D3DLIGHT9 light;		// ���C�g���
		D3DXVECTOR4 posLight;	// ���C�g�̈ʒu
		D3DXVECTOR3 pos, normal;	// ���ʏ�̔C�ӂ̓_�A�@���x�N�g��
		D3DXPLANE plane;		// ���ʏ��

		// ���C�g�̈ʒu�ݒ�
		pDevice->GetLight(0, &light);
		posLight = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.05f);

		// ���ʏ��𐶐�
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXPlaneFromPointNormal(&plane, &pos, &normal);

		// �V���h�E�}�g���b�N�X�̏�����


		// �V���h�E�}�g���b�N�X�̍쐬
		D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

		// �V���h�E�}�g���b�N�X�̐ݒ�
	}

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_UseMultiMatrix != nullptr)
	{
		//�Z�o�����}�g���N�X���������킹��
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			m_UseMultiMatrix);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_bUseStencil == true)
	{
		//�X�e���V���o�b�t�@�L��
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		//�X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�̐ݒ� => ref
		pDevice->SetRenderState(D3DRS_STENCILREF, 1);

		//�X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
		pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

		//�X�e���V���o�b�t�@�̔�r���@ => (�Q�ƒl => �X�e���V���o�b�t�@�̎Q�ƒl)�Ȃ獇�i
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

		//�X�e���V���e�X�g���ʂɑ΂��Ă̔��f�ݒ�
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z�e�X�g�E�X�e���V���e�X�g����
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����
	}

	//���f���̕`��(�S�p�[�c)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != nullptr)
		{
			m_apModel[nCntModel]->Draw();
		}
	}

	//�X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==========================================
// �ړ���ԕύX����
//==========================================
void CObjectCharacter::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
}

//====================================================================
// �L�����N�^�[�e�L�X�g�ݒ菈��
//====================================================================
void CObjectCharacter::SetTxtCharacter(const char* pFilename)
{
	strcpy(&m_aModelName[0], pFilename);

	CCharacterManager* pCharacterManager = CManager::GetInstance()->GetCharacterManager();

	if (pCharacterManager == nullptr)
	{
		assert(("�L�����N�^�[�Ǘ��N���X���Ȃ�", false));
		return;
	}

	int nNum = pCharacterManager->Regist(this,pFilename);

#if 0 // ����̃f�o�b�O�p�i���f���ǂݍ��݂����킩�킩��܂ŃR�����g�A�E�g�j

	strcpy(&m_aModelName[0], pFilename);

	//���f���̐���
	LoadModel(pFilename);

	//���[�V�����̐���
	if (m_pMotion == nullptr)
	{
		//���[�V�����̐���
		m_pMotion = new CMotion;
	}

	//����������
	if (m_pMotion != nullptr)
	{
		m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
		m_pMotion->LoadData(pFilename);
	}

#endif

}

//====================================================================
// ���f���ݒ菈��
//====================================================================
void CObjectCharacter::SetModel(CModel* pModel, int nCnt)
{
	if (m_apModel[nCnt] == nullptr)
	{
		m_apModel[nCnt] = pModel;
	}
	else
	{
		assert(("���f���㏑������", false));
		return;
	}
}

//====================================================================
// ���f���擾����
//====================================================================
CModel* CObjectCharacter::GetModel(int nCnt)
{
	if (m_apModel[nCnt] != nullptr)
	{
		return m_apModel[nCnt];
	}

	assert(("���f���擾���s", false));

	return nullptr;
}

//====================================================================
// ���f���擾����
//====================================================================
CModel** CObjectCharacter::GetModel(void)
{
	if (m_apModel[0] != nullptr)
	{
		return &m_apModel[0];
	}

	assert(("���f���擾���s", false));

	return nullptr;
}


//====================================================================
// ���[�V�����ݒ菈��
//====================================================================
void CObjectCharacter::SetMotion(CMotion* pMotion)
{
	if (m_pMotion == nullptr)
	{
		m_pMotion = pMotion;
	}
	else
	{
		assert(("���[�V�����㏑������", false));
		return;
	}
}

//====================================================================
// ���[�V�����擾����
//====================================================================
CMotion* CObjectCharacter::GetMotion(void)
{
	if (m_pMotion != nullptr)
	{
		return m_pMotion;
	}

	assert(("���[�V�����擾���s", false));

	return nullptr;
}

//====================================================================
// ���f�����[�h����
//====================================================================
void CObjectCharacter::LoadModel(const char* pFilename)
{
	//�t�@�C�����J��
	FILE* pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ
		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;

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
						fscanf(pFile, "%d", &m_nNumModel);		//���f�����̐ݒ�
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
						fscanf(pFile, "%s", &ModelName[0]);		//�ǂݍ��ރ��f���̃p�X���擾

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						m_apModel[nCntModel]->SetColorType(CModel::COLORTYPE_FALSE);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
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
								fscanf(pFile, "%d", &nCntModel);	//�C���f�b�N�X��ݒ�
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//�e���f���̃C���f�b�N�X��ݒ�

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(nullptr);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.y);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.z);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.y);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.z);				////�����̏����ݒ�

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
						{
							break;
						}
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
// �w�胂�f���J���[�ύX
//====================================================================
void CObjectCharacter::SetModelColor(CModel::COLORTYPE Type, D3DXCOLOR Col)
{
	// ���f�����̎擾
	int nNumModel = GetNumModel();

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		// ���f���̎擾
		CModel* pModel = GetModel(nCnt);

		if (pModel != nullptr)
		{
			pModel->SetColorType(Type);
			pModel->SetColor(Col);
		}
	}
}