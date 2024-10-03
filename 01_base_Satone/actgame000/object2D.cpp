//==============================================================
//
//�I�u�W�F�N�g2D����[object2D.cpp]
//Author:����������
//
//==============================================================
#include "object2D.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//�}�N����`
#define PRIORITY		(3)					//�D�揇��
#define ANGLE_UP		(0.75f)				//��̊p�x
#define ANGLE_DOWN		(0.25f)				//���̊p�x

//==============================================================
//�R���X�g���N�^
//==============================================================
CObject2D::CObject2D()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_type = CObject::TYPE_NONE;		//���
	m_fHeight = 0.0f;		//�c��
	m_fWidth = 0.0f;		//����
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CObject2D::~CObject2D()
{

}

//==============================================================
//��������
//==============================================================
CObject2D *CObject2D::Create(void)
{
	CObject2D *pObject2D = NULL;

	if (pObject2D == NULL)
	{//���������g�p����ĂȂ�������

		//�I�u�W�F�N�g2D�̐���
		pObject2D = new CObject2D;
	}

	//����������
	pObject2D->Init();

	return pObject2D;
}

//==============================================================
//�I�u�W�F�N�g2D�̏���������
//==============================================================
HRESULT CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI * ANGLE_UP) * m_fWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI * ANGLE_UP) * m_fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI * ANGLE_UP) * m_fWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI * ANGLE_UP) * m_fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI * ANGLE_DOWN) * m_fWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI * ANGLE_DOWN) * m_fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI * ANGLE_DOWN) * m_fWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI * ANGLE_DOWN) * m_fHeight;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//��ސݒ�
	CObject::SetType(CObject::TYPE_NONE);

	return S_OK;
}

//==============================================================
//�I�u�W�F�N�g2D�̏I������
//==============================================================
void CObject2D::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�I�u�W�F�N�g2D�̍X�V����
//==============================================================
void CObject2D::Update(void)
{
	
}

//==============================================================
//�I�u�W�F�N�g2D�̕`�揈��
//==============================================================
void CObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();		//�e�N�X�`���̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//2D�I�u�W�F�N�g�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,												//�v���~�e�B�u�i�v���C���[�̐��j
		2);												//�`�悷��v���~�e�B�u��
}

//==============================================================
//�e�N�X�`�����蓖�ď���
//==============================================================
void CObject2D::BindTexture(int nIdxTexture)
{
	m_nIdxTexture = nIdxTexture;		//�e�N�X�`�����蓖��
}

//==============================================================
//�I�u�W�F�N�g2D�̈ʒu�ݒ菈��
//==============================================================
void CObject2D::SetPosition(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	m_pos = pos;			//�ʒu

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_type)
	{
	case CObject::TYPE_NONE:

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		break;

	case CObject::TYPE_PLAYER:		//�v���C���[

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);

		break;

	case CObject::TYPE_FADE:		//�t�F�[�h

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

		break;

	case CObject::TYPE_EFFECT:		//�G�t�F�N�g

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

		break;

	case CObject::TYPE_SCORE:		//�X�R�A

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

		break;

	case CObject::TYPE_BLOCK:		//�u���b�N

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

		break;

	default:

		//���_���W�̐ݒ�
		pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI * ANGLE_UP) * m_fWidth;
		pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI * ANGLE_UP) * m_fHeight;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI * ANGLE_UP) * m_fWidth;
		pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI * ANGLE_UP) * m_fHeight;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI * ANGLE_DOWN) * m_fWidth;
		pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI * ANGLE_DOWN) * m_fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI * ANGLE_DOWN) * m_fWidth;
		pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI * ANGLE_DOWN) * m_fHeight;
		pVtx[3].pos.z = 0.0f;

		break;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================
//�I�u�W�F�N�g2D�̐F�ݒ菈��
//==============================================================
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	m_col = col;		//�F

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_type)
	{
	case CObject::TYPE_EFFECT:		//�G�t�F�N�g

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

		break;

	default:

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

		break;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================
//�A�j���[�V�����ݒ菈��
//==============================================================
void CObject2D::SetAnim(int nPatternAnim)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_type)
	{
	case TYPE_EXPLOSION:

		//�e�N�X�`�����W�̍Đݒ�
		pVtx[0].tex = D3DXVECTOR2(nPatternAnim * 0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nPatternAnim * 0.125f + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nPatternAnim * 0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nPatternAnim * 0.125f + 0.125f, 1.0f);

		break;
	}
	
	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================
//�w�i�ݒ菈��
//==============================================================
void CObject2D::SetbgMulti(float fTexU, float fTexV)
{
	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍X�V
	pVtx[0].tex = D3DXVECTOR2(fTexU, fTexV);
	pVtx[1].tex = D3DXVECTOR2(fTexU + 1.0f, fTexV);
	pVtx[2].tex = D3DXVECTOR2(fTexU, fTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTexU + 1.0f, fTexV + 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================
//�I�u�W�F�N�g2D�̕`��ݒ�
//==============================================================
void CObject2D::SetDraw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();		//�e�N�X�`���̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//2D�I�u�W�F�N�g�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,												//�v���~�e�B�u�i�v���C���[�̐��j
		2);												//�`�悷��v���~�e�B�u��

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================
//�I�u�W�F�N�g2D�̎�ސݒ�ݒ�
//==============================================================
void CObject2D::SetType(CObject::TYPE type)
{
	m_type = type;

	//��ސݒ�
	CObject::SetType(m_type);
}