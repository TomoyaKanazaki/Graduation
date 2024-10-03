//==============================================================
//
//���f������[model.cpp]
//Author:����������
//
//==============================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "material.h"
#include "player.h"
#include "game.h"
#include "sound.h"

//�}�N����`
#define NUM_MODEL			(2)			//���f���̎��
#define POS					(10.0f)		//pos�����l
#define MODEL_LEN			(3.0f)		//����
#define MOVE_DOWN			(-10.0f)		//�����ړ���

//�ÓI�����o�ϐ��錾
LPD3DXMESH CModel::m_pMesh = NULL;						//���b�V���i���_���j�ւ̃|�C���^
LPD3DXBUFFER CModel::m_pBuffMat = NULL;					//�}�e���A���ւ̃|�C���^
DWORD CModel::m_dwNumMat = NULL;
const char *CModel::m_pFilename = NULL;					//�t�@�C���̖��O

//==============================================================
//�R���X�g���N�^
//==============================================================
CModel::CModel()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�O��̈ʒu
	m_move = D3DXVECTOR3(0.0f, MOVE_DOWN, 0.0f);		//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

	m_bActionDown = false;		//�u���b�N�𗎂Ƃ���
	m_bDown = false;			//���Ƃ���Ă邩
	m_bCollision = false;		//���̃u���b�N�Ɠ������Ă邩
}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CModel::CModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		//�ʒu
	m_posOld = m_pos;	//�O��̈ʒu
	m_move = D3DXVECTOR3(0.0f, MOVE_DOWN, 0.0f);		//�ړ���
	m_rot = rot;		//����

	m_bActionDown = false;		//�u���b�N�𗎂Ƃ���
	m_bDown = false;			//���Ƃ���Ă邩
	m_bCollision = false;		//���̃u���b�N�Ɠ������Ă邩

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CModel::~CModel()
{

}

//==============================================================
//X�t�@�C���̓ǂݍ���
//==============================================================
HRESULT CModel::Load(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	return S_OK;
}

//==============================================================
//�}�e���A���E���b�V���̔j��
//==============================================================
void CModel::Unload(void)
{
	//���b�V���̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//==============================================================
//���f���̐�������
//==============================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *pFilename)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{//���������g�p����ĂȂ�������

		//���f���̐���
		pModel = new CModel(pos, rot);
	}

	//�t�@�C�������
	m_pFilename = pFilename;

	//����������
	pModel->Init();

	return pModel;
}

//==============================================================
//���f���̏���������
//==============================================================
HRESULT CModel::Init(void)
{
	CMaterial *pMaterial = CManager::GetInstance()->GetMaterial();

	//���f���̓ǂݍ���
	m_nIdxModel = pMaterial->Regist(m_pFilename);

	//�}�e���A���̊��蓖��
	CObjectX::BindMaterial(m_nIdxModel);

	//�I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	//�ʒu�ݒ菈��
	CObjectX::SetPosition(m_pos);

	//��ނ̐ݒ�
	if (strcmp(m_pFilename, "data/MODEL/map/wall_alpha.x") == 0)
	{
		m_type = CObject::TYPE_ALPHA_BLOCK;
	}
	else if (strcmp(m_pFilename, "data/MODEL/map/move_6block.x") == 0)
	{
		m_type = CObject::TYPE_DOWN_6BLOCK;

	}
	else if (strcmp(m_pFilename, "data/MODEL/map/move_9block.x") == 0)
	{
		m_type = CObject::TYPE_DOWN_9BLOCK;
	}
	else if (strcmp(m_pFilename, "data/MODEL/map/needleD.x") == 0 ||
		strcmp(m_pFilename, "data/MODEL/map/needleL.x") == 0 || 
		strcmp(m_pFilename, "data/MODEL/map/needleR.x") == 0 || 
		strcmp(m_pFilename, "data/MODEL/map/needleU.x") == 0)
	{
		m_type = CObject::TYPE_NEEDLE;
	}
	else
	{
		m_type = CObject::TYPE_MODEL;
	}

	CObject::SetType(m_type);

	return S_OK;
}

//==============================================================
//���f���̏I������
//==============================================================
void CModel::Uninit(void)
{
	//�I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}

//==============================================================
//���f���̍X�V����
//==============================================================
void CModel::Update(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//if (m_bActionDown == true)
	{//����������Ƃ�

		m_pos = CObjectX::GetPosition();		//�ʒu�擾

		m_posOld = m_pos;	//�O��̈ʒu

		m_pos += m_move;	//�ړ��ʉ��Z

		CObjectX::SetPosition(m_pos);

		if (m_bCollision == false)
		{//���̃u���b�N�Ɠ������ĂȂ��Ƃ�

			if (CObjectX::Collision(&m_pos, &m_posOld, &m_move, CObjectX::GetSizeMin(), CObjectX::GetSizeMax()) == true)
			{//����������

				pSound->Play(pSound->SOUND_LABEL_SE_COLLAPSE01);

				m_bCollision = true;
			}
		}
	}

	//�I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
}

//==============================================================
//���f���̕`�揈��
//==============================================================
void CModel::Draw(void)
{
	//�I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}

//==============================================================
//�I�u�W�F�N�g�̎�ސݒ菈��
//==============================================================
void CModel::SetType(TYPE type)
{
	m_type = type;

	//��ނ̐ݒ�
	CObject::SetType(m_type);
}

//==============================================================
//���f���̃q�b�g����
//==============================================================
void CModel::Hit(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	CSound *pSound = CManager::GetInstance()->GetSound();

	switch (m_type)
	{
	case TYPE_ALPHA_BLOCK:		//�����u���b�N

		//�v���C���[�����_�b�V��������
		pPlayer->SetDashAuto(true);

		break;

	case TYPE_DOWN_6BLOCK:		//�����u���b�N

		if (m_bDown == false)
		{
			pSound->Play(pSound->SOUND_LABEL_SE_COLLAPSE00);
			m_bDown = true;			//���Ƃ�����Ԃɂ���

		}

		m_bActionDown = true;	//�u���b�N�𗎂Ƃ�

		break;

	case TYPE_DOWN_9BLOCK:		//�����u���b�N

		if (m_bDown == false)
		{
			pSound->Play(pSound->SOUND_LABEL_SE_COLLAPSE00);
			m_bDown = true;			//���Ƃ�����Ԃɂ���

		}

		m_bActionDown = true;	//�u���b�N�𗎂Ƃ�
		m_bDown = true;			//���Ƃ�����Ԃɂ���

		break;
	}
}