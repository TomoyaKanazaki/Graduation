//=======================================
//
//�t�F�[�h[fade.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================
#include "fade.h"

//=======================================
//�R���X�g���N�^
//=======================================
CFade::CFade() :
	m_State(FADE_NONE),					// �������Ȃ����
	m_modeNext(CScene::MODE_TITLE),		// �^�C�g��
	m_pVtxBuff(nullptr), 
	m_pObject2D(nullptr)
{//�l�N���A
}

//=======================================
//�f�X�g���N�^
//=======================================
CFade::~CFade()
{
}

//=======================================
//������
//=======================================
HRESULT CFade::Init(CScene::MODE modenext)
{
	// �I�u�W�F�N�g2D�̃|�C���^
	m_pObject2D = CObject2D::Create();

	// �F�擾
	D3DXCOLOR col = m_pObject2D->GetCol();
	col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// �ʒu�ݒ�
	m_pObject2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	// �T�C�Y�ݒ�
	m_pObject2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// �F�ݒ�
	m_pObject2D->SetCol(col);

	// ���_���
	m_pObject2D->SetVertex();

	//�t�F�[�h�ݒ�
	SetFade(modenext);

	m_pObject2D->SetDrawFrag(true);

	return S_OK;
}

//=======================================
//�I��
//=======================================
void CFade::Uninit()
{
	if (m_pObject2D != nullptr)
	{// �I��
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}
}

//=======================================
//�X�V
//=======================================
void CFade::Update()
{
	if (m_pObject2D == nullptr)return;

	// �F�擾
	D3DXCOLOR col = m_pObject2D->GetCol();

	// �X�V
	m_pObject2D->Update();

	if (m_State != FADE_NONE)
	{
		switch (m_State)
		{
		case FADE_OUT:
			col.a += 0.03f;
			if (col.a >= 1.0f)
			{
				col.a = 1.0f;

				//�t�F�[�h�C��
				m_State = FADE_IN;

				//���[�h�ݒ�
				CManager::GetInstance()->SetMode(m_modeNext);
			}
			break;

		case FADE_IN:
			col.a -= 0.03f;
			if (col.a <= 0.0f)
			{
				col.a = 0.0f;

				//�t�F�[�h�C��
				m_State = FADE_NONE;
			}
			break;
		}
	}

	// �F�ݒ�
	m_pObject2D->SetCol(col);
}

//=======================================
//�`��
//=======================================
void CFade::Draw()
{
	if (m_pObject2D == nullptr) return;

	// �`��
	m_pObject2D->Draw();
}

//=======================================
//�ݒ�
//=======================================
void CFade::SetFade(CScene::MODE modenext)
{
	// �F�擾
	D3DXCOLOR col = m_pObject2D->GetCol();

	if (m_State != FADE_OUT)
	{
		// ��Ԑݒ�
		m_State = FADE_OUT;

		// ���̉��
		m_modeNext = modenext;

		// �s�����|���S��
		col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//=======================================
// ��Ԑݒ�
//=======================================
void CFade::SetState(FADE state)
{
	if (m_pObject2D == nullptr)return;

	// �F�擾
	D3DXCOLOR col = m_pObject2D->GetCol();
	col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_State = state;

	switch (state)
	{
	case FADE_NONE:
		col.a = 0.0f;
		break;

	case FADE_IN:
		col.a = 1.0f;
		break;

	case FADE_OUT:
		col.a = 0.0f;
		break;

	default:
		break;
	}

	// �F�ݒ�
	m_pObject2D->SetCol(col);
}

//=======================================
//����
//=======================================
CFade *CFade::Create(CScene::MODE modenext)
{
	CFade* pFade = new CFade;

	pFade->Init(modenext);
	pFade->SetState(FADE_IN);

	return pFade;
}
