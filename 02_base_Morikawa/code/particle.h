//=======================================
//
//�p�[�e�B�N������[particle.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _PARTICLE_H_	//���̃}�N����`����`����Ă��Ȃ�������
#define _PARTICLE_H_	//2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//===========================================
// ���O���
//===========================================
namespace Mypartcile_Func
{
	void WALK_PARTICLE();
	void DEATH_PARTICLE();
	void BULLET_PARTICLE();	// �e
	void ROLLING_TURTLE();
	void CHANGE();
}

//===========================================
// ���O���
//===========================================
namespace Myparticle
{
	enum TYPE
	{// �񋭂̎��
		TYPE_WALK = 0,		// ��������
		TYPE_DEATH,			// ���S������
		TYPE_BULLET,		// �e�̎�
		TYPE_ROLLINGTURTLE,		// �J���̉�]
		TYPE_CHANGE,		// �J���̉�]
		TYPE_MAX
	};

	// �֐��|�C���^
	typedef void(*TYPE_FUNC)();

	// �֐����X�g��
	static TYPE_FUNC ParticleList[] =
	{
		&Mypartcile_Func::WALK_PARTICLE,	// ���s��
		&Mypartcile_Func::DEATH_PARTICLE,	// ���S��
		&Mypartcile_Func::BULLET_PARTICLE,	// �e
		&Mypartcile_Func::ROLLING_TURTLE,	// �T��]
		&Mypartcile_Func::CHANGE,	// �T��]
	};

	// ����
	void Create(TYPE nType, D3DXVECTOR3 pos);
}

#endif
