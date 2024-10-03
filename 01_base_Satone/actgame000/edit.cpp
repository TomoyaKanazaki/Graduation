//==============================================================
//
//�z�u����[edit.cpp]
//Author:����������
//
//==============================================================
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "input.h"
#include "object.h"
#include "model.h"
#include "debugproc.h"
//#include "playerModel.h"
#include "game.h"

//�}�N����`
#define PRIORITY			(3)			//�D�揇��
#define MODEL_LEN			(3.0f)		//����
#define CURVE_RL			(0.5f)		//���E�̊p�x
#define CURVE_UP			(0.0f)		//��̊p�x
#define CURVE_DOWN			(1.0f)		//���̊p�x
#define MOVE_MODEL			(1.0f)		//���f���̈ړ����x
#define FILE_MAP			"data\\TEXT\\map.txt"		//�}�b�v�̔z�u���t�@�C��

//�ÓI�����o�ϐ��錾
LPD3DXMESH CEdit::m_pMesh = NULL;						//���b�V���i���_���j�ւ̃|�C���^
LPD3DXBUFFER CEdit::m_pBuffMat = NULL;					//�}�e���A���ւ̃|�C���^
DWORD CEdit::m_dwNumMat = NULL;
const char *CEdit::m_apFilename[MAX_DEBUG_MODEL] = {};		//���f���̔z�u�̃t�@�C����
int CEdit::m_nNumAll = 0;			//���f���̑���
CObjectX *CEdit::m_apObjectX[MAX_DEBUG_MODEL] = {};
int CEdit::m_nCnt = 0;

//==============================================================
//�R���X�g���N�^
//==============================================================
CEdit::CEdit()
{
	//���f���̍\���̏�����
	for (int nCntEdit = 0; nCntEdit < MAX_DEBUG_MODEL; nCntEdit++)
	{
		m_aEdit[nCntEdit].dwNumMat = NULL;
		m_aEdit[nCntEdit].pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^
		m_aEdit[nCntEdit].pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
		m_aEdit[nCntEdit].pTexture = NULL;		//�e�N�X�`���̃|�C���^
		m_aEdit[nCntEdit].vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);		//���_���̍ŏ��l
		m_aEdit[nCntEdit].vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);		//���_���̍ő�l
		m_aEdit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		m_aEdit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		m_aEdit[nCntEdit].nIndex = -1;			//���f���ԍ�

		m_aSetEdit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		m_aSetEdit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		m_aSetEdit[nCntEdit].nIndex = -1;

		for (int nCntName = 0; nCntName < MAX_STR; nCntName++)
		{
			m_aEdit[nCntEdit].aFilename[nCntName] = NULL;			//�t�@�C����
			m_aEdit[nCntEdit].aFilecomment[nCntName] = NULL;		//�t�@�C���R�����g
		}
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
	m_nNumModel = 0;			//���f���̔ԍ�
	m_nMaxModel = 0;			//���f���̐ݒu�ő吔
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CEdit::~CEdit()
{
	
}

//==============================================================
//X�t�@�C���̓ǂݍ���
//==============================================================
HRESULT CEdit::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	CEdit::LoadFile();		//�t�@�C���ǂݍ���

	for (int nCntEdit = 0; nCntEdit < m_nNumAll; nCntEdit++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&m_aEdit[nCntEdit].aFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_aEdit[nCntEdit].pBuffMat,
			NULL,
			&m_aEdit[nCntEdit].dwNumMat,
			&m_aEdit[nCntEdit].pMesh);
	}

	return S_OK;
}

//==============================================================
//�}�e���A���E���b�V���̔j��
//==============================================================
void CEdit::Unload(void)
{
	for (int nCntEdit = 0; nCntEdit < MAX_DEBUG_MODEL; nCntEdit++)
	{
		//���b�V���̔j��
		if (m_aEdit[nCntEdit].pMesh != NULL)
		{
			m_aEdit[nCntEdit].pMesh->Release();
			m_aEdit[nCntEdit].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (m_aEdit[nCntEdit].pBuffMat != NULL)
		{
			m_aEdit[nCntEdit].pBuffMat->Release();
			m_aEdit[nCntEdit].pBuffMat = NULL;
		}
	}
}

//==============================================================
//���f���̔z�u�̐�������
//==============================================================
CEdit *CEdit::Create(void)
{
	CEdit *pEdit = NULL;

	if (pEdit == NULL)
	{//���������g�p����ĂȂ�������

		//���f���̔z�u�̐���
		pEdit = new CEdit;

	}

	//����������
	pEdit->Init();

	m_nCnt++;

	return pEdit;
}

//==============================================================
//���f���̔z�u�̏���������
//==============================================================
HRESULT CEdit::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	//���f���̔z�u���̓ǂݍ���
	CEdit::Load();

	//�傫���ݒ�
	CEdit::SetSize();

	return S_OK;
}

