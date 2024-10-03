//==============================================================
//
//�t�F�[�h����[fade.cpp]
//Author:����������
//
//==============================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//�}�N����`
#define WIDTH_BG		(SCREEN_WIDTH * 0.5f)		//����
#define HEIGHT_BG		(SCREEN_HEIGHT * 0.5f)		//�c��

//==============================================================
//�R���X�g���N�^
//==============================================================
CFade::CFade()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�

	m_fade = FADE_IN;										//�������ĂȂ����
	m_modeNext = CScene::MODE_TITLE;						//���̉�ʁi���[�h�j��ݒ�
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		//�����|���S���i�s�����j�ɂ��Ă���

	m_bTrans = false;		//�����̃t�F�[�h��
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CFade::~CFade()
{

}

//==============================================================
//��������
//==============================================================
CFade *CFade::Create()
{
	CFade *pFade = NULL;

	if (pFade == NULL)
	{//���������g�p����ĂȂ�������

		//�t�F�[�h�̐���
		pFade = new CFade;
	}

	//����������
	pFade->Init();

	return pFade;
}

//==============================================================
//�t�F�[�h�̏���������
//==============================================================
HRESULT CFade::Init(void)
{
	int mode = CManager::GetInstance()->GetMode();		//���݂̃Q�[�����[�h�擾

	//�e�N�X�`���̊��蓖��
	CObject2D::BindTexture(NULL);

	//�I�u�W�F�N�g2D�̏���������
	CObject2D::Init();

	//��ނ̐ݒ�
	CObject2D::SetType(CObject::TYPE_FADE);

	//�t�F�[�h�̑傫���ݒ�
	CObject2D::SetSize(WIDTH_BG, HEIGHT_BG);

	//�ʒu�ݒ�
	CObject2D::SetPosition(D3DXVECTOR3(WIDTH_BG, HEIGHT_BG, 0.0f));

	//���_�J���[�̐ݒ�
	CObject2D::SetColor(m_colorFade);

	return S_OK;
}

//==============================================================
//�t�F�[�h�̏I������
//==============================================================
void CFade::Uninit(void)
{
	//2D�I�u�W�F�N�g�̏I������
	//CObject2D::Uninit();

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�t�F�[�h�̍X�V����
//==============================================================
void CFade::Update(void)
{
	CScene *pScene = CManager::GetInstance()->GetScene();

	if (m_fade != FADE_NONE)
	{//�������Ă��Ȃ���Ԃ���Ȃ��Ƃ�

		if (m_fade == FADE_IN)
		{//�t�F�[�h�C�����

			m_colorFade.a -= 0.03f;		//�|���S���𓧖��ɂ��Ă���

			if (m_colorFade.a <= 0.0f)
			{//�����ɂȂ�����

				m_colorFade.a = 0.0f;
				m_fade = FADE_NONE;		//�������Ă��Ȃ���Ԃ�
			}
		}
		else if (m_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���

			m_colorFade.a += 0.03f;		//�|���S����s�����ɂ��Ă���

			if (m_colorFade.a >= 1.0f)
			{//�������ɂȂ�����

				m_colorFade.a = 1.0f;
				m_fade = FADE_IN;		//�t�F�[�h�C����Ԃ�

				if (m_bTrans == true)
				{//��ʑJ�ڂ�����t�F�[�h�̂Ƃ�

					//���[�h�ݒ�i���̉�ʂɈڍs�j
					CManager::GetInstance()->SetMode(m_modeNext);
				}
			}
		}

		//���_�J���[�̐ݒ�
		CObject2D::SetColor(m_colorFade);

	}

	//2D�I�u�W�F�N�g�̍X�V����
	CObject2D::Update();
}

//==============================================================
//�t�F�[�h�̕`�揈��
//==============================================================
void CFade::Draw(void)
{
	if (m_fade != FADE_NONE)
	{//�������ĂȂ���Ԃ̂Ƃ�

		//2D�I�u�W�F�N�g�̍X�V����
		CObject2D::Draw();
	}
}

//==============================================================
//�t�F�[�h�̐ݒ菈��
//==============================================================
void CFade::SetFade(CScene::MODE modeNext)
{
	if (m_fade == FADE_NONE)
	{//�������Ă��Ȃ���Ԃ̂Ƃ�

		m_bTrans = true;			//��ʑJ�ڂ����Ȃ�
		m_fade = FADE_OUT;										//�t�F�[�h�A�E�g���
		m_modeNext = modeNext;									//���̉�ʁi���[�h�j��ݒ�
		m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//�����|���S���i�����j�ɂ��Ă���
	}
}

//==============================================================
//�����̃t�F�[�h�̐ݒ菈��
//==============================================================
void CFade::SetNormalFade(void)
{
	if (m_fade == FADE_NONE)
	{//�������Ă��Ȃ���Ԃ̂Ƃ�

		m_bTrans = false;			//��ʑJ�ڂ�����
		m_fade = FADE_OUT;										//�t�F�[�h�A�E�g���
		m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//�����|���S���i�����j�ɂ��Ă���
	}
}