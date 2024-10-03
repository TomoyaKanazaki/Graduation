//==============================================================
//
//��������[number.cpp]
//Author:����������
//
//==============================================================
#include "number.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//�}�N����`
#define PRIORITY		(4)				//�D�揇��
#define ANGLE_UP		(0.75f)			//��̊p�x
#define ANGLE_DOWN		(0.25f)			//���̊p�x

//==============================================================
//�R���X�g���N�^
//==============================================================
CNumber::CNumber()
{
	m_nIdxTexture = -1;		//�e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_type = CObject::TYPE_NONE;		//���
	m_fHeight = 0.0f;		//�c��
	m_fWidth = 0.0f;		//����
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CNumber::~CNumber()
{

}

//==============================================================
//��������
//==============================================================
CNumber *CNumber::Create(void)
{
	CNumber *pNumber = NULL;

	if (pNumber == NULL)
	{//���������g�p����ĂȂ�������

		//�����̐���
		pNumber = new CNumber;
	}

	//����������
	pNumber->Init();

	return pNumber;
}

//==============================================================
//�����̏���������
//==============================================================
HRESULT CNumber::Init(void)
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

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

	return S_OK;
}

//==============================================================
//�����̏I������
//==============================================================
void CNumber::Uninit(void)
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
//�����̍X�V����
//==============================================================
void CNumber::Update(void)
{
	
}

//==============================================================
//�����̕`�揈��
//==============================================================
void CNumber::Draw(void)
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
void CNumber::BindTexture(int nIdxTexture)
{
	m_nIdxTexture = nIdxTexture;		//�e�N�X�`�����蓖��
}

//==============================================================
//�����̈ʒu�ݒ菈��
//==============================================================
void CNumber::SetPosition(TYPE type, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	m_pos = pos;		//�ʒu

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (type)
	{
	case CObject::TYPE_SCORE:		//�X�R�A

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHeight, 0.0f);

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
//�����̐F�ݒ菈��
//==============================================================
void CNumber::SetColor(TYPE type, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	m_col = col;		//�F

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================
//�A�j���[�V�����̐ݒ菈��
//==============================================================
void CNumber::SetAnim(int nPatternAnim, float fDigitX)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	/*switch (m_type)
	{
	case TYPE_SCORE:*/

		//�e�N�X�`�����W�̍Đݒ�
		pVtx[0].tex = D3DXVECTOR2(nPatternAnim * fDigitX, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nPatternAnim * fDigitX + fDigitX, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nPatternAnim * fDigitX, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nPatternAnim * fDigitX + fDigitX, 1.0f);

		/*break;
	}*/
	
	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}