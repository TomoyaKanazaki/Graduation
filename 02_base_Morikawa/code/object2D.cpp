//========================================
//
//�I�u�W�F�N�g2D[object.cp]
//Author�F�X��x��
//
//========================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "input.h"
#include "texture.h"

//========================================
// �萔��`
//========================================
namespace
{
const int NUM_PLACE = 8;	// �X�R�A�̌���
const float SCORE_WIDTH = 20.0f;	// �X�R�A�̕�
const float SCORE_HEIGHT = 20.0f;	// �X�R�A�̍���
const float SCORE_SPACE = 40.0f;	// �X�R�A�̌������̊Ԋu
}

//========================================
//�R���X�g���N�^
//========================================
CObject2D::CObject2D(int nPriority) :
	CObject(nPriority),
	m_col(1.0f, 1.0f, 1.0f, 1.0f),	//�F
	m_fLength(0.0f),		//�Ίp���̒���
	m_fAngle(0.0f),			//�Ίp���̊p�x
	m_nCounterAnim(0),		//�A�j���[�V�����J�E���^�[
	m_nPatternAnim(0),		//�A�j���[�V�����p�^�[��No
	m_pVtxBuff(nullptr),	//���_�����i�[
	m_nIdxTexture(0)		//�e�N�X�`���̔ԍ�
{//�l�N���A

	m_size = D3DXVECTOR2(0.0f, 0.0f);		// �T�C�Y
	m_sizeOrigin = D3DXVECTOR2(0.0f, 0.0f);	// ���̃T�C�Y
}

//========================================
//�f�X�g���N�^
//========================================
CObject2D::~CObject2D(void)
{
}

//========================================
//�|���S���̐���
//========================================
CObject2D *CObject2D::Create()
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//2D�I�u�W�F�N�g�̐���
		pObject2D = new CObject2D;

		//������
		pObject2D->Init();
	}

	//�|�C���^��Ԃ�
	return pObject2D;
}

//========================================
//�|���S���̏�����
//========================================
HRESULT CObject2D::Init(void)
{
	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr)
	{
		//���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr)))
		{// ���s��Ԃ�
			return E_FAIL;
		}

		//�ϐ��̏�����
		m_fLength = sqrtf(50.0f * 50.0f + 50.0f * 50.0f) * 1.0f;	//�Ίp���̒������Z�o����
		m_fAngle = atan2f(50.0f, 50.0f);	//�Ίp���̊p�x���Z�o����

		//���_���
		SetVertex();
	}

	return S_OK;
}

//========================================
//�|���S���̏I��
//========================================
void CObject2D::Uninit(void)
{
	//���_���̏I��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//�I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//========================================
//�|���S���̍X�V
//========================================
void CObject2D::Update(void)
{
	//���_���W
	SetVertex();
}

//========================================
//�|���S���̕`��
//========================================
void CObject2D::Draw(void)
{
	//CRenderer�^�̃|�C���^
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//CTexture�^�̃|�C���^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
//���_���̐ݒ�
//========================================
void CObject2D::SetVertex(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXCOLOR col = GetCol();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z - D3DX_PI + m_fAngle) *  m_fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - D3DX_PI + m_fAngle) *  m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + D3DX_PI - m_fAngle) *  m_fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + D3DX_PI - m_fAngle) *  m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - m_fAngle) *  m_fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - m_fAngle) *  m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + m_fAngle) *  m_fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + m_fAngle) *  m_fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	SetPos(pos);
	SetRot(rot);
}

//========================================
//�|���S���̃T�C�Y����
//========================================
void  CObject2D::SetSize(float fWidht, float fHeight)
{
	//�Ίp���̒���(���A�c)
	m_fLength = sqrtf(fWidht * fWidht + fHeight * fHeight) * 0.5f;

	//�Ίp���̌���(���A�c)
	m_fAngle = atan2f(fWidht, fHeight);

	// �T�C�Y
	m_size = D3DXVECTOR2(fWidht, fHeight);

	//���_���
	SetVertex();
}

