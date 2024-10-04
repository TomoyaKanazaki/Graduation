//=======================================
//
//パーティクル処理[particle.h]
//Author : MORIKAWA SHUNYA
//
//=======================================

#ifndef _PARTICLE_H_	//このマクロ定義が定義されていなかったら
#define _PARTICLE_H_	//2連インクルード防止のマクロを定義する

#include "main.h"

//===========================================
// 名前空間
//===========================================
namespace Mypartcile_Func
{
	void WALK_PARTICLE();
	void DEATH_PARTICLE();
	void BULLET_PARTICLE();	// 弾
	void ROLLING_TURTLE();
	void CHANGE();
}

//===========================================
// 名前空間
//===========================================
namespace Myparticle
{
	enum TYPE
	{// 列強の種類
		TYPE_WALK = 0,		// 歩いた時
		TYPE_DEATH,			// 死亡した時
		TYPE_BULLET,		// 弾の時
		TYPE_ROLLINGTURTLE,		// カメの回転
		TYPE_CHANGE,		// カメの回転
		TYPE_MAX
	};

	// 関数ポインタ
	typedef void(*TYPE_FUNC)();

	// 関数リスト名
	static TYPE_FUNC ParticleList[] =
	{
		&Mypartcile_Func::WALK_PARTICLE,	// 歩行時
		&Mypartcile_Func::DEATH_PARTICLE,	// 死亡時
		&Mypartcile_Func::BULLET_PARTICLE,	// 弾
		&Mypartcile_Func::ROLLING_TURTLE,	// 亀回転
		&Mypartcile_Func::CHANGE,	// 亀回転
	};

	// 生成
	void Create(TYPE nType, D3DXVECTOR3 pos);
}

#endif