//==============================================================
//���f���̔z�u�̏I������
//==============================================================
void CEdit::Uninit(void)
{
	

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//���f���̔z�u�̍X�V����
//==============================================================
void CEdit::Update(void)
{
	
}

//==============================================================
//���f���̔z�u�̕`�揈��
//==============================================================
void CEdit::Draw(void)
{
	
}

//==============================================================
//���f���̑傫���ݒ菈��
//==============================================================
void CEdit::SetSize(void)
{
	int nNumVtx;					//���_��
	DWORD dwSizeFVF;				//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;					//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntEdit = 0; nCntEdit < m_nNumAll; nCntEdit++)
	{
		//���_�����擾
		nNumVtx = m_aEdit[nCntEdit].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(m_aEdit[nCntEdit].pMesh->GetFVF());

		//���_�o�b�t�@�����b�N
		m_aEdit[nCntEdit].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//���_���W�̑��

			//�S�Ă̒��_���r���ă��f���̍ő�l�𔲂��o��
			if (vtx.x > m_aEdit[nCntEdit].vtxMax.x)
			{
				m_aEdit[nCntEdit].vtxMax.x = vtx.x;
			}
			else if (vtx.y > m_aEdit[nCntEdit].vtxMax.y)
			{
				m_aEdit[nCntEdit].vtxMax.y = vtx.y;
			}
			else if (vtx.z > m_aEdit[nCntEdit].vtxMax.z)
			{
				m_aEdit[nCntEdit].vtxMax.z = vtx.z;
			}

			//�S�Ă̒��_���r���ă��f���̍ŏ��l�𔲂��o��
			if (vtx.x < m_aEdit[nCntEdit].vtxMin.x)
			{
				m_aEdit[nCntEdit].vtxMin.x = vtx.x;
			}
			else if (vtx.y < m_aEdit[nCntEdit].vtxMin.y)
			{
				m_aEdit[nCntEdit].vtxMin.y = vtx.y;
			}
			else if (vtx.z < m_aEdit[nCntEdit].vtxMin.z)
			{
				m_aEdit[nCntEdit].vtxMin.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;								//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		}

		//���_�o�b�t�@���A�����b�N
		m_aEdit[nCntEdit].pMesh->UnlockVertexBuffer();
	}
}

//==============================================================
//���f���̔z�u���ǂݍ��ݏ���
//==============================================================
void CEdit::LoadFile(void)
{
	FILE *pFile;		//�t�@�C���|�C���^
	char aString[MAX_STR];		//�������
	int nCntNum = 0, nCntName = 0;			//���f���̔ԍ��A�t�@�C�����ԍ�
	int nResult = 0;			//�Ō�̍s�܂ŗ�����

	pFile = fopen(FILE_MAP, "r");

	if (pFile != NULL)
	{//�t�@�C�����J������

		//���f�����ǂݍ���
		while (strcmp(&aString[0], "NUM_MODEL") != 0)
		{//���f������ǂݍ��ނ܂ł̊�

			fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

			if (strcmp(&aString[0], "NUM_MODEL") == 0)
			{//[NUM_MODEL]��ǂݍ��񂾂�

				fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���
				fscanf(pFile, "%d", &m_nNumAll);		//���f�����ǂݍ���

				break;
			}
		}

		//���f���̃t�@�C�����ǂݍ���
		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{//[MODEL_FILENAME]��ǂݍ��񂾂�

				fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

				fscanf(pFile, "%s", &m_aEdit[nCntName].aFilename[0]);	//�t�@�C�����ǂݍ���
				fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

				fscanf(pFile, "%s", &m_aEdit[nCntName].aFilecomment[0]);	//�t�@�C���R�����g�ǂݍ���

				if (nCntName == m_nNumAll - 1)
				{//�ő�l�Ɠ����ԍ��̎�

					break;
				}
				else
				{
					//���̔ԍ��ɐi��
					nCntName++;
				}
			}
		}

		//���f���̔z�u���ǂݍ���
		while (strcmp(&aString[0], "MODELSET") != 0 && nResult != EOF)
		{//[MODELSET]��ǂݍ��ނ܂ł̊� && �Ō�̍s����Ȃ���

			nResult = fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

			if (strcmp(&aString[0], "MODELSET") == 0)
			{//[MODELSET]��ǂݍ��񂾂�

				while (strcmp(&aString[0], "END_MODELSET") != 0)
				{//[END_MODELSET]��ǂݍ��ނ܂ł̊�

					fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

					if (strcmp(&aString[0], "INDEX") == 0)
					{//[INDEX]��ǂݍ��񂾂�

						fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���
						fscanf(pFile, "%d", &m_aSetEdit[m_nMaxModel].nIndex);		//���f���ԍ��ǂݍ���

					}
					else if (strcmp(&aString[0], "POS") == 0)
					{//[POS]��ǂݍ��񂾂�

						fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

						//�ʒu�ǂݍ���
						fscanf(pFile, "%f", &m_aSetEdit[m_nMaxModel].pos.x);
						fscanf(pFile, "%f", &m_aSetEdit[m_nMaxModel].pos.y);
						fscanf(pFile, "%f", &m_aSetEdit[m_nMaxModel].pos.z);

					}
				}

				m_nMaxModel++;		//���f���̍ő�ݒu�����Z
			}
		}

		//�t�@�C������
		fclose(pFile);
	}

	//���f���ݒu
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		//���f������
		CModel::Create(m_aSetEdit[nCntModel].pos, m_aSetEdit[nCntModel].rot, &m_aEdit[m_aSetEdit[nCntModel].nIndex].aFilename[0]);

		//CModel::SetType(TYPE_ALPHA_BLOCK);
	}
}