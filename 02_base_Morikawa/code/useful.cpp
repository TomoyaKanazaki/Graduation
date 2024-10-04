//=======================================
//
// •Ö—˜ŠÖ”[useful.h]
// Author@:@MORIKAWA SHUNYA
//
//=======================================

#include "useful.h"

//=======================================
// ³‹K‰»
//=======================================
void USEFUL::NormalizeRotAngle(float& fRotAngle)
{
	//Œü‚«‚Ì³‹K‰»
	if (fRotAngle > D3DX_PI)
	{//3.14‚ð’´‚¦‚½‚Æ‚«‚É”½‘Î‚É‚·‚é
		fRotAngle -= D3DX_PI * 2.0f;
	}

	//-3.14‚ð’´‚¦‚½‚Æ‚«‚É”½‘Î‚É‚·‚é
	if (fRotAngle < -D3DX_PI)
	{
		fRotAngle += D3DX_PI * 2.0f;
	}
}
