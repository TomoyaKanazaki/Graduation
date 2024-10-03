//==============================================================
//
//�ǂ̏���[wall.h]
//Author:����������
//
//==============================================================
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//�}�N����`
#define WIDTH_FIELD			(1000.0f * 0.5f)		//�ǂ̉���
#define HEIGHT_FIELD		(1000.0f * 0.5f)		//�ǂ̏c��
#define COL_R				(1.0f)		//�ԐF�̐��l
#define COL_G				(1.0f)		//�ΐF�̐��l
#define COL_B				(1.0f)		//�F�̐��l
#define COL_A				(1.0f)		//�s�����x�̐��l
#define TEX_LEFT_X			(0.0f)		//�e�N�X�`���̍�x�̏����l
#define TEX_RIGHT_X			(1.0f)		//�e�N�X�`���̉Ex�̏����l
#define TEX_UP_Y			(0.0f)		//�e�N�X�`���̏�y�̏����l
#define TEX_DOWN_Y			(1.0f)		//�e�N�X�`���̉�y�̏����l

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CWall::m_pTexture = NULL;

//==============================================================
//�R���X�g���N�^
//==============================================================
CWall::CWall()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_nType = 0;
}

//==============================================================
//�R���X�g���N�^
//==============================================================
CWall::CWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = pos;		//�ʒu
	m_nType = nType;	//�ǂ̎��

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CWall::~CWall()
{

}

//==============================================================
//�ǂ̐�������
//==============================================================
CWall *CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CWall *pWall = NULL;

	if (pWall == NULL)
	{//���������g�p����ĂȂ�������

		//�ǂ̐���
		pWall = new CWall(pos, rot, nType);
	}

	//����������
	pWall->Init();

	return pWall;
}

//==============================================================
//�ǂ̏���������
//==============================================================
HRESULT CWall::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	m_nIdxTexture = pTexture->TEX_WALL;

	//�e�N�X�`���̊��蓖��
	CObject3D::BindTexture(m_nIdxTexture);

	//�r���{�[�h�̏���������
	CObject3D::Init();

	//��ނ̐ݒ�
	CObject::SetType(CObject::TYPE_WALL);

	//�ʒu�̐ݒ�
	CObject3D::SetPosition(m_pos);

	//�ǂ̑傫���ݒ�
	CObject3D::SetSize(WIDTH_FIELD, HEIGHT_FIELD);

	return S_OK;
}

//==============================================================
//�ǂ̏I������
//==============================================================
void CWall::UnInit(void)
{
	//�I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//==============================================================
//�ǂ̍X�V����
//==============================================================
void CWall::Update(void)
{

}

//==============================================================
//�ǂ̕`�揈��
//==============================================================
void CWall::Draw(void)
{
	//�I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
}