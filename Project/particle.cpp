//============================================
//
//	�p�[�e�B�N������ [particle.cpp]
//	Author:sakamoto kai
//
//============================================
#include "particle.h"
#include "effect.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"

//�}�N����`
namespace
{
	const float STANDERD_SPEED1(1.0f);		//�p�[�e�B�N���̊�̃X�s�[�h
	const float STANDERD_SPEED2(15.0f);		//�p�[�e�B�N���̊�̃X�s�[�h
	const float STANDERD_SPEED3(400.0f);	//�p�[�e�B�N���̊�̃X�s�[�h
	const float STANDERD_SPEED5(2.0f);		//�p�[�e�B�N���̊�̃X�s�[�h

	const int RAND_PAI1(731);			//�p�x�̃����_��
	const int PUT_PARTICLE1(15);		//�P�t���[���ɏo���p�[�e�B�N���̐�
	const int RAND_PAI2(731);			//�p�x�̃����_��
	const int PUT_PARTICLE2(45);		//�P�t���[���ɏo���p�[�e�B�N���̐�

	const int RAND_PAI3(731);			//�p�x�̃����_��
	const int PUT_PARTICLE3(1);			//�P�t���[���ɏo���p�[�e�B�N���̐�

	const int RAND_PAI5(629);			//�p�x�̃����_��
	const int PUT_PARTICLE5(20);		//�P�t���[���ɏo���p�[�e�B�N���̐�
	const int MOVE_SPEED5(8 + 1);		//�����_���ړ���
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CParticle::CParticle()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_nLife = 0;
	m_nSize = 0.0f;
	m_nType = 0;
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CParticle::CParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float nLife, float nSize, int nType)
{
	m_pos = pos;
	m_rot = rot;
	m_col = col;
	m_nLife = nLife;
	m_nSize = nSize;
	m_nType = nType;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CParticle::~CParticle()
{

}

//====================================================================
//��������
//====================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float nLife, float nSize, int nType)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pParticle = new CParticle(pos, rot, col, nLife, nSize, nType);
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pParticle->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

 	return pParticle;
}

