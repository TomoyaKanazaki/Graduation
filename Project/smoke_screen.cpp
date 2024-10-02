//============================================
//
//	2D�������� [smoke_screen.h]
//	Author:Kai Takada
//
//============================================
#include "smoke_screen.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//�萔��`
namespace
{
	const float SMALLER(2.0f);
	const char* TEX_PASS("data\\TEXTURE\\screenEffect\\smoke_screen.png");
	const D3DXCOLOR DEF_COL(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CSmokeScreen::CSmokeScreen(int nPriority) : CObject2D(nPriority)
{
	m_nLife = 50;
	m_nLifeMax = m_nLife;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSmokeScreen::~CSmokeScreen()
{

}

//====================================================================
//��������
//====================================================================
CSmokeScreen* CSmokeScreen::Create()
{
	CSmokeScreen* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pPlayer = new CSmokeScreen();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pPlayer->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pPlayer;
}

//====================================================================
//����������
//====================================================================
HRESULT CSmokeScreen::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();;
	SetIdx(pTexture->Regist(TEX_PASS));

	SetType(CObject::TYPE_EFFECT);

	CObject2D::Init();

	//�T�C�Y
	SetWidth(SCREEN_WIDTH);
	SetHeight(SCREEN_HEIGHT);
	SetPos(D3DXVECTOR3( SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	//���_�J���[�̐ݒ�
	SetColor(DEF_COL);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CSmokeScreen::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CSmokeScreen::Update(void)
{
	//�T�C�Y�̒���

	m_nLife--;

	//�s�����x�̐ݒ�
	float fRad = (float)m_nLife / (float)m_nLifeMax;

	SetColorA(fRad * DEF_COL.a);

	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}

	//���_���̍X�V
	CObject2D::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CSmokeScreen::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//a�u�����f�B���O�����Z�����ɐݒ�
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//a�u�����f�B���O��ʏ�ɐݒ�
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}