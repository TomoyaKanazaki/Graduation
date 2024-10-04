//========================================
//
//�V���[�e�B���O�A�N�V����[object3D.h]
//Author�F�X��x��
//
//========================================
#include "object3D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//========================================
// �萔��`
//========================================
namespace
{
const float FIELD_SIZE = 4000.0f;		// �t�B�[���h�̃T�C�Y
const float BILLBOARD_WIDTH = 40.0f;	// �r���{�[�h�̕�
const float BILLBOARD_HEIGHT = 40.0f;	// �r���{�[�h�̍���
}

//========================================
//�R���X�g���N�^
//========================================
CObject3D::CObject3D(int nPriority) :
	CObject(nPriority)
{
	m_pTexture = nullptr;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = nullptr;		// ���_���ւ̃|�C���^
	m_mtxWorld;					// ���[���h�}�g���b�N�X
	m_nIdxTexture = 0;
}

//========================================
//�f�X�g���N�^
//========================================
CObject3D::~CObject3D()
{

}

//========================================
//�I�u�W�F�N�g����
//========================================
CObject3D *CObject3D::Create()
{
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{//pObject3D��nullptr�̎�
		//3D�I�u�W�F�N�g�̐���
		pObject3D = new CObject3D;

		//������
		pObject3D->Init();
	}

	//�|�C���^��Ԃ�
	return pObject3D;
}

//========================================
//�e�N�X�`�����蓖��
//========================================
void CObject3D::BindTexture(int pTexture)
{
	m_nIdxTexture = pTexture;
}

//========================================
//������
//========================================
HRESULT CObject3D::Init(void)
{
	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr)
	{
		//���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr)))
		{// ���s��Ԃ�
			return E_FAIL;
		}

		//���_���
		SetVertex3D();
	}

	//������Ԃ�
	return S_OK;
}

//========================================
//�I��
//========================================
void CObject3D::Uninit(void)
{
	//���_���̏I��
	if (m_pVtxBuff != nullptr)
	{//m_pVtxBuff��nullptr����Ȃ��Ƃ�
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//�I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//========================================
//�X�V
//========================================
void CObject3D::Update(void)
{
	//���_���
	SetVertex3D();
}

//========================================
//�`��
//========================================
void CObject3D::Draw(void)
{
	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//CTexture�^�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// �ʒu�E�����擾
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 Rot = GetRot();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture)); //�e�N�X�`�����g�p���ĂȂ��Ƃ���nullptr

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
//3D�|���S���̒��_���
//========================================
void CObject3D::SetVertex3D(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, FIELD_SIZE);
	pVtx[1].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, FIELD_SIZE);
	pVtx[2].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, -FIELD_SIZE);
	pVtx[3].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, -FIELD_SIZE);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 10.0f);
	pVtx[3].tex = D3DXVECTOR2(10.0f, 10.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//========================================
//�N���̓����蔻��
//========================================
void CObject3D::Collision(void)
{
	//D3DXVECTOR3 vec1, vec2, nor;

	//float fHeight;		//���߂鍂��

	//if ()
	//{
	//	//�O��
	//	D3DXVec3Cross(&nor, &vec1, &vec2);

	//	//�@���𐳋K������
	//	D3DXVec3Normalize(&nor, &nor);

	//	//����
	//	(pos.x - vtx0.x) * nor.x + (fHeight - vtx0.y) * nor.y + (pos.z - vtx0.z) * nor.z = 0.0f;

	//	if (nor.y != 0.0f)
	//	{
	//		fHeight;
	//	}
	//}
}

//========================================
//���b�V���t�B�[���h�̒��_���
//========================================
void CObject3D::MeshVertex(void)
{
	//CRenderer�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr)
	{
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			NULL);

		//���_���ւ̃|�C���^
		VERTEX_3D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntZ = 0; nCntZ < 3; nCntZ++)
		{
			for (int nCntX = 0; nCntX < 3; nCntX++)
			{
				//���_���W�̐ݒ�
				pVtx[nCntX + nCntZ * 3].pos = D3DXVECTOR3(nCntX * 1000.0f, 0.0f, nCntZ * -1000.0f);

				//�@���̐ݒ�
				pVtx[nCntX + nCntZ * 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//���_�J���[�̐ݒ�
				pVtx[nCntX + nCntZ * 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[nCntX + nCntZ * 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}

		//���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		//�C���f�b�N�X�o�b�t�@�̐���
		pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&m_pIdxBuff,
			NULL);

		//�C���f�b�N�X���ւ̃|�C���^
		WORD *pIdx;

		//�C���f�b�N�X�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdx = 0; nCntIdx < 2; nCntIdx++)
		{
			for (int nCntIdx2 = 0; nCntIdx2 < 3; nCntIdx2++)
			{
				pIdx[(nCntIdx2 * 2) + (nCntIdx * 8) + 0] = 3 + nCntIdx2 * 1 + nCntIdx * 3;
				pIdx[(nCntIdx2 * 2) + (nCntIdx * 8) + 1] = 0 + nCntIdx2 * 1 + nCntIdx * 3;
			}
		}

		//�k��
		pIdx[6] = 2;
		pIdx[7] = 6;

		//���_�ԍ��f�[�^�̐ݒ�
		//pIdx[0] = 3;
		//pIdx[1] = 0;
		//pIdx[2] = 4;
		//pIdx[3] = 1;
		//pIdx[4] = 5;
		//pIdx[5] = 2;
		//pIdx[6] = 2;
		//pIdx[7] = 6;
		//pIdx[8] = 6;
		//pIdx[9] = 3;
		//pIdx[10] = 7;
		//pIdx[11] = 4;
		//pIdx[12] = 8;
		//pIdx[13] = 5;

		//�C���f�b�N�X�o�b�t�@���A�����b�N
		m_pIdxBuff->Unlock();
	}
}

//========================================
// �t�B�[���h�̎擾
//========================================
float CObject3D::GetField()
{
	float fField = FIELD_SIZE;

	return fField;
}

//========================================
//�r���{�[�h�̒��_���
//========================================
void CObject3D::SetVerTexBillboard(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_WIDTH, BILLBOARD_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILLBOARD_WIDTH, BILLBOARD_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_WIDTH, -BILLBOARD_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILLBOARD_WIDTH, -BILLBOARD_HEIGHT, 0.0f);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

void CObject3D::SetVertex(void)
{
}

void CObject3D::SetSize(float fWidth, float fHeight)
{
}