//====================================================================
//������
//====================================================================
HRESULT CParticle::Init(void)
{
	CEffect *pEffect = NULL;
	SetType(CObject::TYPE_PARTICLE);

	if (m_nType == 1)
	{
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE1; nCntAppear++)
		{
			//�ړ��ʂ̐ݒ�
			m_move.x = sinf((float)(rand() % RAND_PAI1 - (RAND_PAI1 - 1) / 2) / 100.0f + D3DX_PI * -0.5f)*(float)((rand() % 100) / 10.0f) + STANDERD_SPEED1;
			m_move.y = cosf((float)(rand() % RAND_PAI1 - (RAND_PAI1 - 1) / 2) / 100.0f + D3DX_PI * -0.5f)*(float)((rand() % 100) / 10.0f) + STANDERD_SPEED1;
			m_move.z = 0.0f;

			//�G�t�F�N�g�̐���
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
		}
	}
	if (m_nType == 2)
	{
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE2; nCntAppear++)
		{
			//�ړ��ʂ̐ݒ�
			m_move.x = sinf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);
			m_move.y = cosf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);
			m_move.z = 0.0f;

			D3DXVec3Normalize(&m_move, &m_move);
			m_move *= STANDERD_SPEED2;

			//�G�t�F�N�g�̐���
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetDel(4.0f);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
		}
	}
	if (m_nType == 3)
	{
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE3; nCntAppear++)
		{
			//�ʒu�̐ݒ�
			m_pos.x = sinf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);
			m_pos.y = 0.0f;
			m_pos.z = cosf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);

			D3DXVec3Normalize(&m_pos, &m_pos);
			m_pos *= STANDERD_SPEED3;
			m_pos.y = CManager::GetInstance()->GetCamera()->GetPosY() - 400.0f;

			//�G�t�F�N�g�̐���
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetColor(m_col);
			pEffect->SetLife(m_nLife);
		}
	}
	if (m_nType == 4)
	{
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE2; nCntAppear++)
		{
			//�ړ��ʂ̐ݒ�
			m_move.x = sinf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);
			m_move.y = 0.0f;
			m_move.z = cosf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);

			D3DXVec3Normalize(&m_move, &m_move);
			m_move *= 8.0f;

			//�G�t�F�N�g�̐���
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetDel(-3.5f);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
		}
	}
	else if (m_nType == 5)
	{//����
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE5; nCntAppear++)
		{
			//�ړ��ʂ̐ݒ�
			m_move.x = sinf((float)(rand() % RAND_PAI5 - (RAND_PAI5 - 1)/ 2));
			m_move.y = sinf((float)(rand() % RAND_PAI5 - (RAND_PAI5 - 1)/ 2));
			m_move.z = cosf((float)(rand() % RAND_PAI5 - (RAND_PAI5 - 1)/ 2));
			D3DXVec3Normalize(&m_move, &m_move);
			m_move *= STANDERD_SPEED5 * (float)(rand() % MOVE_SPEED5 - (MOVE_SPEED5 - 1) / 2);
			
			//�G�t�F�N�g�̐���
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetDel(-3.5f);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
			pEffect->SetTexName("data\\TEXTURE\\effect\\effect002.jpg");
		}
	}
	else if (m_nType == 6)
	{//�E�e��
		for (int nCntAppear = 0; nCntAppear < 15; nCntAppear++)
		{
			//�ړ��ʂ̐ݒ�
			m_move.x = sinf(m_rot.y + D3DX_PI * 1.0f) * 100.0f;
			m_move.y = rand() % 200 * 2.0f - 100.0f;
			m_move.z = cosf(m_rot.y + D3DX_PI * 1.0f) * 100.0f;
			D3DXVec3Normalize(&m_move, &m_move);
			m_move *= (float)(rand() % 10) + 1.0f;

			//�G�t�F�N�g�̐���
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetDel(0.0f);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
			pEffect->SetTexName("data\\TEXTURE\\effect\\Particle01.png");
		}
	}
	else if (m_nType == 7)
	{//���e��
		for (int nCntAppear = 0; nCntAppear < 15; nCntAppear++)
		{
			//�ړ��ʂ̐ݒ�
			m_move.x = sinf(m_rot.y + D3DX_PI * 0.0f) * 100.0f;
			m_move.y = rand() % 200 * 2.0f - 100.0f;
			m_move.z = cosf(m_rot.y + D3DX_PI * 0.0f) * 100.0f;
			D3DXVec3Normalize(&m_move, &m_move);
			m_move *= (float)(rand() % 10) + 1.0f;

			//�G�t�F�N�g�̐���
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetDel(0.0f);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
			pEffect->SetTexName("data\\TEXTURE\\effect\\Particle01.png");
		}
	}
	else if (m_nType == 8)
	{//��e��
		for (int nCntAppear = 0; nCntAppear < 15; nCntAppear++)
		{
			//�ړ��ʂ̐ݒ�
			m_move.x = sinf(m_rot.y + D3DX_PI * (float)(rand() % 2)) * 100.0f;
			m_move.y = (float)(rand() % 200);
			m_move.z = cosf(m_rot.y + D3DX_PI * (float)(rand() % 2)) * 100.0f;
			D3DXVec3Normalize(&m_move, &m_move);
			m_move *= (float)(rand() % 10) + 1.0f;

			//�G�t�F�N�g�̐���
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetDel(0.0f);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
			pEffect->SetTexName("data\\TEXTURE\\effect\\Particle01.png");
		}
	}

	Uninit();

	return S_OK;
}

//====================================================================
//�p�[�e�B�N���̏I������
//====================================================================
void CParticle::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//�p�[�e�B�N���̍X�V����
//====================================================================
void CParticle::Update(void)
{

}

//====================================================================
//�p�[�e�B�N���̕`�揈��
//====================================================================
void CParticle::Draw(void)
{

}