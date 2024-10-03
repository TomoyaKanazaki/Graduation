//==============================================================
//
//�`���[�g���A���摜�̏���[tutorial_texture.cpp]
//Author:����������
//
//==============================================================
#include "test.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "tutorial_texture.h"
#include "effect.h"
#include "testline.h"

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CTest::m_pTexture = NULL;
const char *CTest::m_apFilename[STEP_MAX] =
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
CTest::CTest()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

	m_nType = 0;

	m_pTest2 = nullptr;
	m_pCircle = nullptr;
}

//==============================================================
//�R���X�g���N�^
//==============================================================
CTest::CTest(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = pos;		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_distance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_nType = nType;
	m_pTest2 = nullptr;
	m_pCircle = nullptr;

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CTest::~CTest()
{

}

//==============================================================
//�ǂ̐�������
//==============================================================
CTest *CTest::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CTest *pTest = NULL;

	if (pTest == NULL)
	{//���������g�p����ĂȂ�������

		//�ǂ̐���
		pTest = new CTest(pos, rot, nType);
	}

	//����������
	pTest->Init();

	return pTest;
}

//==============================================================
//�ǂ̏���������
//==============================================================
HRESULT CTest::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�I�u�W�F�N�g3D�̏���������
	CObject3D::Init();

	//��ނ̐ݒ�
	CObject::SetType(CObject::TYPE_WALL);

	//�ʒu�̐ݒ�
	CObject3D::SetPosition(m_pos);

	//�ǂ̑傫���ݒ�
	CObject3D::SetSize(WIDTH, HEIGHT);

	// �v���C���[1�̐F�ݒ�
	CObject3D::SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	m_pTest2 = CTutorialUI::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	m_pTestLine = CTestLine::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

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
void CTest::UnInit(void)
{
	if (m_pCircle != nullptr)
	{
		m_pCircle->Uninit();
		m_pCircle = nullptr;
	}

	//�I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//==============================================================
//�ǂ̍X�V����
//==============================================================
void CTest::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	m_posOld = m_pos;

	m_pTest2->SetTest2Position(m_pos);

	m_distance = m_pTest2->GetPosition();

	m_fDistance = (m_pos.x - m_distance.x) * (m_pos.x - m_distance.x) + (m_pos.y - m_distance.y) * (m_pos.y - m_distance.y);
	m_fDistance = sqrtf(m_fDistance);

	CircleColllllll();

	MoveKeyboard();

	D3DXVECTOR3 vecline = m_distance - m_pos;
	D3DXVECTOR3 linepos = m_pos + (vecline * 0.5f);
	float fRot = atan2f(vecline.y, vecline.x);

	m_pTestLine->SetPosition(linepos);
	m_pTestLine->SetRot(D3DXVECTOR3(0.0f, 0.0f, fRot));
	m_pTestLine->SetSize(m_fDistance * 0.5f, 10.0f);

	m_pos += m_move;			//�ړ��ʉ��Z

	ScreenLimit();

	//�ړ��ʂ��X�V
	m_move.x += (0.0f - m_move.x) * 0.008f;
	m_move.y += (0.0f - m_move.y) * 0.008f;
	m_move.z += (0.0f - m_move.z) * 0.008f;

	CObject3D::SetPosition(m_pos);
	m_pCircle->SetPosition(m_pos);

	pDebugProc->Print("\n�v���C���[1�̈ʒu    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("\n����1�F%f\n", m_fDistance);
	pDebugProc->Print("\n�{��1�F%f\n", m_fScale);

}

//==============================================================
//�ǂ̕`�揈��
//==============================================================
void CTest::Draw(void)
{
	//�I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
}

//==============================================================
//�v���C���[�L�[�{�[�h�̈ړ�����
//==============================================================
void CTest::MoveKeyboard(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	if (m_fScale < 0.01f)
	{
		m_fScale = 0.01f;
	}

	//�ړ�
	if (pInputKeyboard->GetTrigger(DIK_D) == true)
	{//�E

		m_move.x = sinf(pCamera->GetRotation().y + -D3DX_PI * 0.5f) * MOVE / m_fScale;
		m_move.z = cosf(pCamera->GetRotation().y + -D3DX_PI * 0.5f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_A) == true)
	{//��
		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.5f) * MOVE / m_fScale;
		m_move.z = cosf(pCamera->GetRotation().y + D3DX_PI * 0.5f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_W) == true)
	{//��

		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 1.0f) * MOVE / m_fScale;
		m_move.y = cosf(pCamera->GetRotation().y + D3DX_PI * 1.0f) * MOVE / m_fScale;
	}
	else if (pInputKeyboard->GetTrigger(DIK_S) == true)
	{//��

		m_move.x = sinf(pCamera->GetRotation().y + D3DX_PI * 0.0f) * MOVE / m_fScale;
		m_move.y = cosf(pCamera->GetRotation().y + D3DX_PI * 0.0f) * MOVE / m_fScale;
	}
}

//==============================================================
// ��ʊO����
//==============================================================
void CTest::ScreenLimit(void)
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
void CTest::CircleColllllll(void)
{
	if (m_fDistance <= 400.0f + 400.0f)
	{
		float Base = 800.0f;

		m_fScale = (m_fDistance / Base);

		return;
	}

	m_fScale = 1.0f;
}