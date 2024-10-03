//==============================================================
//
//�G�t�F�N�g�̏���[effect.cpp]
//Author:����������
//
//==============================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"

//�}�N����`
#define RADIUS_EFFECT		(40.0f * 0.5f)		//�G�t�F�N�g�̔��a
#define NUM_PATTERN			(10)				//�p�^�[����
#define MOVE_SPEED			(3.0f)				//�G�t�F�N�g�̈ړ����x
#define DEST_EFFECT			(8)					//�G�t�F�N�g�����ł���܂ł̎���
#define SUB_RADIUS			(0.5f)				//�G�t�F�N�g�̑傫��

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;		//�e�N�X�`��

//==============================================================
//�R���X�g���N�^
//==============================================================
CEffect::CEffect()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_fRadius = RADIUS_EFFECT;			//���a�i�傫���j
	m_nLife = DEST_EFFECT;		//�G�t�F�N�g���ł̎���
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CEffect::~CEffect()
{

}

//==============================================================
//�G�t�F�N�g�̐�������
//==============================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type)
{
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{//���������g�p����ĂȂ�������

		//�G�t�F�N�g�̐���
		pEffect = new CEffect;
	}

	//����������
	pEffect->Init(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�G�t�F�N�g�̐ݒ�
	pEffect->Set(pos, move, col, fRadius, nLife, type);

	return pEffect;
}

//==============================================================
//�G�t�F�N�g�̏���������
//==============================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect001.png");

	//�e�N�X�`���̊��蓖��
	CBillboard::BindTexture(m_nIdxTexture);

	//��ސݒ�
	CBillboard::SetType(CObject::TYPE_EFFECT);

	//�r���{�[�h�̏���������
	CBillboard::Init();

	//�ʒu�ݒ�
	CBillboard::SetPosition(pos);

	//�G�t�F�N�g�̑傫���ݒ�
	CBillboard::SetSize(m_fRadius, m_fRadius);

	return S_OK;
}

//==============================================================
//�G�t�F�N�g�̏I������
//==============================================================
void CEffect::Uninit(void)
{
	//�r���{�[�h�̏I������
	CBillboard::Uninit();
}

//==============================================================
//�G�t�F�N�g�̍X�V����
//==============================================================
void CEffect::Update(void)
{
	//���ł܂ł̎��Ԍ��Z
	m_nLife--;

	//�ʒu�擾
	m_pos = CBillboard::GetPosition();

	//�F�擾
	m_col = CBillboard::GetColor();

	//�ړ��ʉ��Z
	m_pos += m_move;

	//�F���Z
	m_col -= D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.0f);

	//���a���Z
	m_fRadius += -SUB_RADIUS;

	//�ʒu�ݒ�
	CBillboard::SetPosition(m_pos);

	//�T�C�Y�ݒ�
	CBillboard::SetSize(m_fRadius, m_fRadius);

	//�F�ݒ�
	CBillboard::SetColor(m_col);

	//�r���{�[�h�̍X�V����
	CBillboard::Update();

	//�G�t�F�N�g�폜
	if (m_nLife <= 0 && m_fRadius <= 0.0f)
	{//�G�t�F�N�g�����ł��鎞�ԂɂȂ�����

		//�G�t�F�N�g�폜
		CEffect::Release();
	}
}

//==============================================================
//�G�t�F�N�g�̕`�揈��
//==============================================================
void CEffect::Draw(void)
{
	//�r���{�[�h�̕`�揈��
	//if (m_type != PARTICLETYPE_ENEMY && m_type != PARTICLETYPE_LIFE && m_type != PARTICLETYPE_MOVE)
	/*{
		CBillboard::SetDraw();
	}
	else*/
	{
		CBillboard::Draw();

	}
}

//==============================================================
//�G�t�F�N�g�̐ݒ菈��
//==============================================================
void CEffect::Set(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type)
{
	//�ϐ����
	m_pos = pos;
	m_move = move;
	m_col = col;
	m_fRadius = fRadius;
	m_nLife = nLife;
	m_type = type;

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	switch (m_type)
	{
	case PARTICLETYPE_NONE:		//�����Ȃ�

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect004.png");

		break;

	case PARTICLETYPE_EXPLOSION:	//����

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect000.jpg");

		break;

	case PARTICLETYPE_LIFE:			//��

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect005.png");

		break;

	case PARTICLETYPE_MOVE:			//�ړ�

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect005.png");

		break;

	case PARTICLETYPE_A:

		break;

	default:		//���̑�

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect000.jpg");

		break;
	}

	//�e�N�X�`���̊��蓖��
	CBillboard::BindTexture(m_nIdxTexture);

	//�ʒu�ݒ�
	CBillboard::SetPosition(m_pos);

	//�F�ݒ�
	CBillboard::SetColor(m_col);
}