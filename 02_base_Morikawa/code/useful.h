//=======================================
//
// 便利関数、便利定数[useful.h]
// Author:森川駿弥
//
//=======================================

#ifndef _USEFUL_H_     //このマクロ定義が定義されていなかったら
#define _USEFUL_H_     //2連インクルード防止のマクロを定義する

#include "main.h"

//=======================================
// 関数定義
//=======================================
namespace USEFUL
{// 便利関数

	// 向きと角度の正規化
	void NormalizeRotAngle(float& fRotAngle);

	// 円の判定
	inline bool CollisionCircle(D3DXVECTOR3 pos, float fSize)
	{
		D3DXVECTOR3 vec;
		D3DXVec3Normalize(&vec, &pos);

		//対角線の長さ(横、縦)
		float fLength = sqrtf(pos.x * pos.x + pos.z * pos.z);

		if (fLength >= fSize)
		{// サイズ超えたら
			return true;
		}

		return false;
	}
}

//=======================================
// 定数定義
//=======================================
namespace Constance
{// 便利定数
const float ARENA_SIZE = 2250.0f;	// 闘技場のサイズ
}

#endif
