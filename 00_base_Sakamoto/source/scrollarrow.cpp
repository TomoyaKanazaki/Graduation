//============================================
//
//	�X�N���[���A���[ [scrollarrow.cpp]
//	Author:sakamoto kai
//
//============================================
#include "scrollarrow.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float SAMPLE_WIGHT = 70.0f;		//����
	const float SAMPLE_HEIGHT = 70.0f;		//�c��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CScrollArrow::CScrollArrow(int nPriority) : CObject2D(nPriority)
{
	SetWidth(SAMPLE_WIGHT);
	SetHeight(SAMPLE_HEIGHT);
	m_nSample = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CScrollArrow::~CScrollArrow()
{

}

//====================================================================
//��������
//====================================================================
CScrollArrow* CScrollArrow::Create(int nPriority)
{
	CScrollArrow* pSample2D = NULL;

	if (pSample2D == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample2D = new CScrollArrow(nPriority);
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample2D->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pSample2D;
}

//====================================================================
//����������
//====================================================================
HRESULT CScrollArrow::Init(void)
{
	CObject2D::Init();

	//�e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\UI\\signal.png");

	//�V����cpp���쐬�������͐V����TYPE��񋓂ɒǉ����Ďw�肷�邱��
	SetType(CObject::TYPE_SCROLLALLOW);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CScrollArrow::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CScrollArrow::Update(void)
{
	//���_���̍X�V
	CObject2D::Update();

	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		m_State = STATE_UP;
	}
	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		m_State = STATE_DOWN;

	}
	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		m_State = STATE_RIGHT;

	}
	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		m_State = STATE_LEFT;

	}

	D3DXVECTOR3 rot = GetRot();

	switch (m_State)
	{
	case STATE_UP:
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;

	case STATE_LEFT:
		rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f);
		break;

	case STATE_DOWN:
		rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);
		break;

	case STATE_RIGHT:
		rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 1.5f);
		break;
	}

	SetRot(rot);
#endif

	//----���_����ύX�������ꍇ�͈ȉ��̃R�[�h���g�p���邱��-----
	//VERTEX_2D* pVtx;	//���_�|�C���^������

	////���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	//GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	////�ʒu
	//pVtx[0].pos;
	//pVtx[1].pos;
	//pVtx[2].pos;
	//pVtx[3].pos;

	////�F
	//pVtx[0].col;
	//pVtx[1].col;
	//pVtx[2].col;
	//pVtx[3].col;

	////�e�N�X�`��
	//pVtx[0].tex;
	//pVtx[1].tex;
	//pVtx[2].tex;
	//pVtx[3].tex;

	////���_�o�b�t�@���A�����b�N����
	//GetVtxBuff()->Unlock();
}

//====================================================================
//�`�揈��
//====================================================================
void CScrollArrow::Draw(void)
{
	CObject2D::Draw();
}