//==============================================================
//
//�p�[�e�B�N���̏���[particle.cpp]
//Author:����������
//
//==============================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "effect.h"
#include "input.h"

//�}�N����`
#define MAX_PARTICLE		(512)				//�p�[�e�B�N���̍ő吔
#define WIDTH_PARTICLE		(40.0f * 0.5f)		//�p�[�e�B�N���̉���
#define HEIGHT_PARTICLE		(40.0f * 0.5f)		//�p�[�e�B�N���̏c��
#define RADIUS_PARTICLE		(40.0f * 0.5f)		//�p�[�e�B�N���̔��a
#define NUM_PATTERN			(10)				//�p�^�[����
#define MOVE_SPEED			(3.0f)				//�p�[�e�B�N���̈ړ����x
#define DEST_PARTICLE		(8)					//�p�[�e�B�N�������ł���܂ł̎���
#define MOVE_WATER			(70)				//���̈ړ���
#define MOVE_ICE			(20)				//�X�̈ړ���
#define MOVE_VAPOR			(10)				//���C�̈ړ���
#define MOVE_EXPLOSION		(50)				//�����̈ړ���
#define LIFE_ICE			(35)				//�X�̎���
#define LIFE_VAPOR			(50)				//���C�̎���

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;		//�e�N�X�`��

//==============================================================
//�R���X�g���N�^
//==============================================================
CParticle::CParticle()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);		//�F
	m_fRadius = 0.0f;			//���a�i�傫���j
	m_type = PARTICLETYPE_NONE;				//���
	m_nLife = DEST_PARTICLE;	//�p�[�e�B�N�����ł̎���
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CParticle::~CParticle()
{

}

//==============================================================
//�p�[�e�B�N���̐�������
//==============================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{//���������g�p����ĂȂ�������

		//�p�[�e�B�N���̐���
		pParticle = new CParticle;
	}

	//����������
	pParticle->Init();

	//�ݒ菈��
	pParticle->Set(pos, col, type, nLife, fRadius);

	return pParticle;
}

//==============================================================
//�p�[�e�B�N���̏���������
//==============================================================
HRESULT CParticle::Init(void)
{

	return S_OK;
}

//==============================================================
//�p�[�e�B�N���̏I������
//==============================================================
void CParticle::Uninit(void)
{
	CObject::Release();
}

//==============================================================
//�p�[�e�B�N���̍X�V����
//==============================================================
void CParticle::Update(void)
{
	
}

//==============================================================
//�p�[�e�B�N���̕`�揈��
//==============================================================
void CParticle::Draw(void)
{
	
}

//==============================================================
//�p�[�e�B�N���̐ݒ菈��
//==============================================================
void CParticle::Set(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	m_type = type;

	//�p�[�e�B�N������
	switch (m_type)
	{
	case PARTICLETYPE_NONE:		//�����Ȃ�

		break;

	case PARTICLETYPE_EXPLOSION:	//����

		CParticle::GenerateExplosion(pos, col, m_type, nLife, fRadius);

		break;

	case PARTICLETYPE_LIFE:			//��

		CParticle::GenerateLife(pos, col, m_type, nLife, fRadius);

		break;

	case PARTICLETYPE_JUMP:			//�W�����v

		CParticle::GenerateJump(pos, col, m_type, nLife, fRadius);

		break;

	case PARTICLETYPE_MOVE:			//����

		CParticle::GenerateMove(pos, col, m_type, nLife, fRadius);

		break;
	}

	//�p�[�e�B�N���̏I������
	CParticle::Uninit();
}

//==============================================================
//������
//==============================================================
void CParticle::GenerateWater(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾

	//�p�[�e�B�N���̐���
	for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++)
	{
		//�ʒu�̐ݒ�
		m_pos = pos;

		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 200 - 100) * 0.01f;
		}
		if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 300 + 150) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 100 - 170) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 170 + 50) * 0.01f;
		}

		//�ړ��ʂ����߂�
		m_fMove = (float)(rand() % MOVE_WATER) / 10.0f + 1.0f;

		//�ړ��ʂ̐ݒ�
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//�F�̐ݒ�
		m_col = col;

		//���a�̐ݒ�
		m_fRadius = fRadius;

		//�����̐ݒ�
		m_nLife = nLife;

		m_type = type;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//�X����
