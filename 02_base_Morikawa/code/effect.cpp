//=======================================
//
//�G�t�F�N�g����[effect.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "effect.h"      //�쐬����effect.h���C���N���[�h����
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE[CEffect::TYPE::TYPE_MAX] =
	{
		"data\\TEXTURE\\effect\\effect000.jpg",		// �ʏ�
		"data\\TEXTURE\\effect\\effect_point01.tga",	// �_
		"data\\TEXTURE\\effect\\smoke_07.png",			// ��
		"data\\TEXTURE\\effect\\smoke_05.tga",			// ����
		"data\\TEXTURE\\effect\\effect000.png",			// ��
		"data\\TEXTURE\\effect\\effect001.png",			// �\��
		"data\\TEXTURE\\effect\\Star01.png",			// �\��
		"data\\TEXTURE\\effect\\thunder_02.tga",		// ��
		"",												// NULL
	};
}
int CEffect::m_TexIdx[TYPE::TYPE_MAX] = {};	// �e�N�X�`���C���f�b�N�X
bool CEffect::m_bTexLoad = false;			// �e�N�X�`���̓ǂݍ��ݔ���

//===========================================
//�R���X�g���N�^
//===========================================
CEffect::CEffect(int nPriority) : CBillboard(nPriority)
{
	m_aEffect.nLife = 0;		// ����(�\������)
	m_aEffect.nFirstLife = 0;	// ��������
	m_aEffect.balpha = false;	// ���Z����
	m_fAddSizeValue = 0.0f;		// �T�C�Y���Z��
}

//===========================================
//�f�X�g���N�^
//===========================================
CEffect::~CEffect()
{
}

//===========================================
//����
//===========================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, bool balpha, TYPE type)
{
	// �e�N�X�`���ǂݍ���
	if (!m_bTexLoad) TexLoad();

	//CEffect�^�̃|�C���^
	CEffect *pEffect = nullptr;

	if (pEffect == nullptr)
	{//nullptr�̎�
		//�C���X�^���X����
		pEffect = new CEffect;

		pEffect->SetPos(pos);
		pEffect->SetMove(move);
		pEffect->SetCol(col);
		pEffect->SetSize(fRadius, fRadius);
		pEffect->m_aEffect.nLife = nLife;
		pEffect->m_aEffect.balpha = balpha;

		pEffect->m_aEffect.fFirstAlpha = col.a;

		//������
		pEffect->Init();

		//�e�N�X�`�����蓖��
		pEffect->BindTexture(m_TexIdx[type]);
	}

	//�|�C���^��Ԃ�
	return pEffect;
}

//===========================================
//����������
//===========================================
HRESULT CEffect::Init(void)
{
	//������
	CBillboard::Init();

	// ��������
	m_aEffect.nFirstLife = m_aEffect.nLife;

	//������Ԃ�
	return S_OK;
}

//===========================================
//�e�N�X�`���ǂݍ���
//===========================================
void CEffect::TexLoad()
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_TexIdx[i] = pTexture->Regist(TEXTURE[i]);
	}

	m_bTexLoad = true;
}

//===========================================
//�I������
//===========================================
void CEffect::Uninit(void)
{
	//�I��
	CBillboard::Uninit();
}

//===========================================
//�X�V����
//===========================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXCOLOR col = GetCol();

	//�X�V
	CBillboard::Update();

	//���l(�����鑬�x)
	col.a = m_aEffect.fFirstAlpha * ((float)m_aEffect.nLife / (float)m_aEffect.nFirstLife);

	// �F�ݒ�
	SetCol(col);

	// �ʒu�X�V
	pos += move;

	// �ʒu�ݒ�
	SetPos(pos);

	// ���T�C�Y
	Resize();

	//�̗͌��Z
	m_aEffect.nLife--;

	if (m_aEffect.nLife < 0)
	{
		//�j������
		Uninit();
	}
}

//===========================================
// ���T�C�Y
//===========================================
void CEffect::Resize()
{
	float radius = GetSize();
	radius += m_fAddSizeValue;
	SetSize(radius, radius);
}

//===========================================
//�`�揈��
//===========================================
void CEffect::Draw(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_aEffect.balpha == true)
	{// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�`��
	CBillboard::Draw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
