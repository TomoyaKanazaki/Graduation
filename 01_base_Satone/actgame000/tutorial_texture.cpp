//==============================================================
//
//�`���[�g���A���摜�̏���[tutorial_texture.cpp]
//Author:����������
//
//==============================================================
#include "tutorial_texture.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "test.h"
#include "debugproc.h"

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CTutorialUI::m_pTexture = NULL;
const char *CTutorialUI::m_apFilename[STEP_MAX] =
{
	"data\\TEXTURE\\tutorial000.png",		//�ړ�
	"data\\TEXTURE\\tutorial001.png",		//�W�����v
	"data\\TEXTURE\\tutorial002.png",		//�_�b�V��
	"data\\TEXTURE\\tutorial003.png",		//�ʂ蔲��
	"data\\TEXTURE\\tutorial004.png",		//�A�C�e��
	"data\\TEXTURE\\tutorial005.png",		//�G
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
CTutorialUI::CTutorialUI()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_pCircle = nullptr;

	m_nType = 0;
}

//==============================================================
//�R���X�g���N�^
//==============================================================
CTutorialUI::CTutorialUI(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = pos;		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_pCircle = nullptr;

	m_nType = nType;	//�ǂ̎��

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CTutorialUI::~CTutorialUI()
{

}

//==============================================================
//�ǂ̐�������
//==============================================================
CTutorialUI *CTutorialUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CTutorialUI *pWall = NULL;

	if (pWall == NULL)
	{//���������g�p����ĂȂ�������

		//�ǂ̐���
		pWall = new CTutorialUI(pos, rot, nType);
	}

	//����������
	pWall->Init();

	return pWall;
}

//==============================================================
//�ǂ̏���������
//==============================================================
HRESULT CTutorialUI::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	//m_nIdxTexture = pTexture->Regist(m_apFilename[m_nType]);

	//�e�N�X�`���̊��蓖��
	//CObject3D::BindTexture(m_nIdxTexture);

	//�r���{�[�h�̏���������
	CObject3D::Init();

	//��ނ̐ݒ�
	CObject::SetType(CObject::TYPE_WALL);

	//�ʒu�̐ݒ�
	CObject3D::SetPosition(m_pos);

	//�ǂ̑傫���ݒ�
	CObject3D::SetSize(WIDTH, HEIGHT);

	// �v���C���[2�̐F�ݒ�
	CObject3D::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	// �~�̐���
	m_pCircle = CObject3D::Create();

	//�e�N�X�`���̓ǂݍ���
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\item00.png");
	BindTexture(m_nIdxTexture);

	//�e�N�X�`���̊��蓖��
	m_pCircle->BindTexture(m_nIdxTexture);

	m_pCircle->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pCircle->SetSize(400.0f, 400.0f);
	m_pCircle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0, 0.5f));

	return S_OK;
}

//==============================================================
//�ǂ̏I������
//==============================================================
void CTutorialUI::UnInit(void)
{
	//�I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//==============================================================
//�ǂ̍X�V����
//==============================================================
void CTutorialUI::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	m_posOld = m_pos;

	m_distance = m_posTest2;

	m_fDistance = (m_pos.x - m_distance.x) * (m_pos.x - m_distance.x) + (m_pos.y - m_distance.y) * (m_pos.y - m_distance.y);
	m_fDistance = sqrtf(m_fDistance);

	CircleColllllll();

	MoveKeyboard();

	m_pos += m_move;			//�ړ��ʉ��Z

	ScreenLimit();

	//�ړ��ʂ��X�V
	m_move.x += (0.0f - m_move.x) * 0.008f;
	m_move.y += (0.0f - m_move.y) * 0.008f;
	m_move.z += (0.0f - m_move.z) * 0.008f;

	CObject3D::SetPosition(m_pos);
	m_pCircle->SetPosition(m_pos);

	pDebugProc->Print("\n�v���C���[2�̈ʒu    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("\n����2�F%f\n", m_fDistance);
	pDebugProc->Print("\n�{��2�F%f\n", m_fScale);
}

//==============================================================
//�ǂ̕`�揈��
//==============================================================
void CTutorialUI::Draw(void)
{
	//�I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
}

//==============================================================
//�v���C���[�L�[�{�[�h�̈ړ�����
//==============================================================
void CTutorialUI::MoveKeyboard(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	if (m_fScale < 0.01f)
	{
		m_fScale = 0.01f;
	}

	//�ړ�
	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{//�E

		m_move.x = sinf(pCamera->GetRotation().y + -D3DX_PI * 0.5f) * MOVE / m_fScale;
		m_move.z = cosf(pCamera->GetRotation().y + -D3DX_PI * 0.5f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{//��
		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.5f) * MOVE / m_fScale;
		m_move.z = cosf(pCamera->GetRotation().y + D3DX_PI * 0.5f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{//��

		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 1.0f) * MOVE / m_fScale;
		m_move.y = cosf(pCamera->GetRotation().y + D3DX_PI * 1.0f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{//��

		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.0f) * MOVE / m_fScale;
		m_move.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.0f) * MOVE / m_fScale;
	}
}

//==============================================================
// ��ʊO����
//==============================================================
void CTutorialUI::ScreenLimit(void)
{
	if (m_pos.x <= -LIMIT.x)
	{
		m_pos.x = -LIMIT.x;
		m_move.x *= -1.0f;
	}
	else if (m_pos.x >= LIMIT.x)
	{
		m_pos.x = LIMIT.x;
		m_move.x *= -1.0f;
	}

	if (m_pos.y <= -LIMIT.y)
	{
		m_pos.y = -LIMIT.y;
		m_move.y *= -1.0f;
	}
	else if (m_pos.y >= LIMIT.y)
	{
		m_pos.y = LIMIT.y;
		m_move.y *= -1.0f;
	}
}

//==============================================================
// �~�̓����蔻��
//==============================================================
void CTutorialUI::CircleColllllll(void)
{
	if (m_fDistance <= 400.0f + 400.0f)
	{
		float Base = 800.0f;

		m_fScale = (m_fDistance / Base);

		return;
	}

	m_fScale = 1.0f;
}