//==============================================================
void CParticle::GenerateIce(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾

	//�p�[�e�B�N���̐���
	for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++)
	{
		//�ʒu�̐ݒ�
		m_pos = pos;

		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 200 - 100) * 0.01f;
		}
		if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 300 + 150) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 100 - 170) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 170 + 50) * 0.01f;
		}

		//�ړ��ʂ����߂�
		m_fMove = (float)(rand() % MOVE_ICE) / 10.0f + 1.0f;

		//�ړ��ʂ̐ݒ�
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//�F�̐ݒ�
		m_col = col;

		//���a�̐ݒ�
		m_fRadius = 30;

		//�����̐ݒ�
		m_nLife = LIFE_ICE;

		m_type = type;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//���C����
//==============================================================
void CParticle::GenerateVapor(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾

	//�p�[�e�B�N���̐���
	for (int nCntAppear = 0; nCntAppear < 20; nCntAppear++)
	{
		//�ʒu�̐ݒ�
		m_pos = pos;

		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 200 - 100) * 0.01f;
		}
		if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 300 + 150) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 100 - 170) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{//���L�[����������

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 170 + 50) * 0.01f;
		}

		//�ړ��ʂ����߂�
		m_fMove = (float)(rand() % MOVE_VAPOR) / 10.0f + 1.0f;

		//�ړ��ʂ̐ݒ�
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//�F�̐ݒ�
		m_col = col;

		//���a�̐ݒ�
		m_fRadius = 10;

		//�����̐ݒ�
		m_nLife = LIFE_VAPOR;

		m_type = type;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//��������
//==============================================================
void CParticle::GenerateExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	//�p�[�e�B�N���̐���
	for (int nCntAppear = 0; nCntAppear < 30; nCntAppear++)
	{
		//if (m_type == CObject::TYPE_ENEMY)
		{//�_���[�W���󂯂���

			//�ʒu�̐ݒ�
			m_pos = pos;

			//�p�x�����߂�
			m_fAngle = (float)(rand() % 629 - 314) / 1.0f;

			//�ړ��ʂ����߂�
			m_fMove = (float)(rand() % MOVE_EXPLOSION) / 10.0f + 1.0f;

			//�ړ��ʂ̐ݒ�
			m_move.x = sinf(m_fAngle) * m_fMove;
			m_move.y = cosf(m_fAngle) * m_fMove;
			m_move.z = cosf(m_fAngle) * m_fMove;

			//�F�̐ݒ�
			m_col = col;

			//���a�̐ݒ�
			m_fRadius = fRadius;

			//�����̐ݒ�
			m_nLife = nLife;

			m_type = type;
		}

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//�񕜐���
//==============================================================
void CParticle::GenerateLife(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾

	//�p�[�e�B�N���̐���
	for (int nCntAppear = 0; nCntAppear < 30; nCntAppear++)
	{
		//�ʒu�̐ݒ�
		m_pos = D3DXVECTOR3(pos.x + sinf(nCntAppear * 0.1f * D3DX_PI) * 30.0f, pos.y + (nCntAppear * 2), pos.z + cosf(nCntAppear * 0.1f * D3DX_PI) * 30.0f);

		//�ړ��ʂ̐ݒ�
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�F�̐ݒ�
		m_col = col;

		//���a�̐ݒ�
		m_fRadius = fRadius;

		//�����̐ݒ�
		m_nLife = nLife;

		m_type = type;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//�W�����v����
//==============================================================
void CParticle::GenerateJump(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	//�p�[�e�B�N���̐���
	for (int nCntAppear = 0; nCntAppear < 30; nCntAppear++)
	{
		//�ʒu�̐ݒ�
		m_pos = D3DXVECTOR3(pos.x, pos.y + 10.0f, pos.z - 10.0f);

		//�p�x�����߂�
		m_fAngle = (float)(rand() % 300 + 150) * 0.01f;

		//�ړ��ʂ����߂�
		m_fMove = (float)(rand() % MOVE_EXPLOSION) / 10.0f + 1.0f;

		//�ړ��ʂ̐ݒ�
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//�F�̐ݒ�
		m_col = col;

		//���a�̐ݒ�
		m_fRadius = fRadius;

		//�����̐ݒ�
		m_nLife = nLife;

		m_type = type;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//�ړ��p�[�e�B�N������
//==============================================================
void CParticle::GenerateMove(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	//�p�[�e�B�N���̐���
	for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
	{
		//�ʒu�̐ݒ�
		m_pos = pos;

		//�p�x�����߂�
		m_fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//�ړ��ʂ����߂�
		m_fMove = (float)(rand() % MOVE_EXPLOSION) / 10.0f + 1.0f;

		//�ړ��ʂ̐ݒ�
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//�F�̐ݒ�
		m_col = col;

		//���a�̐ݒ�
		m_fRadius = fRadius;

		//�����̐ݒ�
		m_nLife = nLife;

		m_type = type;

		//�G�t�F�N�g�̐���
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}