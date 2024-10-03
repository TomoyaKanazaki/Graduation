//==============================================================
//
//�`���[�g���A���摜�̏���[tutorial_texture.cpp]
//Author:����������
//
//==============================================================
#include "testline.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "tutorial_texture.h"
#include "effect.h"

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CTestLine::m_pTexture = NULL;
const char* CTestLine::m_apFilename[STEP_MAX] =
{
	"data\\TEXTURE\\death.png",		//�ړ�
	"data\\TEXTURE\\death.png",		//�W�����v
};

namespace
{
	const D3DXVECTOR3 LIMIT = D3DXVECTOR3(1300.0f, 700.0f, 0.0f);

	const int MAX_PLAYER = 2;
	const float WIDTH = 50.0f * 0.5f;
	const float HEIGHT = 50.0f * 0.5f;
	const float MOVE = 10.0f;		// �ړ���

}

//==============================================================
//�R���X�g���N�^
//==============================================================
CTestLine::CTestLine()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu

}

//==============================================================
//�R���X�g���N�^
//==============================================================
CTestLine::CTestLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = pos;		//�ʒu


}

//==============================================================
//�f�X�g���N�^
//==============================================================
CTestLine::~CTestLine()
{

}

//==============================================================
//�ǂ̐�������
//==============================================================
CTestLine* CTestLine::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CTestLine* pTest = NULL;

	if (pTest == NULL)
	{//���������g�p����ĂȂ�������

		//�ǂ̐���
		pTest = new CTestLine(pos, rot, nType);
	}

	//����������
	pTest->Init();

	return pTest;
}

//==============================================================
//�ǂ̏���������
//==============================================================
HRESULT CTestLine::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	//m_nIdxTexture = pTexture->Regist(m_apFilename[m_nType]);

	//�e�N�X�`���̊��蓖��
	CObject3D::BindTexture(m_nIdxTexture);

	//�I�u�W�F�N�g3D�̏���������
	CObject3D::Init();

	//��ނ̐ݒ�
	CObject::SetType(CObject::TYPE_WALL);

	//�ʒu�̐ݒ�
	CObject3D::SetPosition(m_pos);

	//�ǂ̑傫���ݒ�
	CObject3D::SetSize(WIDTH, HEIGHT);

	return S_OK;
}

//==============================================================
//�ǂ̏I������
//==============================================================
void CTestLine::UnInit(void)
{
	//�I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//==============================================================
//�ǂ̍X�V����
//==============================================================
void CTestLine::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	pDebugProc->Print("\n�G�t�F�N�g�̈ʒu    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);

}

//==============================================================
//�ǂ̕`�揈��
//==============================================================
void CTestLine::Draw(void)
{
	//�I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
}
