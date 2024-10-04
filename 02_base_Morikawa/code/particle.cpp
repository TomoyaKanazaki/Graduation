//
//
//パーティクル処理[particle.cpp]
//Author : MORIKAWA SHUNYA
//
//=======================================

#include "main.h"        //作成したmain.hをインクルードする
#include "particle.h"    //作成したeffect.hをインクルードする
#include "effect.h"
#include "calculation.h"

//=======================================
// 名前空間
//=======================================
namespace
{
D3DXVECTOR3 POS;	// 位置
}

//=======================================
// 生成
//=======================================
void Myparticle::Create(TYPE nType, D3DXVECTOR3 pos)
{
	POS = pos;

	(*(ParticleList[nType]))();
}

//=======================================
// 歩行時のパーティクル
//=======================================
void Mypartcile_Func::WALK_PARTICLE()
{
	int life = 30;		// 寿命
	D3DXCOLOR col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.6f);	// 色
	const int max = 5;		// 最大数

	for (int i = 0; i < max; i++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 移動量の基準値
		float StandardMove = 1.0f + (rand() % 10) * 0.1f;

		// 半径の基準値
		float StandardRadius = 10.0f + (rand() % 50) * 0.9f;

		// 大きさ
		move.x = sinf((rand() % 629 - 314) / 5.0f) * StandardMove;
		move.z = sinf((rand() % 629 - 314) / 5.0f) * StandardMove;

		CEffect::Create(POS, move, col, StandardRadius, life, false, CEffect::TYPE::TYPE_SMOKEBLACK);
	}
}

//=======================================
// 死亡時のパーティクル
//=======================================
void Mypartcile_Func::DEATH_PARTICLE()
{
	int life = 30;		// 寿命
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 色
	const int max = 32;		// 最大数

	for (int i = 0; i < max; i++)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 移動量の基準値
		float StandardMove = 5.0f + (rand() % 100) * 0.1f;

		// 半径の基準値
		float StandardRadius = 9.0f + (rand() % 100) * 0.9f;

		// 大きさ
		move.x = sinf((rand() % 629 - 314) / 100.0f) * StandardMove;
		move.y = sinf((rand() % 629 - 314) / 100.0f) * StandardMove;
		move.z = sinf((rand() % 629 - 314) / 150.0f) * StandardMove;

		// エフェクト生成
		CEffect::Create(POS, move, col, StandardRadius, life, true, CEffect::TYPE::TYPE_NORMAL);
	}
}

//==========================================================================
// 弾
//==========================================================================
void Mypartcile_Func::BULLET_PARTICLE()
{
	int life = 25;		// 寿命
	float radius;
	D3DXCOLOR col;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	CEffect* pEffect = nullptr;


	//=============================
	// 赤色
	//=============================
	// 移動量
	float fMove = (float)(rand() % 20) / 10 + 1.0f;		//移動量
	float fMoveY = (float)(rand() % 61 - 30) / 100;		//移動量

	//移動量の設定
	move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
	move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;
	move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

	//位置ブレ
	pos = POS;
	pos += move * 0.3f;

	// 半径
	radius = 160.0f + (rand() % 101 - 50) * 0.1f;

	// 色
	col = D3DXCOLOR(
		0.9f + (rand() % 11) * 0.01f,
		0.0f,
		0.0f,
		0.8f);

	// エフェクト生成
	pEffect = CEffect::Create(pos, move, col, radius, life, true, CEffect::TYPE::TYPE_SMOKE);
	pEffect->SetAddSizeValue(-2.0f + ((rand() % 21 - 10) * 0.1f));


	//=============================
	// 黄色
	//=============================
	// 移動量
	fMove = (float)(rand() % 20) / 10 + 1.0f;	//移動量
	fMoveY = (float)(rand() % 61 - 30) / 100;	//移動量

	//移動量の設定
	move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
	move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;
	move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

	//位置ブレ
	pos = POS;
	pos += move * 0.3f;

	// 炎
	radius = 120.0f + (rand() % 101 - 50) * 0.1f;

	// 色
	col = D3DXCOLOR(
		0.8f + (rand() % 11) * 0.01f,
		0.5f,
		0.1f,
		0.5f);

	// エフェクト生成
	pEffect = CEffect::Create(pos, move, col, radius, life, true, CEffect::TYPE::TYPE_SMOKE);
	pEffect->SetAddSizeValue(-2.0f + ((rand() % 21 - 10) * 0.1f));
}

//==========================================================================
// 亀回転
//==========================================================================
void Mypartcile_Func::ROLLING_TURTLE()
{
	int life = 20;		// 寿命
	float radius;
	D3DXCOLOR col;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	CEffect* pEffect = nullptr;

	//=============================
	// 赤色
	//=============================
	// 移動量
	float fMove = UtilFunc::Transformation::Random(50, 80) * 0.1f;		//移動量
	float fMoveY = UtilFunc::Transformation::Random(60, 80) * 0.1f;		//移動量

	//移動量の設定
	move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
	move.y = fMoveY;
	move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

	//位置ブレ
	pos = POS;
	pos += move * 5.0f;

	// 半径
	radius = 50.0f + UtilFunc::Transformation::Random(-80, 80) * 0.1f;

	// 色
	float black = UtilFunc::Transformation::Random(0, 30) * 0.01f;
	col = D3DXCOLOR(
		0.6f + black,
		0.6f + black,
		0.6f + black,
		0.8f);

	// エフェクト生成
	pEffect = CEffect::Create(pos, move, col, radius, life, false, CEffect::TYPE::TYPE_SMOKE);
	pEffect->SetAddSizeValue(-2.0f + ((rand() % 21 - 10) * 0.1f));

	pEffect->SetRot(MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-31, 31) * 0.1f));
}


//==========================================================================
// 亀回転
//==========================================================================
void Mypartcile_Func::CHANGE()
{
	int life = 30;		// 寿命
	float radius;
	D3DXCOLOR col;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	CEffect* pEffect = nullptr;

	for (int i = 0; i < 8; i++)
	{
		// 移動量
		float fMove = UtilFunc::Transformation::Random(50, 80) * 0.1f;		//移動量
		float fMoveY = UtilFunc::Transformation::Random(60, 80) * 0.1f;		//移動量

		//移動量の設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		move.y = fMoveY;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		//位置ブレ
		pos = POS;
		pos += move * 5.0f;

		// 半径
		radius = 150.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;

		// 色
		float black = UtilFunc::Transformation::Random(0, 100) * 0.01f;
		col = D3DXCOLOR(
			UtilFunc::Transformation::Random(0, 100) * 0.01f,
			UtilFunc::Transformation::Random(0, 100) * 0.01f,
			UtilFunc::Transformation::Random(0, 100) * 0.01f,
			0.8f);

		// エフェクト生成
		pEffect = CEffect::Create(pos, move, col, radius, life, true, CEffect::TYPE::TYPE_SMOKE);
		pEffect->SetAddSizeValue(3.0f);

		pEffect->SetRot(MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-31, 31) * 0.1f));
	}
}