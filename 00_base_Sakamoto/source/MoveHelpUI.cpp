//============================================
//
//	�ړ������̃w���vUI [MoveHelpUI.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MoveHelpUI.h"
#include "renderer.h"
#include "manager.h"

//�}�N����`
#define NUMBER_WIGHT (100.0f)		//����
#define NUMBER_HEIGHT (100.0f)		//�c��

//====================================================================
//�R���X�g���N�^
//====================================================================
CMoveHelpUI::CMoveHelpUI(int nPriority) : CObject3D(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_nSample = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMoveHelpUI::~CMoveHelpUI()
{

}

//====================================================================
//��������
//====================================================================
CMoveHelpUI* CMoveHelpUI::Create(void)
{
	CMoveHelpUI* pSample3D = NULL;

	if (pSample3D == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample3D = new CMoveHelpUI();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample3D->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pSample3D;
}

//====================================================================
//����������
//====================================================================
HRESULT CMoveHelpUI::Init(void)
{
	CObject3D::Init();

	VERTEX_3D* pVtx;	//���_�|�C���^������
	float Width = GetWidth();
	float Height = GetHeight();
	float Rot = GetRot().y;

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� 
	float nLength = sqrtf(Width * Width + Height * Height) * 0.5f;
	float fAngle = atan2f(Width, Height);

	pVtx[0].pos.x = sinf(-fAngle + Rot) * nLength;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = cosf(-fAngle + Rot) * nLength;

	pVtx[1].pos.x = sinf(+fAngle + Rot) * nLength;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = cosf(+fAngle + Rot) * nLength;

	pVtx[2].pos.x = sinf(-D3DX_PI + fAngle + Rot) * nLength;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = cosf(-D3DX_PI + fAngle + Rot) * nLength;

	pVtx[3].pos.x = sinf(D3DX_PI - fAngle + Rot) * nLength;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = cosf(D3DX_PI - fAngle + Rot) * nLength;

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	GetVtxBuff()->Unlock();

	//�e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\MoveHelpUI.png");

	//�V����cpp���쐬�������͐V����TYPE��񋓂ɒǉ����Ďw�肷�邱��
	SetType(CObject::TYPE_3DUI);

	SetLighting(false);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CMoveHelpUI::Uninit(void)
{
	CObject3D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CMoveHelpUI::Update(void)
{
	//���_���̍X�V
	CObject3D::Update();

	//���_���W�̍X�V
	SetVerTex();
}

//====================================================================
//�`�揈��
//====================================================================
void CMoveHelpUI::Draw(void)
{
	CObject3D::Draw();
}

//====================================================================
//���_���W�̐ݒ�
//====================================================================
void CMoveHelpUI::SetVerTex(void)
{
	VERTEX_3D* pVtx;	//���_�|�C���^������
	float Width = GetWidth();
	float Height = GetHeight();
	float Rot = GetRot().z;

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� 
	float nLength = sqrtf(Width * Width + Height * Height) * 0.5f;
	float fAngle = atan2f(Width, Height);

	pVtx[0].pos.x = sinf(-fAngle + Rot) * nLength;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = cosf(-fAngle + Rot) * nLength;

	pVtx[1].pos.x = sinf(+fAngle + Rot) * nLength;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = cosf(+fAngle + Rot) * nLength;

	pVtx[2].pos.x = sinf(-D3DX_PI + fAngle + Rot) * nLength;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = cosf(-D3DX_PI + fAngle + Rot) * nLength;

	pVtx[3].pos.x = sinf(D3DX_PI - fAngle + Rot) * nLength;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = cosf(D3DX_PI - fAngle + Rot) * nLength;

	//���_�o�b�t�@���A�����b�N����
	GetVtxBuff()->Unlock();
}