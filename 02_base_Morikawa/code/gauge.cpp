//========================================
//
// �Q�[�W[gauge.h]
// Author�F�X��x��
//
//========================================
#include "gauge.h"
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//========================================
// �萔��`
//========================================
namespace
{
const float LENGTH = 640.0f;		// �Q�[�W�̒���
const char* GAUGETEX[] =
	{// �ǂݍ��ރe�N�X�`��
		"",
		"data\\texture\\gauge.png",
		"data\\texture\\frame.png"
	};
}

//========================================
// �R���X�g���N�^
//========================================
CGauge::CGauge(int nPriority) : CObject(nPriority)
{
	m_nMaxLife = 0;			// �̗͂̍ő�l
	m_nLife = 0;			// ���݂̗̑�
	m_fLength = 0.0f;		// �Ίp���̒���
	m_fAngle = 0.0f;		// �Ίp���̊p�x
	m_aTexU = 0.0f;			// �e�N�X�`����U�l
	memset(m_p2D, 0, sizeof(m_p2D));	// �I�u�W�F�N�g2D�|�C���^
}

//========================================
// �f�X�g���N�^
//========================================
CGauge::~CGauge()
{
}

//========================================
// ����
//========================================
CGauge* CGauge::Create(int nMaxLife)
{
	CGauge* pGauge = new CGauge;

	// �̗͂̍ő�l
	pGauge->m_nMaxLife = nMaxLife;

	pGauge->Init();

	return pGauge;
}

//========================================
// ������
//========================================
HRESULT CGauge::Init()
{
	//�e�N�X�`���̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// �̗͂̏����l
	m_nLife = m_nMaxLife;

	// �e�N�X�`�����W�̏�����(U�l)
	m_aTexU = 0.0f;

	for (int i = 0; i < CGauge::TYPE_MAX; i++)
	{
		if (m_p2D[i] == nullptr)
		{// ��ޕ���
			m_p2D[i] = CObject2D::Create();

			m_p2D[i]->BindTexture(pTexture->Regist(GAUGETEX[i]));
		}
	}

	// ���_���̐ݒ�
	SetVertex();

	// ���_�J���[�̐ݒ�
	m_p2D[CGauge::TYPE_BASE]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//========================================
// �I��
//========================================
void CGauge::Uninit()
{
	// ���g�̏I��
	Release();
}

//========================================
// �X�V
//========================================
void CGauge::Update()
{
	for (int i = 0; i < CGauge::TYPE_MAX; i++)
	{
		// 2D�̍X�V
		if (m_p2D[i] != nullptr)
		{
			m_p2D[i]->Update();
		}
	}

	// ���_���
	SetVertex();

	// �e�N�X�`�����W�̍X�V(U�l)
	m_aTexU += 0.002f;
}

//========================================
// �`��
//========================================
void CGauge::Draw()
{
	for (int i = 0; i < CGauge::TYPE_MAX; i++)
	{
		if (m_p2D[i] != nullptr)
		{// 2D�̕`��
			m_p2D[i]->Draw();
		}
	}
}

//========================================
// �T�C�Y�ݒ�
//========================================
void CGauge::SetSize(float fWidht, float fHeight)
{
	// �Ίp���̒���(���A�c)
	m_fLength = sqrtf(fWidht * fWidht + fHeight * fHeight) * 0.5f;

	// �Ίp���̌���(���A�c)
	m_fAngle = atan2f(fWidht, fHeight);

	// ���_���
	SetVertex();
}

//========================================
// ���_���̐ݒ�
//========================================
void CGauge::SetVertex()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �̗͂̔䗦
	float ratio = (float)m_nLife / (float)m_nMaxLife;

	for (int i = 0; i < CGauge::TYPE_MAX; i++)
	{
		// 2D�̒��_���ݒ�
		m_p2D[i]->SetVertex();

		D3DXCOLOR col = m_p2D[i]->GetCol();

		// ���_���̎擾
		LPDIRECT3DVERTEXBUFFER9 Vertex = m_p2D[i]->GetVtxBuff();

		// ���_���ւ̃|�C���^
		VERTEX_2D* pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		Vertex->Lock(0, 0, (void**)&pVtx, 0);

		switch (i)
		{
		case TYPE_GAUGE:
			// ���_���W�̐ݒ�
			pVtx[0].pos = pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3(LENGTH * ratio, 0.0f, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(0.0f, m_fLength, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3(LENGTH * ratio, m_fLength, 0.0f);
			break;

		default:
			// ���_���W�̐ݒ�
			pVtx[0].pos = pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = pos + D3DXVECTOR3(LENGTH, 0.0f, 0.0f);
			pVtx[2].pos = pos + D3DXVECTOR3(0.0f, m_fLength, 0.0f);
			pVtx[3].pos = pos + D3DXVECTOR3(LENGTH, m_fLength, 0.0f);
			break;
		}

		// �ʒu�E�����ݒ�
		SetPos(pos);
		SetRot(rot);
	}

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 Vertex = m_p2D[CGauge::TYPE_GAUGE]->GetVtxBuff();

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	Vertex->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_aTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_aTexU + ratio, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_aTexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_aTexU + ratio, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	Vertex->Unlock();
}
