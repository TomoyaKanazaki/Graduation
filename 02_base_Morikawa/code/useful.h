//=======================================
//
// �֗��֐��A�֗��萔[useful.h]
// Author:�X��x��
//
//=======================================

#ifndef _USEFUL_H_     //���̃}�N����`����`����Ă��Ȃ�������
#define _USEFUL_H_     //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//=======================================
// �֐���`
//=======================================
namespace USEFUL
{// �֗��֐�

	// �����Ɗp�x�̐��K��
	void NormalizeRotAngle(float& fRotAngle);

	// �~�̔���
	inline bool CollisionCircle(D3DXVECTOR3 pos, float fSize)
	{
		D3DXVECTOR3 vec;
		D3DXVec3Normalize(&vec, &pos);

		//�Ίp���̒���(���A�c)
		float fLength = sqrtf(pos.x * pos.x + pos.z * pos.z);

		if (fLength >= fSize)
		{// �T�C�Y��������
			return true;
		}

		return false;
	}
}

//=======================================
// �萔��`
//=======================================
namespace Constance
{// �֗��萔
const float ARENA_SIZE = 2250.0f;	// ���Z��̃T�C�Y
}

#endif
