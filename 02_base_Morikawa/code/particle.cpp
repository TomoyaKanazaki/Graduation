//
//
//�p�[�e�B�N������[particle.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //�쐬����main.h���C���N���[�h����
#include "particle.h"    //�쐬����effect.h���C���N���[�h����
#include "effect.h"
#include "calculation.h"

//=======================================
// ���O���
//=======================================
namespace
{
D3DXVECTOR3 POS;	// �ʒu
}

//=======================================
// ����
//=======================================
void Myparticle::Create(TYPE nType, D3DXVECTOR3 pos)
{
	POS = pos;

	(*(ParticleList[nType]))();
}

//=======================================
// ���s���̃p�[�e�B�N��
//=======================================
void Mypartcile_Func::WALK_PARTICLE()
{
	int life = 30;		// ����
	D3DXCOLOR col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.6f);	// �F
	const int max = 5;		// �ő吔

	for (int i = 0; i < max; i++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �ړ��ʂ̊�l
		float StandardMove = 1.0f + (rand() % 10) * 0.1f;

		// ���a�̊�l
		float StandardRadius = 10.0f + (rand() % 50) * 0.9f;

		// �傫��
		move.x = sinf((rand() % 629 - 314) / 5.0f) * StandardMove;
		move.z = sinf((rand() % 629 - 314) / 5.0f) * StandardMove;

		CEffect::Create(POS, move, col, StandardRadius, life, false, CEffect::TYPE::TYPE_SMOKEBLACK);
	}
}

//=======================================
// ���S���̃p�[�e�B�N��
//=======================================
void Mypartcile_Func::DEATH_PARTICLE()
{
	int life = 30;		// ����
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �F
	const int max = 32;		// �ő吔

	for (int i = 0; i < max; i++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �ړ��ʂ̊�l
		float StandardMove = 5.0f + (rand() % 100) * 0.1f;

		// ���a�̊�l
		float StandardRadius = 9.0f + (rand() % 100) * 0.9f;

		// �傫��
		move.x = sinf((rand() % 629 - 314) / 100.0f) * StandardMove;
		move.y = sinf((rand() % 629 - 314) / 100.0f) * StandardMove;
		move.z = sinf((rand() % 629 - 314) / 150.0f) * StandardMove;

		// �G�t�F�N�g����
		CEffect::Create(POS, move, col, StandardRadius, life, true, CEffect::TYPE::TYPE_NORMAL);
	}
}

//==========================================================================
// �e
//==========================================================================
void Mypartcile_Func::BULLET_PARTICLE()
{
	int life = 25;		// ����
	float radius;
	D3DXCOLOR col;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	CEffect* pEffect = nullptr;


	//=============================
	// �ԐF
	//=============================
	// �ړ���
	float fMove = (float)(rand() % 20) / 10 + 1.0f;		//�ړ���
	float fMoveY = (float)(rand() % 61 - 30) / 100;		//�ړ���

	//�ړ��ʂ̐ݒ�
	move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
	move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;
	move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

	//�ʒu�u��
	pos = POS;
	pos += move * 0.3f;

	// ���a
	radius = 160.0f + (rand() % 101 - 50) * 0.1f;

	// �F
	col = D3DXCOLOR(
		0.9f + (rand() % 11) * 0.01f,
		0.0f,
		0.0f,
		0.8f);

	// �G�t�F�N�g����
	pEffect = CEffect::Create(pos, move, col, radius, life, true, CEffect::TYPE::TYPE_SMOKE);
	pEffect->SetAddSizeValue(-2.0f + ((rand() % 21 - 10) * 0.1f));


	//=============================
	// ���F
	//=============================
	// �ړ���
	fMove = (float)(rand() % 20) / 10 + 1.0f;	//�ړ���
	fMoveY = (float)(rand() % 61 - 30) / 100;	//�ړ���

	//�ړ��ʂ̐ݒ�
	move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
	move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;
	move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

	//�ʒu�u��
	pos = POS;
	pos += move * 0.3f;

	// ��
	radius = 120.0f + (rand() % 101 - 50) * 0.1f;

	// �F
	col = D3DXCOLOR(
		0.8f + (rand() % 11) * 0.01f,
		0.5f,
		0.1f,
		0.5f);

	// �G�t�F�N�g����
	pEffect = CEffect::Create(pos, move, col, radius, life, true, CEffect::TYPE::TYPE_SMOKE);
	pEffect->SetAddSizeValue(-2.0f + ((rand() % 21 - 10) * 0.1f));
}

//==========================================================================
// �T��]
//==========================================================================
void Mypartcile_Func::ROLLING_TURTLE()
{
	int life = 20;		// ����
	float radius;
	D3DXCOLOR col;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	CEffect* pEffect = nullptr;

	//=============================
	// �ԐF
	//=============================
	// �ړ���
	float fMove = UtilFunc::Transformation::Random(50, 80) * 0.1f;		//�ړ���
	float fMoveY = UtilFunc::Transformation::Random(60, 80) * 0.1f;		//�ړ���

	//�ړ��ʂ̐ݒ�
	move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
	move.y = fMoveY;
	move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

	//�ʒu�u��
	pos = POS;
	pos += move * 5.0f;

	// ���a
	radius = 50.0f + UtilFunc::Transformation::Random(-80, 80) * 0.1f;

	// �F
	float black = UtilFunc::Transformation::Random(0, 30) * 0.01f;
	col = D3DXCOLOR(
		0.6f + black,
		0.6f + black,
		0.6f + black,
		0.8f);

	// �G�t�F�N�g����
	pEffect = CEffect::Create(pos, move, col, radius, life, false, CEffect::TYPE::TYPE_SMOKE);
	pEffect->SetAddSizeValue(-2.0f + ((rand() % 21 - 10) * 0.1f));

	pEffect->SetRot(MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-31, 31) * 0.1f));
}


//==========================================================================
// �T��]
//==========================================================================
void Mypartcile_Func::CHANGE()
{
	int life = 30;		// ����
	float radius;
	D3DXCOLOR col;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	CEffect* pEffect = nullptr;

	for (int i = 0; i < 8; i++)
	{
		// �ړ���
		float fMove = UtilFunc::Transformation::Random(50, 80) * 0.1f;		//�ړ���
		float fMoveY = UtilFunc::Transformation::Random(60, 80) * 0.1f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		move.y = fMoveY;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		//�ʒu�u��
		pos = POS;
		pos += move * 5.0f;

		// ���a
		radius = 150.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;

		// �F
		float black = UtilFunc::Transformation::Random(0, 100) * 0.01f;
		col = D3DXCOLOR(
			UtilFunc::Transformation::Random(0, 100) * 0.01f,
			UtilFunc::Transformation::Random(0, 100) * 0.01f,
			UtilFunc::Transformation::Random(0, 100) * 0.01f,
			0.8f);

		// �G�t�F�N�g����
		pEffect = CEffect::Create(pos, move, col, radius, life, true, CEffect::TYPE::TYPE_SMOKE);
		pEffect->SetAddSizeValue(3.0f);

		pEffect->SetRot(MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-31, 31) * 0.1f));
	}
}