//========================================
//�e�N�X�`���̊��蓖��
//========================================
void CObject2D::BindTexture(int pTexture)
{
	m_nIdxTexture = pTexture;
}

//========================================
//�A�j���[�V�����e�N�X�`�����W�̍X�V
//========================================
void CObject2D::SetVertexAnim(int Num, int Cnt)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍Đݒ�
	pVtx[0].tex = D3DXVECTOR2(1.0f / Num * Cnt, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / Num * (Cnt + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / Num * Cnt, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / Num * (Cnt + 1), 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//���_���W
//========================================
void CObject2D::SetPosVertex(void)
{
	// �ʒu�E����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = pos.x + sinf(rot.z - D3DX_PI + m_fAngle) *  m_fLength;
	pVtx[0].pos.y = pos.y + cosf(rot.z - D3DX_PI + m_fAngle) *  m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(rot.z + D3DX_PI - m_fAngle) *  m_fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + D3DX_PI - m_fAngle) *  m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(rot.z - m_fAngle) *  m_fLength;
	pVtx[2].pos.y = pos.y + cosf(rot.z - m_fAngle) *  m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(rot.z + m_fAngle) *  m_fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z + m_fAngle) *  m_fLength;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	SetPos(pos);
	SetRot(rot);
}

//========================================
//�w�i�̒��_���
//========================================
void CObject2D::SetVertexBg(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//�e�N�X�`�����W�̊J�n�ʒu(V�l)
//========================================
void CObject2D::SetVertexMoveBg(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W(V�l�̍X�V)
	m_aTexV -= 0.001f;

	//�e�N�X�`�����W�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f, m_aTexV);
	pVtx[1].tex = D3DXVECTOR2(1.0f, m_aTexV);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_aTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, m_aTexV + 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//�G�t�F�N�g�̒��_���W
//========================================
void CObject2D::SetVertexEffect(float radius, D3DXCOLOR col)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W���X�V
	pVtx[0].pos = D3DXVECTOR3(pos.x - radius, pos.y - radius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + radius, pos.y - radius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - radius, pos.y + radius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + radius, pos.y + radius, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	// �ʒu�E�����ݒ�
	SetPos(pos);
	SetRot(rot);
}

//========================================
// ���_���̐F�ݒ�
//========================================
void CObject2D::SetVertexColor(D3DXCOLOR col)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//�X�R�A���_���
//========================================
void CObject2D::SetScoreVertex(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - SCORE_WIDTH, pos.y - SCORE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + SCORE_WIDTH, pos.y - SCORE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - SCORE_WIDTH, pos.y + SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + SCORE_WIDTH, pos.y + SCORE_HEIGHT, 0.0f);

		//�X�R�A�̌��������炷
		pos.x += SCORE_SPACE;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //0.0�`1.0f�Őݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + m_nPatternAnim * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + m_nPatternAnim * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + m_nPatternAnim * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + m_nPatternAnim * 0.1f, 1.0f);

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	SetPos(pos);
	SetRot(rot);
}

//========================================
//�X�R�A�ݒ�̒��_���
//========================================
void CObject2D::SetScore(int nNum[8])
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//���_���W���X�V
		pVtx[0].pos = D3DXVECTOR3(pos.x - SCORE_WIDTH, pos.y - SCORE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + SCORE_WIDTH, pos.y - SCORE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - SCORE_WIDTH, pos.y + SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + SCORE_WIDTH, pos.y + SCORE_HEIGHT, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //0.0�`1.0f�Őݒ�
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + nNum[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + nNum[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + nNum[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + nNum[nCntScore] * 0.1f, 1.0f);

		pVtx += 4;

		//�X�R�A�̌��������炷
		pos.x += SCORE_SPACE;
	}
	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	SetPos(pos);
	SetRot(rot);
}
