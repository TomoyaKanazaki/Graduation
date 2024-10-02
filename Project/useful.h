//========================================================================================
//
//　便利関数[useful.h]
//	Author:sakamoto kai
//
//========================================================================================
#ifndef _USEFUL_H_
#define _USEFUL_H_

#include "main.h"

// 名前空間
namespace useful
{
	//当たり判定の使用軸
	enum COLLISION
	{
		COLLISION_XY = 0,
		COLLISION_YZ,
		COLLISION_ZX,
		COLLISION_X,
		COLLISION_Y,
		COLLISION_Z,
		COLLISION_MAX,
	};

	bool CollisionRectangle2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 Size1, D3DXVECTOR3 Size2, useful::COLLISION Shaft);
	bool CameraFront(D3DXVECTOR3 pos);
	bool CollisionLine(D3DXVECTOR3 Mypos, D3DXVECTOR3 PairPos, float Angle);
	bool SlashCollision(D3DXVECTOR3* EnemyPos, int Max, float SlashRot);
	void CollisionWindow(D3DXVECTOR3* pos);
	bool CollisionWindow(D3DXVECTOR3 pos);
	bool CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut);	//円の当たり判定
	bool CollisionRectangle(D3DXVECTOR3 posPair, D3DXVECTOR3 sizePair, D3DXVECTOR3 posMy, D3DXVECTOR3 sizeMy);
	bool CollisionBlock(D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 MovePair, D3DXVECTOR3 SizePair,D3DXVECTOR3* pPosMy, D3DXVECTOR3 pPosOldMy, D3DXVECTOR3* pMoveMy, D3DXVECTOR3* pObjMoveMy, D3DXVECTOR3 SizeMy, bool* bJump, useful::COLLISION XYZ);
	void NormalizeAngle(D3DXVECTOR3* rot);
	void NormalizeRot(float* rot);
	void Vec3Project(D3DXVECTOR3* pOut, D3DXVECTOR3 v);	// 2D座標に変換
	D3DXVECTOR3 Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);		//単位ベクトルを取得
	D3DXVECTOR3 CrossIntersection(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1,D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, float SeatchDistance);		//外積の判定の交点との距離を求める
	bool DecisionAbsolute(float fDataMy, float fDec);
	bool DecisionAbsolute(D3DXVECTOR3 dataMy, float fDec);
	float PosDirection(D3DXVECTOR3 MyPos, D3DXVECTOR3 PairPos, float ChageRot);		//引数の位置の方向を向く処理
	float DegreesToRadians(float degrees);			//度からラジアン変換
}

#endif