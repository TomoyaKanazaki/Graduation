//=======================================
//
// �֗��֐�[useful.h]
// Author�@:�@MORIKAWA SHUNYA
//
//=======================================

#include "useful.h"

//=======================================
// ���K��
//=======================================
void USEFUL::NormalizeRotAngle(float& fRotAngle)
{
	//�����̐��K��
	if (fRotAngle > D3DX_PI)
	{//3.14�𒴂����Ƃ��ɔ��΂ɂ���
		fRotAngle -= D3DX_PI * 2.0f;
	}

	//-3.14�𒴂����Ƃ��ɔ��΂ɂ���
	if (fRotAngle < -D3DX_PI)
	{
		fRotAngle += D3DX_PI * 2.0f;
	}
}
